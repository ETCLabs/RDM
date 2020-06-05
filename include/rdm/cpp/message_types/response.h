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

/// \file rdm/cpp/message_types/response.h
/// \brief Definitions for the RDM response type.

#ifndef RDM_CPP_MESSAGE_TYPES_RESPONSE_H_
#define RDM_CPP_MESSAGE_TYPES_RESPONSE_H_

#include <cstdint>
#include <vector>
#include "etcpal/cpp/common.h"
#include "etcpal/cpp/error.h"
#include "rdm/cpp/message_types/nack_reason.h"
#include "rdm/cpp/message_types/response_header.h"
#include "rdm/cpp/uid.h"
#include "rdm/message.h"

namespace rdm
{
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
  std::vector<uint8_t> GetData() const;

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
  Response& AppendData(const uint8_t* data, size_t data_len);
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
inline Response::Response(const ResponseHeader& header, const uint8_t* data, size_t data_len) : header_(header)
{
  SetData(data, data_len);
}

/// \brief Construct a response from a header and data.
/// \param header Header to assign to this response.
/// \param data The RDM parameter data of this response (nullptr for responses with no data).
/// \param data_len The length of the RDM parameter data (0 for responses with no data).
inline Response::Response(const ::RdmResponseHeader& header, const uint8_t* data, size_t data_len) : header_(header)
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
/// \return A pointer to a byte buffer with the parameter data, or nullptr if this response has no data.
inline const uint8_t* Response::data() const noexcept
{
  return (data_.empty() ? nullptr : data_.data());
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

/// \brief Copy out the data from an RDM response.
/// \return A copied vector containing any parameter data associated with this response.
inline std::vector<uint8_t> Response::GetData() const
{
  return data_;
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

/// \brief Append more data to this response's parameter data.
inline Response& Response::AppendData(const uint8_t* data, size_t data_len)
{
  if (data && data_len)
    data_.insert(data_.end(), data, data + data_len);
  return *this;
}

/// \brief Remove the parameter data associated with this response.
inline Response& Response::ClearData() noexcept
{
  data_.clear();
  return *this;
}
};  // namespace rdm

#endif  // RDM_CPP_MESSAGE_TYPES_RESPONSE_H_
