/******************************************************************************
 * Copyright 2020 ETC Inc.
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
 ******************************************************************************
 * This file is a part of RDM. For more information, go to:
 * https://github.com/ETCLabs/RDM
 *****************************************************************************/

/**
 * @file rdm/controller.h
 * @brief Contains types and functions for implementing basic RDM controller logic.
 */

#ifndef RDM_CONTROLLER_H_
#define RDM_CONTROLLER_H_

#include <stdint.h>
#include "etcpal/error.h"
#include "rdm/message.h"
#include "rdm/uid.h"

/**
 * @defgroup controller Controller
 * @ingroup rdm_c
 * @brief Implementation of basic RDM controller functionality.
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

typedef enum
{
  kRdmDubResultInitial,
  kRdmDubResultTimeout,
  kRdmDubResultInvalidCollision,
  kRdmDubResultValidResponse,
} rdm_dub_result_t;

typedef struct RdmUidRange
{
  RdmUid lower;
  RdmUid upper;
} RdmUidRange;

typedef struct RdmControllerContext
{
  RdmUid  my_uid;
  uint8_t port_num;

  uint8_t     transaction_num;
  RdmUidRange current_range;
  int         range_stack_index;
  RdmUidRange range_stack[48];
} RdmControllerContext;

etcpal_error_t rdm_controller_init(RdmControllerContext* context, const RdmUid* controller_uid, uint8_t port_number);

void           rdm_controller_init_discovery(RdmControllerContext* context);
etcpal_error_t rdm_controller_pack_next_dub(RdmControllerContext* context,
                                            rdm_dub_result_t      last_dub_result,
                                            RdmBuffer*            buf);
etcpal_error_t rdm_controller_pack_mute(RdmControllerContext* context, const RdmUid* dest, RdmBuffer* buf);
etcpal_error_t rdm_controller_pack_unmute(RdmControllerContext* context, const RdmUid* dest, RdmBuffer* buf);
etcpal_error_t rdm_controller_pack_unmute_all(RdmControllerContext* context, RdmBuffer* buf);

etcpal_error_t rdm_controller_pack_command(RdmControllerContext* context,
                                           const RdmUid*         dest,
                                           uint16_t              subdevice,
                                           rdm_command_class_t   cc,
                                           uint16_t              param_id,
                                           const uint8_t*        param_data,
                                           uint8_t               param_data_len);

bool           rdm_controller_is_dub_response(const RdmBuffer* buffer);
etcpal_error_t rdm_controller_unpack_dub_response(const RdmBuffer* buffer, RdmUid* responder_uid);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* RDM_CONTROLLER_H_ */
