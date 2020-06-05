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

/// \file rdm/cpp/message_types/command.h
/// \brief Definitions for the RDM command type.

#ifndef RDM_CPP_MESSAGE_TYPES_COMMAND_H_
#define RDM_CPP_MESSAGE_TYPES_COMMAND_H_

#include <cstdint>
#include <vector>
#include "etcpal/cpp/common.h"
#include "etcpal/cpp/error.h"
#include "rdm/message.h"
#include "rdm/cpp/message_types/command_header.h"
#include "rdm/cpp/uid.h"

namespace rdm
{
/// \ingroup cpp_message
/// \brief A class representing an RDM command.
///
/// Similar to CommandHeader, except that this class heap-allocates a buffer to hold any
/// accompanying RDM parameter data and thus represents a complete RDM command.
class Command
{
public:
  Command() = default;
  Command(const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num, uint8_t port_id, uint16_t subdevice,
          rdm_command_class_t command_class, uint16_t param_id, const uint8_t* data = nullptr, uint8_t data_len = 0);
  Command(const CommandHeader& header, const uint8_t* data = nullptr, uint8_t data_len = 0);
  Command(const ::RdmCommandHeader& header, const uint8_t* data = nullptr, uint8_t data_len = 0);

  constexpr Uid source_uid() const noexcept;
  constexpr Uid dest_uid() const noexcept;
  constexpr uint8_t transaction_num() const noexcept;
  constexpr uint8_t port_id() const noexcept;
  constexpr uint16_t subdevice() const noexcept;
  constexpr rdm_command_class_t command_class() const noexcept;
  constexpr uint16_t param_id() const noexcept;

  constexpr const CommandHeader& header() const noexcept;
  ETCPAL_CONSTEXPR_14 CommandHeader& header() noexcept;
  const uint8_t* data() const noexcept;
  uint8_t data_len() const noexcept;

  bool IsValid() const noexcept;
  bool HasData() const noexcept;

  constexpr bool IsGet() const noexcept;
  constexpr bool IsSet() const noexcept;
  constexpr bool IsDiscovery() const noexcept;

  Command& SetSourceUid(const Uid& uid) noexcept;
  Command& SetSourceUid(const ::RdmUid& uid) noexcept;
  Command& SetDestUid(const Uid& uid) noexcept;
  Command& SetDestUid(const ::RdmUid& uid) noexcept;
  Command& SetTransactionNum(uint8_t transaction_num) noexcept;
  Command& SetPortId(uint8_t port_id) noexcept;
  Command& SetSubdevice(uint16_t subdevice) noexcept;
  Command& SetCommandClass(rdm_command_class_t command_class) noexcept;
  Command& SetParamId(uint16_t param_id) noexcept;
  Command& SetHeader(const CommandHeader& header) noexcept;
  Command& SetData(const uint8_t* data, uint8_t data_len);
  Command& ClearData() noexcept;

  size_t PackedSize() const noexcept;
  etcpal::Error Serialize(RdmBuffer& buffer) const noexcept;
  etcpal::Error Serialize(uint8_t* buf, size_t buflen) const noexcept;

  /// \name High-level command generators
  /// @{
  static Command Get(uint16_t param_id, const Uid& source_uid, const Uid& dest_uid, uint16_t subdevice = 0,
                     const uint8_t* data = nullptr, uint8_t data_len = 0, uint8_t transaction_num = 0,
                     uint8_t port_id = 1);
  static Command Set(uint16_t param_id, const Uid& source_uid, const Uid& dest_uid, uint16_t subdevice = 0,
                     const uint8_t* data = nullptr, uint8_t data_len = 0, uint8_t transaction_num = 0,
                     uint8_t port_id = 1);
  /// @}

private:
  CommandHeader header_;
  std::vector<uint8_t> data_;
};

/// \brief Construct a command from its component values.
/// \param source_uid The RDM UID of the controller generating this command.
/// \param dest_uid The RDM UID of the responder to which this command is addressed.
/// \param transaction_num The RDM transaction number (should increase monotonically with each command).
/// \param port_id The port ID of the port on which this command is being sent.
/// \param subdevice The sub-device to which this command is addressed, or 0 for the root device.
/// \param command_class The RDM command class for this command.
/// \param param_id The RDM parameter ID (PID) of this command.
/// \param data The RDM parameter data of this command (nullptr for commands with no data).
/// \param data_len The length of the RDM parameter data (0 for commands with no data).
inline Command::Command(const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num, uint8_t port_id,
                        uint16_t subdevice, rdm_command_class_t command_class, uint16_t param_id, const uint8_t* data,
                        uint8_t data_len)
    : header_(source_uid, dest_uid, transaction_num, port_id, subdevice, command_class, param_id)
{
  SetData(data, data_len);
}

/// \brief Construct a command from a header and data.
/// \param header Header to assign to this command.
/// \param data The RDM parameter data of this command (nullptr for commands with no data).
/// \param data_len The length of the RDM parameter data (0 for commands with no data).
inline Command::Command(const CommandHeader& header, const uint8_t* data, uint8_t data_len) : header_(header)
{
  SetData(data, data_len);
}

/// \brief Construct a command from a header and data.
/// \param header Header to assign to this command.
/// \param data The RDM parameter data of this command (nullptr for commands with no data).
/// \param data_len The length of the RDM parameter data (0 for commands with no data).
inline Command::Command(const ::RdmCommandHeader& header, const uint8_t* data, uint8_t data_len) : header_(header)
{
  SetData(data, data_len);
}

/// \brief Get the UID of the controller generating the command.
constexpr Uid Command::source_uid() const noexcept
{
  return header_.source_uid();
}

/// \brief Get the UID of the responder to which the command is addressed.
constexpr Uid Command::dest_uid() const noexcept
{
  return header_.dest_uid();
}

/// \brief Get the transaction number of the command.
constexpr uint8_t Command::transaction_num() const noexcept
{
  return header_.transaction_num();
}

/// \brief Get the port ID of the port on which the command is being sent.
constexpr uint8_t Command::port_id() const noexcept
{
  return header_.port_id();
}

/// \brief Get the sub-device to which the command is addressed (0 means the root device).
constexpr uint16_t Command::subdevice() const noexcept
{
  return header_.subdevice();
}

/// \brief Get the command's command class.
constexpr rdm_command_class_t Command::command_class() const noexcept
{
  return header_.command_class();
}

/// \brief Get the RDM Parameter ID of the command.
///
/// Valid values are listed in ANSI E1.20 Table A-3, any of the relevant E1.37 extension standards,
/// ANSI E1.33 Table A-15, or manufacturer-specific values in the range 0x8000 - 0xffdf.
constexpr uint16_t Command::param_id() const noexcept
{
  return header_.param_id();
}

/// \brief Get a const reference to the command's header.
constexpr const CommandHeader& Command::header() const noexcept
{
  return header_;
}

/// \brief Get a mutable reference to the command's header.
ETCPAL_CONSTEXPR_14_OR_INLINE CommandHeader& Command::header() noexcept
{
  return header_;
}

/// \brief Get a pointer to the command's parameter data.
/// \return A pointer to a byte buffer with the parameter data, or nullptr if this command has no data.
inline const uint8_t* Command::data() const noexcept
{
  return (data_.empty() ? nullptr : data_.data());
}

/// \brief Get the length of the command's parameter data.
inline uint8_t Command::data_len() const noexcept
{
  return static_cast<uint8_t>(data_.size());
}

/// \brief Whether the values contained in this command are valid for an RDM command.
inline bool Command::IsValid() const noexcept
{
  return header_.IsValid();
}

/// \brief Whether this command has parameter data associated with it.
inline bool Command::HasData() const noexcept
{
  return !data_.empty();
}

/// \brief Whether this command is an RDM GET_COMMAND.
constexpr bool Command::IsGet() const noexcept
{
  return header_.IsGet();
}

/// \brief Whether this command is an RDM SET_COMMAND.
constexpr bool Command::IsSet() const noexcept
{
  return header_.IsSet();
}

/// \brief Whether this command is an RDM DISCOVERY_COMMAND.
constexpr bool Command::IsDiscovery() const noexcept
{
  return header_.IsDiscovery();
}

/// \brief Set the UID of the controller generating the command.
inline Command& Command::SetSourceUid(const Uid& uid) noexcept
{
  header_.SetSourceUid(uid);
  return *this;
}

/// \brief Set the UID of the controller generating the command.
inline Command& Command::SetSourceUid(const ::RdmUid& uid) noexcept
{
  header_.SetSourceUid(uid);
  return *this;
}

/// \brief Set the UID of the responder to which the command is addressed.
inline Command& Command::SetDestUid(const Uid& uid) noexcept
{
  header_.SetDestUid(uid);
  return *this;
}

/// \brief Set the UID of the responder to which the command is addressed.
inline Command& Command::SetDestUid(const ::RdmUid& uid) noexcept
{
  header_.SetDestUid(uid);
  return *this;
}

/// \brief Set the transaction number of the command.
inline Command& Command::SetTransactionNum(uint8_t transaction_num) noexcept
{
  header_.SetTransactionNum(transaction_num);
  return *this;
}

/// \brief Set the port ID of the port on which the command is being sent.
inline Command& Command::SetPortId(uint8_t port_id) noexcept
{
  header_.SetPortId(port_id);
  return *this;
}

/// \brief Set the sub-device to which the command is addressed (0 means the root device).
inline Command& Command::SetSubdevice(uint16_t subdevice) noexcept
{
  header_.SetSubdevice(subdevice);
  return *this;
}

/// \brief Set the command's command class.
inline Command& Command::SetCommandClass(rdm_command_class_t command_class) noexcept
{
  header_.SetCommandClass(command_class);
  return *this;
}

/// \brief Set the RDM Parameter ID of the command.
///
/// Valid values are listed in ANSI E1.20 Table A-3, any of the relevant E1.37 extension standards,
/// ANSI E1.33 Table A-15, or manufacturer-specific values in the range 0x8000 - 0xffdf.
inline Command& Command::SetParamId(uint16_t param_id) noexcept
{
  header_.SetParamId(param_id);
  return *this;
}

/// \brief Replace this command's header with a new header.
inline Command& Command::SetHeader(const CommandHeader& header) noexcept
{
  header_ = header;
  return *this;
}

/// \brief Set the parameter data associated with this command.
inline Command& Command::SetData(const uint8_t* data, uint8_t data_len)
{
  if (data && data_len)
  {
    uint8_t truncated_length = (data_len > RDM_MAX_PDL ? RDM_MAX_PDL : data_len);
    data_.assign(data, data + truncated_length);
  }
  return *this;
}

/// \brief Remove the parameter data associated with this command.
inline Command& Command::ClearData() noexcept
{
  data_.clear();
  return *this;
}

/// \brief Get the number of bytes that the command will occupy when converted to wire format.
inline size_t Command::PackedSize() const noexcept
{
  return RDM_PACKED_SIZE(data_.size());
}

/// \brief Convert the command to the RDM wire format.
/// \param[out] buffer RdmBuffer struct to hold the serialized command.
/// \return Result of serialize operation.
inline etcpal::Error Command::Serialize(RdmBuffer& buffer) const noexcept
{
  return header_.Serialize(buffer, data(), data_len());
}

/// \brief Convert the command to the RDM wire format.
/// \param[out] buf Data buffer to hold the serialized command.
/// \param[out] buf_len Length in bytes of buf.
/// \return Result of serialize operation.
inline etcpal::Error Command::Serialize(uint8_t* buf, size_t buf_len) const noexcept
{
  return header_.Serialize(buf, buf_len, data(), data_len());
}

/// \brief Create an RDM GET_COMMAND.
/// \param param_id The RDM parameter ID (PID) of this command.
/// \param source_uid The RDM UID of the controller generating this command.
/// \param dest_uid The RDM UID of the responder to which this command is addressed.
/// \param subdevice The sub-device to which this command is addressed, or 0 for the root device.
/// \param data The RDM parameter data of this command (nullptr for commands with no data).
/// \param data_len The length of the RDM parameter data (0 for commands with no data).
/// \param transaction_num The RDM transaction number (should increase monotonically with each command).
/// \param port_id The port ID of the port on which this command is being sent.
inline Command Command::Get(uint16_t param_id, const Uid& source_uid, const Uid& dest_uid, uint16_t subdevice,
                            const uint8_t* data, uint8_t data_len, uint8_t transaction_num, uint8_t port_id)
{
  return Command(source_uid, dest_uid, transaction_num, port_id, subdevice, kRdmCCGetCommand, param_id, data, data_len);
}

/// \brief Create an RDM SET_COMMAND.
/// \param param_id The RDM parameter ID (PID) of this command.
/// \param source_uid The RDM UID of the controller generating this command.
/// \param dest_uid The RDM UID of the responder to which this command is addressed.
/// \param subdevice The sub-device to which this command is addressed, or 0 for the root device.
/// \param data The RDM parameter data of this command (nullptr for commands with no data).
/// \param data_len The length of the RDM parameter data (0 for commands with no data).
/// \param transaction_num The RDM transaction number (should increase monotonically with each command).
/// \param port_id The port ID of the port on which this command is being sent.
inline Command Command::Set(uint16_t param_id, const Uid& source_uid, const Uid& dest_uid, uint16_t subdevice,
                            const uint8_t* data, uint8_t data_len, uint8_t transaction_num, uint8_t port_id)
{
  return Command(source_uid, dest_uid, transaction_num, port_id, subdevice, kRdmCCSetCommand, param_id, data, data_len);
}

};  // namespace rdm

#endif  // RDM_CPP_MESSAGE_TYPES_COMMAND_H_
