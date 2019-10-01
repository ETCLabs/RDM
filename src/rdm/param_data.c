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

etcpal_error_t rdmpd_unpack_get_resp_tcp_comms_status(const RdmParamData *pd, RdmPdTcpCommsEntry *entry_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_tcp_comms_status(const RdmPdTcpCommsEntry *entry, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_tcp_comms_status(const RdmParamData *pd, char *scope_out, size_t *scope_len_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_tcp_comms_status(const char *scope, size_t scope_len, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_broker_status(const RdmParamData *pd, bool *set_allowed_out,
                                                   uint8_t *broker_state_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_broker_status(bool set_allowed, uint8_t broker_state, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_broker_status(const RdmParamData *pd, RdmPdComponentScope *scope_out,
                                              uint8_t *broker_state_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_broker_status(uint8_t broker_state, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_component_scope(const RdmParamData *pd, RdmPdComponentScope *scope_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_component_scope(const RdmPdComponentScope *scope, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_search_domain(const RdmParamData *pd, char *domain_name_out,
                                                   size_t *domain_name_len_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_search_domain(const char *domain_name, size_t domain_name_len, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_search_domain(const RdmParamData *pd, char *domain_name_out,
                                              size_t *domain_name_len_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_search_domain(const char *domain_name, size_t domain_name_len, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_parameter_description(const RdmParamData *pd,
                                                           RdmPdParameterDescription *description_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_parameter_description(const RdmPdParameterDescription *description,
                                                         RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_manufacturer_label(const RdmParamData *pd, char *label_out, size_t *label_len_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_manufacturer_label(const char *label, size_t label_len, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_device_model_description(const RdmParamData *pd, char *description_out,
                                                              size_t *description_len_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_device_model_description(const char *description, size_t description_len,
                                                            RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_software_version_label(const RdmParamData *pd, char *label_out,
                                                            size_t *label_len_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_software_version_label(const char *label, size_t label_len, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_device_label(const RdmParamData *pd, char *label_out, size_t *label_len_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_device_label(const char *label, size_t label_len, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_device_label(const RdmParamData *pd, char *label_out, size_t *label_len_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_device_label(const char *label, size_t label_len, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_component_scope(const RdmParamData *pd, uint16_t *scope_slot_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_component_scope(uint16_t scope_slot, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_identify_device(const RdmParamData *pd, bool *identify_state_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_identify_device(bool identify_state, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_set_identify_device(const RdmParamData *pd, bool *identify_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_set_identify_device(bool identify, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_resp_supported_parameters(const RdmParamData *pd, RdmPdSupportedParameters *params_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_resp_supported_parameters(const RdmPdSupportedParameters *params, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_unpack_get_parameter_description(const RdmParamData *pd, uint16_t *requested_pid_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}

etcpal_error_t rdmpd_pack_get_parameter_description(uint16_t requested_pid, RdmParamData *pd_out)
{
  return kEtcPalErrNotImpl; // TODO - Not yet implemented
}
