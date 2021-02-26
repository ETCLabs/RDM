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
#include "rdm/private/util.h"

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
 * @brief Serialize an RDM DISC_UNIQUE_BRANCH response.
 *
 * The DISC_UNIQUE_BRANCH response does not adhere to the normal RDM packet format and is sent
 * without a break. The only information encoded is this responder's UID.
 *
 * @param[in] responder_uid UID of the responder responding to the DISC_UNIQUE_BRANCH command.
 * @param[out] buffer Data buffer filled in with serialized response.
 * @return #kEtcPalErrOk: Response packed successfully.
 * @return #kEtcPalErrInvalid: Invalid argument.
 */
etcpal_error_t rdm_responder_pack_dub_response(const RdmUid* responder_uid, RdmBuffer* buffer)
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
  uint16_t checksum = calc_rdm_checksum(&buffer->data[8], 12);
  *cur_ptr++ = ((uint8_t)(checksum >> 8) | 0xaa);
  *cur_ptr++ = ((uint8_t)(checksum >> 8) | 0x55);
  *cur_ptr++ = ((uint8_t)(checksum) | 0xaa);
  *cur_ptr++ = ((uint8_t)(checksum) | 0x55);

  buffer->data_len = 24;
  return kEtcPalErrOk;
}
