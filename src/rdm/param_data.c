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
#include "rdm/param_data.h"

// e1.33 Table 7-1: Required E1.20 Parameter IDs for the Default Responder
// Get/Set IDENTIFY_DEVICE See Section 10.11.1 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_identify_device(const RdmParamData *pd, bool *identify_state)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_identify_device(bool identify_state, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_identify_device(const RdmParamData *pd, bool *identify)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_identify_device(bool identify, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get SUPPORTED_PARAMETERS See Section 10.4.1 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_supported_parameters(const RdmParamData *pd, RdmPdSupportedParameters *params)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_supported_parameters(const RdmPdSupportedParameters *params, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get PARAMETER_DESCRIPTION See Section 10.4.2 of [RDM]
etcpal_error_t rdmpd_unpack_get_parameter_description(const RdmParamData *pd, uint16_t *requested_pid)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_parameter_description(uint16_t requested_pid, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_parameter_description(const RdmParamData *pd,
                                                           RdmPdParameterDescription *description)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_parameter_description(const RdmPdParameterDescription *description,
                                                         RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get MANUFACTURER_LABEL See Section 10.5.4 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_manufacturer_label(const RdmParamData *pd, RdmPdString *label)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_manufacturer_label(const RdmPdString *label, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get DEVICE_MODEL_DESCRIPTION See Section 10.5.3 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_device_model_description(const RdmParamData *pd, RdmPdString *description)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_device_model_description(const RdmPdString *description, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get SOFTWARE_VERSION_LABEL See Section 10.5.9 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_software_version_label(const RdmParamData *pd, RdmPdString *label)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_software_version_label(const RdmPdString *label, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set DEVICE_LABEL See Section 10.5.5 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_device_label(const RdmParamData *pd, RdmPdString *label)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_device_label(const RdmPdString *label, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_device_label(const RdmParamData *pd, RdmPdString *label)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_device_label(const RdmPdString *label, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// e1.33 Table A-15: RDM Parameter ID Defines
// Get/Set COMPONENT_SCOPE
etcpal_error_t rdmpd_unpack_get_component_scope(const RdmParamData *pd, uint16_t *scope_slot)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_component_scope(uint16_t scope_slot, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set SEARCH_DOMAIN
etcpal_error_t rdmpd_unpack_get_resp_search_domain(const RdmParamData *pd, RdmPdSearchDomain *domain_name)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_search_domain(const RdmPdSearchDomain *domain_name, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_search_domain(const RdmParamData *pd, RdmPdSearchDomain *domain_name)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_search_domain(const RdmPdSearchDomain *domain_name, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set TCP_COMMS_STATUS
etcpal_error_t rdmpd_unpack_get_resp_tcp_comms_status(const RdmParamData *pd, RdmPdTcpCommsEntry *entry)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_tcp_comms_status(const RdmPdTcpCommsEntry *entry, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_tcp_comms_status(const RdmParamData *pd, RdmPdScopeString *scope)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_tcp_comms_status(const RdmPdScopeString *scope, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set BROKER_STATUS
etcpal_error_t rdmpd_unpack_get_resp_broker_status(const RdmParamData *pd, RdmPdBrokerStatus *status)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_broker_status(const RdmPdBrokerStatus *status, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_broker_status(const RdmParamData *pd, uint8_t *broker_state)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_broker_status(uint8_t broker_state, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}


/* e1.37-7 Table A-1: RDM Parameter ID Defines */
// Get ENDPOINT_LIST
etcpal_error_t rdmpd_unpack_get_resp_endpt_list(const RdmParamData *pd, RdmPdEndpointList *list)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_list(const RdmPdEndpointList *list, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



etcpal_error_t rdmpd_unpack_get_resp_endpt_list_overflow(const RdmParamData *pd, RdmPdEndpointListOverflow *list)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_list_overflow(const RdmPdEndpointListOverflow *list, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get ENDPOINT_LIST_CHANGE
etcpal_error_t rdmpd_unpack_get_resp_endpt_list_change(const RdmParamData *pd, uint32_t *list_change_number)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_list_change(uint32_t list_change_number, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set IDENTIFY_ENDPOINT
etcpal_error_t rdmpd_unpack_get_identify_endpt(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_identify_endpt(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_identify_endpt(const RdmParamData *pd, RdmPdIdentifyEndpoint *identify_endpt)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_identify_endpt(const RdmPdIdentifyEndpoint *identify_endpt, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_identify_endpt(const RdmParamData *pd, RdmPdIdentifyEndpoint *identify_endpt)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_identify_endpt(const RdmPdIdentifyEndpoint *identify_endpt, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_resp_identify_endpt(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_resp_identify_endpt(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set ENDPOINT_TO_UNIVERSE
etcpal_error_t rdmpd_unpack_get_endpt_to_universe(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_endpt_to_universe(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_endpt_to_universe(const RdmParamData *pd,
                                                       RdmPdEndpointUniverse *endpt_universe)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_to_universe(const RdmPdEndpointUniverse *endpt_universe, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_endpt_to_universe(const RdmParamData *pd, RdmPdEndpointUniverse *endpt_universe)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_endpt_to_universe(const RdmPdEndpointUniverse *endpt_universe, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_resp_endpt_to_universe(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_resp_endpt_to_universe(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set ENDPOINT_MODE
etcpal_error_t rdmpd_unpack_get_endpt_mode(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_endpt_mode(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_endpt_mode(const RdmParamData *pd, RdmPdEndpointMode *endpt_mode)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_mode(const RdmPdEndpointMode *endpt_mode, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_endpt_mode(const RdmParamData *pd, RdmPdEndpointMode *endpt_mode)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_endpt_mode(const RdmPdEndpointMode *endpt_mode, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_resp_endpt_mode(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_resp_endpt_mode(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set ENDPOINT_LABEL
etcpal_error_t rdmpd_unpack_get_endpt_label(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_endpt_label(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_endpt_label(const RdmParamData *pd, RdmPdEndpointLabel *endpt_label)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_label(const RdmPdEndpointLabel *endpt_label, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_endpt_label(const RdmParamData *pd, RdmPdEndpointLabel *endpt_label)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_endpt_label(const RdmPdEndpointLabel *endpt_label, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_resp_endpt_label(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_resp_endpt_label(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set RDM_TRAFFIC_ENABLE
etcpal_error_t rdmpd_unpack_get_traffic_enable(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_traffic_enable(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_traffic_enable(const RdmParamData *pd, RdmPdTrafficEnable *traffic_enable)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_traffic_enable(const RdmPdTrafficEnable *traffic_enable, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_traffic_enable(const RdmParamData *pd, RdmPdTrafficEnable *traffic_enable)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_traffic_enable(const RdmPdTrafficEnable *traffic_enable, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_resp_traffic_enable(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_resp_traffic_enable(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set DISCOVERY_STATE
etcpal_error_t rdmpd_unpack_get_discovery_state(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_discovery_state(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_discovery_state(const RdmParamData *pd, RdmPdDeviceCountDiscState *state)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_discovery_state(const RdmPdDeviceCountDiscState *state, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_discovery_state(const RdmParamData *pd, RdmPdDiscoveryState *state)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_discovery_state(const RdmPdDiscoveryState *state, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_resp_discovery_state(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_resp_discovery_state(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set BACKGROUND_DISCOVERY
etcpal_error_t rdmpd_unpack_get_background_discovery(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_background_discovery(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_background_discovery(const RdmParamData *pd,
                                                          RdmPdBackgroundDiscovery *background_discovery)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_background_discovery(const RdmPdBackgroundDiscovery *background_discovery,
                                                        RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_background_discovery(const RdmParamData *pd,
                                                     RdmPdBackgroundDiscovery *background_discovery)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_background_discovery(const RdmPdBackgroundDiscovery *background_discovery,
                                                   RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_resp_background_discovery(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_resp_background_discovery(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set ENDPOINT_TIMING
etcpal_error_t rdmpd_unpack_get_endpt_timing(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_endpt_timing(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_endpt_timing(const RdmParamData *pd, RdmPdEndpointTiming *timing)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_timing(const RdmPdEndpointTiming *timing, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_endpt_timing(const RdmParamData *pd, RdmPdEndpointTimingSetting *timing)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_endpt_timing(const RdmPdEndpointTimingSetting *timing, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_resp_endpt_timing(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_resp_endpt_timing(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get ENDPOINT_TIMING_DESCRIPTION
etcpal_error_t rdmpd_unpack_get_endpt_timing_description(const RdmParamData *pd, uint8_t *setting)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_endpt_timing_description(uint8_t setting, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_endpt_timing_description(const RdmParamData *pd,
                                                              RdmPdEndptTimingDescription *description)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_timing_description(const RdmPdEndptTimingDescription *description,
                                                            RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get ENDPOINT_RESPONDERS
etcpal_error_t rdmpd_unpack_get_endpt_resp_list(const RdmParamData *pd, uint16_t *endpt_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_endpt_resp_list(uint16_t endpt_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_endpt_resp_list(const RdmParamData *pd, RdmPdEndptResponderList *list)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_resp_list(const RdmPdEndptResponderList *list, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_endpt_resp_list_overflow(const RdmParamData *pd,
                                                              RdmPdEndptResponderListOverflow *list)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_resp_list_overflow(const RdmPdEndptResponderListOverflow *list,
                                                            RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get ENDPOINT_RESPONDER_LIST_CHANGE
etcpal_error_t rdmpd_unpack_get_endpt_resp_list_change(const RdmParamData *pd, uint16_t *endpoint_id)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_endpt_resp_list_change(uint16_t endpoint_id, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_endpt_resp_list_change(const RdmParamData *pd,
                                                            RdmPdEndptRespListChange *change)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_endpt_resp_list_change(const RdmPdEndptRespListChange *change, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get BINDING_CONTROL_FIELDS
etcpal_error_t rdmpd_unpack_get_binding_control_fields(const RdmParamData *pd, RdmPdEndpointUid *endpoint_uid)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_binding_control_fields(const RdmPdEndpointUid *endpoint_uid, RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_binding_control_fields(const RdmParamData *pd,
                                                            RdmPdBindingControlFields *binding_ctrl_fields)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_binding_control_fields(const RdmPdBindingControlFields *binding_ctrl_fields,
                                                          RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get/Set BACKGROUND_QUEUED_STATUS_POLICY
etcpal_error_t rdmpd_unpack_get_resp_backgnd_qd_status_policy(const RdmParamData *pd,
                                                              RdmPdBackgroundQdStatusPolicy *policy)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_backgnd_qd_status_policy(const RdmPdBackgroundQdStatusPolicy *policy,
                                                            RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_backgnd_qd_status_policy(const RdmParamData *pd,
                                                         rdmpd_backgnd_qd_status_policy_setting_t *policy)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_backgnd_qd_status_policy(rdmpd_backgnd_qd_status_policy_setting_t policy,
                                                       RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}



// Get BACKGROUND_QUEUED_STATUS_POLICY_DESCRIPTION
etcpal_error_t rdmpd_unpack_get_bkgnd_qd_status_policy_desc(const RdmParamData *pd,
                                                            rdmpd_backgnd_qd_status_policy_setting_t *policy)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_bkgnd_qd_status_policy_desc(rdmpd_backgnd_qd_status_policy_setting_t policy,
                                                          RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_bkgnd_qd_status_policy_desc(const RdmParamData *pd,
                                                                 RdmPdBkgndQdStatusPolicyDescription *description)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_bkgnd_qd_status_policy_desc(const RdmPdBkgndQdStatusPolicyDescription *description,
                                                               RdmParamData *pd)
{
  return kEtcPalErrNotImpl;  // TODO - Not yet implemented
}
