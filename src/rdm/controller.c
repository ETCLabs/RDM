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

#include "rdm/controller.h"

#include <string.h>
#include "etcpal/common.h"
#include "etcpal/pack.h"
#include "rdm/defs.h"
#include "rdm/private/util.h"

/***************************** Private macros ********************************/

#define RDMCMD_PORT_ID_VALID(portidval) ((portidval) > 0)
// clang-format off
#define RDMCMD_COMMAND_CLASS_VALID(ccval)    \
  ((ccval) == kRdmCCDiscoveryCommand      || \
   (ccval) == kRdmCCGetCommand            || \
   (ccval) == kRdmCCSetCommand)
// clang-format on

/*********************** Private function prototypes *************************/

static uint16_t get_u16_from_dub_response(const uint8_t* buffer);
static uint32_t get_u32_from_dub_response(const uint8_t* buffer);

/*************************** Function definitions ****************************/

etcpal_error_t rdm_controller_init(RdmControllerContext* context, const RdmUid* controller_uid, uint8_t port_number)
{
  ETCPAL_UNUSED_ARG(context);
  ETCPAL_UNUSED_ARG(controller_uid);
  ETCPAL_UNUSED_ARG(port_number);
  return kEtcPalErrNotImpl;
}

void rdm_controller_init_discovery(RdmControllerContext* context)
{
  ETCPAL_UNUSED_ARG(context);
}

etcpal_error_t rdm_controller_get_next_dub(rdm_dub_result_t      last_dub_result,
                                           RdmControllerContext* context,
                                           RdmBuffer*            next_dub)
{
  ETCPAL_UNUSED_ARG(last_dub_result);
  ETCPAL_UNUSED_ARG(context);
  ETCPAL_UNUSED_ARG(next_dub);
  return kEtcPalErrNotImpl;
}

/**
 * @brief Deserialize an RDM DISC_UNIQUE_BRANCH from its wire format.
 *
 * DISC_UNIQUE_BRANCH (aka DUB) responses have a special format. The only information encoded is
 * the responder's UID.
 *
 * @param[in] buffer Buffer holding serialized RDM DISC_UNIQUE_BRANCH response data.
 * @param[out] responder_uid Filled in with the RDM responder's UID.
 * @return #kEtcPalErrOk: DISC_UNIQUE_BRANCH response unpacked successfully.
 * @return #kEtcPalErrInvalid: Invalid argument provided.
 * @return #kEtcPalErrProtocol: RDM protocol error in packed response.
 */
etcpal_error_t rdm_controller_unpack_dub_response(const RdmBuffer* buffer, RdmUid* responder_uid)
{
  if (!buffer || !responder_uid)
    return kEtcPalErrInvalid;
  if (buffer->data_len < 17)  // 17 bytes = encoded UID (16 bytes) + preamble separator (1 byte)
    return kEtcPalErrProtocol;

  const uint8_t* cur_ptr = buffer->data;
  // Strip any preamble bytes
  while (*cur_ptr == 0xfe && cur_ptr - buffer->data < 7)
    ++cur_ptr;
  if (*cur_ptr != 0xaa)
    return kEtcPalErrProtocol;

  ++cur_ptr;

  // Validate the checksum
  uint16_t       expected_sum = calc_rdm_checksum(cur_ptr, 12);
  const uint8_t* checksum_offset = cur_ptr + 12;
  if (expected_sum != get_u16_from_dub_response(checksum_offset))
    return kEtcPalErrProtocol;

  // Fill in the responder UID
  responder_uid->manu = get_u16_from_dub_response(cur_ptr);
  responder_uid->id = get_u32_from_dub_response(cur_ptr + 4);

  return kEtcPalErrOk;
}

uint16_t get_u16_from_dub_response(const uint8_t* buffer)
{
  uint8_t condensed_buf[2];
  condensed_buf[0] = buffer[0] & buffer[1];
  condensed_buf[1] = buffer[2] & buffer[3];
  return etcpal_unpack_u16b(condensed_buf);
}

uint32_t get_u32_from_dub_response(const uint8_t* buffer)
{
  uint8_t condensed_buf[4];
  condensed_buf[0] = buffer[0] & buffer[1];
  condensed_buf[1] = buffer[2] & buffer[3];
  condensed_buf[2] = buffer[4] & buffer[5];
  condensed_buf[3] = buffer[6] & buffer[7];
  return etcpal_unpack_u32b(condensed_buf);
}
