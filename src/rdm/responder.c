/******************************************************************************
 * Copyright 2019 ETC Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *    http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *******************************************************************************
 * This file is a part of RDMnet. For more information, go to:
 * https://github.com/ETCLabs/RDMnet
 ******************************************************************************/
#include "rdm/responder.h"

#include <string.h>
#include "etcpal/int.h"
#include "etcpal/pack.h"
#include "rdm/defs.h"

/***************************** Private macros ********************************/

// clang-format off
#define RDMRESP_RESP_TYPE_VALID(resptypeval)        \
  ((resptypeval) == kRdmResponseTypeAck          || \
   (resptypeval) == kRdmResponseTypeAckTimer     || \
   (resptypeval) == kRdmResponseTypeNackReason   || \
   (resptypeval) == kRdmResponseTypeAckOverflow)

#define RDMRESP_COMMAND_CLASS_VALID(ccval)      \
  ((ccval) == kRdmCCDiscoveryCommandResponse || \
   (ccval) == kRdmCCGetCommandResponse       || \
   (ccval) == kRdmCCSetCommandResponse)
// clang-format on

/*********************** Private function prototypes *************************/

static bool rdm_resp_data_valid(const RdmResponse *resp_data);

/*************************** Function definitions ****************************/

/*! \brief Unpack an RDM command.
 *  \param[in] buffer The packed RDM command.
 *  \param[out] cmd The RDM command data that was unpacked from buffer.
 *  \return #kEtcPalErrOk: Command unpacked successfully.
 *  \return #kEtcPalErrInvalid: Invalid argument provided.
 *  \return #kEtcPalErrProtocol: Packed RDM command was invalid.
 */
etcpal_error_t rdmresp_unpack_command(const RdmBuffer *buffer, RdmCommand *cmd)
{
  const uint8_t *cur_ptr;

  if (!buffer || !cmd)
    return kEtcPalErrInvalid;
  if (!rdm_validate_msg(buffer))
    return kEtcPalErrProtocol;

  cur_ptr = &buffer->data[RDM_OFFSET_DEST_MANUFACTURER];
  cmd->dest_uid.manu = etcpal_upack_16b(cur_ptr);
  cur_ptr += 2;
  cmd->dest_uid.id = etcpal_upack_32b(cur_ptr);
  cur_ptr += 4;
  cmd->source_uid.manu = etcpal_upack_16b(cur_ptr);
  cur_ptr += 2;
  cmd->source_uid.id = etcpal_upack_32b(cur_ptr);
  cur_ptr += 4;
  cmd->transaction_num = *cur_ptr++;
  cmd->port_id = *cur_ptr++;
  cur_ptr++; /* Message Count field is ignored */
  cmd->subdevice = etcpal_upack_16b(cur_ptr);
  cur_ptr += 2;
  cmd->command_class = (rdm_command_class_t)*cur_ptr++;
  cmd->param_id = etcpal_upack_16b(cur_ptr);
  cur_ptr += 2;
  cmd->datalen = *cur_ptr++;
  memcpy(cmd->data, cur_ptr, cmd->datalen);
  return kEtcPalErrOk;
}

/*! \brief Determine whether a packed RDM message is a non-discovery RDM command.
 *
 *  More specifically, whether the command class of the response is one of GET_COMMAND or
 *  SET_COMMAND.
 *
 *  \param[in] buffer The packed RDM message.
 *  \return true (the message is a valid non-discovery RDM command) or false (the message is invalid
 *          RDM or not a non-discovery command).
 */
bool rdmresp_is_non_disc_command(const RdmBuffer *buffer)
{
  if (buffer && rdm_validate_msg(buffer))
  {
    return (buffer->data[RDM_OFFSET_COMMAND_CLASS] == E120_GET_COMMAND ||
            buffer->data[RDM_OFFSET_COMMAND_CLASS] == E120_SET_COMMAND);
  }
  return false;
}

/*! \brief Create a packed RDM response.
 *  \param[in] resp_data The data that will be used for this RDM response packet.
 *  \param[out] buffer The buffer into which to pack this RDM response.
 *  \return #kEtcPalErrOk: Response created successfully.
 *  \return #kEtcPalErrInvalid: Invalid argument provided.
 *  \return #kEtcPalErrMsgSize: The parameter data was too long.
 */
etcpal_error_t rdmresp_pack_response(const RdmResponse *resp_data, RdmBuffer *buffer)
{
  uint8_t *cur_ptr;
  uint8_t rdm_length;

  /* Check for invalid parameters */
  if (!resp_data || !buffer)
    return kEtcPalErrInvalid;
  if (resp_data->datalen > RDM_MAX_PDL)
    return kEtcPalErrMsgSize;

  cur_ptr = buffer->data;
  rdm_length = resp_data->datalen + RDM_HEADER_SIZE;

  /* Pack the header and data into the buffer */
  *cur_ptr++ = E120_SC_RDM;
  *cur_ptr++ = E120_SC_SUB_MESSAGE;
  *cur_ptr++ = rdm_length;
  etcpal_pack_16b(cur_ptr, resp_data->dest_uid.manu);
  cur_ptr += 2;
  etcpal_pack_32b(cur_ptr, resp_data->dest_uid.id);
  cur_ptr += 4;
  etcpal_pack_16b(cur_ptr, resp_data->source_uid.manu);
  cur_ptr += 2;
  etcpal_pack_32b(cur_ptr, resp_data->source_uid.id);
  cur_ptr += 4;
  *cur_ptr++ = resp_data->transaction_num;
  *cur_ptr++ = (uint8_t)resp_data->resp_type;
  *cur_ptr++ = resp_data->msg_count;
  etcpal_pack_16b(cur_ptr, resp_data->subdevice);
  cur_ptr += 2;
  *cur_ptr++ = (uint8_t)resp_data->command_class;
  etcpal_pack_16b(cur_ptr, resp_data->param_id);
  cur_ptr += 2;
  *cur_ptr++ = resp_data->datalen;
  memcpy(cur_ptr, resp_data->data, resp_data->datalen);

  /* pack checksum and set packet length */
  rdm_pack_checksum(buffer->data, rdm_length);
  buffer->datalen = rdm_length + 2;
  return kEtcPalErrOk;
}

/* Do some basic validation on an RDM response provided by a library user. */
static bool rdm_resp_data_valid(const RdmResponse *resp_data)
{
  return (!RDM_UID_IS_BROADCAST(&resp_data->source_uid) && RDMRESP_RESP_TYPE_VALID(resp_data->resp_type) &&
          RDMRESP_COMMAND_CLASS_VALID(resp_data->command_class));
}
