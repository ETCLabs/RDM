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
 * This file is a part of RDM. For more information, go to:
 * https://github.com/ETCLabs/RDM
 ******************************************************************************/
#include "rdm/controller.h"

#include <string.h>
#include "lwpa/pack.h"
#include "rdm/defs.h"

/***************************** Private macros ********************************/

#define RDMCMD_PORT_ID_VALID(portidval) ((portidval) > 0)
// clang-format off
#define RDMCMD_COMMAND_CLASS_VALID(ccval)    \
  ((ccval) == kRdmCCDiscoveryCommand      || \
   (ccval) == kRdmCCGetCommand            || \
   (ccval) == kRdmCCSetCommand)
// clang-format on

/*********************** Private function prototypes *************************/

static bool rdm_cmd_data_valid(const RdmCommand *cmd_data);

/*************************** Function definitions ****************************/

/*! \brief Create a packed RDM command.
 *  \param[in] cmd_data The data that will be used for this RDM command packet.
 *  \param[out] buffer The buffer into which to pack this RDM command.
 *  \return #kLwpaErrOk: Command created successfully.\n
 *          #kLwpaErrInvalid: Invalid argument provided (including invalid RDM values in cmd_data).\n
 *          #kLwpaErrMsgSize: The parameter data was too long.\n
 */
lwpa_error_t rdmctl_pack_command(const RdmCommand *cmd_data, RdmBuffer *buffer)
{
  uint8_t *cur_ptr;
  uint8_t rdm_length;

  if (!cmd_data || !buffer || !rdm_cmd_data_valid(cmd_data))
    return kLwpaErrInvalid;
  if (cmd_data->datalen > RDM_MAX_PDL)
    return kLwpaErrMsgSize;

  cur_ptr = buffer->data;
  rdm_length = cmd_data->datalen + RDM_HEADER_SIZE;

  *cur_ptr++ = E120_SC_RDM;
  *cur_ptr++ = E120_SC_SUB_MESSAGE;
  *cur_ptr++ = rdm_length;
  lwpa_pack_16b(cur_ptr, cmd_data->dest_uid.manu);
  cur_ptr += 2;
  lwpa_pack_32b(cur_ptr, cmd_data->dest_uid.id);
  cur_ptr += 4;
  lwpa_pack_16b(cur_ptr, cmd_data->source_uid.manu);
  cur_ptr += 2;
  lwpa_pack_32b(cur_ptr, cmd_data->source_uid.id);
  cur_ptr += 4;
  *cur_ptr++ = cmd_data->transaction_num;
  *cur_ptr++ = cmd_data->port_id;
  *cur_ptr++ = 0;
  lwpa_pack_16b(cur_ptr, cmd_data->subdevice);
  cur_ptr += 2;
  *cur_ptr++ = (uint8_t)cmd_data->command_class;
  lwpa_pack_16b(cur_ptr, cmd_data->param_id);
  cur_ptr += 2;
  *cur_ptr++ = cmd_data->datalen;
  memcpy(cur_ptr, cmd_data->data, cmd_data->datalen);

  /* pack checksum and set packet length */
  rdm_pack_checksum(buffer->data, rdm_length);
  buffer->datalen = rdm_length + 2;
  return kLwpaErrOk;
}

/*! \brief Determine whether a packed RDM message is a non-discovery RDM command response.
 *
 *  More specifically, whether the command class of the response is one of GET_COMMAND_RESPONSE or
 *  SET_COMMAND_RESPONSE.
 *
 *  \param[in] buffer The packed RDM message.
 *  \return true (the message is a valid non-discovery RDM command response) or false (the message
 *          is invalid RDM or not a non-discovery command response).
 */
bool rdmctl_is_non_disc_response(const RdmBuffer *buffer)
{
  if (buffer && rdm_validate_msg(buffer))
  {
    return (buffer->data[RDM_OFFSET_COMMAND_CLASS] == E120_GET_COMMAND_RESPONSE ||
            buffer->data[RDM_OFFSET_COMMAND_CLASS] == E120_SET_COMMAND_RESPONSE);
  }
  return false;
}

/*! \brief Unpack an RDM repsonse.
 *  \param[in] buffer The packed RDM response.
 *  \param[out] resp The RDM response data that was unpacked from buffer.
 *  \return #kLwpaErrOk: Response unpacked successfully.\n
 *          #kLwpaErrInvalid: Invalid argument provided.\n
 *          #kLwpaErrProtocol: Packed RDM response was invalid.\n
 */
lwpa_error_t rdmctl_unpack_response(const RdmBuffer *buffer, RdmResponse *resp)
{
  if (!buffer || !resp)
    return kLwpaErrInvalid;
  if (!rdm_validate_msg(buffer))
    return kLwpaErrProtocol;

  const uint8_t *cur_ptr = &buffer->data[RDM_OFFSET_DEST_MANUFACTURER];
  resp->dest_uid.manu = lwpa_upack_16b(cur_ptr);
  cur_ptr += 2;
  resp->dest_uid.id = lwpa_upack_32b(cur_ptr);
  cur_ptr += 4;
  resp->source_uid.manu = lwpa_upack_16b(cur_ptr);
  cur_ptr += 2;
  resp->source_uid.id = lwpa_upack_32b(cur_ptr);
  cur_ptr += 4;
  resp->transaction_num = *cur_ptr++;
  resp->resp_type = (rdm_response_type_t)*cur_ptr++;
  resp->msg_count = *cur_ptr++;
  resp->subdevice = lwpa_upack_16b(cur_ptr);
  cur_ptr += 2;
  resp->command_class = (rdm_command_class_t)*cur_ptr++;
  resp->param_id = lwpa_upack_16b(cur_ptr);
  cur_ptr += 2;
  resp->datalen = *cur_ptr++;
  memcpy(resp->data, cur_ptr, resp->datalen);
  return kLwpaErrOk;
}

/* Do some basic validation on an RDM command provided by a library user. */
static bool rdm_cmd_data_valid(const RdmCommand *cmd_data)
{
  return (!RDM_UID_IS_BROADCAST(&cmd_data->source_uid) && RDMCMD_PORT_ID_VALID(cmd_data->port_id) &&
          RDMCMD_COMMAND_CLASS_VALID(cmd_data->command_class));
}
