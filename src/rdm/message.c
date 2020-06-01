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
static uint16_t calc_checksum(const uint8_t* buffer, size_t data_len_without_checksum);

/*************************** Function definitions ****************************/

/* Internal function to calculate the correct checksum of an RDM packet. */
uint16_t calc_checksum(const uint8_t* buffer, size_t data_len_without_checksum)
{
  size_t i;
  uint16_t sum = 0;
  for (i = 0; i < data_len_without_checksum; ++i)
    sum += buffer[i];
  return sum;
}

/*!
 * \brief Calculate and pack an RDM checksum at the end of an RDM message.
 * \param[in,out] buffer Buffer representing packed RDM message without checksum. Two-byte
 *                       checksum is packed starting at &buffer[data_len_without_checksum].
 * \param[in] data_len_without_checksum Length of the RDM message without checksum.
 */
void rdm_pack_checksum(uint8_t* buffer, size_t data_len_without_checksum)
{
  if (buffer)
  {
    uint16_t sum = calc_checksum(buffer, data_len_without_checksum);
    etcpal_pack_u16b(&buffer[data_len_without_checksum], sum);
  }
}

/*!
 * \brief Perform basic validation of a packed RDM message.
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

  if (!buffer || buffer->data_len < RDM_MIN_BYTES || buffer->data_len > RDM_MAX_BYTES ||
      buffer->data_len < (size_t)buffer->data[RDM_OFFSET_LENGTH] + 2 ||
      buffer->data[RDM_OFFSET_STARTCODE] != E120_SC_RDM || buffer->data[RDM_OFFSET_SUBSTART] != E120_SC_SUB_MESSAGE)
  {
    return false;
  }

  sum = calc_checksum(buffer->data, buffer->data_len - 2);
  if (sum != etcpal_unpack_u16b(&buffer->data[buffer->data[RDM_OFFSET_LENGTH]]))
    return false;

  return true;
}

/*!
 * \brief Determine whether the values contained in a command header are valid for an RDM command.
 * \param[in] cmd_header Header to validate.
 */
bool rdm_command_header_is_valid(const RdmCommandHeader* cmd_header)
{
  if (cmd_header)
  {
    return (!RDM_UID_IS_BROADCAST(&cmd_header->source_uid) && (cmd_header->port_id != 0) &&
            ((cmd_header->command_class == kRdmCCGetCommand) || (cmd_header->command_class == kRdmCCSetCommand) ||
             (cmd_header->command_class == kRdmCCDiscoveryCommand)) &&
            (cmd_header->param_id != 0));
  }
  return false;
}

/*!
 * \brief Determine whether the values contained in a response header are valid for an RDM response.
 * \param[in] resp_header Header to validate.
 */
bool rdm_response_header_is_valid(const RdmResponseHeader* resp_header)
{
  if (resp_header)
  {
    return (!RDM_UID_IS_BROADCAST(&resp_header->source_uid) &&
            ((resp_header->resp_type == kRdmResponseTypeAck) ||
             (resp_header->resp_type == kRdmResponseTypeAckOverflow) ||
             (resp_header->resp_type == kRdmResponseTypeAckTimer) ||
             (resp_header->resp_type == kRdmResponseTypeNackReason)) &&
            ((resp_header->command_class == kRdmCCGetCommandResponse) ||
             (resp_header->command_class == kRdmCCSetCommandResponse) ||
             (resp_header->command_class == kRdmCCDiscoveryCommandResponse)) &&
            (resp_header->param_id != 0));
  }
  return false;
}

/*!
 * \brief Convert an RDM command header and data to the RDM wire format.
 *
 * \param[in] cmd_header The header data for the command.
 * \param[in] cmd_data The parameter data that accompanies the command, or NULL if no data.
 * \param[in] cmd_data_len Length in bytes of cmd_data, or 0 if no data.
 * \param[out] buffer RdmBuffer struct to hold the serialized command.
 * \return #kEtcPalErrOk: Command created successfully.
 * \return #kEtcPalErrInvalid: Invalid argument provided.
 */
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
 * \brief Convert an RDM command header and data to the RDM wire format in a user-provided buffer.
 *
 * \param[in] cmd_header The header data for the command.
 * \param[in] cmd_data The parameter data that accompanies the command, or NULL if no data.
 * \param[in] cmd_data_len Length in bytes of cmd_data, or 0 if no data.
 * \param[out] buf Data buffer to hold the serialized command.
 * \param[out] buf_len Length in bytes of buf.
 * \return #kEtcPalErrOk: Command created successfully.
 * \return #kEtcPalErrInvalid: Invalid argument provided.
 * \return #kEtcPalErrBufSize: Buffer not long enough to hold RDM message.
 */
etcpal_error_t rdm_create_command_with_custom_buf(const RdmCommandHeader* cmd_header, const uint8_t* cmd_data,
                                                  uint8_t cmd_data_len, uint8_t* buf, size_t buf_len)
{
  ETCPAL_UNUSED_ARG(cmd_header);
  ETCPAL_UNUSED_ARG(cmd_data);
  ETCPAL_UNUSED_ARG(cmd_data_len);
  ETCPAL_UNUSED_ARG(buf);
  ETCPAL_UNUSED_ARG(buf_len);
  return kEtcPalErrNotImpl;
}

/*!
 * \brief Create an RDM ACK response to a previously-received command.
 *
 * Creates and serializes the ACK response into a format suitable for sending on the wire. Response
 * data must fit in a single response (must be <= RDM_MAX_PDL); if it doesn't, split the data into
 * N chunks and call rdm_create_overflow_response() for the first N - 1, and this function for the
 * Nth chunk.
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
  buffer->data_len = rdm_length + 2;
  return kEtcPalErrOk;
}

/*!
 * \brief Create an RDM ACK_OVERFLOW response to a previously-received command.
 *
 * ACK_OVERFLOW responses represent part of an RDM response whose data is too large to send in a
 * single response. Creates and serializes the ACK_OVERFLOW response into a format suitable for
 * sending on the wire.
 *
 * \param[in] cmd_header Header from the received RDM command.
 * \param[in] response_data The portion of the response parameter data being sent in this overflow response.
 * \param[in] response_data_len The length of the response_data.
 * \param[out] buffer Data buffer filled in with serialized response.
 * \return #kEtcPalErrOk: Response created successfully.
 * \return #kEtcPalErrInvalid: Invalid argument.
 * \return #kEtcPalErrMsgSize: Response data too long.
 */
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
  buffer->data_len = rdm_length + 2;
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
etcpal_error_t rdm_create_timer_response(const RdmCommandHeader* cmd_header, uint8_t msg_count,
                                         unsigned int delay_time_ms, RdmBuffer* buffer)
{
  if (!cmd_header || !buffer || !validate_received_cmd_header(cmd_header) || delay_time_ms == 0 ||
      delay_time_ms > 6553500)
  {
    return kEtcPalErrInvalid;
  }

  uint8_t rdm_length = RDM_HEADER_SIZE + 2;  // 2 bytes for timer value
  pack_rdm_response_header(cmd_header, rdm_length, kRdmResponseTypeAckTimer, msg_count, buffer->data);

  // Calculate the on-wire ACK_TIMER value
  uint16_t timer_val = (uint16_t)(delay_time_ms / 100);
  // Always round up to the next 100ms
  if (delay_time_ms % 100)
    ++timer_val;
  etcpal_pack_u16b(&buffer->data[RDM_HEADER_SIZE], timer_val);

  // pack checksum and set packet length
  rdm_pack_checksum(buffer->data, rdm_length);
  buffer->data_len = rdm_length + 2;
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

  buffer->data_len = 24;
  return kEtcPalErrOk;
}

/*!
 * \brief Get the number of RDM ACK_OVERFLOW responses needed to send the given response data.
 *
 * This function does not necessarily return (response_data_len / RDM_MAX_PDL); it takes into
 * account parameter data which the RDM standard family requires to be split on specified
 * boundaries.
 *
 * \param[in] param_id The RDM parameter ID for the response being generated.
 * \param[in] response_data_len The length in bytes of the requested response data.
 * \return The number of ACK_OVERFLOW responses that will be necessary, or 0 if an invalid argument
 *         was given.
 */
size_t rdm_get_num_overflow_responses_needed(uint16_t param_id, size_t response_data_len)
{
  ETCPAL_UNUSED_ARG(param_id);
  ETCPAL_UNUSED_ARG(response_data_len);
  return 0;
}

/*!
 * \brief Create a full ACK_OVERFLOW/ACK sequence representing an oversized RDM response.
 *
 * This function is most often used by higher-level protocols like RDMnet.
 *
 * \param[in] cmd_header Header from the received RDM command.
 * \param[in] response_data The full response parameter data.
 * \param[in] response_data_len The length of the response parameter data.
 * \param[out] buffers Set of buffers into which to pack serialized RDM ACK_OVERFLOW/ACK responses in order.
 * \param[out] num_buffers Size of buffers array.
 * \return #kEtcPalErrOk: ACK_OVERFLOW responses created successfully.
 * \return #kEtcPalErrInvalid: Invalid argument provided.
 * \return #kEtcPalErrBufSize: Not enough RdmBuffers to hold the response data.
 */
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

/*!
 * \brief Add additional parameter data to an already-serialized RDM command.
 *
 * This function is useful when building an RDM command progressively. The length and checksum will
 * be updated.
 *
 * \param[in,out] buffer Packed RDM command to which to append data.
 * \param[in] additional_data Parameter data to append.
 * \param[in] additional_data_len Length of the parameter data to append.
 * \return #kEtcPalErrOk: Data appended successfully.
 * \return #kEtcPalErrInvalid: Invalid argument provided.
 * \return #kEtcPalErrBufSize: Not enough space in RDM command for additional data.
 */
etcpal_error_t rdm_append_parameter_data(RdmBuffer* buffer, const uint8_t* additional_data, uint8_t additional_data_len)
{
  ETCPAL_UNUSED_ARG(buffer);
  ETCPAL_UNUSED_ARG(additional_data);
  ETCPAL_UNUSED_ARG(additional_data_len);
  return kEtcPalErrNotImpl;
}

/*!
 * \brief Deserialize an RDM command from its wire format.
 *
 * \param[in] buffer Buffer holding serialized RDM command data.
 * \param[out] cmd_header Filled in with the RDM command header data.
 * \param[out] param_data Filled in with pointer to the start of the RDM parameter data in the original buffer.
 * \param[out] param_data_len Filled in with the length of the RDM parameter data accompanying the command.
 * \return #kEtcPalErrOk: Command unpacked successfully.
 * \return #kEtcPalErrInvalid: Invalid argument provided.
 * \return #kEtcPalErrProtocol: RDM protocol error in packed command.
 */
etcpal_error_t rdm_unpack_command(const RdmBuffer* buffer, RdmCommandHeader* cmd_header, const uint8_t** param_data,
                                  uint8_t* param_data_len)
{
  ETCPAL_UNUSED_ARG(buffer);
  ETCPAL_UNUSED_ARG(cmd_header);
  ETCPAL_UNUSED_ARG(param_data);
  ETCPAL_UNUSED_ARG(param_data_len);
  return kEtcPalErrNotImpl;
}

/*!
 * \brief Deserialize an RDM response from its wire format.
 *
 * \param[in] buffer Buffer holding serialized RDM response data.
 * \param[out] resp_header Filled in with the RDM response header data.
 * \param[out] param_data Filled in with pointer to the start of the RDM parameter data in the original buffer.
 * \param[out] param_data_len Filled in with the length of the RDM parameter data accompanying the response.
 * \return #kEtcPalErrOk: Response unpacked successfully.
 * \return #kEtcPalErrInvalid: Invalid argument provided.
 * \return #kEtcPalErrProtocol: RDM protocol error in packed response.
 */
etcpal_error_t rdm_unpack_response(const RdmBuffer* buffer, RdmResponseHeader* resp_header, const uint8_t** param_data,
                                   uint8_t* param_data_len)
{
  ETCPAL_UNUSED_ARG(buffer);
  ETCPAL_UNUSED_ARG(resp_header);
  ETCPAL_UNUSED_ARG(param_data);
  ETCPAL_UNUSED_ARG(param_data_len);
  return kEtcPalErrNotImpl;
}

/*!
 * \brief Deserialize an RDM DISC_UNIQUE_BRANCH from its wire format.
 *
 * DISC_UNIQUE_BRANCH responses have a special format. The only information encoded is the
 * responder's UID.
 *
 * \param[in] buffer Buffer holding serialized RDM DISC_UNIQUE_BRANCH response data.
 * \param[out] responder_uid Filled in with the RDM responder's UID.
 * \return #kEtcPalErrOk: DISC_UNIQUE_BRANCH response unpacked successfully.
 * \return #kEtcPalErrInvalid: Invalid argument provided.
 * \return #kEtcPalErrProtocol: RDM protocol error in packed response.
 */
etcpal_error_t rdm_unpack_dub_response(const RdmBuffer* buffer, RdmUid* responder_uid)
{
  ETCPAL_UNUSED_ARG(buffer);
  ETCPAL_UNUSED_ARG(responder_uid);
  return kEtcPalErrNotImpl;
}

/*!
 * \brief Get the ACK_TIMER delay in milliseconds from an ACK_TIMER message
 * \param[in] param_data The message parameter data. Must be at least 2 bytes in length.
 * \return The ACK_TIMER delay in milliseconds, or 0 if invalid argument provided.
 */
unsigned int rdm_get_ack_timer_delay(const uint8_t* param_data)
{
  if (param_data)
  {
    return (((unsigned int)etcpal_unpack_u16b(param_data)) * 100);
  }
  return 0;
}

/*!
 * \brief Get the NACK reason code from a NACK_REASON message.
 * \param[in] param_data The message parameter data. Must be at least 2 bytes in length.
 * \return The NACK reason code, or an invalid code if an invalid argument was provided.
 */
rdm_nack_reason_t rdm_get_nack_reason_code(const uint8_t* param_data)
{
  if (param_data)
  {
    return ((rdm_nack_reason_t)etcpal_unpack_u16b(param_data));
  }
  return (rdm_nack_reason_t)0xffff;
}

/*!
 * \brief Get a string representation of an RDM command class.
 * \param[in] command_class Command class for which to get a string representation.
 * \return The command class string, or a placeholder string if an invalid command class was given.
 */
const char* rdm_command_class_to_string(rdm_command_class_t command_class)
{
  switch (command_class)
  {
    case kRdmCCDiscoveryCommand:
      return "DISCOVERY_COMMAND";
    case kRdmCCDiscoveryCommandResponse:
      return "DISCOVERY_COMMAND_RESPONSE";
    case kRdmCCGetCommand:
      return "GET_COMMAND";
    case kRdmCCGetCommandResponse:
      return "GET_COMMAND_RESPONSE";
    case kRdmCCSetCommand:
      return "SET_COMMAND";
    case kRdmCCSetCommandResponse:
      return "SET_COMMAND_RESPONSE";
    default:
      return "Unknown Command Class";
  }
}

static const char* kResponseTypeStrings[kRdmNumResponseTypes] = {
    "ACK",          // kRdmResponseTypeAck
    "ACK_TIMER",    // kRdmResponseTypeAckTimer
    "NACK_REASON",  // kRdmResponseTypeNackReason
    "ACK_OVERFLOW"  // kRdmResponseTypeAckOverflow
};

/*!
 * \brief Get a string representation of an RDM response type.
 * \param[in] resp_type Response type for which to get a string representation.
 * \return The response type string, or a placeholder string if an invalid response type was given.
 */
const char* rdm_response_type_to_string(rdm_response_type_t resp_type)
{
  if (resp_type >= 0 && resp_type < kRdmNumResponseTypes)
  {
    return kResponseTypeStrings[resp_type];
  }
  return "Unknown Response Type";
}

static const char* kNackReasonStrings[kRdmNumStandardNRCodes] = {
    "Unknown or Unsupported PID",                     // kRdmNRUnknownPid
    "Message Format Error",                           // kRdmNRFormatError
    "Internal Hardware Fault",                        // kRdmNRHardwareFault
    "Proxy Cannot Comply With Message",               // kRdmNRProxyReject
    "Write Protect Enabled",                          // kRdmNRWriteProtect
    "Command Class Not Supported For This Command",   // kRdmNRUnsupportedCommandClass
    "Data Out of Allowable Range",                    // kRdmNRDataOutOfRange
    "No Free Space in Queue",                         // kRdmNRBufferFull
    "Message Too Large",                              // kRdmNRPacketSizeUnsupported
    "Sub-device Out Of Range or Unknown",             // kRdmNRSubDeviceOutOfRange
    "No Free Space in Proxy Queue",                   // kRdmNRProxyBufferFull
    "Specified Action is Not Supported",              // kRdmNRActionNotSupported
    "Specified Endpoint is Invalid",                  // kRdmNREndpointNumberInvalid
    "Action Not Supported in Current Endpoint Mode",  // kRdmNRInvalidEndpointMode
    "Specified UID is Not Recognized",                // kRdmNRUnknownUid
    "Not Participating in Specified Scope",           // kRdmNRUnknownScope
    "Specified Static Config Type is Invalid",        // kRdmNRInvalidStaticConfigType
    "Specified IPv4 Address is Invalid",              // kRdmNRInvalidIpv4Address
    "Specified IPv6 Address is Invalid",              // kRdmNRInvalidIpv6Address
    "Specified Transport Layer Port is Invalid",      // kRdmNRInvalidPort
};

/*!
 * \brief Get a string representation of an RDM NACK reason code.
 * \param[in] reason_code Reason code for which to get a string representation.
 * \return The reason string, or a placeholder string if an invalid or manufacturer-specific reason
 *         code was given.
 */
const char* rdm_nack_reason_to_string(rdm_nack_reason_t reason_code)
{
  if (reason_code >= 0 && reason_code < kRdmNumStandardNRCodes)
  {
    return kNackReasonStrings[reason_code];
  }
  return "Unknown or Manufacturer-Specific NACK Reason Code";
}

bool validate_received_cmd_header(const RdmCommandHeader* header)
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
