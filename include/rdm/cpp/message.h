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
/// \brief C++ wrapper and utilities for rdm/message.h

#ifndef RDM_CPP_MESSAGE_H_
#define RDM_CPP_MESSAGE_H_

#include <algorithm>
#include <cstddef>
#include <cstring>
#include <vector>
#include "rdm/message.h"
#include "rdm/cpp/uid.h"
#include "etcpal/cpp/common.h"

namespace rdm
{
class Command
{
public:
  Command() = default;
  constexpr Command(rdm_command_class_t command_class, uint16_t param_id, const Uid& source_uid, const Uid& dest_uid,
                    uint8_t transaction_num, uint8_t port_id, uint16_t subdevice = 0, const uint8_t* data = nullptr,
                    uint8_t datalen = 0);

  constexpr Command(const RdmCommand& c_cmd) noexcept;
  Command& operator=(const RdmCommand& c_cmd) noexcept;

  constexpr Uid source_uid() const noexcept;
  constexpr Uid dest_uid() const noexcept;
  constexpr uint8_t transaction_num() const noexcept;
  constexpr uint8_t port_id() const noexcept;
  constexpr uint16_t subdevice() const noexcept;
  constexpr rdm_command_class_t command_class() const noexcept;
  constexpr uint16_t param_id() const noexcept;
  constexpr uint8_t datalen() const noexcept;
  constexpr const uint8_t* data() const noexcept;

  ETCPAL_CONSTEXPR_14 RdmCommand& get() noexcept;
  constexpr const RdmCommand& get() const noexcept;

  constexpr bool IsValid() const noexcept;

  Command& SetSourceUid(const Uid& uid) noexcept;
  Command& SetSourceUid(const RdmUid& uid) noexcept;
  Command& SetDestUid(const Uid& uid) noexcept;
  Command& SetDestUid(const RdmUid& uid) noexcept;
  Command& SetTransactionNum(uint8_t transaction_num) noexcept;
  Command& SetPortId(uint8_t port_id) noexcept;
  Command& SetSubdevice(uint16_t subdevice) noexcept;
  Command& SetCommandClass(rdm_command_class_t command_class) noexcept;
  Command& SetParamId(uint16_t param_id) noexcept;
  Command& SetData(const uint8_t* data, uint8_t datalen) noexcept;

  constexpr size_t PackedSize() const noexcept;
  bool ToBytes(uint8_t* buf, size_t buflen) const noexcept;
  bool ToBytes(RdmBuffer& buffer) const;
  std::vector<uint8_t> ToBytes() const;

private:
  RdmCommand cmd_{};
};

constexpr Command::Command(rdm_command_class_t command_class, uint16_t param_id, const Uid& source_uid,
                           const Uid& dest_uid, uint8_t transaction_num, uint8_t port_id, uint16_t subdevice,
                           const uint8_t* data, uint8_t datalen)
    : cmd_{source_uid.get(), dest_uid.get(), transaction_num, port_id, subdevice, command_class, param_id, datalen}
{
  if (data && datalen)
  {
    uint8_t clamped_datalen = std::min(datalen, static_cast<uint8_t>(RDM_MAX_PDL));
    std::memcpy(cmd_.data, data, clamped_datalen);
  }
}

constexpr Uid Command::source_uid() const noexcept
{
  return cmd_.source_uid;
}

constexpr Uid Command::dest_uid() const noexcept
{
  return cmd_.dest_uid;
}

constexpr uint8_t Command::transaction_num() const noexcept
{
  return cmd_.transaction_num;
}

constexpr uint8_t Command::port_id() const noexcept
{
  return cmd_.port_id;
}

constexpr uint16_t Command::subdevice() const noexcept
{
  return cmd_.subdevice;
}

constexpr rdm_command_class_t Command::command_class() const noexcept
{
  return cmd_.command_class;
}

constexpr uint16_t Command::param_id() const noexcept
{
  return cmd_.param_id;
}

constexpr uint8_t Command::datalen() const noexcept
{
  return cmd_.datalen;
}

constexpr const uint8_t* Command::data() const noexcept
{
  return (cmd_.datalen != 0 ? cmd_.data : nullptr);
}

ETCPAL_CONSTEXPR_14_OR_INLINE RdmCommand& Command::get() noexcept
{
  return cmd_;
}

constexpr const RdmCommand& Command::get() const noexcept
{
  return cmd_;
}

constexpr bool Command::IsValid() const noexcept
{
  // TODO
  return false;
}

inline Command& Command::SetSourceUid(const Uid& uid) noexcept
{
  return SetSourceUid(uid.get());
}

inline Command& Command::SetSourceUid(const RdmUid& uid) noexcept
{
  cmd_.source_uid = uid;
  return *this;
}

inline Command& Command::SetDestUid(const Uid& uid) noexcept
{
  return SetDestUid(uid.get());
}

inline Command& Command::SetDestUid(const RdmUid& uid) noexcept
{
  cmd_.dest_uid = uid;
  return *this;
}

inline Command& Command::SetTransactionNum(uint8_t transaction_num) noexcept
{
  cmd_.transaction_num = transaction_num;
  return *this;
}

inline Command& Command::SetPortId(uint8_t port_id) noexcept
{
  cmd_.port_id = port_id;
  return *this;
}

inline Command& Command::SetSubdevice(uint16_t subdevice) noexcept
{
  cmd_.subdevice = subdevice;
  return *this;
}

inline Command& Command::SetCommandClass(rdm_command_class_t command_class) noexcept
{
  cmd_.command_class = command_class;
  return *this;
}

inline Command& Command::SetParamId(uint16_t param_id) noexcept
{
  cmd_.param_id = param_id;
  return *this;
}

inline Command& Command::SetData(const uint8_t* data, uint8_t datalen) noexcept
{
  uint8_t datalen_clamped = std::min(datalen, static_cast<uint8_t>(RDM_MAX_PDL));
  if (data && datalen_clamped)
    std::memcpy(cmd_.data, data, datalen_clamped);
  cmd_.datalen = datalen_clamped;
  return *this;
}

constexpr size_t Command::PackedSize() const noexcept
{
  return cmd_.datalen + RDM_HEADER_SIZE;
}

inline bool Command::ToBytes(uint8_t* buf, size_t buflen) const noexcept
{
  // TODO
  return false;
}

inline std::vector<uint8_t> Command::ToBytes() const
{
  // TODO
  return std::vector<uint8_t>{};
}

class SingleResponse
{
public:
  SingleResponse() = default;
  constexpr SingleResponse(rdm_command_class_t command_class, rdm_response_type_t response_type, uint16_t param_id,
                           const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num, uint16_t subdevice = 0,
                           uint8_t msg_count = 0, const uint8_t* data = nullptr, uint8_t datalen = 0);

  Uid source_uid() const noexcept;
  Uid dest_uid() const noexcept;
  uint8_t transaction_num() const noexcept;
  rdm_response_type_t response_type() const noexcept;
  uint8_t message_count() const noexcept;
  uint16_t subdevice() const noexcept;
  rdm_command_class_t command_class() const noexcept;
  uint16_t param_id() const noexcept;
  uint8_t datalen() const noexcept;
  const uint8_t* data() const noexcept;

  ETCPAL_CONSTEXPR_14 RdmResponse& get() noexcept;
  constexpr const RdmResponse& get() const noexcept;

  constexpr bool IsValid() const noexcept;

  SingleResponse& SetSourceUid(const Uid& uid) noexcept;
  SingleResponse& SetSourceUid(const RdmUid& uid) noexcept;
  SingleResponse& SetDestUid(const Uid& uid) noexcept;
  SingleResponse& SetDestUid(const RdmUid& uid) noexcept;
  SingleResponse& SetTransactionNum(uint8_t transaction_num) noexcept;
  SingleResponse& SetResponseType(rdm_response_type_t response_type) noexcept;
  SingleResponse& SetMessageCount(uint8_t msg_count) noexcept;
  SingleResponse& SetSubdevice(uint16_t subdevice) noexcept;
  SingleResponse& SetCommandClass(rdm_command_class_t command_class) noexcept;
  SingleResponse& SetParamId(uint16_t param_id) noexcept;
  SingleResponse& SetData(const uint8_t* data, uint8_t datalen) noexcept;

  constexpr size_t PackedSize() const noexcept;
  bool ToBytes(uint8_t* buf, size_t buflen) const noexcept;
  bool ToBytes(RdmBuffer& buffer) const;
  std::vector<uint8_t> ToBytes() const;

  static SingleResponse Ack(const Command& cmd, const uint8_t* data = nullptr, uint8_t datalen = 0,
                            uint8_t msg_count = 0);
  static SingleResponse AckTimer(const Command& cmd, uint16_t delay_ms, uint8_t msg_count = 0);
  static SingleResponse AckOverflow(const Command& cmd, const uint8_t* data = nullptr, uint8_t datalen = 0,
                                    uint8_t msg_count = 0);
  static SingleResponse Nack(const Command& cmd, uint16_t nack_reason, uint8_t msg_count = 0);

private:
  RdmResponse resp_{};
};

class Response
{
public:
  Response() = default;
  constexpr Response(rdm_command_class_t command_class, rdm_response_type_t response_type, uint16_t param_id,
                     const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num, uint16_t subdevice = 0,
                     uint8_t msg_count = 0, const uint8_t* data = nullptr, size_t datalen = 0);

  Uid source_uid() const noexcept;
  Uid dest_uid() const noexcept;
  uint8_t transaction_num() const noexcept;
  rdm_response_type_t response_type() const noexcept;
  uint8_t message_count() const noexcept;
  uint16_t subdevice() const noexcept;
  rdm_command_class_t command_class() const noexcept;
  uint16_t param_id() const noexcept;
  size_t datalen() const noexcept;
  const uint8_t* data() const noexcept;

  constexpr bool IsValid() const noexcept;

  Response& SetSourceUid(const Uid& uid) noexcept;
  Response& SetSourceUid(const RdmUid& uid) noexcept;
  Response& SetDestUid(const Uid& uid) noexcept;
  Response& SetDestUid(const RdmUid& uid) noexcept;
  Response& SetTransactionNum(uint8_t transaction_num) noexcept;
  Response& SetResponseType(rdm_response_type_t response_type) noexcept;
  Response& SetMessageCount(uint8_t msg_count) noexcept;
  Response& SetSubdevice(uint16_t subdevice) noexcept;
  Response& SetCommandClass(rdm_command_class_t command_class) noexcept;
  Response& SetParamId(uint16_t param_id) noexcept;
  Response& SetData(const uint8_t* data, size_t datalen) noexcept;

  constexpr size_t NumSingleResponsesRequired() const noexcept;
  bool ToBytes(RdmBuffer* buffers, size_t num_buffers) const noexcept;
  std::vector<RdmBuffer> ToBytes() const;
  bool ToSingleResponses(SingleResponse* response_buf, size_t response_buf_size) const noexcept;
  std::vector<SingleResponse> ToSingleResponses() const;

  static Response Ack(const Command& cmd, const uint8_t* data = nullptr, size_t datalen = 0, uint8_t msg_count = 0);
  static Response AckTimer(const Command& cmd, uint16_t delay_ms, uint8_t msg_count = 0);
  static Response Nack(const Command& cmd, uint16_t nack_reason, uint8_t msg_count = 0);

private:
  Uid source_uid_;
  Uid dest_uid_;
  uint8_t transaction_num_;
  rdm_response_type_t resp_type_;
  uint8_t msg_count_;
  uint16_t subdevice_;
  rdm_command_class_t command_class_;
  uint16_t param_id_;
  std::vector<uint8_t> data_;
};

};  // namespace rdm
}
;  // namespace rdm

#endif  // RDM_CPP_MESSAGE_H_
