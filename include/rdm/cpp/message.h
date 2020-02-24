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
#include <memory>
#include <vector>
#include "rdm/message.h"
#include "rdm/cpp/uid.h"
#include "etcpal/cpp/common.h"

namespace rdm
{
class CommandHeader
{
public:
  CommandHeader() = default;
  constexpr CommandHeader(rdm_command_class_t command_class, uint16_t param_id, const Uid& source_uid,
                          const Uid& dest_uid, uint8_t transaction_num, uint8_t port_id, uint16_t subdevice = 0);

  constexpr CommandHeader(const RdmCommandHeader& c_header) noexcept;
  CommandHeader& operator=(const RdmCommandHeader& c_header) noexcept;

  constexpr Uid source_uid() const noexcept;
  constexpr Uid dest_uid() const noexcept;
  constexpr uint8_t transaction_num() const noexcept;
  constexpr uint8_t port_id() const noexcept;
  constexpr uint16_t subdevice() const noexcept;
  constexpr rdm_command_class_t command_class() const noexcept;
  constexpr uint16_t param_id() const noexcept;

  ETCPAL_CONSTEXPR_14 RdmCommandHeader& get() noexcept;
  constexpr const RdmCommandHeader& get() const noexcept;

  constexpr bool IsValid() const noexcept;

  CommandHeader& SetSourceUid(const Uid& uid) noexcept;
  CommandHeader& SetSourceUid(const RdmUid& uid) noexcept;
  CommandHeader& SetDestUid(const Uid& uid) noexcept;
  CommandHeader& SetDestUid(const RdmUid& uid) noexcept;
  CommandHeader& SetTransactionNum(uint8_t transaction_num) noexcept;
  CommandHeader& SetPortId(uint8_t port_id) noexcept;
  CommandHeader& SetSubdevice(uint16_t subdevice) noexcept;
  CommandHeader& SetCommandHeaderClass(rdm_command_class_t command_class) noexcept;
  CommandHeader& SetParamId(uint16_t param_id) noexcept;

  constexpr size_t PackedSize(uint8_t datalen = 0) const noexcept;
  bool ToBytes(uint8_t* buf, size_t buflen, const uint8_t* data = nullptr, uint8_t datalen = 0) const noexcept;
  bool ToBytes(RdmBuffer& buffer, const uint8_t* data = nullptr, uint8_t datalen = 0) const noexcept;
  std::vector<uint8_t> ToBytes(const uint8_t* data = nullptr, uint8_t datalen = 0) const;

private:
  RdmCommandHeader cmd_header_{};
};

constexpr CommandHeader::CommandHeader(rdm_command_class_t command_class, uint16_t param_id, const Uid& source_uid,
                                       const Uid& dest_uid, uint8_t transaction_num, uint8_t port_id,
                                       uint16_t subdevice)
    : cmd_header_{source_uid.get(), dest_uid.get(), transaction_num, port_id, subdevice, command_class, param_id}
{
}

constexpr Uid CommandHeader::source_uid() const noexcept
{
  return cmd_header_.source_uid;
}

constexpr Uid CommandHeader::dest_uid() const noexcept
{
  return cmd_header_.dest_uid;
}

constexpr uint8_t CommandHeader::transaction_num() const noexcept
{
  return cmd_header_.transaction_num;
}

constexpr uint8_t CommandHeader::port_id() const noexcept
{
  return cmd_header_.port_id;
}

constexpr uint16_t CommandHeader::subdevice() const noexcept
{
  return cmd_header_.subdevice;
}

constexpr rdm_command_class_t CommandHeader::command_class() const noexcept
{
  return cmd_header_.command_class;
}

constexpr uint16_t CommandHeader::param_id() const noexcept
{
  return cmd_header_.param_id;
}

ETCPAL_CONSTEXPR_14_OR_INLINE RdmCommandHeader& CommandHeader::get() noexcept
{
  return cmd_header_;
}

constexpr const RdmCommandHeader& CommandHeader::get() const noexcept
{
  return cmd_header_;
}

constexpr bool CommandHeader::IsValid() const noexcept
{
  // TODO
  return false;
}

inline CommandHeader& CommandHeader::SetSourceUid(const Uid& uid) noexcept
{
  return SetSourceUid(uid.get());
}

inline CommandHeader& CommandHeader::SetSourceUid(const RdmUid& uid) noexcept
{
  cmd_header_.source_uid = uid;
  return *this;
}

inline CommandHeader& CommandHeader::SetDestUid(const Uid& uid) noexcept
{
  return SetDestUid(uid.get());
}

inline CommandHeader& CommandHeader::SetDestUid(const RdmUid& uid) noexcept
{
  cmd_header_.dest_uid = uid;
  return *this;
}

inline CommandHeader& CommandHeader::SetTransactionNum(uint8_t transaction_num) noexcept
{
  cmd_header_.transaction_num = transaction_num;
  return *this;
}

inline CommandHeader& CommandHeader::SetPortId(uint8_t port_id) noexcept
{
  cmd_header_.port_id = port_id;
  return *this;
}

inline CommandHeader& CommandHeader::SetSubdevice(uint16_t subdevice) noexcept
{
  cmd_header_.subdevice = subdevice;
  return *this;
}

inline CommandHeader& CommandHeader::SetCommandHeaderClass(rdm_command_class_t command_class) noexcept
{
  cmd_header_.command_class = command_class;
  return *this;
}

inline CommandHeader& CommandHeader::SetParamId(uint16_t param_id) noexcept
{
  cmd_header_.param_id = param_id;
  return *this;
}

constexpr size_t CommandHeader::PackedSize(uint8_t datalen) const noexcept
{
  return datalen + RDM_HEADER_SIZE;
}

inline bool CommandHeader::ToBytes(uint8_t* buf, size_t buflen, const uint8_t* data, uint8_t datalen) const noexcept
{
  // TODO
  return false;
}

inline bool CommandHeader::ToBytes(RdmBuffer& buffer, const uint8_t* data, uint8_t datalen) const noexcept
{
  // TODO
  return false;
}

inline std::vector<uint8_t> CommandHeader::ToBytes(const uint8_t* data, uint8_t datalen) const
{
  // TODO
  return std::vector<uint8_t>{};
}

class Command
{
public:
  Command() = default;
  constexpr Command(rdm_command_class_t command_class, uint16_t param_id, const Uid& source_uid, const Uid& dest_uid,
                    uint8_t transaction_num, uint8_t port_id, uint16_t subdevice = 0, const uint8_t* data = nullptr,
                    uint8_t datalen = 0);

  constexpr Uid source_uid() const noexcept;
  constexpr Uid dest_uid() const noexcept;
  constexpr uint8_t transaction_num() const noexcept;
  constexpr uint8_t port_id() const noexcept;
  constexpr uint16_t subdevice() const noexcept;
  constexpr rdm_command_class_t command_class() const noexcept;
  constexpr uint16_t param_id() const noexcept;
  constexpr CommandHeader& header() const noexcept;
  constexpr const uint8_t* data() const noexcept;
  constexpr uint8_t datalen() const noexcept;

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
  Command& SetHeader(const CommandHeader& header) noexcept;
  Command& SetData(const uint8_t* data, size_t size) noexcept;

  constexpr size_t PackedSize() const noexcept;
  bool ToBytes(uint8_t* buf, size_t buflen) const noexcept;
  bool ToBytes(RdmBuffer& buffer) const noexcept;
  std::vector<uint8_t> ToBytes() const;

private:
  CommandHeader header_;
  std::unique_ptr<uint8_t[]> data_;
  uint8_t datalen_;
};

class ResponseHeader
{
public:
  ResponseHeader() = default;
  constexpr ResponseHeader(rdm_command_class_t command_class, rdm_response_type_t response_type, uint16_t param_id,
                           const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num, uint16_t subdevice = 0,
                           uint8_t msg_count = 0);

  constexpr ResponseHeader(const RdmResponseHeader& c_header) noexcept;
  ResponseHeader& operator=(const RdmResponseHeader& c_header) noexcept;

  Uid source_uid() const noexcept;
  Uid dest_uid() const noexcept;
  uint8_t transaction_num() const noexcept;
  rdm_response_type_t response_type() const noexcept;
  uint8_t message_count() const noexcept;
  uint16_t subdevice() const noexcept;
  rdm_command_class_t command_class() const noexcept;
  uint16_t param_id() const noexcept;

  ETCPAL_CONSTEXPR_14 RdmResponseHeader& get() noexcept;
  constexpr const RdmResponseHeader& get() const noexcept;

  constexpr bool IsValid() const noexcept;

  ResponseHeader& SetSourceUid(const Uid& uid) noexcept;
  ResponseHeader& SetSourceUid(const RdmUid& uid) noexcept;
  ResponseHeader& SetDestUid(const Uid& uid) noexcept;
  ResponseHeader& SetDestUid(const RdmUid& uid) noexcept;
  ResponseHeader& SetTransactionNum(uint8_t transaction_num) noexcept;
  ResponseHeader& SetResponseHeaderType(rdm_response_type_t response_type) noexcept;
  ResponseHeader& SetMessageCount(uint8_t msg_count) noexcept;
  ResponseHeader& SetSubdevice(uint16_t subdevice) noexcept;
  ResponseHeader& SetCommandClass(rdm_command_class_t command_class) noexcept;
  ResponseHeader& SetParamId(uint16_t param_id) noexcept;

private:
  RdmResponseHeader resp_header_;
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

  // bool ToBytes(RdmBuffer* buffers, size_t num_buffers) const noexcept;
  // std::vector<RdmBuffer> ToBytes() const;

private:
  ResponseHeader header_;
  std::unique_ptr<uint8_t[]> data_;
  size_t datalen_;
};

};  // namespace rdm

#endif  // RDM_CPP_MESSAGE_H_
