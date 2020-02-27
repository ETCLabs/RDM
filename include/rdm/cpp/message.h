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
constexpr NackReason::NackReason(uint16_t code) noexcept : code_(code)
{
}

/// \brief Assign a raw code value to this NackReason.
inline NackReason& NackReason::operator=(uint16_t code) noexcept
{
  code_ = code;
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
/// \return The string representation, or an empty string if IsStandard() == false.
inline std::string NackReason::ToString() const noexcept
{
  return rdm_nack_reason_to_string(code_);
}

/// \brief Get a descriptive string for this NACK reason code as a C string.
/// \return The string representation, or nullptr if IsStandard() == false.
inline const char* NackReason::ToCString() const noexcept
{
  return rdm_nack_reason_to_string(code_);
}

/// \brief Whether this instance contains a standard RDM NACK reason code.
constexpr bool NackReason::IsStandard() const noexcept
{
  return code_ < kRdmMaxStandardNRCodes;
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
/// rdm::CommandHeader get_cmd(kRdmCCGetCommand, E120_SUPPORTED_PARAMETERS, rdm::Uid(0x6574, 0x12345678),
///                            rdm::Uid(0x6574, 0x87654321), 0x22, 1);
///
/// EXPECT_TRUE(get_cmd.IsValid());
/// EXPECT_TRUE(get_cmd.IsGet());
///
/// // Create a packed on-wire representation
/// RdmBuffer cmd_buf;
/// get_cmd.Serialize(cmd_buf);
///
/// // For a command that has parameter data, supply the parameter data when serializing:
/// rdm::CommandHeader set_cmd(kRdmCCSetCommand, E120_IDENTIFY_DEVICE, rdm::Uid(0x6574, 0x12345678),
///                            rdm::Uid(0x6574, 0x87654321), 0x23, 1);
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
  constexpr CommandHeader(rdm_command_class_t command_class, uint16_t param_id, const Uid& source_uid,
                          const Uid& dest_uid, uint8_t transaction_num, uint8_t port_id, uint16_t subdevice = 0);

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
  CommandHeader& SetSourceUid(const RdmUid& uid) noexcept;
  CommandHeader& SetDestUid(const Uid& uid) noexcept;
  CommandHeader& SetDestUid(const RdmUid& uid) noexcept;
  CommandHeader& SetTransactionNum(uint8_t transaction_num) noexcept;
  CommandHeader& SetPortId(uint8_t port_id) noexcept;
  CommandHeader& SetSubdevice(uint16_t subdevice) noexcept;
  CommandHeader& SetCommandClass(rdm_command_class_t command_class) noexcept;
  CommandHeader& SetParamId(uint16_t param_id) noexcept;

  constexpr size_t PackedSize(uint8_t datalen = 0) const noexcept;
  etcpal::Error Serialize(RdmBuffer& buffer, const uint8_t* data = nullptr, uint8_t datalen = 0) const noexcept;
  // etcpal::Error ToBytes(uint8_t* buf, size_t buflen, const uint8_t* data = nullptr, uint8_t datalen = 0) const
  // noexcept; std::vector<uint8_t> ToBytes(const uint8_t* data = nullptr, uint8_t datalen = 0) const;

private:
  ::RdmCommandHeader cmd_header_{};
};

/// \brief Construct a command header from its component values.
constexpr CommandHeader::CommandHeader(rdm_command_class_t command_class, uint16_t param_id, const Uid& source_uid,
                                       const Uid& dest_uid, uint8_t transaction_num, uint8_t port_id,
                                       uint16_t subdevice)
    : cmd_header_{source_uid.get(), dest_uid.get(), transaction_num, port_id, subdevice, command_class, param_id}
{
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
///
/// Valid values are listed in ANSI E1.20 Table A-3, any of the relevant E1.37 extension standards,
/// ANSI E1.33 Table A-15, or manufacturer-specific values in the range 0x8000 - 0xffdf.
constexpr uint16_t CommandHeader::param_id() const noexcept
{
  return cmd_header_.param_id;
}

/// \brief Get a const reference to the underlying C type.
constexpr const RdmCommandHeader& CommandHeader::get() const noexcept
{
  return cmd_header_;
}

/// \brief Get a mutable reference to the underlying C type.
ETCPAL_CONSTEXPR_14_OR_INLINE RdmCommandHeader& CommandHeader::get() noexcept
{
  return cmd_header_;
}

/// \brief Whether the values contained in this command header are valid for an RDM command.
inline bool CommandHeader::IsValid() const noexcept
{
  return (!RDM_UID_IS_BROADCAST(&cmd_header_.source_uid) && (cmd_header_.port_id != 0) &&
          (IsGet() || IsSet() || IsDiscovery()));
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

inline CommandHeader& CommandHeader::SetCommandClass(rdm_command_class_t command_class) noexcept
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

/// \brief Convert the header along with accompanying data to the RDM wire format.
/// \param[out] buffer Buffer to hold the serialized command.
/// \param[in] data RDM parameter data that accompanies this header, if any.
/// \param[in] data_len Length of parameter data.
/// \return Result of serialize operation.
inline etcpal::Error CommandHeader::Serialize(RdmBuffer& buffer, const uint8_t* data, uint8_t data_len) const noexcept
{
  return rdm_create_command(&cmd_header_, data, data_len, &buffer);
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
  constexpr const std::vector<uint8_t>& data() const noexcept;

  constexpr bool IsValid() const noexcept;
  bool HasData() const noexcept;

  constexpr bool IsGet() const noexcept;
  constexpr bool IsSet() const noexcept;
  constexpr bool IsDiscovery() const noexcept;

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
  Command& SetData(const std::vector<uint8_t>& data) noexcept;
  Command& SetData(std::vector<uint8_t>&& data) noexcept;

  constexpr size_t PackedSize() const noexcept;
  bool Serialize(uint8_t* buf, size_t buflen) const noexcept;
  bool Serialize(RdmBuffer& buffer) const noexcept;
  std::vector<uint8_t> Serialize() const;

private:
  CommandHeader header_;
  std::vector<uint8_t> data_;
};

constexpr Uid Command::source_uid() const noexcept
{
}

constexpr Uid Command::dest_uid() const noexcept
{
}

constexpr uint8_t Command::transaction_num() const noexcept
{
}

constexpr uint8_t Command::port_id() const noexcept
{
}

constexpr uint16_t Command::subdevice() const noexcept
{
}

constexpr rdm_command_class_t Command::command_class() const noexcept
{
}

constexpr uint16_t Command::param_id() const noexcept
{
}

constexpr CommandHeader& Command::header() const noexcept
{
}

constexpr const std::vector<uint8_t>& Command::data() const noexcept
{
}

inline bool Command::IsValid() const noexcept
{
}

inline bool Command::HasData() const noexcept
{
}

constexpr bool Command::IsGet() const noexcept
{
}

constexpr bool Command::IsSet() const noexcept
{
}

constexpr bool Command::IsDiscovery() const noexcept
{
}

inline Command& Command::SetSourceUid(const Uid& uid) noexcept
{
}

inline Command& Command::SetSourceUid(const RdmUid& uid) noexcept
{
}

inline Command& Command::SetDestUid(const Uid& uid) noexcept
{
}

inline Command& Command::SetDestUid(const RdmUid& uid) noexcept
{
}

inline Command& Command::SetTransactionNum(uint8_t transaction_num) noexcept
{
}

inline Command& Command::SetPortId(uint8_t port_id) noexcept
{
}

inline Command& Command::SetSubdevice(uint16_t subdevice) noexcept
{
}

inline Command& Command::SetCommandClass(rdm_command_class_t command_class) noexcept
{
}

inline Command& Command::SetParamId(uint16_t param_id) noexcept
{
}

inline Command& Command::SetHeader(const CommandHeader& header) noexcept
{
}

inline Command& Command::SetData(const uint8_t* data, size_t size) noexcept
{
}

inline Command& Command::SetData(const std::vector<uint8_t>& data) noexcept
{
}

inline Command& Command::SetData(std::vector<uint8_t>&& data) noexcept
{
}

constexpr size_t Command::PackedSize() const noexcept
{
}

inline bool Command::Serialize(uint8_t* buf, size_t buflen) const noexcept
{
}

inline bool Command::Serialize(RdmBuffer& buffer) const noexcept
{
}

inline std::vector<uint8_t> Command::Serialize() const
{
}

class ResponseHeader
{
public:
  ResponseHeader() = default;
  constexpr ResponseHeader(rdm_command_class_t command_class, rdm_response_type_t response_type, uint16_t param_id,
                           const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num, uint16_t subdevice = 0,
                           uint8_t msg_count = 0);

  constexpr ResponseHeader(const ::RdmResponseHeader& c_header) noexcept;
  ResponseHeader& operator=(const ::RdmResponseHeader& c_header) noexcept;

  Uid source_uid() const noexcept;
  Uid dest_uid() const noexcept;
  uint8_t transaction_num() const noexcept;
  rdm_response_type_t response_type() const noexcept;
  uint8_t message_count() const noexcept;
  uint16_t subdevice() const noexcept;
  rdm_command_class_t command_class() const noexcept;
  uint16_t param_id() const noexcept;

  ETCPAL_CONSTEXPR_14 ::RdmResponseHeader& get() noexcept;
  constexpr const ::RdmResponseHeader& get() const noexcept;

  constexpr bool IsValid() const noexcept;

  constexpr bool IsAck() const noexcept;
  constexpr bool IsAckTimer() const noexcept;
  constexpr bool IsAckOverflow() const noexcept;
  constexpr bool IsNack() const noexcept;

  constexpr bool IsGetResponse() const noexcept;
  constexpr bool IsSetResponse() const noexcept;
  constexpr bool IsDiscoveryResponse() const noexcept;

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
  ::RdmResponseHeader resp_header_;
};

/// \ingroup cpp_message
/// \brief A class representing an RDM response.
///
/// Similar to ResponseHeader, except that this class heap-allocates a buffer to hold any
/// accompanying RDM parameter data and thus represents a complete RDM response. Furthermore, this
/// class is capable of holding a reassembled RDM ACK_OVERFLOW response and thus can have parameter
/// data in excess of RDM_MAX_PDL.
class Response
{
public:
  /// Construct an invalid response by default.
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
  bool HasData() const noexcept;

  constexpr bool IsAck() const noexcept;
  constexpr bool IsAckTimer() const noexcept;
  constexpr bool IsAckOverflow() const noexcept;
  constexpr bool IsNack() const noexcept;

  constexpr bool IsGetResponse() const noexcept;
  constexpr bool IsSetResponse() const noexcept;
  constexpr bool IsDiscoveryResponse() const noexcept;

  etcpal::Expected<unsigned int> AckTimerDelayMs() const noexcept;
  etcpal::Expected<rdm_nack_reason_t> NackReasonCode() const noexcept;
  etcpal::Expected<NackReason> NackReason() const noexcept;

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

private:
  ResponseHeader header_;
  std::vector<uint8_t> data_;
};

};  // namespace rdm

#endif  // RDM_CPP_MESSAGE_H_
