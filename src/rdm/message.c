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

#include "rdm/message.h"

#include "etcpal/pack.h"
#include "rdm/defs.h"

/*********************** Private function prototypes *************************/

static uint16_t calc_checksum(const uint8_t* buffer, size_t datalen_without_checksum);

/*************************** Function definitions ****************************/

/* Internal function to calculate the correct checksum of an RDM packet. */
uint16_t calc_checksum(const uint8_t* buffer, size_t datalen_without_checksum)
{
  size_t i;
  uint16_t sum = 0;
  for (i = 0; i < datalen_without_checksum; ++i)
    sum += buffer[i];
  return sum;
}

/*!
 * \brief Calculate and pack an RDM checksum at the end of an RDM message.
 * \param[in,out] buffer Buffer representing packed RDM message without checksum. Two-byte
 *                       checksum is packed starting at &buffer[datalen_without_checksum].
 * \param[in] datalen_without_checksum Length of the RDM message without checksum.
 */
void rdm_pack_checksum(uint8_t* buffer, size_t datalen_without_checksum)
{
  if (buffer)
  {
    uint16_t sum = calc_checksum(buffer, datalen_without_checksum);
    etcpal_pack_u16b(&buffer[datalen_without_checksum], sum);
  }
}

/*!
 * \brief Perform basic validation of an RDM message.
 *
 * Checks that the message has a correctly formed length, the correct start code values, and that
 * the checksum is correct.
 *
 * \param[in] buffer RDM message to validate.
 * \return true (RDM message is valid) or false (RDM message is invalid).
 */
bool rdm_validate_msg(const RdmBuffer* buffer)
{
  uint16_t sum;

  if (!buffer || buffer->datalen < RDM_MIN_BYTES || buffer->datalen > RDM_MAX_BYTES ||
      buffer->datalen < (size_t)buffer->data[RDM_OFFSET_LENGTH] + 2 ||
      buffer->data[RDM_OFFSET_STARTCODE] != E120_SC_RDM || buffer->data[RDM_OFFSET_SUBSTART] != E120_SC_SUB_MESSAGE)
  {
    return false;
  }

  sum = calc_checksum(buffer->data, buffer->datalen - 2);
  if (sum != etcpal_unpack_u16b(&buffer->data[buffer->data[RDM_OFFSET_LENGTH]]))
    return false;

  return true;
}

/*!
 * \brief Initialize a NACK_REASON RdmResponse to a received RdmCommand.
 *
 * Provide the received command and the NACK reason code.
 *
 * \param[in] cmd Received command.
 * \param[in] nack_reason NACK Reason code to send.
 * \param[out] resp Response to initialize.
 */
void rdm_create_nack_from_command(const RdmCommand* cmd, uint16_t nack_reason, RdmResponse* resp)
{
  rdm_create_nack_from_command_with_msg_count(cmd, nack_reason, 0, resp);
}

/*!
 * \brief Initialize a NACK_REASON RdmResponse to a received RdmCommand, specifying a queued
 *        message count.
 *
 * Provide the received command, the NACK reason code and the message count.
 *
 * \param[in] cmd Received command.
 * \param[in] nack_reason NACK Reason code to send.
 * \param[in] msg_count Message count to send.
 * \param[out] resp Response to initialize.
 */
void rdm_create_nack_from_command_with_msg_count(const RdmCommand* cmd, uint16_t nack_reason, uint8_t msg_count,
                                                 RdmResponse* resp)
{
  if (!cmd || !resp)
    return;

  resp->source_uid = cmd->dest_uid;
  resp->dest_uid = cmd->source_uid;
  resp->transaction_num = cmd->transaction_num;
  resp->resp_type = kRdmResponseTypeNackReason;
  resp->msg_count = msg_count;
  resp->subdevice = cmd->subdevice;
  resp->command_class = (cmd->command_class == kRdmCCSetCommand ? kRdmCCSetCommandResponse : kRdmCCGetCommandResponse);
  resp->param_id = cmd->param_id;
  resp->datalen = 2;
  etcpal_pack_u16b(resp->data, nack_reason);
}
