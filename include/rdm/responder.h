/******************************************************************************
* Copyright 2018 ETC Inc.
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

/*! \file rdm/responder.h
 *  \brief Contains functions for unpacking RDM commands and creating responses.
 *  \author Sam Kearney */
#ifndef _RDM_RESPONDER_H_
#define _RDM_RESPONDER_H_

#include "lwpa_int.h"
#include "lwpa_error.h"
#include "rdm/message.h"

/*! \addtogroup rdm
 *  @{
 */

#ifdef __cplusplus
extern "C" {
#endif

lwpa_error_t rdmresp_unpack_command(const RdmBuffer *buffer, RdmCommand *cmd);
lwpa_error_t rdmresp_create_response(const RdmResponse *resp_data, RdmBuffer *buffer);

#ifdef __cplusplus
};
#endif

/*!@}*/

#endif /* _RDM_RESPONDER_H_ */
