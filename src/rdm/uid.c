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

#include "rdm/uid.h"

#include <inttypes.h>
#include <stdio.h>

/**************************** Global constants *******************************/

const RdmUid kRdmNullUid = {0x0000u, 0x00000000u};
const RdmUid kRdmBroadcastUid = {0xffffu, 0xffffffffu};
const RdmUid kRdmnetControllerBroadcastUid = {0xfffcu, 0xffffffffu};
const RdmUid kRdmnetDeviceBroadcastUid = {0xfffdu, 0xffffffffu};

/****************************** Private macros *******************************/

#ifdef _MSC_VER
#define SPRINTF_NO_WARN __pragma(warning(suppress : 4996)) sprintf
#define SSCANF_NO_WARN __pragma(warning(suppress : 4996)) sscanf
#else
#define SPRINTF_NO_WARN sprintf
#define SSCANF_NO_WARN sscanf
#endif

/*************************** Function definitions ****************************/

/*!
 * \brief Create a string representation of a UID.
 *
 * The resulting string will be of the form `mmmm:dddddddd`, where `mmmm` is the 4-digit ESTA
 * manufacturer ID represented in hexadecimal, and `dddddddd` is the 8-digit device ID represented
 * in hexadecimal.
 *
 * \param[in] uid UID to convert to a string.
 * \param[out] buf Character buffer to which to write the resulting string. Must be at least of
 *                 size #RDM_UID_STRING_BYTES.
 * \return true (conversion successful) or false (invalid argument).
 */
bool rdm_uid_to_string(const RdmUid* uid, char* buf)
{
  if (!uid || !buf)
    return false;

  SPRINTF_NO_WARN(buf, "%04" PRIx16 ":%08" PRIx32, uid->manu, uid->id);
  return true;
}

/*!
 * \brief Create a UID from a string representation.
 *
 * There is no standard for string representations of UIDs, but two common conventions exist:
 * `mmmm:dddddddd` and `mmmmdddddddd`, where `mmmm` is the 4-digit ESTA manufacturer ID represented
 * in hexadecimal, and `dddddddd` is the 8-digit device ID represented in hexadecimal. This
 * function accepts either of these forms as input.
 *
 * \param[in] str The null-terminated string to convert.
 * \param[out] uid UID to fill in with the parse result.
 * \return true (parse successful) or false (invalid argment/parse failure).
 */
bool rdm_string_to_uid(const char* str, RdmUid* uid)
{
  if (!str || !uid)
    return false;

  if (SSCANF_NO_WARN(str, "%4" SCNx16 ":%8" SCNx32, &uid->manu, &uid->id) == 2)
    return true;
  else if (SSCANF_NO_WARN(str, "%4" SCNx16 "%8" SCNx32, &uid->manu, &uid->id) == 2)
    return true;
  else
    return false;
}
