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

/*! \file rdm/param_data.h
 *  \brief Structures and packing/unpacking functions for the parameter data of standard PIDs.
 *  \author Christian Reese*/
#ifndef _RDM_PARAM_DATA_H_
#define _RDM_PARAM_DATA_H_

#include "etcpal/bool.h"
#include "etcpal/error.h"
#include "etcpal/inet.h"
#include "rdm/message.h"

//e1.33 Table 7-1: Required E1.20 Parameter IDs for the Default Responder
//Get/Set IDENTIFY_DEVICE See Section 10.11.1 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_identify_device(const RdmParamData *pd, bool *identify_state_out);
etcpal_error_t rdmpd_pack_get_resp_identify_device(bool identify_state, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_identify_device(const RdmParamData *pd, bool *identify_out);
etcpal_error_t rdmpd_pack_set_identify_device(bool identify, RdmParamData *pd_out);

//Get SUPPORTED_PARAMETERS See Section 10.4.1 of [RDM]
/*! The maximum number of 16-bit parameter IDs in the Get Supported Parameters Response PD. */
#define RDMPD_MAX_SUPPORTED_PARAMETERS 115

/*! A structure representing supported parameters. */
typedef struct RdmPdSupportedParameters
{
  uint16_t parameters[RDMPD_MAX_SUPPORTED_PARAMETERS]; /*!< The array of supported PIDs. */
  size_t length; /*!< The number of supported PIDs in the array. */
} RdmPdSupportedParameters;

etcpal_error_t rdmpd_unpack_get_resp_supported_parameters(const RdmParamData *pd,
                                                          RdmPdSupportedParameters *params_out);
etcpal_error_t rdmpd_pack_get_resp_supported_parameters(const RdmPdSupportedParameters *params, RdmParamData *pd_out);

//Get PARAMETER_DESCRIPTION See Section 10.4.2 of [RDM]
etcpal_error_t rdmpd_unpack_get_parameter_description(const RdmParamData *pd, uint16_t *requested_pid_out);
etcpal_error_t rdmpd_pack_get_parameter_description(uint16_t requested_pid, RdmParamData *pd_out);

/*! The maximum length of a parameter description's description string. */
#define RDMPD_MAX_PARAMETER_DESCRIPTION_STR_LEN 32

/*! A structure representing a parameter description. */
typedef struct RdmPdParameterDescription
{
  uint16_t pid_number_requested; /*!< The manufacturer specific PID requested by the controller. */
  uint8_t pdl_size; /*!< The number used for the PDL field in all GET_RESPONSE and SET messages associated with this PID. */
  uint8_t data_type; /*!< The size of the data entries in the PD of the message for this PID. */
  uint8_t command_class; /*!< Whether Get and or Set messages are implemented for the specified PID. */
  uint8_t type; /*!< No longer has any meaning, should be filled with 0x00. */
  uint8_t unit; /*!< The SI (International System of units) unit of the specified PID data. */
  uint8_t prefix; /*!< The SI Prefix and multiplication factor of the units. */
  uint32_t min_valid_value; /*!< The lowest value the data can reach. */
  uint32_t max_valid_value; /*!< The highest value the data can reach. */
  uint32_t default_value; /*!< The default value of the data. */
  char description[RDMPD_MAX_PARAMETER_DESCRIPTION_STR_LEN]; /*!< Describes the function of the specified PID. */
  size_t description_length; /*!< Length of the description string. */
} RdmPdParameterDescription;

etcpal_error_t rdmpd_unpack_get_resp_parameter_description(const RdmParamData *pd,
                                                           RdmPdParameterDescription *description_out);
etcpal_error_t rdmpd_pack_get_resp_parameter_description(const RdmPdParameterDescription *description,
                                                         RdmParamData *pd_out);

//Get MANUFACTURER_LABEL See Section 10.5.4 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_manufacturer_label(const RdmParamData *pd, char *label_out, size_t *label_len_out);
etcpal_error_t rdmpd_pack_get_resp_manufacturer_label(const char *label, size_t label_len, RdmParamData *pd_out);

//Get DEVICE_MODEL_DESCRIPTION See Section 10.5.3 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_device_model_description(const RdmParamData *pd, char *description_out,
                                                              size_t *description_len_out);
etcpal_error_t rdmpd_pack_get_resp_device_model_description(const char *description, size_t description_len,
                                                            RdmParamData *pd_out);

//Get SOFTWARE_VERSION_LABEL See Section 10.5.9 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_software_version_label(const RdmParamData *pd, char *label_out,
                                                            size_t *label_len_out);
etcpal_error_t rdmpd_pack_get_resp_software_version_label(const char *label, size_t label_len, RdmParamData *pd_out);

//Get/Set DEVICE_LABEL See Section 10.5.5 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_device_label(const RdmParamData *pd, char *label_out, size_t *label_len_out);
etcpal_error_t rdmpd_pack_get_resp_device_label(const char *label, size_t label_len, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_device_label(const RdmParamData *pd, char *label_out, size_t *label_len_out);
etcpal_error_t rdmpd_pack_set_device_label(const char *label, size_t label_len, RdmParamData *pd_out);

//e1.33 Table A-15: RDM Parameter ID Defines
//Get/Set COMPONENT_SCOPE
etcpal_error_t rdmpd_unpack_get_component_scope(const RdmParamData *pd, uint16_t *scope_slot_out);
etcpal_error_t rdmpd_pack_get_component_scope(uint16_t scope_slot, RdmParamData *pd_out);

#define RDMPD_MAX_SCOPE_STR_LEN 63

typedef struct RdmPdComponentScope
{
  uint16_t scope_slot;
  char scope_string[RDMPD_MAX_SCOPE_STR_LEN];
  size_t scope_string_length;
  EtcPalSockaddr static_broker_saddr;
} RdmPdComponentScope;

etcpal_error_t rdmpd_unpack_get_resp_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope_out);
etcpal_error_t rdmpd_pack_get_resp_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope_out);
etcpal_error_t rdmpd_pack_set_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd_out);

//Get/Set SEARCH_DOMAIN
etcpal_error_t rdmpd_unpack_get_resp_search_domain(const RdmParamData *pd, char *domain_name_out,
                                                   size_t *domain_name_len_out);
etcpal_error_t rdmpd_pack_get_resp_search_domain(const char *domain_name, size_t domain_name_len, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_search_domain(const RdmParamData *pd, char *domain_name_out,
                                              size_t *domain_name_len_out);
etcpal_error_t rdmpd_pack_set_search_domain(const char *domain_name, size_t domain_name_len, RdmParamData *pd_out);

//Get/Set TCP_COMMS_STATUS
typedef struct RdmPdTcpCommsEntry
{
  char scope_string[RDMPD_MAX_SCOPE_STR_LEN];
  size_t scope_string_length;
  EtcPalSockaddr broker_saddr;
  uint16_t unhealthy_tcp_events;
} RdmPdTcpCommsEntry;

etcpal_error_t rdmpd_unpack_get_resp_tcp_comms_status(const RdmParamData *pd, RdmPdTcpCommsEntry *entry_out);
etcpal_error_t rdmpd_pack_get_resp_tcp_comms_status(const RdmPdTcpCommsEntry *entry, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_tcp_comms_status(const RdmParamData *pd, char *scope_out, size_t *scope_len_out);
etcpal_error_t rdmpd_pack_set_tcp_comms_status(const char *scope, size_t scope_len, RdmParamData *pd_out);

//Get/Set BROKER_STATUS
etcpal_error_t rdmpd_unpack_get_resp_broker_status(const RdmParamData *pd, bool *set_allowed_out,
                                                   uint8_t *broker_state_out);
etcpal_error_t rdmpd_pack_get_resp_broker_status(bool set_allowed, uint8_t broker_state, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_broker_status(const RdmParamData *pd, RdmPdComponentScope *scope_out,
                                              uint8_t *broker_state_out);
etcpal_error_t rdmpd_pack_set_broker_status(uint8_t broker_state, RdmParamData *pd_out);

//e1.37-7 Table A-1: RDM Parameter ID Defines
//Get ENDPOINT_LIST
//Get ENDPOINT_LIST_CHANGE
//Get/Set IDENTIFY_ENDPOINT
//Get/Set ENDPOINT_TO_UNIVERSE
//Get/Set ENDPOINT_MODE
//Get/Set ENDPOINT_LABEL
//Get/Set RDM_TRAFFIC_ENABLE
//Get/Set DISCOVERY_STATE
//Get/Set BACKGROUND_DISCOVERY
//Get/Set ENDPOINT_TIMING
//Get ENDPOINT_TIMING_DESCRIPTION
//Get ENDPOINT_RESPONDERS
//Get ENDPOINT_RESPONDER_LIST_CHANGE
//Get BINDING_CONTROL_FIELDS
//Get/Set BACKGROUND_QUEUED_STATUS_POLICY
//Get BACKGROUND_QUEUED_STATUS_POLICY_DESCRIPTION

#endif /* _RDM_PARAM_DATA_H_ */
