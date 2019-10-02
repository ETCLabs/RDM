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

/*! The maximum length of a typical string in the parameter data, including one extra byte for a null terminator. */
#define RDMPD_STRING_MAX_LENGTH 33

/*! A structure representing a typical string in the parameter data. */
typedef struct RdmPdString
{
  char string[RDMPD_STRING_MAX_LENGTH]; /*!< The string data. */
} RdmPdString;

/* e1.33 Table 7-1: Required E1.20 Parameter IDs for the Default Responder */
// Get/Set IDENTIFY_DEVICE See Section 10.11.1 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_identify_device(const RdmParamData *pd, bool *identify_state_out);
etcpal_error_t rdmpd_pack_get_resp_identify_device(bool identify_state, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_identify_device(const RdmParamData *pd, bool *identify_out);
etcpal_error_t rdmpd_pack_set_identify_device(bool identify, RdmParamData *pd_out);

// Get SUPPORTED_PARAMETERS See Section 10.4.1 of [RDM]
/*! The maximum number of 16-bit parameter IDs in the Get Supported Parameters Response PD. */
#define RDMPD_MAX_SUPPORTED_PARAMETERS 115

/*! A structure representing supported parameters. */
typedef struct RdmPdSupportedParameters
{
  uint16_t parameters[RDMPD_MAX_SUPPORTED_PARAMETERS]; /*!< The array of supported PIDs. */
  size_t length;                                       /*!< The number of supported PIDs in the array. */
} RdmPdSupportedParameters;

etcpal_error_t rdmpd_unpack_get_resp_supported_parameters(const RdmParamData *pd, RdmPdSupportedParameters *params_out);
etcpal_error_t rdmpd_pack_get_resp_supported_parameters(const RdmPdSupportedParameters *params, RdmParamData *pd_out);

// Get PARAMETER_DESCRIPTION See Section 10.4.2 of [RDM]
etcpal_error_t rdmpd_unpack_get_parameter_description(const RdmParamData *pd, uint16_t *requested_pid_out);
etcpal_error_t rdmpd_pack_get_parameter_description(uint16_t requested_pid, RdmParamData *pd_out);

/*! Data type enumeration */
typedef enum
{
  kRdmPdDtNotDefined = 0x00,
  kRdmPdDtBitField = 0x01,
  kRdmPdDtAscii = 0x02,
  kRdmPdDtUnsignedByte = 0x03,
  kRdmPdDtSignedByte = 0x04,
  kRdmPdDtUnsignedWord = 0x05,
  kRdmPdDtSignedWord = 0x06,
  kRdmPdDtUnsignedDword = 0x07,
  kRdmPdDtSignedDword = 0x08
} rdmpd_data_type_t;

/*! Command class enumeration (get, set, or both) */
typedef enum
{
  kRdmPdCcGet = 0x01,
  kRdmPdCcSet = 0x02,
  kRdmPdCcGetSet = 0x03
} rdmpd_param_desc_cc_t;

/*! A structure representing a parameter description. */
typedef struct RdmPdParameterDescription
{
  uint16_t pid_number_requested; /*!< The manufacturer specific PID requested by the controller. */
  uint8_t
      pdl_size; /*!< The number used for the PDL field in all GET_RESPONSE and SET messages associated with this PID. */
  rdmpd_data_type_t data_type;         /*!< The size of the data entries in the PD of the message for this PID. */
  rdmpd_param_desc_cc_t command_class; /*!< Whether Get and or Set messages are implemented for the specified PID. */
  uint8_t unit;                        /*!< The SI (International System of units) unit of the specified PID data. */
  uint8_t prefix;                      /*!< The SI Prefix and multiplication factor of the units. */
  uint32_t min_valid_value;            /*!< The lowest value the data can reach. */
  uint32_t max_valid_value;            /*!< The highest value the data can reach. */
  uint32_t default_value;              /*!< The default value of the data. */
  RdmPdString description;             /*!< Describes the function of the specified PID. */
} RdmPdParameterDescription;

etcpal_error_t rdmpd_unpack_get_resp_parameter_description(const RdmParamData *pd,
                                                           RdmPdParameterDescription *description_out);
etcpal_error_t rdmpd_pack_get_resp_parameter_description(const RdmPdParameterDescription *description,
                                                         RdmParamData *pd_out);

// Get MANUFACTURER_LABEL See Section 10.5.4 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_manufacturer_label(const RdmParamData *pd, RdmPdString *label_out);
etcpal_error_t rdmpd_pack_get_resp_manufacturer_label(const RdmPdString *label, RdmParamData *pd_out);

// Get DEVICE_MODEL_DESCRIPTION See Section 10.5.3 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_device_model_description(const RdmParamData *pd, RdmPdString *description_out);
etcpal_error_t rdmpd_pack_get_resp_device_model_description(const RdmPdString *description, RdmParamData *pd_out);

// Get SOFTWARE_VERSION_LABEL See Section 10.5.9 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_software_version_label(const RdmParamData *pd, RdmPdString *label_out);
etcpal_error_t rdmpd_pack_get_resp_software_version_label(const RdmPdString *label, RdmParamData *pd_out);

// Get/Set DEVICE_LABEL See Section 10.5.5 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_device_label(const RdmParamData *pd, RdmPdString *label_out);
etcpal_error_t rdmpd_pack_get_resp_device_label(const RdmPdString *label, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_device_label(const RdmParamData *pd, RdmPdString *label_out);
etcpal_error_t rdmpd_pack_set_device_label(const RdmPdString *label, RdmParamData *pd_out);

/* e1.33 Table A-15: RDM Parameter ID Defines */
// Get/Set COMPONENT_SCOPE
etcpal_error_t rdmpd_unpack_get_component_scope(const RdmParamData *pd, uint16_t *scope_slot_out);
etcpal_error_t rdmpd_pack_get_component_scope(uint16_t scope_slot, RdmParamData *pd_out);

/*! The maximum length of a scope string, including one extra byte for a null terminator. */
#define RDMPD_MAX_SCOPE_STR_LEN 64

/*! A structure representing a scope string. */
typedef struct RdmPdScopeString
{
  char string[RDMPD_MAX_SCOPE_STR_LEN]; /*!< The string data. */
} RdmPdScopeString;

typedef struct RdmPdComponentScope
{
  uint16_t scope_slot;
  RdmPdScopeString scope_string;
  EtcPalSockaddr static_broker_saddr;
} RdmPdComponentScope;

etcpal_error_t rdmpd_unpack_get_resp_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope_out);
etcpal_error_t rdmpd_pack_get_resp_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope_out);
etcpal_error_t rdmpd_pack_set_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd_out);

// Get/Set SEARCH_DOMAIN
/*! The maximum length of a search domain string, including one extra byte for a null terminator. */
#define RDMPD_MAX_SEARCH_DOMAIN_STR_LEN 232

/*! A structure representing a search domain string. */
typedef struct RdmPdSearchDomain
{
  char string[RDMPD_MAX_SEARCH_DOMAIN_STR_LEN]; /*!< The string data. */
} RdmPdSearchDomain;

etcpal_error_t rdmpd_unpack_get_resp_search_domain(const RdmParamData *pd, RdmPdSearchDomain *domain_name_out);
etcpal_error_t rdmpd_pack_get_resp_search_domain(const RdmPdSearchDomain *domain_name, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_search_domain(const RdmParamData *pd, RdmPdSearchDomain *domain_name_out);
etcpal_error_t rdmpd_pack_set_search_domain(const RdmPdSearchDomain *domain_name, RdmParamData *pd_out);

// Get/Set TCP_COMMS_STATUS
typedef struct RdmPdTcpCommsEntry
{
  RdmPdScopeString scope_string;
  EtcPalSockaddr broker_saddr;
  uint16_t unhealthy_tcp_events;
} RdmPdTcpCommsEntry;

etcpal_error_t rdmpd_unpack_get_resp_tcp_comms_status(const RdmParamData *pd, RdmPdTcpCommsEntry *entry_out);
etcpal_error_t rdmpd_pack_get_resp_tcp_comms_status(const RdmPdTcpCommsEntry *entry, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_tcp_comms_status(const RdmParamData *pd, RdmPdScopeString *scope_out);
etcpal_error_t rdmpd_pack_set_tcp_comms_status(const RdmPdScopeString *scope, RdmParamData *pd_out);

// Get/Set BROKER_STATUS
typedef struct RdmPdBrokerStatus
{
  bool set_allowed;
  uint8_t broker_state;
} RdmPdBrokerStatus;

etcpal_error_t rdmpd_unpack_get_resp_broker_status(const RdmParamData *pd, RdmPdBrokerStatus *status_out);
etcpal_error_t rdmpd_pack_get_resp_broker_status(const RdmPdBrokerStatus *status, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_broker_status(const RdmParamData *pd, uint8_t *broker_state_out);
etcpal_error_t rdmpd_pack_set_broker_status(uint8_t broker_state, RdmParamData *pd_out);

/* e1.37-7 Table A-1: RDM Parameter ID Defines */
// Get ENDPOINT_LIST
typedef struct RdmPdEndpointListEntry
{
  uint16_t endpoint_id;
  uint8_t endpoint_type;
} RdmPdEndpointListEntry;

/*! The maximum number of 24-bit endpoint list entries in an ACK (not overflow) response. */
#define RDMPD_MAX_ENDPOINT_LIST_ENTRIES 74

typedef struct RdmPdEndpointList
{
  uint32_t list_change_number;
  RdmPdEndpointListEntry entries[RDMPD_MAX_ENDPOINT_LIST_ENTRIES];
  size_t number_of_entries;
} RdmPdEndpointList;

/*! The maximum number of 24-bit endpoint list entries in an ACK overflow response. */
#define RDMPD_MAX_ENDPOINT_LIST_OVERFLOW_ENTRIES 76

typedef struct RdmPdEndpointListOverflow
{
  RdmPdEndpointListEntry entries[RDMPD_MAX_ENDPOINT_LIST_OVERFLOW_ENTRIES];
  size_t number_of_entries;
} RdmPdEndpointListOverflow;

etcpal_error_t rdmpd_unpack_get_resp_endpt_list(const RdmParamData *pd, RdmPdEndpointList *list_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_list(const RdmPdEndpointList *list, RdmParamData *pd_out);

etcpal_error_t rdmpd_unpack_get_resp_endpt_list_overflow(const RdmParamData *pd, RdmPdEndpointListOverflow *list_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_list_overflow(const RdmPdEndpointListOverflow *list, RdmParamData *pd_out);

// Get ENDPOINT_LIST_CHANGE
etcpal_error_t rdmpd_unpack_get_resp_endpt_list_change(const RdmParamData *pd, uint32_t *list_change_number_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_list_change(uint32_t list_change_number, RdmParamData *pd_out);

// Get/Set IDENTIFY_ENDPOINT
typedef struct RdmPdIdentifyEndpoint
{
  uint16_t endpoint_id;
  bool identify_state;
} RdmPdIdentifyEndpoint;

etcpal_error_t rdmpd_unpack_get_identify_endpt(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_get_identify_endpt(uint16_t endpt_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_identify_endpt(const RdmParamData *pd, RdmPdIdentifyEndpoint *identify_endpt_out);
etcpal_error_t rdmpd_pack_get_resp_identify_endpt(const RdmPdIdentifyEndpoint *identify_endpt, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_identify_endpt(const RdmParamData *pd, RdmPdIdentifyEndpoint *identify_endpt_out);
etcpal_error_t rdmpd_pack_set_identify_endpt(const RdmPdIdentifyEndpoint *identify_endpt, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_resp_identify_endpt(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_set_resp_identify_endpt(uint16_t endpt_id, RdmParamData *pd_out);

// Get/Set ENDPOINT_TO_UNIVERSE
typedef struct RdmPdEndpointUniverse
{
  uint16_t endpoint_id;
  uint16_t universe_number;
} RdmPdEndpointUniverse;

etcpal_error_t rdmpd_unpack_get_endpt_to_universe(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_get_endpt_to_universe(uint16_t endpt_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_endpt_to_universe(const RdmParamData *pd,
                                                       RdmPdEndpointUniverse *endpt_universe_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_to_universe(const RdmPdEndpointUniverse *endpt_universe, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_endpt_to_universe(const RdmParamData *pd, RdmPdEndpointUniverse *endpt_universe_out);
etcpal_error_t rdmpd_pack_set_endpt_to_universe(const RdmPdEndpointUniverse *endpt_universe, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_resp_endpt_to_universe(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_set_resp_endpt_to_universe(uint16_t endpt_id, RdmParamData *pd_out);

// Get/Set ENDPOINT_MODE
typedef struct RdmPdEndpointMode
{
  uint16_t endpoint_id;
  uint8_t endpoint_mode;
} RdmPdEndpointMode;

etcpal_error_t rdmpd_unpack_get_endpt_mode(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_get_endpt_mode(uint16_t endpt_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_endpt_mode(const RdmParamData *pd, RdmPdEndpointMode *endpt_mode_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_mode(const RdmPdEndpointMode *endpt_mode, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_endpt_mode(const RdmParamData *pd, RdmPdEndpointMode *endpt_mode_out);
etcpal_error_t rdmpd_pack_set_endpt_mode(const RdmPdEndpointMode *endpt_mode, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_resp_endpt_mode(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_set_resp_endpt_mode(uint16_t endpt_id, RdmParamData *pd_out);

// Get/Set ENDPOINT_LABEL
typedef struct RdmPdEndpointLabel
{
  uint16_t endpoint_id;
  RdmPdString endpoint_label;
} RdmPdEndpointLabel;

etcpal_error_t rdmpd_unpack_get_endpt_label(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_get_endpt_label(uint16_t endpt_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_endpt_label(const RdmParamData *pd, RdmPdEndpointLabel *endpt_label_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_label(const RdmPdEndpointLabel *endpt_label, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_endpt_label(const RdmParamData *pd, RdmPdEndpointLabel *endpt_label_out);
etcpal_error_t rdmpd_pack_set_endpt_label(const RdmPdEndpointLabel *endpt_label, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_resp_endpt_label(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_set_resp_endpt_label(uint16_t endpt_id, RdmParamData *pd_out);

// Get/Set RDM_TRAFFIC_ENABLE
typedef struct RdmPdTrafficEnable
{
  uint16_t endpoint_id;
  bool rdm_enabled;
} RdmPdTrafficEnable;

etcpal_error_t rdmpd_unpack_get_traffic_enable(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_get_traffic_enable(uint16_t endpt_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_traffic_enable(const RdmParamData *pd, RdmPdTrafficEnable *traffic_enable_out);
etcpal_error_t rdmpd_pack_get_resp_traffic_enable(const RdmPdTrafficEnable *traffic_enable, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_traffic_enable(const RdmParamData *pd, RdmPdTrafficEnable *traffic_enable_out);
etcpal_error_t rdmpd_pack_set_traffic_enable(const RdmPdTrafficEnable *traffic_enable, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_resp_traffic_enable(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_set_resp_traffic_enable(uint16_t endpt_id, RdmParamData *pd_out);

// Get/Set DISCOVERY_STATE
typedef struct RdmPdDiscoveryState
{
  uint16_t endpoint_id;
  uint8_t discovery_state;
} RdmPdDiscoveryState;

typedef struct RdmPdDeviceCountDiscState
{
  RdmPdDiscoveryState state;
  uint16_t device_count;
} RdmPdDeviceCountDiscState;

etcpal_error_t rdmpd_unpack_get_discovery_state(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_get_discovery_state(uint16_t endpt_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_discovery_state(const RdmParamData *pd, RdmPdDeviceCountDiscState *state_out);
etcpal_error_t rdmpd_pack_get_resp_discovery_state(const RdmPdDeviceCountDiscState *state, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_discovery_state(const RdmParamData *pd, RdmPdDiscoveryState *state_out);
etcpal_error_t rdmpd_pack_set_discovery_state(const RdmPdDiscoveryState *state, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_resp_discovery_state(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_set_resp_discovery_state(uint16_t endpt_id, RdmParamData *pd_out);

// Get/Set BACKGROUND_DISCOVERY
typedef struct RdmPdBackgroundDiscovery
{
  uint16_t endpoint_id;
  bool background_discovery_enabled;
} RdmPdBackgroundDiscovery;

etcpal_error_t rdmpd_unpack_get_background_discovery(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_get_background_discovery(uint16_t endpt_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_background_discovery(const RdmParamData *pd,
                                                          RdmPdBackgroundDiscovery *background_discovery_out);
etcpal_error_t rdmpd_pack_get_resp_background_discovery(const RdmPdBackgroundDiscovery *background_discovery,
                                                        RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_background_discovery(const RdmParamData *pd,
                                                     RdmPdBackgroundDiscovery *background_discovery_out);
etcpal_error_t rdmpd_pack_set_background_discovery(const RdmPdBackgroundDiscovery *background_discovery,
                                                   RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_resp_background_discovery(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_set_resp_background_discovery(uint16_t endpt_id, RdmParamData *pd_out);

// Get/Set ENDPOINT_TIMING
typedef struct RdmPdEndpointTimingSetting
{
  uint16_t endpoint_id;
  uint8_t endpt_timing_setting;
} RdmPdEndpointTimingSetting;

typedef struct RdmPdEndpointTiming
{
  RdmPdEndpointTimingSetting current_setting;
  uint8_t num_settings_available;
} RdmPdEndpointTiming;

etcpal_error_t rdmpd_unpack_get_endpt_timing(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_get_endpt_timing(uint16_t endpt_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_endpt_timing(const RdmParamData *pd, RdmPdEndpointTiming *timing_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_timing(const RdmPdEndpointTiming *timing, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_endpt_timing(const RdmParamData *pd, RdmPdEndpointTimingSetting *timing_out);
etcpal_error_t rdmpd_pack_set_endpt_timing(const RdmPdEndpointTimingSetting *timing, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_resp_endpt_timing(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_set_resp_endpt_timing(uint16_t endpt_id, RdmParamData *pd_out);

// Get ENDPOINT_TIMING_DESCRIPTION
typedef struct RdmPdEndptTimingDescription
{
  uint8_t setting_requested;
  RdmPdString description;
} RdmPdEndptTimingDescription;

etcpal_error_t rdmpd_unpack_get_endpt_timing_description(const RdmParamData *pd, uint8_t *setting_out);
etcpal_error_t rdmpd_pack_get_endpt_timing_description(uint8_t setting, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_endpt_timing_description(const RdmParamData *pd,
                                                              RdmPdEndptTimingDescription *description_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_timing_description(const RdmPdEndptTimingDescription *description,
                                                            RdmParamData *pd_out);

// Get ENDPOINT_RESPONDERS
/*! The maximum number of endpoint responder list UIDs in an ACK (not overflow) response. */
#define RDMPD_MAX_ENDPOINT_RESPONDER_LIST_ENTRIES 37

typedef struct RdmPdEndptResponderList
{
  uint16_t endpoint_id;
  uint32_t list_change_number;
  RdmUid uids[RDMPD_MAX_ENDPOINT_RESPONDER_LIST_ENTRIES];
  size_t number_of_uids;
} RdmPdEndptResponderList;

/*! The maximum number of endpoint responder list UIDs in an ACK overflow response. */
#define RDMPD_MAX_ENDPOINT_RESPONDER_LIST_OVERFLOW_ENTRIES 38

typedef struct RdmPdEndptResponderListOverflow
{
  RdmUid uids[RDMPD_MAX_ENDPOINT_RESPONDER_LIST_OVERFLOW_ENTRIES];
  size_t number_of_uids;
} RdmPdEndptResponderListOverflow;

etcpal_error_t rdmpd_unpack_get_endpt_resp_list(const RdmParamData *pd, uint16_t *endpt_id_out);
etcpal_error_t rdmpd_pack_get_endpt_resp_list(uint16_t endpt_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_endpt_resp_list(const RdmParamData *pd, RdmPdEndptResponderList *list_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_resp_list(const RdmPdEndptResponderList *list, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_endpt_resp_list_overflow(const RdmParamData *pd,
                                                              RdmPdEndptResponderListOverflow *list_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_resp_list_overflow(const RdmPdEndptResponderListOverflow *list,
                                                            RdmParamData *pd_out);

// Get ENDPOINT_RESPONDER_LIST_CHANGE
typedef struct RdmPdEndptRespListChange
{
  uint16_t endpoint_id;
  uint32_t list_change_number;
} RdmPdEndptRespListChange;

etcpal_error_t rdmpd_unpack_get_endpt_resp_list_change(const RdmParamData *pd, uint16_t *endpoint_id_out);
etcpal_error_t rdmpd_pack_get_endpt_resp_list_change(uint16_t endpoint_id, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_endpt_resp_list_change(const RdmParamData *pd,
                                                            RdmPdEndptRespListChange *change_out);
etcpal_error_t rdmpd_pack_get_resp_endpt_resp_list_change(const RdmPdEndptRespListChange *change, RdmParamData *pd_out);

// Get BINDING_CONTROL_FIELDS
typedef struct RdmPdEndpointUid
{
  uint16_t endpoint;
  RdmUid uid;
} RdmPdEndpointUid;

typedef struct RdmPdBindingControlFields
{
  RdmPdEndpointUid endpoint_uid;
  uint16_t control_field;
  RdmUid binding_uid;
} RdmPdBindingControlFields;

etcpal_error_t rdmpd_unpack_get_binding_control_fields(const RdmParamData *pd, RdmPdEndpointUid *endpoint_uid_out);
etcpal_error_t rdmpd_pack_get_binding_control_fields(const RdmPdEndpointUid *endpoint_uid, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_binding_control_fields(const RdmParamData *pd,
                                                            RdmPdBindingControlFields *binding_ctrl_fields_out);
etcpal_error_t rdmpd_pack_get_resp_binding_control_fields(const RdmPdBindingControlFields *binding_ctrl_fields,
                                                          RdmParamData *pd_out);

// Get/Set BACKGROUND_QUEUED_STATUS_POLICY
typedef struct RdmPdBackgroundQdStatusPolicy
{
  uint8_t current_policy_setting;
  uint8_t num_policy_settings;
} RdmPdBackgroundQdStatusPolicy;

etcpal_error_t rdmpd_unpack_get_resp_backgnd_qd_status_policy(const RdmParamData *pd,
                                                              RdmPdBackgroundQdStatusPolicy *policy_out);
etcpal_error_t rdmpd_pack_get_resp_backgnd_qd_status_policy(const RdmPdBackgroundQdStatusPolicy *policy,
                                                            RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_set_backgnd_qd_status_policy(const RdmParamData *pd, uint8_t *policy_out);
etcpal_error_t rdmpd_pack_set_backgnd_qd_status_policy(uint8_t policy, RdmParamData *pd_out);

// Get BACKGROUND_QUEUED_STATUS_POLICY_DESCRIPTION
typedef struct RdmPdBkgndQdStatusPolicyDescription
{
  uint8_t policy_number_requested;
  RdmPdString description;
} RdmPdBkgndQdStatusPolicyDescription;

etcpal_error_t rdmpd_unpack_get_bkgnd_qd_status_policy_desc(const RdmParamData *pd, uint8_t *policy_out);
etcpal_error_t rdmpd_pack_get_bkgnd_qd_status_policy_desc(uint8_t policy, RdmParamData *pd_out);
etcpal_error_t rdmpd_unpack_get_resp_bkgnd_qd_status_policy_desc(const RdmParamData *pd,
                                                                 RdmPdBkgndQdStatusPolicyDescription *description_out);
etcpal_error_t rdmpd_pack_get_resp_bkgnd_qd_status_policy_desc(const RdmPdBkgndQdStatusPolicyDescription *description,
                                                               RdmParamData *pd_out);

#endif /* _RDM_PARAM_DATA_H_ */
