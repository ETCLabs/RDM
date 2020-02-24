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

/*!
 * \file rdm/message.h
 * \brief Contains type definitions for use with RDM (E1.20) messages.
 */

#ifndef RDM_MESSAGE_H_
#define RDM_MESSAGE_H_

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "etcpal/error.h"
#include "rdm/uid.h"
#include "rdm/defs.h"

/*!
 * \defgroup message Message
 * \ingroup rdm
 * \brief Basic RDM message packing and unpacking.
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*! The maximum size for an RDM packet, including the two checksum bytes. */
#define RDM_MAX_BYTES 257

/*! The minimum size for an RDM packet. */
#define RDM_MIN_BYTES 26

/*! The maximum length of the Parameter Data in an RDM packet. */
#define RDM_MAX_PDL 231

/*! The length of the RDM message preceding the Parameter Data. */
#define RDM_HEADER_SIZE 24

/*!
 * \name RDM packet offsets
 * @{
 */
#define RDM_OFFSET_STARTCODE 0
#define RDM_OFFSET_SUBSTART 1
#define RDM_OFFSET_LENGTH 2
#define RDM_OFFSET_DEST_MANUFACTURER 3
#define RDM_OFFSET_DEST_DEVICE 5
#define RDM_OFFSET_SRC_MANUFACTURER 9
#define RDM_OFFSET_SRC_DEVICE 11
#define RDM_OFFSET_TRANSACTION 15
#define RDM_OFFSET_PORTID_RESPTYPE 16
#define RDM_OFFSET_MSGCOUNT 17
#define RDM_OFFSET_SUBDEVICE 18
#define RDM_OFFSET_COMMAND_CLASS 20
#define RDM_OFFSET_PARAM_ID 21
#define RDM_OFFSET_PARAM_DATA_LEN 23
#define RDM_OFFSET_PARAM_DATA 24
/*!
 * @}
 */

/*! An enumeration representing the RDM Command Class (CC) field. */
typedef enum
{
  kRdmCCDiscoveryCommand = E120_DISCOVERY_COMMAND,
  kRdmCCDiscoveryCommandResponse = E120_DISCOVERY_COMMAND_RESPONSE,
  kRdmCCGetCommand = E120_GET_COMMAND,
  kRdmCCGetCommandResponse = E120_GET_COMMAND_RESPONSE,
  kRdmCCSetCommand = E120_SET_COMMAND,
  kRdmCCSetCommandResponse = E120_SET_COMMAND_RESPONSE
} rdm_command_class_t;

/*! An enumeration representing the RDM Response Type field. */
typedef enum
{
  kRdmResponseTypeAck = E120_RESPONSE_TYPE_ACK,
  kRdmResponseTypeAckTimer = E120_RESPONSE_TYPE_ACK_TIMER,
  kRdmResponseTypeNackReason = E120_RESPONSE_TYPE_NACK_REASON,
  kRdmResponseTypeAckOverflow = E120_RESPONSE_TYPE_ACK_OVERFLOW
} rdm_response_type_t;

/*! An enumeration representing an RDM NACK reason. */
typedef enum
{
  kRdmNRUnknownPid = E120_NR_UNKNOWN_PID,
  kRdmNRFormatError = E120_NR_FORMAT_ERROR,
  kRdmNRHardwareFault = E120_NR_HARDWARE_FAULT,
  kRdmNRProxyReject = E120_NR_PROXY_REJECT,
  kRdmNRWriteProtect = E120_NR_WRITE_PROTECT,
  kRdmNRUnsupportedCommandClass = E120_NR_UNSUPPORTED_COMMAND_CLASS,
  kRdmNRDataOutOfRange = E120_NR_DATA_OUT_OF_RANGE,
  kRdmNRPacketSizeUnsupported = E120_NR_PACKET_SIZE_UNSUPPORTED,
  kRdmNRSubDeviceOutOfRange = E120_NR_SUB_DEVICE_OUT_OF_RANGE,
  kRdmNRProxyBufferFull = E120_NR_PROXY_BUFFER_FULL,
  kRdmNRActionNotSupported = E137_2_NR_ACTION_NOT_SUPPORTED,
  kRdmNREndpointNumberInvalid = E137_7_NR_ENDPOINT_NUMBER_INVALID,
  kRdmNRInvalidEndpointMode = E137_7_NR_INVALID_ENDPOINT_MODE,
  kRdmNRUnknownUid = E137_7_NR_UNKNOWN_UID,
  kRdmNRUnknownScope = E133_NR_UNKNOWN_SCOPE,
  kRdmNRInvalidStaticConfigType = E133_NR_INVALID_STATIC_CONFIG_TYPE,
  kRdmNRInvalidIpv4Address = E133_NR_INVALID_IPV4_ADDRESS,
  kRdmNRInvalidIpv6Address = E133_NR_INVALID_IPV6_ADDRESS,
  kRdmNRInvalidPort = E133_NR_INVALID_PORT
} rdm_nack_reason_t;

/*! A structure that represents a packed RDM message. */
typedef struct RdmBuffer
{
  uint8_t data[RDM_MAX_BYTES]; /*!< The RDM message bytes. */
  size_t datalen;              /*!< The length of the RDM message. */
} RdmBuffer;

/*!
 * \brief Get the command class value from a packed RDM command.
 * \param rdmbufptr Pointer to RdmBuffer containing the packed RDM command.
 * \return Command class (uint8_t).
 */
#define RDM_GET_COMMAND_CLASS(rdmbufptr) ((rdmbufptr)->data[RDM_OFFSET_COMMAND_CLASS])

/*!
 * \brief Get the RDM transaction number from a packed RDM command.
 * \param rdmbufptr Pointer to RdmBuffer containing the packed RDM command.
 * \return Transaction number (uint8_t).
 */
#define RDM_GET_TRANSACTION_NUM(rdmbufptr) ((rdmbufptr)->data[RDM_OFFSET_TRANSACTION])

/*! A structure that represents an RDM command message. */
typedef struct RdmCommandHeader
{
  /*! UID of the controller generating this command. */
  RdmUid source_uid;
  /*! UID of the responder to which this command is addressed. */
  RdmUid dest_uid;
  /*! Transaction number, monotonically incrementing */
  uint8_t transaction_num;
  /*! Port ID of the port on which this command is being sent. */
  uint8_t port_id;
  /*! The sub-device to which this command is addressed, or 0 for the root device. */
  uint16_t subdevice;
  /*! The command class for this command. */
  rdm_command_class_t command_class;
  /*! The RDM Parameter ID of this command. One of the values from E1.20 Table A-3, or any of the
   *  relevant extension standards. */
  uint16_t param_id;
} RdmCommandHeader;

typedef struct RdmResponseHeader
{
  /*! UID of the responder generating this response. */
  RdmUid source_uid;
  /*! UID of the controller to which this response is addressed. */
  RdmUid dest_uid;
  /*! Transaction number, copied from the corresponding command. */
  uint8_t transaction_num;
  /*! Response type, indicating the response status. */
  rdm_response_type_t resp_type;
  /*! Current count of queued messages waiting to be retrieved. */
  uint8_t msg_count;
  /*! The sub-device generating this response, or 0 for the root device. */
  uint16_t subdevice;
  /*! The command class for this response. */
  rdm_command_class_t command_class;
  /*! The RDM Parameter ID of this response. One of the values from E1.20 Table A-3, or any of the
   *  relevant extension standards. */
  uint16_t param_id;
} RdmResponseHeader;

void rdm_pack_checksum(uint8_t* buffer, size_t datalen_without_checksum);
bool rdm_validate_msg(const RdmBuffer* buffer);

etcpal_error_t rdm_create_command(const RdmCommandHeader* cmd_header, const uint8_t* cmd_data, uint8_t cmd_data_len,
                                  RdmBuffer* buffer);

etcpal_error_t rdm_create_response(const RdmCommandHeader* cmd_header, uint8_t msg_count, const uint8_t* response_data,
                                   uint8_t response_data_len, RdmBuffer* buffer);
etcpal_error_t rdm_create_overflow_response(const RdmCommandHeader* cmd_header, const uint8_t* response_data,
                                            uint8_t response_data_len, RdmBuffer* buffer);
etcpal_error_t rdm_create_nack_response(const RdmCommandHeader* cmd_header, uint8_t msg_count,
                                        rdm_nack_reason_t nack_reason, RdmBuffer* buffer);
etcpal_error_t rdm_create_timer_response(const RdmCommandHeader* cmd_header, uint8_t msg_count, uint32_t delay_time_ms,
                                         RdmBuffer* buffer);
etcpal_error_t rdm_create_dub_response(const RdmUid* responder_uid, RdmBuffer* buffer);

size_t rdm_get_num_overflow_responses_needed(uint16_t param_id, size_t response_data_len);
etcpal_error_t rdm_create_full_overflow_response(const RdmCommandHeader* cmd_header, const uint8_t* response_data,
                                                 size_t response_data_len, RdmBuffer* buffers, size_t num_buffers);

etcpal_error_t rdm_unpack_command(const RdmBuffer* buffer, RdmCommandHeader* cmd_header, const uint8_t** param_data,
                                  uint8_t* param_data_len);
etcpal_error_t rdm_unpack_response(const RdmBuffer* buffer, RdmResponseHeader* resp_header, const uint8_t** param_data,
                                   uint8_t* param_data_len);
etcpal_error_t rdm_unpack_dub_response(const RdmBuffer* buffer, RdmUid* responder_uid);

#ifdef __cplusplus
}
#endif

/*!
 * @}
 */

#endif /* RDM_MESSAGE_H_ */
