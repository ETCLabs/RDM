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

#include "rdm/private/util.h"

/* Internal function to calculate the correct checksum of an RDM packet. */
uint16_t calc_rdm_checksum(const uint8_t* buffer, size_t data_len_without_checksum)
{
  size_t   i;
  uint16_t sum = 0;
  for (i = 0; i < data_len_without_checksum; ++i)
    sum += buffer[i];
  return sum;
}
