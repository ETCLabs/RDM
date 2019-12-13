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
 * \file rdm/responder.h
 * \brief Contains functions for unpacking RDM commands and creating responses.
 */

#ifndef RDM_RESPONDER_H_
#define RDM_RESPONDER_H_

#include <stdint.h>
#include "etcpal/error.h"
#include "etcpal/pack.h"
#include "rdm/message.h"

/*!
 * \defgroup responder Responder
 * \ingroup rdm
 * \brief Implementation of RDM responder functionality.
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*!
 * \brief Initialize a NACK_REASON RdmResponse to a received RdmCommand.
 *
 * Provide the received command and the NACK reason code.
 *
 * \param nack_resp Response to initialize (RdmResponse *).
 * \param cmd Received command (RdmCommand *).
 * \param nack_reason NACK Reason code to send (uint16_t).
 */
#define RDM_CREATE_NACK_FROM_COMMAND(nack_resp, cmd, nack_reason) \
  RDM_CREATE_NACK_FROM_COMMAND_WITH_MSG_COUNT(nack_resp, cmd, nack_reason, 0)

/*!
 * \brief Initialize a NACK_REASON RdmResponse indicating a nonzero message count to a received
 *        RdmCommand.
 *
 * Provide the received command, the NACK reason code and the message count.
 *
 * \param nack_resp Response to initialize (RdmResponse *).
 * \param cmd Received command (RdmCommand *).
 * \param nack_reason NACK Reason code to send (uint16_t).
 * \param msgcount Message count to send (uint8_t).
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
    (nack_resp)->datalen = 2;                                                                             \
    etcpal_pack_16b((nack_resp)->data, nack_reason);                                                      \
  } while (0)

etcpal_error_t rdmresp_unpack_command(const RdmBuffer* buffer, RdmCommand* cmd);
bool rdmresp_is_non_disc_command(const RdmBuffer* buffer);
etcpal_error_t rdmresp_pack_response(const RdmResponse* resp_data, RdmBuffer* buffer);

#ifdef __cplusplus
};
#endif

/*!
 * @}
 */

#endif /* RDM_RESPONDER_H_ */
