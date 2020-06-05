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
 * @brief Contains functions for creating RDM commands and unpacking responses.
 */

#ifndef RDM_CONTROLLER_H_
#define RDM_CONTROLLER_H_

#include "etcpal/error.h"
#include "rdm/message.h"

/**
 * @defgroup controller Controller
 * @ingroup rdm_c
 * @brief Implementation of RDM controller functionality.
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

bool rdmctl_is_non_disc_response(const RdmBuffer* buffer);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* RDM_CONTROLLER_H_ */
