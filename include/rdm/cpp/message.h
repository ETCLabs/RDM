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
#include <chrono>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <memory>
#include <vector>
#include "rdm/message.h"
#include "rdm/cpp/uid.h"
#include "etcpal/cpp/common.h"
#include "etcpal/cpp/error.h"

namespace rdm
{
/// \defgroup cpp_message Message
/// \ingroup rdm_cpp
/// \brief C++ classes representing RDM messages.

/// \ingroup cpp_message
/// \brief A wrapper class for an RDM NACK reason code.
///
/// Provides C++ syntactic sugar on top of #rdm_nack_reason_t codes. A NackReason can be created
/// from an #rdm_nack_reason_t or a manufacturer-specific NACK reason value.
///
/// Example Usage:
/// \code
/// auto reason = rdm::NackReason(kRdmNRUnknownPid); // Create a NackReason representing E120_NR_UNKNOWN_PID
/// std::cout << "Got NACK with reason code " << reason.code() << ", reason '" << reason.ToCString() << "'.\n";
/// // Prints "Got NACK with reason code 0, reason 'Unknown PID'."
///
/// auto manufacturer_reason = rdm::NackReason(0x8001);
/// EXPECT_FALSE(manufacturer_reason.IsStandard());
/// // Caution: manufacturer_reason.ToCString() will return nullptr.
/// std::cout << "Got NACK with manufacturer-specific reason code " << manufacturer_reason.raw_code() << ".\n";
/// // Prints "Got NACK with manufacturer-specific reason code 32769"
/// \endcode
class NackReason
{
public:
  NackReason() = delete;

  constexpr NackReason(rdm_nack_reason_t code) noexcept;
  NackReason& operator=(rdm_nack_reason_t code) noexcept;

  constexpr NackReason(uint16_t code) noexcept;
  NackReason& operator=(uint16_t code) noexcept;

  constexpr rdm_nack_reason_t code() const noexcept;
  constexpr uint16_t raw_code() const noexcept;
  std::string ToString() const;
  const char* ToCString() const noexcept;

  constexpr bool IsStandard() const noexcept;

private:
  rdm_nack_reason_t code_;
};

/// \brief Construct a NackReason from a standard NACK reason code.
constexpr NackReason::NackReason(rdm_nack_reason_t code) noexcept : code_(code)
{
}

/// \brief Assign a standard NACK reason code to this NackReason.
inline NackReason& NackReason::operator=(rdm_nack_reason_t code) noexcept
{
  code_ = code;
  return *this;
}

/// \brief Construct a NackReason from a raw code value (useful for manufacturer-specific codes).
constexpr NackReason::NackReason(uint16_t code) noexcept : code_(static_cast<rdm_nack_reason_t>(code))
{
}

/// \brief Assign a raw code value to this NackReason.
inline NackReason& NackReason::operator=(uint16_t code) noexcept
{
  code_ = static_cast<rdm_nack_reason_t>(code);
  return *this;
}

/// \brief Get the standard NACK reason code value.
constexpr rdm_nack_reason_t NackReason::code() const noexcept
{
  return code_;
}

/// \brief Get the raw code value as a uint16_t.
constexpr uint16_t NackReason::raw_code() const noexcept
{
  return static_cast<uint16_t>(code_);
}

/// \brief Get a descriptive string for this NACK reason code as a std::string.
/// \return The string representation. If IsStandard() == false, returns a placeholder string.
inline std::string NackReason::ToString() const
{
  return rdm_nack_reason_to_string(code_);
}

/// \brief Get a descriptive string for this NACK reason code as a C string.
/// \return The string representation. If IsStandard() == false, returns a placeholder string.
inline const char* NackReason::ToCString() const noexcept
{
  return rdm_nack_reason_to_string(code_);
}

/// \brief Whether this instance contains a standard RDM NACK reason code.
constexpr bool NackReason::IsStandard() const noexcept
{
  return code_ < kRdmNumStandardNRCodes;
}

/// \ingroup cpp_message
/// \brief A class that represents the header fields of an RDM command.
///
/// Includes everything except the RDM parameter data. Example usage:
/// \code
/// // Default-constructed commands are not valid
/// rdm::CommandHeader invalid_cmd;
/// EXPECT_FALSE(invalid_cmd.IsValid());
///
/// // Create a valid command
/// rdm::CommandHeader get_cmd(rdm::Uid(0x6574, 0x12345678), // Source UID
///                            rdm::Uid(0x6574, 0x87654321), // Destination UID
///                            0x22,                         // Transaction number
///                            1,                            // Port ID
///                            0,                            // Subdevice
///                            kRdmCCGetCommand,             // Command class
///                            E120_SUPPORTED_PARAMETERS     // Parameter ID
///                           );
///
/// // The static generator functions have nicer syntax for higher-level RDM logic that doesn't
/// // necessarily care about the Port ID and Transaction Number fields, etc.
/// auto get_cmd = rdm::CommandHeader::Get(E120_SUPPORTED_PARAMETERS, rdm::Uid(0x6574, 0x12345678),
///                                        rdm::Uid(0x6574, 0x87654321));
///
/// EXPECT_TRUE(get_cmd.IsValid());
/// EXPECT_TRUE(get_cmd.IsGet());
///
/// // Create a packed on-wire representation
/// RdmBuffer cmd_buf;
/// get_cmd.SetPortId(port_to_send_on).SetTransactionNum(next_transaction_num++).Serialize(cmd_buf);
///
/// // For a command that has parameter data, supply the parameter data when serializing:
/// auto set_cmd = rdm::CommandHeader::Set(E120_IDENTIFY_DEVICE, rdm::Uid(0x6574, 0x12345678),
///                                        rdm::Uid(0x6574, 0x87654321));
///
/// uint8_t identify_val = 1; // Start identifying
///
/// set_cmd.Serialize(cmd_buf, &identify_val, 1);
/// \endcode
class CommandHeader
{
public:
  /// Construct an invalid command header by default.
  CommandHeader() = default;
  constexpr CommandHeader(const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num, uint8_t port_id,
                          uint16_t subdevice, rdm_command_class_t command_class, uint16_t param_id) noexcept;

  constexpr CommandHeader(const ::RdmCommandHeader& c_header) noexcept;
  CommandHeader& operator=(const ::RdmCommandHeader& c_header) noexcept;

  constexpr Uid source_uid() const noexcept;
  constexpr Uid dest_uid() const noexcept;
  constexpr uint8_t transaction_num() const noexcept;
  constexpr uint8_t port_id() const noexcept;
  constexpr uint16_t subdevice() const noexcept;
  constexpr rdm_command_class_t command_class() const noexcept;
  constexpr uint16_t param_id() const noexcept;

  constexpr const ::RdmCommandHeader& get() const noexcept;
  ETCPAL_CONSTEXPR_14 ::RdmCommandHeader& get() noexcept;

  bool IsValid() const noexcept;

  constexpr bool IsGet() const noexcept;
  constexpr bool IsSet() const noexcept;
  constexpr bool IsDiscovery() const noexcept;

  CommandHeader& SetSourceUid(const Uid& uid) noexcept;
  CommandHeader& SetSourceUid(const ::RdmUid& uid) noexcept;
  CommandHeader& SetDestUid(const Uid& uid) noexcept;
  CommandHeader& SetDestUid(const ::RdmUid& uid) noexcept;
  CommandHeader& SetTransactionNum(uint8_t transaction_num) noexcept;
  CommandHeader& SetPortId(uint8_t port_id) noexcept;
  CommandHeader& SetSubdevice(uint16_t subdevice) noexcept;
  CommandHeader& SetCommandClass(rdm_command_class_t command_class) noexcept;
  CommandHeader& SetParamId(uint16_t param_id) noexcept;

  constexpr size_t PackedSize(uint8_t data_len = 0) const noexcept;
  etcpal::Error Serialize(RdmBuffer& buffer, const uint8_t* data = nullptr, uint8_t data_len = 0) const noexcept;
  etcpal::Error Serialize(uint8_t* buf, size_t buf_len, const uint8_t* data = nullptr, uint8_t data_len = 0) const
      noexcept;

  /// \name High-level command header generators
  /// @{
  static CommandHeader Get(uint16_t param_id, const Uid& source_uid, const Uid& dest_uid, uint16_t subdevice = 0,
                           uint8_t transaction_num = 0, uint8_t port_id = 1) noexcept;
  static CommandHeader Set(uint16_t param_id, const Uid& source_uid, const Uid& dest_uid, uint16_t subdevice = 0,
                           uint8_t transaction_num = 0, uint8_t port_id = 1) noexcept;
  /// @}

private:
  ::RdmCommandHeader cmd_header_{};
};

/// \brief Construct a command header from its component values.
/// \param source_uid The RDM UID of the controller generating this command.
/// \param dest_uid The RDM UID of the responder to which this command is addressed.
/// \param transaction_num The RDM transaction number (should increase monotonically with each command).
/// \param port_id The port ID of the port on which this command is being sent.
/// \param subdevice The sub-device to which this command is addressed, or 0 for the root device.
/// \param command_class The RDM command class for this command.
/// \param param_id The RDM parameter ID (PID) of this command.
constexpr CommandHeader::CommandHeader(const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num,
                                       uint8_t port_id, uint16_t subdevice, rdm_command_class_t command_class,
                                       uint16_t param_id) noexcept
    : cmd_header_{source_uid.get(), dest_uid.get(), transaction_num, port_id, subdevice, command_class, param_id}
{
}

/// \brief Construct a command header copied from an instance of the C RdmCommandHeader type.
constexpr CommandHeader::CommandHeader(const ::RdmCommandHeader& c_header) noexcept : cmd_header_(c_header)
{
}

/// \brief Assign an instance of the C RdmCommandHeader type to an instance of this class.
inline CommandHeader& CommandHeader::operator=(const ::RdmCommandHeader& c_header) noexcept
{
  cmd_header_ = c_header;
  return *this;
}

/// \brief Get the UID of the controller generating the command.
constexpr Uid CommandHeader::source_uid() const noexcept
{
  return cmd_header_.source_uid;
}

/// \brief Get the UID of the responder to which the command is addressed.
constexpr Uid CommandHeader::dest_uid() const noexcept
{
  return cmd_header_.dest_uid;
}

/// \brief Get the transaction number of the command.
constexpr uint8_t CommandHeader::transaction_num() const noexcept
{
  return cmd_header_.transaction_num;
}

/// \brief Get the port ID of the port on which the command is being sent.
constexpr uint8_t CommandHeader::port_id() const noexcept
{
  return cmd_header_.port_id;
}

/// \brief Get the sub-device to which the command is addressed (0 means the root device).
constexpr uint16_t CommandHeader::subdevice() const noexcept
{
  return cmd_header_.subdevice;
}

/// \brief Get the command's command class.
constexpr rdm_command_class_t CommandHeader::command_class() const noexcept
{
  return cmd_header_.command_class;
}

/// \brief Get the RDM Parameter ID of the command.
constexpr uint16_t CommandHeader::param_id() const noexcept
{
  return cmd_header_.param_id;
}

/// \brief Get a const reference to the underlying C type.
constexpr const ::RdmCommandHeader& CommandHeader::get() const noexcept
{
  return cmd_header_;
}

/// \brief Get a mutable reference to the underlying C type.
ETCPAL_CONSTEXPR_14_OR_INLINE ::RdmCommandHeader& CommandHeader::get() noexcept
{
  return cmd_header_;
}

/// \brief Whether the values contained in this command header are valid for an RDM command.
inline bool CommandHeader::IsValid() const noexcept
{
  return rdm_command_header_is_valid(&cmd_header_);
}

/// \brief Whether this header represents an RDM GET_COMMAND.
constexpr bool CommandHeader::IsGet() const noexcept
{
  return cmd_header_.command_class == kRdmCCGetCommand;
}

/// \brief Whether this header represents an RDM SET_COMMAND.
constexpr bool CommandHeader::IsSet() const noexcept
{
  return cmd_header_.command_class == kRdmCCSetCommand;
}

/// \brief Whether this header represents an RDM DISCOVERY_COMMAND.
constexpr bool CommandHeader::IsDiscovery() const noexcept
{
  return cmd_header_.command_class == kRdmCCDiscoveryCommand;
}

/// \brief Set the UID of the controller generating the command.
inline CommandHeader& CommandHeader::SetSourceUid(const Uid& uid) noexcept
{
  return SetSourceUid(uid.get());
}

/// \brief Set the UID of the controller generating the command.
inline CommandHeader& CommandHeader::SetSourceUid(const ::RdmUid& uid) noexcept
{
  cmd_header_.source_uid = uid;
  return *this;
}

/// \brief Set the UID of the responder to which the command is addressed.
inline CommandHeader& CommandHeader::SetDestUid(const Uid& uid) noexcept
{
  return SetDestUid(uid.get());
}

/// \brief Set the UID of the responder to which the command is addressed.
inline CommandHeader& CommandHeader::SetDestUid(const ::RdmUid& uid) noexcept
{
  cmd_header_.dest_uid = uid;
  return *this;
}

/// \brief Set the transaction number of the command.
inline CommandHeader& CommandHeader::SetTransactionNum(uint8_t transaction_num) noexcept
{
  cmd_header_.transaction_num = transaction_num;
  return *this;
}

/// \brief Set the port ID of the port on which the command is being sent.
inline CommandHeader& CommandHeader::SetPortId(uint8_t port_id) noexcept
{
  cmd_header_.port_id = port_id;
  return *this;
}

/// \brief Set the sub-device to which the command is addressed (0 means the root device).
inline CommandHeader& CommandHeader::SetSubdevice(uint16_t subdevice) noexcept
{
  cmd_header_.subdevice = subdevice;
  return *this;
}

/// \brief Set the command's command class.
inline CommandHeader& CommandHeader::SetCommandClass(rdm_command_class_t command_class) noexcept
{
  cmd_header_.command_class = command_class;
  return *this;
}

/// \brief Set the RDM Parameter ID of the command.
///
/// Valid values are listed in ANSI E1.20 Table A-3, any of the relevant E1.37 extension standards,
/// ANSI E1.33 Table A-15, or manufacturer-specific values in the range 0x8000 - 0xffdf.
inline CommandHeader& CommandHeader::SetParamId(uint16_t param_id) noexcept
{
  cmd_header_.param_id = param_id;
  return *this;
}

/// \brief Get the number of bytes that the command header will occupy when converted to wire format.
/// \param data_len The size of the parameter data that will accompany the command header.
constexpr size_t CommandHeader::PackedSize(uint8_t data_len) const noexcept
{
  return data_len + RDM_HEADER_SIZE;
}

/// \brief Convert the header along with accompanying data to the RDM wire format.
/// \param[out] buffer RdmBuffer struct to hold the serialized command.
/// \param[in] data RDM parameter data that accompanies this header, if any.
/// \param[in] data_len Length of parameter data.
/// \return Result of serialize operation.
inline etcpal::Error CommandHeader::Serialize(RdmBuffer& buffer, const uint8_t* data, uint8_t data_len) const noexcept
{
  return rdm_create_command(&cmd_header_, data, data_len, &buffer);
}

/// \brief Convert the header along with accompanying data to the RDM wire format.
/// \param[out] buf Data buffer to hold the serialized command.
/// \param[out] buf_len Length in bytes of buf.
/// \param[in] data RDM parameter data that accompanies this header, if any.
/// \param[in] data_len Length of parameter data.
/// \return Result of serialize operation.
inline etcpal::Error CommandHeader::Serialize(uint8_t* buf, size_t buf_len, const uint8_t* data, uint8_t data_len) const
    noexcept
{
  return rdm_create_command_with_custom_buf(&cmd_header_, data, data_len, buf, buf_len);
}

/// \brief Create a header for an RDM GET_COMMAND.
/// \param param_id The RDM parameter ID (PID) of this command.
/// \param source_uid The RDM UID of the controller generating this command.
/// \param dest_uid The RDM UID of the responder to which this command is addressed.
/// \param subdevice The sub-device to which this command is addressed, or 0 for the root device.
/// \param transaction_num The RDM transaction number (should increase monotonically with each command).
/// \param port_id The port ID of the port on which this command is being sent.
inline CommandHeader CommandHeader::Get(uint16_t param_id, const Uid& source_uid, const Uid& dest_uid,
                                        uint16_t subdevice, uint8_t transaction_num, uint8_t port_id) noexcept
{
  return CommandHeader(source_uid, dest_uid, transaction_num, port_id, subdevice, kRdmCCGetCommand, param_id);
}

/// \brief Create a header for an RDM SET_COMMAND.
/// \param param_id The RDM parameter ID (PID) of this command.
/// \param source_uid The RDM UID of the controller generating this command.
/// \param dest_uid The RDM UID of the responder to which this command is addressed.
/// \param subdevice The sub-device to which this command is addressed, or 0 for the root device.
/// \param transaction_num The RDM transaction number (should increase monotonically with each command).
/// \param port_id The port ID of the port on which this command is being sent.
inline CommandHeader CommandHeader::Set(uint16_t param_id, const Uid& source_uid, const Uid& dest_uid,
                                        uint16_t subdevice, uint8_t transaction_num, uint8_t port_id) noexcept
{
  return CommandHeader(source_uid, dest_uid, transaction_num, port_id, subdevice, kRdmCCSetCommand, param_id);
}

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
inline Command::Command(const CommandHeader& header, const uint8_t* data = nullptr, uint8_t data_len = 0)
    : header_(header)
{
  SetData(data, data_len);
}

/// \brief Construct a command from a header and data.
/// \param header Header to assign to this command.
/// \param data The RDM parameter data of this command (nullptr for commands with no data).
/// \param data_len The length of the RDM parameter data (0 for commands with no data).
inline Command::Command(const ::RdmCommandHeader& header, const uint8_t* data = nullptr, uint8_t data_len = 0)
    : header_(header)
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
inline const uint8_t* Command::data() const noexcept
{
  return data_.data();
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
  return data_.size() + RDM_HEADER_SIZE;
}

/// \brief Convert the command to the RDM wire format.
/// \param[out] buffer RdmBuffer struct to hold the serialized command.
/// \return Result of serialize operation.
inline etcpal::Error Command::Serialize(RdmBuffer& buffer) const noexcept
{
  return Serialize(buffer.data, RDM_MAX_PDL);
}

/// \brief Convert the command to the RDM wire format.
/// \param[out] buf Data buffer to hold the serialized command.
/// \param[out] buf_len Length in bytes of buf.
/// \return Result of serialize operation.
inline etcpal::Error Command::Serialize(uint8_t* buf, size_t buf_len) const noexcept
{
  if (!data_.empty())
  {
    return header_.Serialize(buf, buf_len, data_.data(), static_cast<uint8_t>(data_.size()));
  }
  else
  {
    return header_.Serialize(buf, buf_len);
  }
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

/// \ingroup cpp_message
/// \brief A class that represents the header fields of an RDM response.
///
/// Includes everything except the RDM parameter data. RDM responses are typically created within
/// RDM libraries rather than built manually, so this class's API is optimized for controllers
/// reading responses rather than responders generating them.
///
/// Example usage:
/// \code
/// // Default-constructed responses are not valid
/// rdm::ResponseHeader invalid_resp;
/// EXPECT_FALSE(invalid_resp.IsValid());
///
/// // Create a valid response
/// rdm::ResponseHeader set_resp(rdm::Uid(0x6574, 0x87654321), rdm::Uid(0x6574, 0x87654321), 0x22,
/// kRdmResponseTypeAck,
///                              0, 0, kRdmCCSetCommandResponse, E120_IDENTIFY_DEVICE);
///
/// EXPECT_TRUE(set_resp.IsValid());
/// EXPECT_TRUE(set_resp.IsSetResponse());
/// EXPECT_TRUE(set_resp.IsAck());
/// \endcode
class ResponseHeader
{
public:
  /// Construct an invalid response header by default.
  ResponseHeader() = default;
  constexpr ResponseHeader(const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num,
                           rdm_response_type_t response_type, uint8_t msg_count, uint16_t subdevice,
                           rdm_command_class_t command_class, uint16_t param_id) noexcept;

  constexpr ResponseHeader(const ::RdmResponseHeader& c_header) noexcept;
  ResponseHeader& operator=(const ::RdmResponseHeader& c_header) noexcept;

  constexpr Uid source_uid() const noexcept;
  constexpr Uid dest_uid() const noexcept;
  constexpr uint8_t transaction_num() const noexcept;
  constexpr rdm_response_type_t response_type() const noexcept;
  constexpr uint8_t message_count() const noexcept;
  constexpr uint16_t subdevice() const noexcept;
  constexpr rdm_command_class_t command_class() const noexcept;
  constexpr uint16_t param_id() const noexcept;

  constexpr const ::RdmResponseHeader& get() const noexcept;
  ETCPAL_CONSTEXPR_14 ::RdmResponseHeader& get() noexcept;

  bool IsValid() const noexcept;

  /// \name Response Type Inspectors
  /// @{
  constexpr bool IsAck() const noexcept;
  constexpr bool IsAckTimer() const noexcept;
  constexpr bool IsAckOverflow() const noexcept;
  constexpr bool IsNack() const noexcept;
  /// @}

  /// \name Command Class Inspectors
  /// @{
  constexpr bool IsGetResponse() const noexcept;
  constexpr bool IsSetResponse() const noexcept;
  constexpr bool IsDiscoveryResponse() const noexcept;
  /// @}

  ResponseHeader& SetSourceUid(const Uid& uid) noexcept;
  ResponseHeader& SetSourceUid(const ::RdmUid& uid) noexcept;
  ResponseHeader& SetDestUid(const Uid& uid) noexcept;
  ResponseHeader& SetDestUid(const ::RdmUid& uid) noexcept;
  ResponseHeader& SetTransactionNum(uint8_t transaction_num) noexcept;
  ResponseHeader& SetResponseType(rdm_response_type_t response_type) noexcept;
  ResponseHeader& SetMessageCount(uint8_t msg_count) noexcept;
  ResponseHeader& SetSubdevice(uint16_t subdevice) noexcept;
  ResponseHeader& SetCommandClass(rdm_command_class_t command_class) noexcept;
  ResponseHeader& SetParamId(uint16_t param_id) noexcept;

private:
  ::RdmResponseHeader resp_header_;
};

/// \brief Construct a response header from its component values.
/// \param source_uid The RDM UID of the responder generating this response.
/// \param dest_uid The RDM UID of the controller to which this response is addressed.
/// \param transaction_num The RDM transaction number, copied from the corresponding command.
/// \param response_type The RDM response type, indicating the response status.
/// \param msg_count Current count of queued messages waiting to be retrieved.
/// \param subdevice The sub-device generating this response, or 0 for the root device.
/// \param command_class The RDM command class for this response.
/// \param param_id The RDM parameter ID (PID) of this response.
constexpr ResponseHeader::ResponseHeader(const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num,
                                         rdm_response_type_t response_type, uint8_t msg_count, uint16_t subdevice,
                                         rdm_command_class_t command_class, uint16_t param_id) noexcept
    : resp_header_{source_uid.get(), dest_uid.get(), transaction_num, response_type,
                   msg_count,        subdevice,      command_class,   param_id}
{
}

/// \brief Construct a response header copied from an instance of the C RdmResponseHeader type.
constexpr ResponseHeader::ResponseHeader(const ::RdmResponseHeader& c_header) noexcept : resp_header_(c_header)
{
}

/// \brief Assign an instance of the C RdmResponseHeader type to an instance of this class.
inline ResponseHeader& ResponseHeader::operator=(const ::RdmResponseHeader& c_header) noexcept
{
  resp_header_ = c_header;
  return *this;
}

/// \brief Get the UID of the responder generating the response.
constexpr Uid ResponseHeader::source_uid() const noexcept
{
  return resp_header_.source_uid;
}

/// \brief Get the UID of the controller to which the response is addressed.
constexpr Uid ResponseHeader::dest_uid() const noexcept
{
  return resp_header_.dest_uid;
}

/// \brief Get the transaction number of the response.
constexpr uint8_t ResponseHeader::transaction_num() const noexcept
{
  return resp_header_.transaction_num;
}

/// \brief Get the RDM response type of the response.
constexpr rdm_response_type_t ResponseHeader::response_type() const noexcept
{
  return resp_header_.resp_type;
}

/// \brief Get the current count of queued messages being stored by the responder.
constexpr uint8_t ResponseHeader::message_count() const noexcept
{
  return resp_header_.msg_count;
}

/// \brief Get the sub-device from which the response was generated (0 means the root device).
constexpr uint16_t ResponseHeader::subdevice() const noexcept
{
  return resp_header_.subdevice;
}

/// \brief Get the response's command class.
constexpr rdm_command_class_t ResponseHeader::command_class() const noexcept
{
  return resp_header_.command_class;
}

/// \brief Get the RDM Parameter ID of the response.
constexpr uint16_t ResponseHeader::param_id() const noexcept
{
  return resp_header_.param_id;
}

/// \brief Get a const reference to the underlying C type.
constexpr const ::RdmResponseHeader& ResponseHeader::get() const noexcept
{
  return resp_header_;
}

/// \brief Get a mutable reference to the underlying C type.
ETCPAL_CONSTEXPR_14_OR_INLINE ::RdmResponseHeader& ResponseHeader::get() noexcept
{
  return resp_header_;
}

/// \brief Whether the values contained in this response header are valid for an RDM response.
inline bool ResponseHeader::IsValid() const noexcept
{
  return rdm_response_header_is_valid(&resp_header_);
}

/// \brief Whether this header represents an RDM ACK response.
constexpr bool ResponseHeader::IsAck() const noexcept
{
  return resp_header_.resp_type == kRdmResponseTypeAck;
}

/// \brief Whether this header represents an RDM ACK_TIMER response.
constexpr bool ResponseHeader::IsAckTimer() const noexcept
{
  return resp_header_.resp_type == kRdmResponseTypeAckTimer;
}

/// \brief Whether this header represents an RDM ACK_OVERFLOW response.
constexpr bool ResponseHeader::IsAckOverflow() const noexcept
{
  return resp_header_.resp_type == kRdmResponseTypeAckOverflow;
}

/// \brief Whether this header represents an RDM NACK_REASON response.
constexpr bool ResponseHeader::IsNack() const noexcept
{
  return resp_header_.resp_type == kRdmResponseTypeNackReason;
}

/// \brief Whether this header represents an RDM GET_COMMAND_RESPONSE.
constexpr bool ResponseHeader::IsGetResponse() const noexcept
{
  return resp_header_.command_class == kRdmCCGetCommandResponse;
}

/// \brief Whether this header represents an RDM SET_COMMAND_RESPONSE.
constexpr bool ResponseHeader::IsSetResponse() const noexcept
{
  return resp_header_.command_class == kRdmCCSetCommandResponse;
}

/// \brief Whether this header represents an RDM DISCOVERY_COMMAND_RESPONSE.
constexpr bool ResponseHeader::IsDiscoveryResponse() const noexcept
{
  return resp_header_.command_class == kRdmCCDiscoveryCommandResponse;
}

/// \brief Set the UID of the responder generating the response.
inline ResponseHeader& ResponseHeader::SetSourceUid(const Uid& uid) noexcept
{
  return SetSourceUid(uid.get());
}

/// \brief Set the UID of the responder generating the response.
inline ResponseHeader& ResponseHeader::SetSourceUid(const ::RdmUid& uid) noexcept
{
  resp_header_.source_uid = uid;
  return *this;
}

/// \brief Set the UID of the controller to which the response is addressed.
inline ResponseHeader& ResponseHeader::SetDestUid(const Uid& uid) noexcept
{
  return SetDestUid(uid.get());
}

/// \brief Set the UID of the controller to which the response is addressed.
inline ResponseHeader& ResponseHeader::SetDestUid(const ::RdmUid& uid) noexcept
{
  resp_header_.dest_uid = uid;
  return *this;
}

/// \brief Set the transaction number of the response.
inline ResponseHeader& ResponseHeader::SetTransactionNum(uint8_t transaction_num) noexcept
{
  resp_header_.transaction_num = transaction_num;
  return *this;
}

/// \brief Set the RDM response type of the response.
inline ResponseHeader& ResponseHeader::SetResponseType(rdm_response_type_t response_type) noexcept
{
  resp_header_.resp_type = response_type;
  return *this;
}

/// \brief Set the current count of queued messages being stored by the responder.
inline ResponseHeader& ResponseHeader::SetMessageCount(uint8_t msg_count) noexcept
{
  resp_header_.msg_count = msg_count;
  return *this;
}

/// \brief Set the sub-device from which the response was generated (0 means the root device).
inline ResponseHeader& ResponseHeader::SetSubdevice(uint16_t subdevice) noexcept
{
  resp_header_.subdevice = subdevice;
  return *this;
}

/// \brief Set the response's command class.
inline ResponseHeader& ResponseHeader::SetCommandClass(rdm_command_class_t command_class) noexcept
{
  resp_header_.command_class = command_class;
  return *this;
}

/// \brief Set the RDM Parameter ID of the response.
///
/// Valid values are listed in ANSI E1.20 Table A-3, any of the relevant E1.37 extension standards,
/// ANSI E1.33 Table A-15, or manufacturer-specific values in the range 0x8000 - 0xffdf.
inline ResponseHeader& ResponseHeader::SetParamId(uint16_t param_id) noexcept
{
  resp_header_.param_id = param_id;
  return *this;
}

/// \ingroup cpp_message
/// \brief A class representing an RDM response.
///
/// Similar to ResponseHeader, except that this class heap-allocates a buffer to hold any
/// accompanying RDM parameter data and thus represents a complete RDM response. Furthermore, this
/// class is capable of holding a reassembled RDM ACK_OVERFLOW response and thus can have parameter
/// data in excess of RDM_MAX_PDL.
///
/// RDM responses are typically created within RDM libraries rather than built manually, so this
/// class's API is optimized for controllers reading responses rather than responders generating
/// them.
///
/// Example Usage:
/// \code
/// void HandleRdmResponse(const rdm::Response& response)
/// {
///   if ((response.IsAck() || response.IsAckOverflow()) && response.HasData())
///   {
///     HandleRdmResponseData(response.param_id(), response.data(), response.data_len());
///     if (response.IsAckOverflow())
///     {
///       // Queue another GET_COMMAND to get the next ACK_OVERFLOW response...
///     }
///   }
///   else if (response.IsAckTimer())
///   {
///     // Start the timer to wait before re-requesting data.
///     etcpal::Timer retry_timer(response.AckTimerDelayMs().value());
///     // Store the timer somewhere...
///   }
///   else if (response.IsNack())
///   {
///     rdm::NackReason nack_reason = response.NackReason().value();
///     if (nack_reason.IsStandard())
///     {
///       Log("Got RDM NACK with reason code %d, description '%s'", nack_reason.code(), nack_reason.ToCString());
///     }
///     else
///     {
///       Log("Got RDM NACK with manufacturer-specific reason code %u", nack_reason.raw_code());
///     }
///   }
/// }
/// \endcode
class Response
{
public:
  /// Construct an invalid response by default.
  Response() = default;
  Response(const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num, rdm_response_type_t response_type,
           uint8_t msg_count, uint16_t subdevice, rdm_command_class_t command_class, uint16_t param_id,
           const uint8_t* data = nullptr, size_t data_len = 0);
  Response(const ResponseHeader& header, const uint8_t* data = nullptr, size_t data_len = 0);
  Response(const ::RdmResponseHeader& header, const uint8_t* data = nullptr, size_t data_len = 0);

  constexpr Uid source_uid() const noexcept;
  constexpr Uid dest_uid() const noexcept;
  constexpr uint8_t transaction_num() const noexcept;
  constexpr rdm_response_type_t response_type() const noexcept;
  constexpr uint8_t message_count() const noexcept;
  constexpr uint16_t subdevice() const noexcept;
  constexpr rdm_command_class_t command_class() const noexcept;
  constexpr uint16_t param_id() const noexcept;

  constexpr const ResponseHeader& header() const noexcept;
  ETCPAL_CONSTEXPR_14 ResponseHeader& header() noexcept;
  const uint8_t* data() const noexcept;
  size_t data_len() const noexcept;

  bool IsValid() const noexcept;
  bool HasData() const noexcept;

  /// \name Response Type Inspectors
  /// @{
  constexpr bool IsAck() const noexcept;
  constexpr bool IsAckTimer() const noexcept;
  constexpr bool IsAckOverflow() const noexcept;
  constexpr bool IsNack() const noexcept;
  /// @}

  /// \name Command Class Inspectors
  /// @{
  constexpr bool IsGetResponse() const noexcept;
  constexpr bool IsSetResponse() const noexcept;
  constexpr bool IsDiscoveryResponse() const noexcept;
  /// @}

  etcpal::Expected<unsigned int> AckTimerDelayMs() const noexcept;
  etcpal::Expected<NackReason> NackReason() const noexcept;

  Response& SetSourceUid(const Uid& uid) noexcept;
  Response& SetSourceUid(const ::RdmUid& uid) noexcept;
  Response& SetDestUid(const Uid& uid) noexcept;
  Response& SetDestUid(const ::RdmUid& uid) noexcept;
  Response& SetTransactionNum(uint8_t transaction_num) noexcept;
  Response& SetResponseType(rdm_response_type_t response_type) noexcept;
  Response& SetMessageCount(uint8_t msg_count) noexcept;
  Response& SetSubdevice(uint16_t subdevice) noexcept;
  Response& SetCommandClass(rdm_command_class_t command_class) noexcept;
  Response& SetParamId(uint16_t param_id) noexcept;
  Response& SetHeader(const ResponseHeader& header) noexcept;
  Response& SetData(const uint8_t* data, size_t data_len);
  Response& ClearData() noexcept;

private:
  ResponseHeader header_;
  std::vector<uint8_t> data_;
};

/// \brief Construct a response from its component values.
/// \param source_uid The RDM UID of the responder generating this response.
/// \param dest_uid The RDM UID of the controller to which this response is addressed.
/// \param transaction_num The RDM transaction number (copied from the corresponding command).
/// \param response_type The RDM response type of this response.
/// \param msg_count The current count of queued messages being stored by the responder.
/// \param subdevice The sub-device from which the response was generated, or 0 for the root device.
/// \param command_class The RDM command class for this response.
/// \param param_id The RDM parameter ID (PID) of this response.
/// \param data The RDM parameter data of this response (nullptr for responses with no data).
/// \param data_len The length of the RDM parameter data (0 for responses with no data).
inline Response::Response(const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num,
                          rdm_response_type_t response_type, uint8_t msg_count, uint16_t subdevice,
                          rdm_command_class_t command_class, uint16_t param_id, const uint8_t* data, size_t data_len)
    : header_(source_uid, dest_uid, transaction_num, response_type, msg_count, subdevice, command_class, param_id)
{
  SetData(data, data_len);
}

/// \brief Construct a response from a header and data.
/// \param header Header to assign to this response.
/// \param data The RDM parameter data of this response (nullptr for responses with no data).
/// \param data_len The length of the RDM parameter data (0 for responses with no data).
inline Response::Response(const ResponseHeader& header, const uint8_t* data = nullptr, size_t data_len = 0)
    : header_(header)
{
  SetData(data, data_len);
}

/// \brief Construct a response from a header and data.
/// \param header Header to assign to this response.
/// \param data The RDM parameter data of this response (nullptr for responses with no data).
/// \param data_len The length of the RDM parameter data (0 for responses with no data).
inline Response::Response(const ::RdmResponseHeader& header, const uint8_t* data = nullptr, size_t data_len = 0)
    : header_(header)
{
  SetData(data, data_len);
}

/// \brief Get the UID of the responder generating the response.
constexpr Uid Response::source_uid() const noexcept
{
  return header_.source_uid();
}

/// \brief Get the UID of the controller to which the response is addressed.
constexpr Uid Response::dest_uid() const noexcept
{
  return header_.dest_uid();
}

/// \brief Get the transaction number of the response.
constexpr uint8_t Response::transaction_num() const noexcept
{
  return header_.transaction_num();
}

/// \brief Get the RDM response type of the response.
constexpr rdm_response_type_t Response::response_type() const noexcept
{
  return header_.response_type();
}

/// \brief Get the current count of queued messages being stored by the responder.
constexpr uint8_t Response::message_count() const noexcept
{
  return header_.message_count();
}

/// \brief Get the sub-device from which the response was generated (0 means the root device).
constexpr uint16_t Response::subdevice() const noexcept
{
  return header_.subdevice();
}

/// \brief Get the response's command class.
constexpr rdm_command_class_t Response::command_class() const noexcept
{
  return header_.command_class();
}

/// \brief Get the RDM Parameter ID of the response.
constexpr uint16_t Response::param_id() const noexcept
{
  return header_.param_id();
}

/// \brief Get a const reference to the response's header.
constexpr const ResponseHeader& Response::header() const noexcept
{
  return header_;
}

/// \brief Get a mutable reference to the response's header.
ETCPAL_CONSTEXPR_14_OR_INLINE ResponseHeader& Response::header() noexcept
{
  return header_;
}

/// \brief Get a pointer to the response's parameter data.
inline const uint8_t* Response::data() const noexcept
{
  return data_.data();
}

/// \brief Get the length of the response's parameter data.
inline size_t Response::data_len() const noexcept
{
  return data_.size();
}

/// \brief Whether the values contained in this response are valid for an RDM response.
inline bool Response::IsValid() const noexcept
{
  return header_.IsValid();
}

/// \brief Whether this response has parameter data associated with it.
inline bool Response::HasData() const noexcept
{
  return !data_.empty();
}

/// \brief Whether this is an RDM ACK response.
constexpr bool Response::IsAck() const noexcept
{
  return header_.IsAck();
}

/// \brief Whether this is an RDM ACK_TIMER response.
constexpr bool Response::IsAckTimer() const noexcept
{
  return header_.IsAckTimer();
}

/// \brief Whether this is an RDM ACK_OVERFLOW response.
constexpr bool Response::IsAckOverflow() const noexcept
{
  return header_.IsAckOverflow();
}

/// \brief Whether this is an RDM NACK_REASON response.
constexpr bool Response::IsNack() const noexcept
{
  return header_.IsNack();
}

/// \brief Whether this is an RDM GET_COMMAND_RESPONSE
constexpr bool Response::IsGetResponse() const noexcept
{
  return header_.IsGetResponse();
}

/// \brief Whether this is an RDM SET_COMMAND_RESPONSE
constexpr bool Response::IsSetResponse() const noexcept
{
  return header_.IsSetResponse();
}

/// \brief Whether this is an RDM DISCOVERY_COMMAND_RESPONSE
constexpr bool Response::IsDiscoveryResponse() const noexcept
{
  return header_.IsDiscoveryResponse();
}

/// \brief Get the ACK_TIMER delay from an ACK_TIMER response.
/// \return The delay time in milliseconds, if this is an ACK_TIMER response with valid data.
/// \return #kEtcPalErrInvalid otherwise.
inline etcpal::Expected<unsigned int> Response::AckTimerDelayMs() const noexcept
{
  if (IsAckTimer() && data_.size() == 2)
  {
    return rdm_get_ack_timer_delay(data_.data());
  }
  return kEtcPalErrInvalid;
}

/// \brief Get the NACK reason code from an NACK_REASON response.
/// \return The NACK reason, if this is a NACK_REASON response with valid data.
/// \return #kEtcPalErrInvalid otherwise.
inline etcpal::Expected<NackReason> Response::NackReason() const noexcept
{
  if (IsNack() && data_.size() == 2)
  {
    return rdm_get_nack_reason_code(data_.data());
  }
  return kEtcPalErrInvalid;
}

/// \brief Set the UID of the responder generating the response.
inline Response& Response::SetSourceUid(const Uid& uid) noexcept
{
  header_.SetSourceUid(uid);
  return *this;
}

/// \brief Set the UID of the responder generating the response.
inline Response& Response::SetSourceUid(const ::RdmUid& uid) noexcept
{
  header_.SetSourceUid(uid);
  return *this;
}

/// \brief Set the UID of the controller to which the response is addressed.
inline Response& Response::SetDestUid(const Uid& uid) noexcept
{
  header_.SetDestUid(uid);
  return *this;
}

/// \brief Set the UID of the controller to which the response is addressed.
inline Response& Response::SetDestUid(const ::RdmUid& uid) noexcept
{
  header_.SetDestUid(uid);
  return *this;
}

/// \brief Set the transaction number of the response.
inline Response& Response::SetTransactionNum(uint8_t transaction_num) noexcept
{
  header_.SetTransactionNum(transaction_num);
  return *this;
}

/// \brief Set the RDM response type of the response.
inline Response& Response::SetResponseType(rdm_response_type_t response_type) noexcept
{
  header_.SetResponseType(response_type);
  return *this;
}

/// \brief Set the current count of queued messages being stored by the responder.
inline Response& Response::SetMessageCount(uint8_t msg_count) noexcept
{
  header_.SetMessageCount(msg_count);
  return *this;
}

/// \brief Set the sub-device from which the response was generated (0 means the root device).
inline Response& Response::SetSubdevice(uint16_t subdevice) noexcept
{
  header_.SetSubdevice(subdevice);
  return *this;
}

/// \brief Set the response's command class.
inline Response& Response::SetCommandClass(rdm_command_class_t command_class) noexcept
{
  header_.SetCommandClass(command_class);
  return *this;
}

/// \brief Set the RDM Parameter ID of the response.
///
/// Valid values are listed in ANSI E1.20 Table A-3, any of the relevant E1.37 extension standards,
/// ANSI E1.33 Table A-15, or manufacturer-specific values in the range 0x8000 - 0xffdf.
inline Response& Response::SetParamId(uint16_t param_id) noexcept
{
  header_.SetParamId(param_id);
  return *this;
}

/// \brief Replace the response's header with a new header.
inline Response& Response::SetHeader(const ResponseHeader& header) noexcept
{
  header_ = header;
  return *this;
}

/// \brief Set the parameter data associated with this response.
inline Response& Response::SetData(const uint8_t* data, size_t data_len)
{
  if (data && data_len)
    data_.assign(data, data + data_len);
  return *this;
}

/// \brief Remove the parameter data associated with this response.
inline Response& Response::ClearData() noexcept
{
  data_.clear();
  return *this;
}

};  // namespace rdm

#endif  // RDM_CPP_MESSAGE_H_
