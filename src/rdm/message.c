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

#include <string.h>
#include "etcpal/common.h"
#include "etcpal/pack.h"
#include "rdm/defs.h"

/*********************** Private function prototypes *************************/

static bool validate_received_cmd_header(const RdmCommandHeader* header);
static void pack_rdm_response_header(const RdmCommandHeader* cmd_header, uint8_t rdm_length,
                                     rdm_response_type_t response_type, uint8_t msg_count, uint8_t* buffer);
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

etcpal_error_t rdm_create_command(const RdmCommandHeader* cmd_header, const uint8_t* cmd_data, uint8_t cmd_data_len,
                                  RdmBuffer* buffer)
{
  ETCPAL_UNUSED_ARG(cmd_header);
  ETCPAL_UNUSED_ARG(cmd_data);
  ETCPAL_UNUSED_ARG(cmd_data_len);
  ETCPAL_UNUSED_ARG(buffer);
  return kEtcPalErrNotImpl;
}

/*!
 * \brief Create an RDM ACK response to a previously-received command.
 *
 * Creates and serializes the ACK response into a format suitable for sending on the wire. Response
 * data must fit in a single response; if it doesn't, use rdm_create_overflow_response().
 *
 * \param[in] cmd_header Header from the received RDM command.
 * \param[in] msg_count The current queued message count.
 * \param[in] response_data (optional) The response parameter data, if any.
 * \param[in] response_data_len (optional) The length of the response parameter data, if any.
 * \param[out] buffer Data buffer filled in with serialized response.
 * \return #kEtcPalErrOk: Response created successfully.
 * \return #kEtcPalErrInvalid: Invalid argument.
 * \return #kEtcPalErrMsgSize: Response data too long.
 */
etcpal_error_t rdm_create_response(const RdmCommandHeader* cmd_header, uint8_t msg_count, const uint8_t* response_data,
                                   uint8_t response_data_len, RdmBuffer* buffer)
{
  if (!cmd_header || !buffer || !validate_received_cmd_header(cmd_header))
    return kEtcPalErrInvalid;
  if (response_data_len > RDM_MAX_PDL)
    return kEtcPalErrMsgSize;

  uint8_t rdm_length = response_data_len + RDM_HEADER_SIZE;
  pack_rdm_response_header(cmd_header, rdm_length, kRdmResponseTypeAck, msg_count, buffer->data);

  // Pack the response parameter data, if present
  if (response_data && response_data_len)
    memcpy(&buffer->data[RDM_HEADER_SIZE], response_data, response_data_len);

  // pack checksum and set packet length
  rdm_pack_checksum(buffer->data, rdm_length);
  buffer->datalen = rdm_length + 2;
  return kEtcPalErrOk;
}

etcpal_error_t rdm_create_overflow_response(const RdmCommandHeader* cmd_header, const uint8_t* response_data,
                                            uint8_t response_data_len, RdmBuffer* buffer)
{
  ETCPAL_UNUSED_ARG(cmd_header);
  ETCPAL_UNUSED_ARG(response_data);
  ETCPAL_UNUSED_ARG(response_data_len);
  ETCPAL_UNUSED_ARG(buffer);
  return kEtcPalErrNotImpl;
}

/*!
 * \brief Create an RDM NACK response to a previously-received command.
 *
 * Creates and serializes the NACK response into a format suitable for sending on the wire.
 *
 * \param[in] cmd_header Header from the received RDM command.
 * \param[in] msg_count The current queued message count.
 * \param[in] nack_reason The RDM NACK reason to send.
 * \param[out] buffer Data buffer filled in with serialized response.
 * \return #kEtcPalErrOk: Response created successfully.
 * \return #kEtcPalErrInvalid: Invalid argument.
 */
etcpal_error_t rdm_create_nack_response(const RdmCommandHeader* cmd_header, uint8_t msg_count,
                                        rdm_nack_reason_t nack_reason, RdmBuffer* buffer)
{
  if (!cmd_header || !buffer || !validate_received_cmd_header(cmd_header))
    return kEtcPalErrInvalid;

  uint8_t rdm_length = RDM_HEADER_SIZE + 2;  // 2 bytes for NACK reason code
  pack_rdm_response_header(cmd_header, rdm_length, kRdmResponseTypeNackReason, msg_count, buffer->data);

  // Pack the NACK reason code
  etcpal_pack_u16b(&buffer->data[RDM_HEADER_SIZE], nack_reason);

  // pack checksum and set packet length
  rdm_pack_checksum(buffer->data, rdm_length);
  buffer->datalen = rdm_length + 2;
  return kEtcPalErrOk;
}

/*!
 * \brief Create an RDM ACK_TIMER response to a previously-received command.
 *
 * Creates and serializes the ACK_TIMER response into a format suitable for sending on the wire.
 *
 * \param[in] cmd_header Header from the received RDM command.
 * \param[in] msg_count The current queued message count.
 * \param[in] delay_time_ms The amount of time in milliseconds before the response will be
 *                          available. Must be in the range [1, 6553500].
 * \param[out] buffer Data buffer filled in with serialized response.
 * \return #kEtcPalErrOk: Response created successfully.
 * \return #kEtcPalErrInvalid: Invalid argument.
 */
etcpal_error_t rdm_create_timer_response(const RdmCommandHeader* cmd_header, uint8_t msg_count, uint32_t delay_time_ms,
                                         RdmBuffer* buffer)
{
  if (!cmd_header || !buffer || !validate_received_cmd_header(cmd_header) || delay_time_ms == 0 ||
      delay_time_ms > 6553500)
  {
    return kEtcPalErrInvalid;
  }

  uint8_t rdm_length = RDM_HEADER_SIZE + 2;  // 2 bytes for timer value
  pack_rdm_response_header(cmd_header, rdm_length, kRdmResponseTypeAckTimer, msg_count, buffer->data);

  // Calculate the on-wire ACK_TIMER value
  uint16_t timer_val = delay_time_ms / 100;
  // Always round up to the next 100ms
  if (delay_time_ms % 100)
    ++timer_val;
  etcpal_pack_u16b(&buffer->data[RDM_HEADER_SIZE], timer_val);

  // pack checksum and set packet length
  rdm_pack_checksum(buffer->data, rdm_length);
  buffer->datalen = rdm_length + 2;
  return kEtcPalErrOk;
}

/*!
 * \brief Create an RDM DISC_UNIQUE_BRANCH response.
 *
 * The DISC_UNIQUE_BRANCH response does not adhere to the normal RDM packet format and is sent
 * without a break. The only information encoded is this responder's UID.
 *
 * \param[in] responder_uid UID of the responder responding to the DISC_UNIQUE_BRANCH command.
 * \param[out] buffer Data buffer filled in with serialized response.
 * \return #kEtcPalErrOk: Response created successfully.
 * \return #kEtcPalErrInvalid: Invalid argument.
 */
etcpal_error_t rdm_create_dub_response(const RdmUid* responder_uid, RdmBuffer* buffer)
{
  if (!responder_uid || !buffer)
    return kEtcPalErrInvalid;

  uint8_t* cur_ptr = buffer->data;

  // Pack the preamble and separator
  memset(cur_ptr, 0xfe, 7);
  cur_ptr += 7;
  *cur_ptr++ = 0xaa;

  // Encode and pack the responder UID
  *cur_ptr++ = ((uint8_t)(responder_uid->manu >> 8) | 0xaa);
  *cur_ptr++ = ((uint8_t)(responder_uid->manu >> 8) | 0x55);
  *cur_ptr++ = ((uint8_t)(responder_uid->manu) | 0xaa);
  *cur_ptr++ = ((uint8_t)(responder_uid->manu) | 0x55);
  *cur_ptr++ = ((uint8_t)(responder_uid->id >> 24) | 0xaa);
  *cur_ptr++ = ((uint8_t)(responder_uid->id >> 24) | 0x55);
  *cur_ptr++ = ((uint8_t)(responder_uid->id >> 16) | 0xaa);
  *cur_ptr++ = ((uint8_t)(responder_uid->id >> 16) | 0x55);
  *cur_ptr++ = ((uint8_t)(responder_uid->id >> 8) | 0xaa);
  *cur_ptr++ = ((uint8_t)(responder_uid->id >> 8) | 0x55);
  *cur_ptr++ = ((uint8_t)(responder_uid->id) | 0xaa);
  *cur_ptr++ = ((uint8_t)(responder_uid->id) | 0x55);

  // Calculate and pack the checksum
  uint16_t checksum = calc_checksum(&buffer->data[8], 12);
  *cur_ptr++ = ((uint8_t)(checksum >> 8) | 0xaa);
  *cur_ptr++ = ((uint8_t)(checksum >> 8) | 0x55);
  *cur_ptr++ = ((uint8_t)(checksum) | 0xaa);
  *cur_ptr++ = ((uint8_t)(checksum) | 0x55);

  buffer->datalen = 24;
  return kEtcPalErrOk;
}

size_t rdm_get_num_overflow_responses_needed(uint16_t param_id, size_t response_data_len)
{
  ETCPAL_UNUSED_ARG(param_id);
  ETCPAL_UNUSED_ARG(response_data_len);
  return 0;
}

etcpal_error_t rdm_create_full_overflow_response(const RdmCommandHeader* cmd_header, const uint8_t* response_data,
                                                 size_t response_data_len, RdmBuffer* buffers, size_t num_buffers)
{
  ETCPAL_UNUSED_ARG(cmd_header);
  ETCPAL_UNUSED_ARG(response_data);
  ETCPAL_UNUSED_ARG(response_data_len);
  ETCPAL_UNUSED_ARG(buffers);
  ETCPAL_UNUSED_ARG(num_buffers);
  return kEtcPalErrNotImpl;
}

etcpal_error_t rdm_unpack_command(const RdmBuffer* buffer, RdmCommandHeader* cmd_header, const uint8_t** param_data,
                                  uint8_t* param_data_len)
{
  ETCPAL_UNUSED_ARG(buffer);
  ETCPAL_UNUSED_ARG(cmd_header);
  ETCPAL_UNUSED_ARG(param_data);
  ETCPAL_UNUSED_ARG(param_data_len);
  return kEtcPalErrNotImpl;
}

etcpal_error_t rdm_unpack_response(const RdmBuffer* buffer, RdmResponseHeader* resp_header, const uint8_t** param_data,
                                   uint8_t* param_data_len)
{
  ETCPAL_UNUSED_ARG(buffer);
  ETCPAL_UNUSED_ARG(resp_header);
  ETCPAL_UNUSED_ARG(param_data);
  ETCPAL_UNUSED_ARG(param_data_len);
  return kEtcPalErrNotImpl;
}

etcpal_error_t rdm_unpack_dub_response(const RdmBuffer* buffer, RdmUid* responder_uid)
{
  ETCPAL_UNUSED_ARG(buffer);
  ETCPAL_UNUSED_ARG(responder_uid);
  return kEtcPalErrNotImpl;
}

static bool validate_received_cmd_header(const RdmCommandHeader* header)
{
  return (!RDM_UID_EQUAL(&header->source_uid, &kRdmBroadcastUid) &&
          !RDM_UID_EQUAL(&header->dest_uid, &kRdmBroadcastUid) &&
          (header->command_class == kRdmCCDiscoveryCommand || header->command_class == kRdmCCGetCommand ||
           header->command_class == kRdmCCSetCommand));
}

void pack_rdm_response_header(const RdmCommandHeader* cmd_header, uint8_t rdm_length, rdm_response_type_t response_type,
                              uint8_t msg_count, uint8_t* buffer)
{
  uint8_t* cur_ptr = buffer;

  *cur_ptr++ = E120_SC_RDM;
  *cur_ptr++ = E120_SC_SUB_MESSAGE;
  *cur_ptr++ = rdm_length;
  etcpal_pack_u16b(cur_ptr, cmd_header->source_uid.manu);
  cur_ptr += 2;
  etcpal_pack_u32b(cur_ptr, cmd_header->source_uid.id);
  cur_ptr += 4;
  etcpal_pack_u16b(cur_ptr, cmd_header->dest_uid.manu);
  cur_ptr += 2;
  etcpal_pack_u32b(cur_ptr, cmd_header->dest_uid.id);
  cur_ptr += 4;
  *cur_ptr++ = cmd_header->transaction_num;
  *cur_ptr++ = (uint8_t)response_type;
  *cur_ptr++ = msg_count;
  etcpal_pack_u16b(cur_ptr, cmd_header->subdevice);
  cur_ptr += 2;
  *cur_ptr++ = ((uint8_t)cmd_header->command_class) + 1;
  etcpal_pack_u16b(cur_ptr, cmd_header->param_id);
  cur_ptr += 2;
  *cur_ptr++ = rdm_length - RDM_HEADER_SIZE;
}
