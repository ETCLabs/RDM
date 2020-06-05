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
#include "etcpal/pack.h"
#include "rdm/defs.h"

/***************************** Private macros ********************************/

#define RDMCMD_PORT_ID_VALID(portidval) ((portidval) > 0)
// clang-format off
#define RDMCMD_COMMAND_CLASS_VALID(ccval)    \
  ((ccval) == kRdmCCDiscoveryCommand      || \
   (ccval) == kRdmCCGetCommand            || \
   (ccval) == kRdmCCSetCommand)
// clang-format on

/*********************** Private function prototypes *************************/

/*************************** Function definitions ****************************/
