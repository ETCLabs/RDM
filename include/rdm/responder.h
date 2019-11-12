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

/*! \brief Parameter support flag definitions.
 */
#define RDM_PS_ROOT 0x00000001
#define RDM_PS_SUBS 0x00000002
#define RDM_PS_ALL (RDM_PS_ROOT | RDM_PS_SUBS)
#define RDM_PS_SHOW_SUPPORTED 0x80000000
#define RDM_PS_GET 0x40000000
#define RDM_PS_SET 0x20000000
#define RDM_PS_GET_SET (RDM_PS_GET | RDM_PS_SET)

#ifdef __cplusplus
extern "C" {
#endif

void rdmresp_create_nack_from_command_with_msg_count(RdmResponse *nack_resp, const RdmCommand *cmd,
                                                     uint16_t nack_reason, uint8_t msgcount);
void rdmresp_create_nack_from_command(RdmResponse *nack_resp, const RdmCommand *cmd, uint16_t nack_reason);

etcpal_error_t rdmresp_unpack_command(const RdmBuffer *buffer, RdmCommand *cmd);
bool rdmresp_is_non_disc_command(const RdmBuffer *buffer);
etcpal_error_t rdmresp_pack_response(const RdmResponse *resp_data, RdmBuffer *buffer);

typedef enum
{
  kRdmRespRtAck = E120_RESPONSE_TYPE_ACK,
  kRdmRespRtAckTimer = E120_RESPONSE_TYPE_ACK_TIMER,
  kRdmRespRtNackReason = E120_RESPONSE_TYPE_NACK_REASON,
  kRdmRespRtAckOverflow = E120_RESPONSE_TYPE_ACK_OVERFLOW,
  kRdmRespRtNoSend
} rdmresp_response_type_t;

typedef enum
{
  kRdmRespTypeRdm,
  kRdmRespTypeController,
  kRdmRespTypeBroker,
  kRdmRespTypeDevice
} rdmresp_type_t;

typedef struct PidHandlerData
{
  uint8_t port;
  rdm_command_class_t cmd_class;
  uint8_t sub_device;
  size_t overflow_index;
  void *context;

  const RdmParamData *pd_in;
  RdmParamData *pd_out;
  rdmresp_response_type_t response_type;
} PidHandlerData;

typedef struct RdmPidHandlerEntry
{
  uint16_t pid;
  etcpal_error_t (*handler)(PidHandlerData *data);
  uint32_t flags;
} RdmPidHandlerEntry;

typedef enum
{
  kRdmRespStatusNone = E120_STATUS_NONE,
  kRdmRespStatusGetLastMessage = E120_STATUS_GET_LAST_MESSAGE,
  kRdmRespStatusAdvisory = E120_STATUS_ADVISORY,
  kRdmRespStatusWarning = E120_STATUS_WARNING,
  kRdmRespStatusError = E120_STATUS_ERROR,
  kRdmRespStatusAdvisoryCleared = E120_STATUS_ADVISORY_CLEARED,
  kRdmRespStatusWarningCleared = E120_STATUS_WARNING_CLEARED,
  kRdmRespStatusErrorCleared = E120_STATUS_ERROR_CLEARED
} rdmresp_status_t;

typedef struct GetNextQueuedMessageData
{
  uint8_t port;
  rdmresp_status_t status_type;
  rdm_response_type_t *response_type;
  uint16_t *sub_device;
  rdm_command_class_t *cmd_class;
  uint16_t *param_id;
  RdmParamData *pd;
} GetNextQueuedMessageData;

typedef struct RdmResponderState
{
  uint8_t port_number;
  RdmUid uid;
  uint8_t number_of_subdevices;
  rdmresp_type_t responder_type;
  void *callback_context;

  const RdmPidHandlerEntry *handler_array;
  size_t handler_array_size;

  uint8_t (*get_message_count)();
  void (*get_next_queued_message)(GetNextQueuedMessageData *data);

} RdmResponderState;

bool rdmresp_validate_pid_handler_data(const PidHandlerData *data, bool check_context);

void rdmresp_sort_handler_array(RdmPidHandlerEntry *handler_array, size_t handler_array_size);
bool rdmresp_validate_state(const RdmResponderState *state);

typedef enum
{
  kRespValid,
  kRespInvalidRdm,
  kRespNotRdm
} rdmresp_validate_result_t;

etcpal_error_t rdmresp_validate_packet(RdmBufferConstRef buffer, uint8_t calc_checksum,
                                       rdmresp_validate_result_t *validate_result);

// CONTRACT NOTES: If you get an error, we packed a NACK in buffer_out for you and we recommend sending that.
etcpal_error_t rdmresp_process_packet(const RdmResponderState *state, RdmBufferConstRef buffer_in,
                                      RdmBufferRef *buffer_out, rdmresp_response_type_t *response_type, bool *no_break);

etcpal_error_t rdmresp_process_packet_shared_buffer(const RdmResponderState *state, RdmBufferRef *buffer_in_out,
                                                    rdmresp_response_type_t *response_type, bool *no_break);

etcpal_error_t rdmresp_process_command(const RdmResponderState *state, const RdmCommand *cmd, RdmResponse *resp,
                                       rdmresp_response_type_t *response_type);

etcpal_error_t rdmresp_process_command_with_discovery(const RdmResponderState *state, const RdmCommand *cmd,
                                                      RdmBufferRef *buffer_out, rdmresp_response_type_t *response_type,
                                                      bool *no_break);

#ifdef __cplusplus
};
#endif

/*!@}*/

#endif /* _RDM_RESPONDER_H_ */
