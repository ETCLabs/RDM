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

/// \file rdm/cpp/message.h
/// \brief RDM C++ message type definitions

#ifndef RDM_CPP_MESSAGE_H_
#define RDM_CPP_MESSAGE_H_

// This monolithic header includes all of the individual message types. This one is used from the
// other API headers.

/// \defgroup cpp_message Message
/// \ingroup rdm_cpp
/// \brief C++ classes representing RDM messages.

#include "rdm/cpp/message_types/command_header.h"
#include "rdm/cpp/message_types/command.h"
#include "rdm/cpp/message_types/nack_reason.h"
#include "rdm/cpp/message_types/response_header.h"
#include "rdm/cpp/message_types/response.h"

#endif  // RDM_CPP_MESSAGE_H_
