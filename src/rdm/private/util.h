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

/*
 * Utilities used by multiple RDM code modules.
 */

#ifndef RDM_PRIVATE_UTIL_H_
#define RDM_PRIVATE_UTIL_H_

#include <stddef.h>
#include <stdint.h>

uint16_t calc_rdm_checksum(const uint8_t* buffer, size_t data_len_without_checksum);

#endif /* RDM_PRIVATE_UTIL_H_ */
