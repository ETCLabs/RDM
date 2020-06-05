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
 * This file is a part of RDMnet. For more information, go to:
 * https://github.com/ETCLabs/RDMnet
 ******************************************************************************/

#include "rdm/responder.h"

#include <stdint.h>
#include <string.h>
#include "etcpal/pack.h"
#include "rdm/defs.h"

/***************************** Private macros ********************************/

// clang-format off
#define RDMRESP_RESP_TYPE_VALID(resptypeval)        \
  ((resptypeval) == kRdmResponseTypeAck          || \
   (resptypeval) == kRdmResponseTypeAckTimer     || \
   (resptypeval) == kRdmResponseTypeNackReason   || \
   (resptypeval) == kRdmResponseTypeAckOverflow)

#define RDMRESP_COMMAND_CLASS_VALID(ccval)      \
  ((ccval) == kRdmCCDiscoveryCommandResponse || \
   (ccval) == kRdmCCGetCommandResponse       || \
   (ccval) == kRdmCCSetCommandResponse)
// clang-format on

/*********************** Private function prototypes *************************/

/*************************** Function definitions ****************************/

/**
 * @brief Determine whether a packed RDM message is a non-discovery RDM command.
 *
 * More specifically, whether the command class of the response is one of GET_COMMAND or
 * SET_COMMAND.
 *
 * @param[in] buffer The packed RDM message.
 * @return true (the message is a valid non-discovery RDM command) or false (the message is invalid
 *         RDM or not a non-discovery command).
 */
bool rdmresp_is_non_disc_command(const RdmBuffer* buffer)
{
  if (buffer && rdm_validate_msg(buffer))
  {
    return (buffer->data[RDM_OFFSET_COMMAND_CLASS] == E120_GET_COMMAND ||
            buffer->data[RDM_OFFSET_COMMAND_CLASS] == E120_SET_COMMAND);
  }
  return false;
}
