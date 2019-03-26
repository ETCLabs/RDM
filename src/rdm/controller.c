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

/*************************** Function definitions ****************************/

/*! \brief Create a packed RDM command.
 *  \param[in] cmd_data The data that will be used for this RDM command packet.
 *  \param[out] buffer The buffer into which to pack this RDM command.
 *  \return #LWPA_OK: Command created successfully.\n
 *          #LWPA_INVALID: Invalid argument provided.\n
 *          #LWPA_MSGSIZE: The parameter data was too long.\n
 */
lwpa_error_t rdmctl_create_command(const RdmCommand *cmd_data, RdmBuffer *buffer)
{
  uint8_t *cur_ptr;
  uint8_t rdm_length;

  if (!cmd_data || !buffer)
    return LWPA_INVALID;
  if (cmd_data->datalen > RDM_MAX_PDL)
    return LWPA_MSGSIZE;

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
  return LWPA_OK;
}

/*! \brief Unpack an RDM repsonse.
 *  \param[in] buffer The packed RDM response.
 *  \param[out] resp The RDM response data that was unpacked from buffer.
 *  \return #LWPA_OK: Response unpacked successfully.\n
 *          #LWPA_INVALID: Invalid argument provided.\n
 *          #LWPA_PROTERR: Packed RDM response was invalid.\n
 */
lwpa_error_t rdmctl_unpack_response(const RdmBuffer *buffer, RdmResponse *resp)
{
  if (!buffer || !resp)
    return LWPA_INVALID;
  if (!rdm_validate_msg(buffer))
    return LWPA_PROTERR;

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
  return LWPA_OK;
}
