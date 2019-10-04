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

/*! \file rdm/responder.h
 *  \brief Contains functions for unpacking RDM commands and creating responses.
 *  \author Sam Kearney */
#ifndef _RDM_RESPONDER_H_
#define _RDM_RESPONDER_H_

#include "etcpal/int.h"
#include "etcpal/error.h"
#include "etcpal/pack.h"
#include "rdm/message.h"

/*! \defgroup responder Responder
 *  \ingroup rdm
 *  \brief Implementation of RDM responder functionality.
 *
 *  @{
 */

/*! \brief Initialize a NACK_REASON RdmResponse to a received RdmCommand.
 *
 *  Provide the received command and the NACK reason code.
 *
 *  \param nack_resp Response to initialize (RdmResponse *).
 *  \param cmd Received command (RdmCommand *).
 *  \param nack_reason NACK Reason code to send (uint16_t).
 */
#define RDM_CREATE_NACK_FROM_COMMAND(nack_resp, cmd, nack_reason) \
  RDM_CREATE_NACK_FROM_COMMAND_WITH_MSG_COUNT(nack_resp, cmd, nack_reason, 0)

/*! \brief Initialize a NACK_REASON RdmResponse indicating a nonzero message count to a received
 *         RdmCommand.
 *
 *  Provide the received command, the NACK reason code and the message count.
 *
 *  \param nack_resp Response to initialize (RdmResponse *).
 *  \param cmd Received command (RdmCommand *).
 *  \param nack_reason NACK Reason code to send (uint16_t).
 *  \param msgcount Message count to send (uint8_t).
 */
#define RDM_CREATE_NACK_FROM_COMMAND_WITH_MSG_COUNT(nack_resp, cmd, nack_reason, msgcount)                \
  do                                                                                                      \
  {                                                                                                       \
    (nack_resp)->source_uid = (cmd)->dest_uid;                                                            \
    (nack_resp)->dest_uid = (cmd)->source_uid;                                                            \
    (nack_resp)->transaction_num = (cmd)->transaction_num;                                                \
    (nack_resp)->resp_type = kRdmResponseTypeNackReason;                                                  \
    (nack_resp)->msg_count = msgcount;                                                                    \
    (nack_resp)->subdevice = (cmd)->subdevice;                                                            \
    (nack_resp)->command_class =                                                                          \
        ((cmd)->command_class == kRdmCCSetCommand ? kRdmCCSetCommandResponse : kRdmCCGetCommandResponse); \
    (nack_resp)->param_id = (cmd)->param_id;                                                              \
    (nack_resp)->parameter_data.datalen = 2;                                                                             \
    etcpal_pack_16b((nack_resp)->parameter_data.data, nack_reason);                                                        \
  } while (0)

/*! \brief Parameter support flag definitions.
 */
#define RDM_PS_ROOT 0x00000001
#define RDM_PS_SUBS 0x00000002
#define RDM_PS_ALL (RDM_PS_ROOT | RDM_PS_SUBS)
#define RDM_PS_SHOWSUP 0x80000000
#define RDM_PS_GET 0x40000000
#define RDM_PS_SET 0x20000000
#define RDM_PS_GET_SET (RDM_PS_GET | RDM_PS_SET)

#ifdef __cplusplus
extern "C" {
#endif

etcpal_error_t rdmresp_unpack_command(const RdmBuffer *buffer, RdmCommand *cmd);
bool rdmresp_is_non_disc_command(const RdmBuffer *buffer);
etcpal_error_t rdmresp_pack_response(const RdmResponse *resp_data, RdmBuffer *buffer);

typedef enum
{
  kRespAck = E120_RESPONSE_TYPE_ACK,
  kRespAckTimer = E120_RESPONSE_TYPE_ACK_TIMER,
  kRespNackReason = E120_RESPONSE_TYPE_NACK_REASON,
  kRespAckOverflow = E120_RESPONSE_TYPE_ACK_OVERFLOW,
  kRespNoSend
} resp_process_result_t;

typedef enum
{
  kRespTypeRdm,
  kRespTypeController,
  kRespTypeBroker,
  kRespTypeDevice
} resp_type_t;

typedef struct PidHandlerData
{
  uint8_t port;
  uint8_t cmd_class;
  uint8_t sub_device;
  size_t overflow_index;
  void *context;

  const RdmParamData *pd_in;
  RdmParamData *pd_out;
} PidHandlerData;

typedef struct RdmPidHandlerEntry
{
  uint16_t pid;
  resp_process_result_t (*handler)(PidHandlerData *data);
  uint32_t flags;
} RdmPidHandlerEntry;

typedef struct GetNextQueuedMessageData
{
  uint8_t port;
  uint8_t status_type;
  uint8_t *response_type;
  uint16_t *sub_device;
  uint8_t *cmd_class;
  uint16_t *param_id;
  RdmParamData *pd;
} GetNextQueuedMessageData;

typedef struct RdmResponderState
{
  uint8_t port_number;
  RdmUid uid;
  uint8_t number_of_subdevices;
  resp_type_t responder_type;
  void *callback_context;

  const RdmPidHandlerEntry *handler_array;
  size_t handler_array_size;

  uint8_t (*get_message_count)();
  void (*get_next_queued_message)(GetNextQueuedMessageData *data);

} RdmResponderState;

void rdmresp_sort_handler_array(RdmPidHandlerEntry *handler_array, size_t handler_array_size);
bool rdmresp_validate_state(const RdmResponderState *state);

typedef enum
{
  kRespValid,
  kRespInvalidRdm,
  kRespNotRdm
} resp_valid_result_t;

resp_valid_result_t rdmresp_validate_packet(RdmBufferConstRef buffer, uint8_t calc_checksum);

resp_process_result_t rdmresp_process_packet(const RdmResponderState *state, RdmBufferConstRef buffer_in,
                                             RdmBufferRef *buffer_out, bool *no_break);

resp_process_result_t rdmresp_process_packet_shared_buffer(const RdmResponderState *state, RdmBufferRef *buffer_in_out,
                                                           bool *no_break);

resp_process_result_t rdmresp_process_command(const RdmResponderState *state, const RdmCommand *cmd, RdmResponse *resp);

resp_process_result_t rdmresp_process_command_with_discovery(const RdmResponderState *state, const RdmCommand *cmd,
                                                             RdmBufferRef *buffer_out, bool *no_break);

#ifdef __cplusplus
};
#endif

/*!@}*/

#endif /* _RDM_RESPONDER_H_ */
