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

/// \file rdm/cpp/message_types/nack_reason.h
/// \brief Definitions for the RDM NACK reason type.

#ifndef RDM_CPP_MESSAGE_TYPES_NACK_REASON_H_
#define RDM_CPP_MESSAGE_TYPES_NACK_REASON_H_

#include <cstdint>
#include <string>
#include "rdm/message.h"

namespace rdm
{
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
  return code_ < RDM_NUM_STANDARD_NR_CODES;
}

};  // namespace rdm

#endif  // RDM_CPP_MESSAGE_TYPES_NACK_REASON_H_
