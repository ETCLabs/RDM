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
#include "rdm/defs.h"
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
etcpal_error_t rdmpd_unpack_get_resp_identify_device(const RdmParamData *pd, bool *identify_state);
etcpal_error_t rdmpd_pack_get_resp_identify_device(bool identify_state, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_identify_device(const RdmParamData *pd, bool *identify);
etcpal_error_t rdmpd_pack_set_identify_device(bool identify, RdmParamData *pd);

// Get SUPPORTED_PARAMETERS See Section 10.4.1 of [RDM]
/*! The maximum number of 16-bit parameter IDs in the Get Supported Parameters Response PD. */
#define RDMPD_MAX_SUPPORTED_PARAMETERS 115

/*! A structure representing supported parameters. */
typedef struct RdmPdSupportedParameters
{
  uint16_t parameters[RDMPD_MAX_SUPPORTED_PARAMETERS]; /*!< The array of supported PIDs. */
  size_t length;                                       /*!< The number of supported PIDs in the array. */
} RdmPdSupportedParameters;

etcpal_error_t rdmpd_unpack_get_resp_supported_parameters(const RdmParamData *pd, RdmPdSupportedParameters *params);
etcpal_error_t rdmpd_pack_get_resp_supported_parameters(const RdmPdSupportedParameters *params, RdmParamData *pd);

// Get PARAMETER_DESCRIPTION See Section 10.4.2 of [RDM]
etcpal_error_t rdmpd_unpack_get_parameter_description(const RdmParamData *pd, uint16_t *requested_pid);
etcpal_error_t rdmpd_pack_get_parameter_description(uint16_t requested_pid, RdmParamData *pd);

/*! Data type enumeration */
typedef enum
{
  kRdmPdDtNotDefined = E120_DS_NOT_DEFINED,
  kRdmPdDtBitField = E120_DS_BIT_FIELD,
  kRdmPdDtAscii = E120_DS_ASCII,
  kRdmPdDtUnsignedByte = E120_DS_UNSIGNED_BYTE,
  kRdmPdDtSignedByte = E120_DS_SIGNED_BYTE,
  kRdmPdDtUnsignedWord = E120_DS_UNSIGNED_WORD,
  kRdmPdDtSignedWord = E120_DS_SIGNED_WORD,
  kRdmPdDtUnsignedDword = E120_DS_UNSIGNED_DWORD,
  kRdmPdDtSignedDword = E120_DS_SIGNED_DWORD,
  kRdmPdDtManuSpecificStart = 0x80,
  kRdmPdDtManuSpecificEnd = 0xdf
} rdmpd_data_type_t;

/*! Command class enumeration (get, set, or both) */
typedef enum
{
  kRdmPdCcGet = E120_CC_GET,
  kRdmPdCcSet = E120_CC_SET,
  kRdmPdCcGetSet = E120_CC_GET_SET
} rdmpd_param_desc_cc_t;

typedef enum
{
  kRdmPdSuNone = E120_UNITS_NONE,
  kRdmPdSuCentigrade = E120_UNITS_CENTIGRADE,
  kRdmPdSuVoltsDc = E120_UNITS_VOLTS_DC,
  kRdmPdSuVoltsAcPeak = E120_UNITS_VOLTS_AC_PEAK,
  kRdmPdSuVoltsAcRms = E120_UNITS_VOLTS_AC_RMS,
  kRdmPdSuAmpereDc = E120_UNITS_AMPERE_DC,
  kRdmPdSuAmpereAcPeak = E120_UNITS_AMPERE_AC_PEAK,
  kRdmPdSuAmpereAcRms = E120_UNITS_AMPERE_AC_RMS,
  kRdmPdSuHertz = E120_UNITS_HERTZ,
  kRdmPdSuOhm = E120_UNITS_OHM,
  kRdmPdSuWatt = E120_UNITS_WATT,
  kRdmPdSuKilogram = E120_UNITS_KILOGRAM,
  kRdmPdSuMeters = E120_UNITS_METERS,
  kRdmPdSuMetersSquared = E120_UNITS_METERS_SQUARED,
  kRdmPdSuMetersCubed = E120_UNITS_METERS_CUBED,
  kRdmPdSuKilogrammesPerMeterCubed = E120_UNITS_KILOGRAMMES_PER_METER_CUBED,
  kRdmPdSuMetersPerSecond = E120_UNITS_METERS_PER_SECOND,
  kRdmPdSuMetersPerSecondSquared = E120_UNITS_METERS_PER_SECOND_SQUARED,
  kRdmPdSuNewton = E120_UNITS_NEWTON,
  kRdmPdSuJoule = E120_UNITS_JOULE,
  kRdmPdSuPascal = E120_UNITS_PASCAL,
  kRdmPdSuSecond = E120_UNITS_SECOND,
  kRdmPdSuDegree = E120_UNITS_DEGREE,
  kRdmPdSuSteradian = E120_UNITS_STERADIAN,
  kRdmPdSuCandela = E120_UNITS_CANDELA,
  kRdmPdSuLumen = E120_UNITS_LUMEN,
  kRdmPdSuLux = E120_UNITS_LUX,
  kRdmPdSuIre = E120_UNITS_IRE,
  kRdmPdSuByte = E120_UNITS_BYTE,
  kRdmPdSuManuSpecificStart = 0x80,
  kRdmPdSuManuSpecificEnd = 0xff
} rdmpd_sensor_unit_t;

typedef enum
{
  kRdmPdSuPrefixNone = E120_PREFIX_NONE,
  kRdmPdSuPrefixDeci = E120_PREFIX_DECI,
  kRdmPdSuPrefixCenti = E120_PREFIX_CENTI,
  kRdmPdSuPrefixMilli = E120_PREFIX_MILLI,
  kRdmPdSuPrefixMicro = E120_PREFIX_MICRO,
  kRdmPdSuPrefixNano = E120_PREFIX_NANO,
  kRdmPdSuPrefixPico = E120_PREFIX_PICO,
  kRdmPdSuPrefixFempto = E120_PREFIX_FEMPTO,
  kRdmPdSuPrefixAtto = E120_PREFIX_ATTO,
  kRdmPdSuPrefixZepto = E120_PREFIX_ZEPTO,
  kRdmPdSuPrefixYocto = E120_PREFIX_YOCTO,
  kRdmPdSuPrefixDeca = E120_PREFIX_DECA,
  kRdmPdSuPrefixHecto = E120_PREFIX_HECTO,
  kRdmPdSuPrefixKilo = E120_PREFIX_KILO,
  kRdmPdSuPrefixMega = E120_PREFIX_MEGA,
  kRdmPdSuPrefixGiga = E120_PREFIX_GIGA,
  kRdmPdSuPrefixTerra = E120_PREFIX_TERRA,
  kRdmPdSuPrefixPeta = E120_PREFIX_PETA,
  kRdmPdSuPrefixExa = E120_PREFIX_EXA,
  kRdmPdSuPrefixZetta = E120_PREFIX_ZETTA,
  kRdmPdSuPrefixYotta = E120_PREFIX_YOTTA
} rdmpd_sensor_unit_prefix_t;

/*! A structure representing a parameter description. */
typedef struct RdmPdParameterDescription
{
  uint16_t pid_number_requested; /*!< The manufacturer specific PID requested by the controller. */
  uint8_t pdl_size; /*!< The number used for the PDL field in all GET_RESPONSE and SET messages for this PID. */
  rdmpd_data_type_t data_type;         /*!< The size of the data entries in the PD of the message for this PID. */
  rdmpd_param_desc_cc_t command_class; /*!< Whether Get and or Set messages are implemented for the specified PID. */
  rdmpd_sensor_unit_t unit;            /*!< The SI (International System of units) unit of the specified PID data. */
  rdmpd_sensor_unit_prefix_t prefix;   /*!< The SI Prefix and multiplication factor of the units. */
  uint32_t min_valid_value;            /*!< The lowest value the data can reach. */
  uint32_t max_valid_value;            /*!< The highest value the data can reach. */
  uint32_t default_value;              /*!< The default value of the data. */
  RdmPdString description;             /*!< Describes the function of the specified PID. */
} RdmPdParameterDescription;

etcpal_error_t rdmpd_unpack_get_resp_parameter_description(const RdmParamData *pd,
                                                           RdmPdParameterDescription *description);
etcpal_error_t rdmpd_pack_get_resp_parameter_description(const RdmPdParameterDescription *description,
                                                         RdmParamData *pd);

// Get MANUFACTURER_LABEL See Section 10.5.4 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_manufacturer_label(const RdmParamData *pd, RdmPdString *label);
etcpal_error_t rdmpd_pack_get_resp_manufacturer_label(const RdmPdString *label, RdmParamData *pd);

// Get DEVICE_MODEL_DESCRIPTION See Section 10.5.3 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_device_model_description(const RdmParamData *pd, RdmPdString *description);
etcpal_error_t rdmpd_pack_get_resp_device_model_description(const RdmPdString *description, RdmParamData *pd);

// Get SOFTWARE_VERSION_LABEL See Section 10.5.9 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_software_version_label(const RdmParamData *pd, RdmPdString *label);
etcpal_error_t rdmpd_pack_get_resp_software_version_label(const RdmPdString *label, RdmParamData *pd);

// Get/Set DEVICE_LABEL See Section 10.5.5 of [RDM]
etcpal_error_t rdmpd_unpack_get_resp_device_label(const RdmParamData *pd, RdmPdString *label);
etcpal_error_t rdmpd_pack_get_resp_device_label(const RdmPdString *label, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_device_label(const RdmParamData *pd, RdmPdString *label);
etcpal_error_t rdmpd_pack_set_device_label(const RdmPdString *label, RdmParamData *pd);

/* e1.33 Table A-15: RDM Parameter ID Defines */
// Get/Set COMPONENT_SCOPE
etcpal_error_t rdmpd_unpack_get_component_scope(const RdmParamData *pd, uint16_t *scope_slot);
etcpal_error_t rdmpd_pack_get_component_scope(uint16_t scope_slot, RdmParamData *pd);

/*! The maximum length of a scope string, including one extra byte for a null terminator. */
#define RDMPD_MAX_SCOPE_STR_LEN 63

/*! A structure representing a scope string. */
typedef struct RdmPdScopeString
{
  char string[RDMPD_MAX_SCOPE_STR_LEN]; /*!< The string data. */
} RdmPdScopeString;

typedef struct RdmPdComponentScope
{
  uint16_t scope_slot;
  RdmPdScopeString scope_string;
  EtcPalSockaddr static_broker_addr;
} RdmPdComponentScope;

etcpal_error_t rdmpd_unpack_get_resp_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope);
etcpal_error_t rdmpd_pack_get_resp_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope);
etcpal_error_t rdmpd_pack_set_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd);

// Get/Set SEARCH_DOMAIN
/*! The maximum length of a search domain string, including one extra byte for a null terminator. */
#define RDMPD_MAX_SEARCH_DOMAIN_STR_LEN 232

/*! A structure representing a search domain string. */
typedef struct RdmPdSearchDomain
{
  char string[RDMPD_MAX_SEARCH_DOMAIN_STR_LEN]; /*!< The string data. */
} RdmPdSearchDomain;

etcpal_error_t rdmpd_unpack_get_resp_search_domain(const RdmParamData *pd, RdmPdSearchDomain *domain_name);
etcpal_error_t rdmpd_pack_get_resp_search_domain(const RdmPdSearchDomain *domain_name, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_search_domain(const RdmParamData *pd, RdmPdSearchDomain *domain_name);
etcpal_error_t rdmpd_pack_set_search_domain(const RdmPdSearchDomain *domain_name, RdmParamData *pd);

// Get/Set TCP_COMMS_STATUS
typedef struct RdmPdTcpCommsEntry
{
  RdmPdScopeString scope_string;
  EtcPalSockaddr broker_addr;
  uint16_t unhealthy_tcp_events;
} RdmPdTcpCommsEntry;

etcpal_error_t rdmpd_unpack_get_resp_tcp_comms_status(const RdmParamData *pd, RdmPdTcpCommsEntry *entry);
etcpal_error_t rdmpd_pack_get_resp_tcp_comms_status(const RdmPdTcpCommsEntry *entry, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_tcp_comms_status(const RdmParamData *pd, RdmPdScopeString *scope);
etcpal_error_t rdmpd_pack_set_tcp_comms_status(const RdmPdScopeString *scope, RdmParamData *pd);

// Get/Set BROKER_STATUS
typedef enum
{
  kRdmPdBrokerStateDisabled = 0x00,
  kRdmPdBrokerStateActive = 0x01,
  kRdmPdBrokerStateStandby = 0x02
} rdmpd_broker_state_t;

typedef struct RdmPdBrokerStatus
{
  bool set_allowed;
  rdmpd_broker_state_t broker_state;
} RdmPdBrokerStatus;

etcpal_error_t rdmpd_unpack_get_resp_broker_status(const RdmParamData *pd, RdmPdBrokerStatus *status);
etcpal_error_t rdmpd_pack_get_resp_broker_status(const RdmPdBrokerStatus *status, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_broker_status(const RdmParamData *pd, uint8_t *broker_state);
etcpal_error_t rdmpd_pack_set_broker_status(uint8_t broker_state, RdmParamData *pd);

/* e1.37-7 Table A-1: RDM Parameter ID Defines */
// Get ENDPOINT_LIST
typedef enum
{
  kRdmPdEndpointTypeVirtual = E137_7_ENDPOINT_TYPE_VIRTUAL,
  kRdmPdEndpointTypePhysical = E137_7_ENDPOINT_TYPE_PHYSICAL
} rdmpd_endpoint_type_t;

typedef struct RdmPdEndpointListEntry
{
  uint16_t endpoint_id;
  rdmpd_endpoint_type_t endpoint_type;
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

etcpal_error_t rdmpd_unpack_get_resp_endpt_list(const RdmParamData *pd, RdmPdEndpointList *list);
etcpal_error_t rdmpd_pack_get_resp_endpt_list(const RdmPdEndpointList *list, RdmParamData *pd);

etcpal_error_t rdmpd_unpack_get_resp_endpt_list_overflow(const RdmParamData *pd, RdmPdEndpointListOverflow *list);
etcpal_error_t rdmpd_pack_get_resp_endpt_list_overflow(const RdmPdEndpointListOverflow *list, RdmParamData *pd);

// Get ENDPOINT_LIST_CHANGE
etcpal_error_t rdmpd_unpack_get_resp_endpt_list_change(const RdmParamData *pd, uint32_t *list_change_number);
etcpal_error_t rdmpd_pack_get_resp_endpt_list_change(uint32_t list_change_number, RdmParamData *pd);

// Get/Set IDENTIFY_ENDPOINT
typedef struct RdmPdIdentifyEndpoint
{
  uint16_t endpoint_id;
  bool identify_state;
} RdmPdIdentifyEndpoint;

etcpal_error_t rdmpd_unpack_get_identify_endpt(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_get_identify_endpt(uint16_t endpt_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_identify_endpt(const RdmParamData *pd, RdmPdIdentifyEndpoint *identify_endpt);
etcpal_error_t rdmpd_pack_get_resp_identify_endpt(const RdmPdIdentifyEndpoint *identify_endpt, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_identify_endpt(const RdmParamData *pd, RdmPdIdentifyEndpoint *identify_endpt);
etcpal_error_t rdmpd_pack_set_identify_endpt(const RdmPdIdentifyEndpoint *identify_endpt, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_resp_identify_endpt(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_set_resp_identify_endpt(uint16_t endpt_id, RdmParamData *pd);

// Get/Set ENDPOINT_TO_UNIVERSE
typedef struct RdmPdEndpointUniverse
{
  uint16_t endpoint_id;
  uint16_t universe_number;
} RdmPdEndpointUniverse;

etcpal_error_t rdmpd_unpack_get_endpt_to_universe(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_get_endpt_to_universe(uint16_t endpt_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_endpt_to_universe(const RdmParamData *pd,
                                                       RdmPdEndpointUniverse *endpt_universe);
etcpal_error_t rdmpd_pack_get_resp_endpt_to_universe(const RdmPdEndpointUniverse *endpt_universe, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_endpt_to_universe(const RdmParamData *pd, RdmPdEndpointUniverse *endpt_universe);
etcpal_error_t rdmpd_pack_set_endpt_to_universe(const RdmPdEndpointUniverse *endpt_universe, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_resp_endpt_to_universe(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_set_resp_endpt_to_universe(uint16_t endpt_id, RdmParamData *pd);

// Get/Set ENDPOINT_MODE
typedef enum
{
  kRdmPdEndptModeDisabled = E137_7_ENDPOINT_MODE_DISABLED,
  kRdmPdEndptModeInput = E137_7_ENDPOINT_MODE_INPUT,
  kRdmPdEndptModeOutput = E137_7_ENDPOINT_MODE_OUTPUT
} rdmpd_endpoint_mode_t;

typedef struct RdmPdEndpointMode
{
  uint16_t endpoint_id;
  rdmpd_endpoint_mode_t endpoint_mode;
} RdmPdEndpointMode;

etcpal_error_t rdmpd_unpack_get_endpt_mode(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_get_endpt_mode(uint16_t endpt_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_endpt_mode(const RdmParamData *pd, RdmPdEndpointMode *endpt_mode);
etcpal_error_t rdmpd_pack_get_resp_endpt_mode(const RdmPdEndpointMode *endpt_mode, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_endpt_mode(const RdmParamData *pd, RdmPdEndpointMode *endpt_mode);
etcpal_error_t rdmpd_pack_set_endpt_mode(const RdmPdEndpointMode *endpt_mode, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_resp_endpt_mode(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_set_resp_endpt_mode(uint16_t endpt_id, RdmParamData *pd);

// Get/Set ENDPOINT_LABEL
typedef struct RdmPdEndpointLabel
{
  uint16_t endpoint_id;
  RdmPdString endpoint_label;
} RdmPdEndpointLabel;

etcpal_error_t rdmpd_unpack_get_endpt_label(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_get_endpt_label(uint16_t endpt_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_endpt_label(const RdmParamData *pd, RdmPdEndpointLabel *endpt_label);
etcpal_error_t rdmpd_pack_get_resp_endpt_label(const RdmPdEndpointLabel *endpt_label, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_endpt_label(const RdmParamData *pd, RdmPdEndpointLabel *endpt_label);
etcpal_error_t rdmpd_pack_set_endpt_label(const RdmPdEndpointLabel *endpt_label, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_resp_endpt_label(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_set_resp_endpt_label(uint16_t endpt_id, RdmParamData *pd);

// Get/Set RDM_TRAFFIC_ENABLE
typedef struct RdmPdTrafficEnable
{
  uint16_t endpoint_id;
  bool rdm_enabled;
} RdmPdTrafficEnable;

etcpal_error_t rdmpd_unpack_get_traffic_enable(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_get_traffic_enable(uint16_t endpt_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_traffic_enable(const RdmParamData *pd, RdmPdTrafficEnable *traffic_enable);
etcpal_error_t rdmpd_pack_get_resp_traffic_enable(const RdmPdTrafficEnable *traffic_enable, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_traffic_enable(const RdmParamData *pd, RdmPdTrafficEnable *traffic_enable);
etcpal_error_t rdmpd_pack_set_traffic_enable(const RdmPdTrafficEnable *traffic_enable, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_resp_traffic_enable(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_set_resp_traffic_enable(uint16_t endpt_id, RdmParamData *pd);

// Get/Set DISCOVERY_STATE
typedef enum
{
  kRdmPdDsDiscoveryIncomplete = E137_7_DISCOVERY_INCOMPLETE,
  kRdmPdDsDiscoveryIncremental = E137_7_DISCOVERY_INCREMENTAL,
  kRdmPdDsDiscoveryFull = E137_7_DISCOVERY_FULL,
  kRdmPdDsDiscoveryNotActive = E137_7_DISCOVERY_NOT_ACTIVE,
  kRdmPdDsManuSpecificStart = 0x80,
  kRdmPdDsManuSpecificEnd = 0xdf
} rdmpd_discovery_state_t;

typedef struct RdmPdDiscoveryState
{
  uint16_t endpoint_id;
  rdmpd_discovery_state_t discovery_state;
} RdmPdDiscoveryState;

typedef struct RdmPdDeviceCountDiscState
{
  RdmPdDiscoveryState state;
  uint16_t device_count;
} RdmPdDeviceCountDiscState;

etcpal_error_t rdmpd_unpack_get_discovery_state(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_get_discovery_state(uint16_t endpt_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_discovery_state(const RdmParamData *pd, RdmPdDeviceCountDiscState *state);
etcpal_error_t rdmpd_pack_get_resp_discovery_state(const RdmPdDeviceCountDiscState *state, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_discovery_state(const RdmParamData *pd, RdmPdDiscoveryState *state);
etcpal_error_t rdmpd_pack_set_discovery_state(const RdmPdDiscoveryState *state, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_resp_discovery_state(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_set_resp_discovery_state(uint16_t endpt_id, RdmParamData *pd);

// Get/Set BACKGROUND_DISCOVERY
typedef struct RdmPdBackgroundDiscovery
{
  uint16_t endpoint_id;
  bool background_discovery_enabled;
} RdmPdBackgroundDiscovery;

etcpal_error_t rdmpd_unpack_get_background_discovery(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_get_background_discovery(uint16_t endpt_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_background_discovery(const RdmParamData *pd,
                                                          RdmPdBackgroundDiscovery *background_discovery);
etcpal_error_t rdmpd_pack_get_resp_background_discovery(const RdmPdBackgroundDiscovery *background_discovery,
                                                        RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_background_discovery(const RdmParamData *pd,
                                                     RdmPdBackgroundDiscovery *background_discovery);
etcpal_error_t rdmpd_pack_set_background_discovery(const RdmPdBackgroundDiscovery *background_discovery,
                                                   RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_resp_background_discovery(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_set_resp_background_discovery(uint16_t endpt_id, RdmParamData *pd);

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

etcpal_error_t rdmpd_unpack_get_endpt_timing(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_get_endpt_timing(uint16_t endpt_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_endpt_timing(const RdmParamData *pd, RdmPdEndpointTiming *timing);
etcpal_error_t rdmpd_pack_get_resp_endpt_timing(const RdmPdEndpointTiming *timing, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_endpt_timing(const RdmParamData *pd, RdmPdEndpointTimingSetting *timing);
etcpal_error_t rdmpd_pack_set_endpt_timing(const RdmPdEndpointTimingSetting *timing, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_resp_endpt_timing(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_set_resp_endpt_timing(uint16_t endpt_id, RdmParamData *pd);

// Get ENDPOINT_TIMING_DESCRIPTION
typedef struct RdmPdEndptTimingDescription
{
  uint8_t setting_requested;
  RdmPdString description;
} RdmPdEndptTimingDescription;

etcpal_error_t rdmpd_unpack_get_endpt_timing_description(const RdmParamData *pd, uint8_t *setting);
etcpal_error_t rdmpd_pack_get_endpt_timing_description(uint8_t setting, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_endpt_timing_description(const RdmParamData *pd,
                                                              RdmPdEndptTimingDescription *description);
etcpal_error_t rdmpd_pack_get_resp_endpt_timing_description(const RdmPdEndptTimingDescription *description,
                                                            RdmParamData *pd);

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

etcpal_error_t rdmpd_unpack_get_endpt_resp_list(const RdmParamData *pd, uint16_t *endpt_id);
etcpal_error_t rdmpd_pack_get_endpt_resp_list(uint16_t endpt_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_endpt_resp_list(const RdmParamData *pd, RdmPdEndptResponderList *list);
etcpal_error_t rdmpd_pack_get_resp_endpt_resp_list(const RdmPdEndptResponderList *list, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_endpt_resp_list_overflow(const RdmParamData *pd,
                                                              RdmPdEndptResponderListOverflow *list);
etcpal_error_t rdmpd_pack_get_resp_endpt_resp_list_overflow(const RdmPdEndptResponderListOverflow *list,
                                                            RdmParamData *pd);

// Get ENDPOINT_RESPONDER_LIST_CHANGE
typedef struct RdmPdEndptRespListChange
{
  uint16_t endpoint_id;
  uint32_t list_change_number;
} RdmPdEndptRespListChange;

etcpal_error_t rdmpd_unpack_get_endpt_resp_list_change(const RdmParamData *pd, uint16_t *endpoint_id);
etcpal_error_t rdmpd_pack_get_endpt_resp_list_change(uint16_t endpoint_id, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_endpt_resp_list_change(const RdmParamData *pd,
                                                            RdmPdEndptRespListChange *change);
etcpal_error_t rdmpd_pack_get_resp_endpt_resp_list_change(const RdmPdEndptRespListChange *change, RdmParamData *pd);

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

etcpal_error_t rdmpd_unpack_get_binding_control_fields(const RdmParamData *pd, RdmPdEndpointUid *endpoint_uid);
etcpal_error_t rdmpd_pack_get_binding_control_fields(const RdmPdEndpointUid *endpoint_uid, RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_binding_control_fields(const RdmParamData *pd,
                                                            RdmPdBindingControlFields *binding_ctrl_fields);
etcpal_error_t rdmpd_pack_get_resp_binding_control_fields(const RdmPdBindingControlFields *binding_ctrl_fields,
                                                          RdmParamData *pd);

// Get/Set BACKGROUND_QUEUED_STATUS_POLICY
typedef enum
{
  kRdmPdBqspsStatusNone = 0x00,
  kRdmPdBqspsStatusAdvisory = 0x01,
  kRdmPdBqspsStatusWarning = 0x02,
  kRdmPdBqspsStatusError = 0x03,
  kRdmPdBqspsManuSpecificStart = 0x04,
  kRdmPdBqspsManuSpecificEnd = 0xff
} rdmpd_backgnd_qd_status_policy_setting_t;

typedef struct RdmPdBackgroundQdStatusPolicy
{
  rdmpd_backgnd_qd_status_policy_setting_t current_policy_setting;
  uint8_t num_policy_settings;
} RdmPdBackgroundQdStatusPolicy;

etcpal_error_t rdmpd_unpack_get_resp_backgnd_qd_status_policy(const RdmParamData *pd,
                                                              RdmPdBackgroundQdStatusPolicy *policy);
etcpal_error_t rdmpd_pack_get_resp_backgnd_qd_status_policy(const RdmPdBackgroundQdStatusPolicy *policy,
                                                            RdmParamData *pd);
etcpal_error_t rdmpd_unpack_set_backgnd_qd_status_policy(const RdmParamData *pd,
                                                         rdmpd_backgnd_qd_status_policy_setting_t *policy);
etcpal_error_t rdmpd_pack_set_backgnd_qd_status_policy(rdmpd_backgnd_qd_status_policy_setting_t policy,
                                                       RdmParamData *pd);

// Get BACKGROUND_QUEUED_STATUS_POLICY_DESCRIPTION
typedef struct RdmPdBkgndQdStatusPolicyDescription
{
  rdmpd_backgnd_qd_status_policy_setting_t policy_number_requested;
  RdmPdString description;
} RdmPdBkgndQdStatusPolicyDescription;

etcpal_error_t rdmpd_unpack_get_bkgnd_qd_status_policy_desc(const RdmParamData *pd,
                                                            rdmpd_backgnd_qd_status_policy_setting_t *policy);
etcpal_error_t rdmpd_pack_get_bkgnd_qd_status_policy_desc(rdmpd_backgnd_qd_status_policy_setting_t policy,
                                                          RdmParamData *pd);
etcpal_error_t rdmpd_unpack_get_resp_bkgnd_qd_status_policy_desc(const RdmParamData *pd,
                                                                 RdmPdBkgndQdStatusPolicyDescription *description);
etcpal_error_t rdmpd_pack_get_resp_bkgnd_qd_status_policy_desc(const RdmPdBkgndQdStatusPolicyDescription *description,
                                                               RdmParamData *pd);

#endif /* _RDM_PARAM_DATA_H_ */
