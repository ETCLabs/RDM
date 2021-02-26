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

#ifndef RDM_CPP_CONTROLLER_H_
#define RDM_CPP_CONTROLLER_H_

#include "etcpal/cpp/error.h"
#include "rdm/controller.h"
#include "rdm/cpp/uid.h"

namespace rdm
{
class Controller
{
public:
  Controller() = default;
  Controller(const rdm::Uid& controller_uid, uint8_t port_number);

  etcpal::Error Init(const rdm::Uid& controller_uid, uint8_t port_number);
  void          InitDiscovery();
  etcpal::Error PackNextDub(rdm_dub_result_t last_dub_result, RdmBuffer& buf);
  etcpal::Error PackMute(const rdm::Uid& dest, RdmBuffer& buf);
  etcpal::Error PackUnMute(const rdm::Uid& dest, RdmBuffer& buf);
  etcpal::Error PackUnMuteAll(RdmBuffer& buf);

  etcpal::Error PackCommand(const rdm::Uid&     dest,
                            uint16_t            subdevice,
                            rdm_command_class_t cc,
                            uint16_t            param_id,
                            const uint8_t*      param_data = nullptr,
                            uint8_t             param_data_len = 0);

  static bool                       IsDubResponse(const RdmBuffer& buf);
  static etcpal::Expected<rdm::Uid> UnpackDubResponse(const RdmBuffer& buf);

  const RdmControllerContext& context() const noexcept;
  RdmControllerContext&       context() noexcept;

private:
  RdmControllerContext ctx_{};
};
};  // namespace rdm

#endif  // RDM_CPP_CONTROLLER_H_
