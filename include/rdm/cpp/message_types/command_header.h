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

/// @file rdm/cpp/message_types/command_header.h
/// @brief Definitions for the RDM command header type.

#ifndef RDM_CPP_MESSAGE_TYPES_COMMAND_HEADER_H_
#define RDM_CPP_MESSAGE_TYPES_COMMAND_HEADER_H_

#include <cstdint>
#include "etcpal/cpp/common.h"
#include "etcpal/cpp/error.h"
#include "rdm/cpp/uid.h"
#include "rdm/message.h"

namespace rdm
{
/// @ingroup cpp_message
/// @brief A class that represents the header fields of an RDM command.
///
/// Includes everything except the RDM parameter data. Example usage:
/// @code
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
/// @endcode
class CommandHeader
{
public:
  /// Construct an invalid command header by default.
  CommandHeader() = default;
  constexpr CommandHeader(const Uid&          source_uid,
                          const Uid&          dest_uid,
                          uint8_t             transaction_num,
                          uint8_t             port_id,
                          uint16_t            subdevice,
                          rdm_command_class_t command_class,
                          uint16_t            param_id) noexcept;

  constexpr CommandHeader(const ::RdmCommandHeader& c_header) noexcept;
  CommandHeader& operator=(const ::RdmCommandHeader& c_header) noexcept;

  constexpr Uid                 source_uid() const noexcept;
  constexpr Uid                 dest_uid() const noexcept;
  constexpr uint8_t             transaction_num() const noexcept;
  constexpr uint8_t             port_id() const noexcept;
  constexpr uint16_t            subdevice() const noexcept;
  constexpr rdm_command_class_t command_class() const noexcept;
  constexpr uint16_t            param_id() const noexcept;

  constexpr const ::RdmCommandHeader&     get() const noexcept;
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
  etcpal::Error    Serialize(RdmBuffer& buffer, const uint8_t* data = nullptr, uint8_t data_len = 0) const noexcept;
  etcpal::Error    Serialize(uint8_t* buf, size_t buf_len, const uint8_t* data = nullptr, uint8_t data_len = 0) const
      noexcept;

  /// @name High-level command header generators
  /// @{
  static CommandHeader Get(uint16_t   param_id,
                           const Uid& source_uid,
                           const Uid& dest_uid,
                           uint16_t   subdevice = 0,
                           uint8_t    transaction_num = 0,
                           uint8_t    port_id = 1) noexcept;
  static CommandHeader Set(uint16_t   param_id,
                           const Uid& source_uid,
                           const Uid& dest_uid,
                           uint16_t   subdevice = 0,
                           uint8_t    transaction_num = 0,
                           uint8_t    port_id = 1) noexcept;
  /// @}

private:
  ::RdmCommandHeader cmd_header_{};
};

/// @brief Construct a command header from its component values.
/// @param source_uid The RDM UID of the controller generating this command.
/// @param dest_uid The RDM UID of the responder to which this command is addressed.
/// @param transaction_num The RDM transaction number (should increase monotonically with each command).
/// @param port_id The port ID of the port on which this command is being sent.
/// @param subdevice The sub-device to which this command is addressed, or 0 for the root device.
/// @param command_class The RDM command class for this command.
/// @param param_id The RDM parameter ID (PID) of this command.
constexpr CommandHeader::CommandHeader(const Uid&          source_uid,
                                       const Uid&          dest_uid,
                                       uint8_t             transaction_num,
                                       uint8_t             port_id,
                                       uint16_t            subdevice,
                                       rdm_command_class_t command_class,
                                       uint16_t            param_id) noexcept
    : cmd_header_{source_uid.get(), dest_uid.get(), transaction_num, port_id, subdevice, command_class, param_id}
{
}

/// @brief Construct a command header copied from an instance of the C RdmCommandHeader type.
constexpr CommandHeader::CommandHeader(const ::RdmCommandHeader& c_header) noexcept : cmd_header_(c_header)
{
}

/// @brief Assign an instance of the C RdmCommandHeader type to an instance of this class.
inline CommandHeader& CommandHeader::operator=(const ::RdmCommandHeader& c_header) noexcept
{
  cmd_header_ = c_header;
  return *this;
}

/// @brief Get the UID of the controller generating the command.
constexpr Uid CommandHeader::source_uid() const noexcept
{
  return cmd_header_.source_uid;
}

/// @brief Get the UID of the responder to which the command is addressed.
constexpr Uid CommandHeader::dest_uid() const noexcept
{
  return cmd_header_.dest_uid;
}

/// @brief Get the transaction number of the command.
constexpr uint8_t CommandHeader::transaction_num() const noexcept
{
  return cmd_header_.transaction_num;
}

/// @brief Get the port ID of the port on which the command is being sent.
constexpr uint8_t CommandHeader::port_id() const noexcept
{
  return cmd_header_.port_id;
}

/// @brief Get the sub-device to which the command is addressed (0 means the root device).
constexpr uint16_t CommandHeader::subdevice() const noexcept
{
  return cmd_header_.subdevice;
}

/// @brief Get the command's command class.
constexpr rdm_command_class_t CommandHeader::command_class() const noexcept
{
  return cmd_header_.command_class;
}

/// @brief Get the RDM Parameter ID of the command.
constexpr uint16_t CommandHeader::param_id() const noexcept
{
  return cmd_header_.param_id;
}

/// @brief Get a const reference to the underlying C type.
constexpr const ::RdmCommandHeader& CommandHeader::get() const noexcept
{
  return cmd_header_;
}

/// @brief Get a mutable reference to the underlying C type.
ETCPAL_CONSTEXPR_14_OR_INLINE ::RdmCommandHeader& CommandHeader::get() noexcept
{
  return cmd_header_;
}

/// @brief Whether the values contained in this command header are valid for an RDM command.
inline bool CommandHeader::IsValid() const noexcept
{
  return rdm_command_header_is_valid(&cmd_header_);
}

/// @brief Whether this header represents an RDM GET_COMMAND.
constexpr bool CommandHeader::IsGet() const noexcept
{
  return cmd_header_.command_class == kRdmCCGetCommand;
}

/// @brief Whether this header represents an RDM SET_COMMAND.
constexpr bool CommandHeader::IsSet() const noexcept
{
  return cmd_header_.command_class == kRdmCCSetCommand;
}

/// @brief Whether this header represents an RDM DISCOVERY_COMMAND.
constexpr bool CommandHeader::IsDiscovery() const noexcept
{
  return cmd_header_.command_class == kRdmCCDiscoveryCommand;
}

/// @brief Set the UID of the controller generating the command.
inline CommandHeader& CommandHeader::SetSourceUid(const Uid& uid) noexcept
{
  return SetSourceUid(uid.get());
}

/// @brief Set the UID of the controller generating the command.
inline CommandHeader& CommandHeader::SetSourceUid(const ::RdmUid& uid) noexcept
{
  cmd_header_.source_uid = uid;
  return *this;
}

/// @brief Set the UID of the responder to which the command is addressed.
inline CommandHeader& CommandHeader::SetDestUid(const Uid& uid) noexcept
{
  return SetDestUid(uid.get());
}

/// @brief Set the UID of the responder to which the command is addressed.
inline CommandHeader& CommandHeader::SetDestUid(const ::RdmUid& uid) noexcept
{
  cmd_header_.dest_uid = uid;
  return *this;
}

/// @brief Set the transaction number of the command.
inline CommandHeader& CommandHeader::SetTransactionNum(uint8_t transaction_num) noexcept
{
  cmd_header_.transaction_num = transaction_num;
  return *this;
}

/// @brief Set the port ID of the port on which the command is being sent.
inline CommandHeader& CommandHeader::SetPortId(uint8_t port_id) noexcept
{
  cmd_header_.port_id = port_id;
  return *this;
}

/// @brief Set the sub-device to which the command is addressed (0 means the root device).
inline CommandHeader& CommandHeader::SetSubdevice(uint16_t subdevice) noexcept
{
  cmd_header_.subdevice = subdevice;
  return *this;
}

/// @brief Set the command's command class.
inline CommandHeader& CommandHeader::SetCommandClass(rdm_command_class_t command_class) noexcept
{
  cmd_header_.command_class = command_class;
  return *this;
}

/// @brief Set the RDM Parameter ID of the command.
///
/// Valid values are listed in ANSI E1.20 Table A-3, any of the relevant E1.37 extension standards,
/// ANSI E1.33 Table A-15, or manufacturer-specific values in the range 0x8000 - 0xffdf.
inline CommandHeader& CommandHeader::SetParamId(uint16_t param_id) noexcept
{
  cmd_header_.param_id = param_id;
  return *this;
}

/// @brief Get the number of bytes that the command header will occupy when converted to wire format.
/// @param data_len The size of the parameter data that will accompany the command header.
constexpr size_t CommandHeader::PackedSize(uint8_t data_len) const noexcept
{
  return RDM_PACKED_SIZE(data_len);
}

/// @brief Convert the header along with accompanying data to the RDM wire format.
/// @param[out] buffer RdmBuffer struct to hold the serialized command.
/// @param[in] data RDM parameter data that accompanies this header, if any.
/// @param[in] data_len Length of parameter data.
/// @return Result of serialize operation.
inline etcpal::Error CommandHeader::Serialize(RdmBuffer& buffer, const uint8_t* data, uint8_t data_len) const noexcept
{
  return rdm_pack_command(&cmd_header_, data, data_len, &buffer);
}

/// @brief Convert the header along with accompanying data to the RDM wire format.
/// @param[out] buf Data buffer to hold the serialized command.
/// @param[out] buf_len Length in bytes of buf.
/// @param[in] data RDM parameter data that accompanies this header, if any.
/// @param[in] data_len Length of parameter data.
/// @return Result of serialize operation.
inline etcpal::Error CommandHeader::Serialize(uint8_t* buf, size_t buf_len, const uint8_t* data, uint8_t data_len) const
    noexcept
{
  return rdm_pack_command_with_custom_buf(&cmd_header_, data, data_len, buf, buf_len);
}

/// @brief Create a header for an RDM GET_COMMAND.
/// @param param_id The RDM parameter ID (PID) of this command.
/// @param source_uid The RDM UID of the controller generating this command.
/// @param dest_uid The RDM UID of the responder to which this command is addressed.
/// @param subdevice The sub-device to which this command is addressed, or 0 for the root device.
/// @param transaction_num The RDM transaction number (should increase monotonically with each command).
/// @param port_id The port ID of the port on which this command is being sent.
inline CommandHeader CommandHeader::Get(uint16_t   param_id,
                                        const Uid& source_uid,
                                        const Uid& dest_uid,
                                        uint16_t   subdevice,
                                        uint8_t    transaction_num,
                                        uint8_t    port_id) noexcept
{
  return CommandHeader(source_uid, dest_uid, transaction_num, port_id, subdevice, kRdmCCGetCommand, param_id);
}

/// @brief Create a header for an RDM SET_COMMAND.
/// @param param_id The RDM parameter ID (PID) of this command.
/// @param source_uid The RDM UID of the controller generating this command.
/// @param dest_uid The RDM UID of the responder to which this command is addressed.
/// @param subdevice The sub-device to which this command is addressed, or 0 for the root device.
/// @param transaction_num The RDM transaction number (should increase monotonically with each command).
/// @param port_id The port ID of the port on which this command is being sent.
inline CommandHeader CommandHeader::Set(uint16_t   param_id,
                                        const Uid& source_uid,
                                        const Uid& dest_uid,
                                        uint16_t   subdevice,
                                        uint8_t    transaction_num,
                                        uint8_t    port_id) noexcept
{
  return CommandHeader(source_uid, dest_uid, transaction_num, port_id, subdevice, kRdmCCSetCommand, param_id);
}

};  // namespace rdm

#endif  // RDM_CPP_MESSAGE_TYPES_COMMAND_HEADER_H_
