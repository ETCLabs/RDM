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

/// @file rdm/cpp/message_types/response_header.h
/// @brief Definitions for the RDM response header type.

#ifndef RDM_CPP_MESSAGE_TYPES_RESPONSE_HEADER_H_
#define RDM_CPP_MESSAGE_TYPES_RESPONSE_HEADER_H_

#include <cstdint>
#include "etcpal/cpp/common.h"
#include "rdm/message.h"
#include "rdm/cpp/uid.h"

namespace rdm
{
/// @ingroup cpp_message
/// @brief A class that represents the header fields of an RDM response.
///
/// Includes everything except the RDM parameter data. RDM responses are typically created within
/// RDM libraries rather than built manually, so this class's API is optimized for controllers
/// reading responses rather than responders generating them.
///
/// Example usage:
/// @code
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
/// @endcode
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

  /// @name Response Type Inspectors
  /// @{
  constexpr bool IsAck() const noexcept;
  constexpr bool IsAckTimer() const noexcept;
  constexpr bool IsAckOverflow() const noexcept;
  constexpr bool IsNack() const noexcept;
  /// @}

  /// @name Command Class Inspectors
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
  ::RdmResponseHeader resp_header_{};
};

/// @brief Construct a response header from its component values.
/// @param source_uid The RDM UID of the responder generating this response.
/// @param dest_uid The RDM UID of the controller to which this response is addressed.
/// @param transaction_num The RDM transaction number, copied from the corresponding command.
/// @param response_type The RDM response type, indicating the response status.
/// @param msg_count Current count of queued messages waiting to be retrieved.
/// @param subdevice The sub-device generating this response, or 0 for the root device.
/// @param command_class The RDM command class for this response.
/// @param param_id The RDM parameter ID (PID) of this response.
constexpr ResponseHeader::ResponseHeader(const Uid& source_uid, const Uid& dest_uid, uint8_t transaction_num,
                                         rdm_response_type_t response_type, uint8_t msg_count, uint16_t subdevice,
                                         rdm_command_class_t command_class, uint16_t param_id) noexcept
    : resp_header_{source_uid.get(), dest_uid.get(), transaction_num, response_type,
                   msg_count,        subdevice,      command_class,   param_id}
{
}

/// @brief Construct a response header copied from an instance of the C RdmResponseHeader type.
constexpr ResponseHeader::ResponseHeader(const ::RdmResponseHeader& c_header) noexcept : resp_header_(c_header)
{
}

/// @brief Assign an instance of the C RdmResponseHeader type to an instance of this class.
inline ResponseHeader& ResponseHeader::operator=(const ::RdmResponseHeader& c_header) noexcept
{
  resp_header_ = c_header;
  return *this;
}

/// @brief Get the UID of the responder generating the response.
constexpr Uid ResponseHeader::source_uid() const noexcept
{
  return resp_header_.source_uid;
}

/// @brief Get the UID of the controller to which the response is addressed.
constexpr Uid ResponseHeader::dest_uid() const noexcept
{
  return resp_header_.dest_uid;
}

/// @brief Get the transaction number of the response.
constexpr uint8_t ResponseHeader::transaction_num() const noexcept
{
  return resp_header_.transaction_num;
}

/// @brief Get the RDM response type of the response.
constexpr rdm_response_type_t ResponseHeader::response_type() const noexcept
{
  return resp_header_.resp_type;
}

/// @brief Get the current count of queued messages being stored by the responder.
constexpr uint8_t ResponseHeader::message_count() const noexcept
{
  return resp_header_.msg_count;
}

/// @brief Get the sub-device from which the response was generated (0 means the root device).
constexpr uint16_t ResponseHeader::subdevice() const noexcept
{
  return resp_header_.subdevice;
}

/// @brief Get the response's command class.
constexpr rdm_command_class_t ResponseHeader::command_class() const noexcept
{
  return resp_header_.command_class;
}

/// @brief Get the RDM Parameter ID of the response.
constexpr uint16_t ResponseHeader::param_id() const noexcept
{
  return resp_header_.param_id;
}

/// @brief Get a const reference to the underlying C type.
constexpr const ::RdmResponseHeader& ResponseHeader::get() const noexcept
{
  return resp_header_;
}

/// @brief Get a mutable reference to the underlying C type.
ETCPAL_CONSTEXPR_14_OR_INLINE ::RdmResponseHeader& ResponseHeader::get() noexcept
{
  return resp_header_;
}

/// @brief Whether the values contained in this response header are valid for an RDM response.
inline bool ResponseHeader::IsValid() const noexcept
{
  return rdm_response_header_is_valid(&resp_header_);
}

/// @brief Whether this header represents an RDM ACK response.
constexpr bool ResponseHeader::IsAck() const noexcept
{
  return resp_header_.resp_type == kRdmResponseTypeAck;
}

/// @brief Whether this header represents an RDM ACK_TIMER response.
constexpr bool ResponseHeader::IsAckTimer() const noexcept
{
  return resp_header_.resp_type == kRdmResponseTypeAckTimer;
}

/// @brief Whether this header represents an RDM ACK_OVERFLOW response.
constexpr bool ResponseHeader::IsAckOverflow() const noexcept
{
  return resp_header_.resp_type == kRdmResponseTypeAckOverflow;
}

/// @brief Whether this header represents an RDM NACK_REASON response.
constexpr bool ResponseHeader::IsNack() const noexcept
{
  return resp_header_.resp_type == kRdmResponseTypeNackReason;
}

/// @brief Whether this header represents an RDM GET_COMMAND_RESPONSE.
constexpr bool ResponseHeader::IsGetResponse() const noexcept
{
  return resp_header_.command_class == kRdmCCGetCommandResponse;
}

/// @brief Whether this header represents an RDM SET_COMMAND_RESPONSE.
constexpr bool ResponseHeader::IsSetResponse() const noexcept
{
  return resp_header_.command_class == kRdmCCSetCommandResponse;
}

/// @brief Whether this header represents an RDM DISCOVERY_COMMAND_RESPONSE.
constexpr bool ResponseHeader::IsDiscoveryResponse() const noexcept
{
  return resp_header_.command_class == kRdmCCDiscoveryCommandResponse;
}

/// @brief Set the UID of the responder generating the response.
inline ResponseHeader& ResponseHeader::SetSourceUid(const Uid& uid) noexcept
{
  return SetSourceUid(uid.get());
}

/// @brief Set the UID of the responder generating the response.
inline ResponseHeader& ResponseHeader::SetSourceUid(const ::RdmUid& uid) noexcept
{
  resp_header_.source_uid = uid;
  return *this;
}

/// @brief Set the UID of the controller to which the response is addressed.
inline ResponseHeader& ResponseHeader::SetDestUid(const Uid& uid) noexcept
{
  return SetDestUid(uid.get());
}

/// @brief Set the UID of the controller to which the response is addressed.
inline ResponseHeader& ResponseHeader::SetDestUid(const ::RdmUid& uid) noexcept
{
  resp_header_.dest_uid = uid;
  return *this;
}

/// @brief Set the transaction number of the response.
inline ResponseHeader& ResponseHeader::SetTransactionNum(uint8_t transaction_num) noexcept
{
  resp_header_.transaction_num = transaction_num;
  return *this;
}

/// @brief Set the RDM response type of the response.
inline ResponseHeader& ResponseHeader::SetResponseType(rdm_response_type_t response_type) noexcept
{
  resp_header_.resp_type = response_type;
  return *this;
}

/// @brief Set the current count of queued messages being stored by the responder.
inline ResponseHeader& ResponseHeader::SetMessageCount(uint8_t msg_count) noexcept
{
  resp_header_.msg_count = msg_count;
  return *this;
}

/// @brief Set the sub-device from which the response was generated (0 means the root device).
inline ResponseHeader& ResponseHeader::SetSubdevice(uint16_t subdevice) noexcept
{
  resp_header_.subdevice = subdevice;
  return *this;
}

/// @brief Set the response's command class.
inline ResponseHeader& ResponseHeader::SetCommandClass(rdm_command_class_t command_class) noexcept
{
  resp_header_.command_class = command_class;
  return *this;
}

/// @brief Set the RDM Parameter ID of the response.
///
/// Valid values are listed in ANSI E1.20 Table A-3, any of the relevant E1.37 extension standards,
/// ANSI E1.33 Table A-15, or manufacturer-specific values in the range 0x8000 - 0xffdf.
inline ResponseHeader& ResponseHeader::SetParamId(uint16_t param_id) noexcept
{
  resp_header_.param_id = param_id;
  return *this;
}
};  // namespace rdm

#endif  // RDM_CPP_MESSAGE_TYPES_RESPONSE_HEADER_H_
