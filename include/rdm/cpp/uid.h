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

/// \file rdm/cpp/uid.h
/// \brief C++ wrapper and utilities for rdm/uid.h

#ifndef RDM_CPP_UID_H_
#define RDM_CPP_UID_H_

#include <string>
#include "etcpal/int.h"
#include "etcpal/cpp/common.h"
#include "rdm/uid.h"

namespace rdm
{
/// \brief A wrapper class for the RDM UID type.
///
/// Provides C++ syntactic sugar for working with RDM UIDs.
class Uid
{
public:
  /// \brief Constructs a null UID by default.
  Uid() = default;
  constexpr Uid(uint16_t manu_val, uint32_t id_val);

  constexpr Uid(const RdmUid& c_uid) noexcept;
  Uid& operator=(const RdmUid& c_uid) noexcept;

  constexpr const RdmUid& get() const;
  ETCPAL_CONSTEXPR_14 RdmUid& get();
  constexpr uint16_t manufacturer_id() const noexcept;
  constexpr uint32_t device_id() const noexcept;
  std::string ToString() const;

  constexpr bool IsValid() const noexcept;
  constexpr bool IsBroadcast() const noexcept;
  constexpr bool IsControllerBroadcast() const noexcept;
  constexpr bool IsDeviceBroadcast() const noexcept;
  constexpr bool IsDeviceManufacturerBroadcast() const noexcept;
  constexpr bool BroadcastManufacturerIdMatches(uint16_t manu_val) const noexcept;
  constexpr uint16_t DeviceBroadcastManufacturerId() const noexcept;

  constexpr bool IsDynamic() const noexcept;
  constexpr bool IsDynamicUidRequest() const noexcept;
  constexpr bool IsStatic() const noexcept;

  void SetManufacturerId(uint16_t manu_val) noexcept;
  void SetDeviceId(uint32_t id_val) noexcept;

  static Uid FromString(const std::string& uid_str) noexcept;
  static Uid Broadcast() noexcept;
  static Uid ControllerBroadcast() noexcept;
  static Uid DeviceBroadcast() noexcept;
  static Uid DeviceBroadcast(uint16_t manu_val) noexcept;
  static Uid Static(uint16_t manu_val, uint32_t id_val) noexcept;
  static Uid DynamicUidRequest(uint16_t manu_val) noexcept;

private:
  RdmUid uid_{};
};

constexpr Uid::Uid(uint16_t manu_val, uint32_t id_val) : uid_{manu_val, id_val}
{
}

constexpr Uid::Uid(const RdmUid& c_uid) noexcept : uid_(c_uid)
{
}

inline Uid& Uid::operator=(const RdmUid& c_uid) noexcept
{
  uid_ = c_uid;
  return *this;
}

constexpr const RdmUid& Uid::get() const
{
  return uid_;
}

ETCPAL_CONSTEXPR_14_OR_INLINE RdmUid& Uid::get()
{
  return uid_;
}

constexpr uint16_t Uid::manufacturer_id() const noexcept
{
  return RDM_GET_MANUFACTURER_ID(&uid_);
}

constexpr uint32_t Uid::device_id() const noexcept
{
  return RDM_GET_DEVICE_ID(&uid_);
}

inline std::string Uid::ToString() const
{
  char str_buf[RDM_UID_STRING_BYTES];
  if (rdm_uid_to_string(&uid_, str_buf) == kEtcPalErrOk)
    return str_buf;
  else
    return std::string();
}

constexpr bool Uid::IsValid() const noexcept
{
  return !RDM_UID_IS_NULL(&uid_);
}

constexpr bool Uid::IsBroadcast() const noexcept
{
  return RDM_UID_IS_BROADCAST(&uid_);
}

constexpr bool Uid::IsControllerBroadcast() const noexcept
{
  return RDMNET_UID_IS_CONTROLLER_BROADCAST(&uid_);
}

constexpr bool Uid::IsDeviceBroadcast() const noexcept
{
  return RDMNET_UID_IS_DEVICE_BROADCAST(&uid_);
}

constexpr bool Uid::IsDeviceManufacturerBroadcast() const noexcept
{
  return RDMNET_UID_IS_DEVICE_MANU_BROADCAST(&uid_);
}

constexpr bool Uid::BroadcastManufacturerIdMatches(uint16_t manu_val) const noexcept
{
  return RDMNET_DEVICE_BROADCAST_MANU_MATCHES(&uid_, manu_val);
}

constexpr uint16_t Uid::DeviceBroadcastManufacturerId() const noexcept
{
  return RDMNET_DEVICE_BROADCAST_MANU_ID(&uid_);
}

constexpr bool Uid::IsDynamic() const noexcept
{
  return RDMNET_UID_IS_DYNAMIC(&uid_);
}

constexpr bool Uid::IsDynamicUidRequest() const noexcept
{
  return RDMNET_UID_IS_DYNAMIC_UID_REQUEST(&uid_);
}

constexpr bool Uid::IsStatic() const noexcept
{
  return RDMNET_UID_IS_STATIC(&uid_);
}

inline void Uid::SetManufacturerId(uint16_t manu_val) noexcept
{
  // Preserve the state of the top bit
  uid_.manu = (uid_.manu & 0x8000) | (manu_val & 0x7fff);
}

inline void Uid::SetDeviceId(uint32_t id_val) noexcept
{
  uid_.id = id_val;
}

inline Uid Uid::FromString(const std::string& uid_str) noexcept
{
  Uid uid;
  if (rdm_string_to_uid(uid_str.c_str(), &uid.uid_) == kEtcPalErrOk)
    return uid;
  else
    return Uid{};
}

inline Uid Uid::Broadcast() noexcept
{
  return kRdmBroadcastUid;
}

inline Uid Uid::ControllerBroadcast() noexcept
{
  return kRdmnetControllerBroadcastUid;
}

inline Uid Uid::DeviceBroadcast() noexcept
{
  return kRdmnetDeviceBroadcastUid;
}

inline Uid Uid::DeviceBroadcast(uint16_t manu_val) noexcept
{
  Uid uid;
  RDMNET_INIT_DEVICE_MANU_BROADCAST(&uid.uid_, manu_val);
  return uid;
}

inline Uid Uid::Static(uint16_t manu_val, uint32_t id_val) noexcept
{
  return Uid((manu_val & 0x7fff), id_val);
}

inline Uid Uid::DynamicUidRequest(uint16_t manu_val) noexcept
{
  Uid uid;
  RDMNET_INIT_DYNAMIC_UID_REQUEST(&uid.uid_, manu_val);
  return uid;
}

/// \addtogroup rdm_cpp_uid
/// @{

/// \name UID Relational Operators
/// @{

// Special operators for comparing with RdmUids

constexpr bool operator==(const RdmUid& c_uid, const Uid& uid) noexcept
{
  return c_uid == uid.get();
}

constexpr bool operator!=(const RdmUid& c_uid, const Uid& uid) noexcept
{
  return !(c_uid == uid);
}

constexpr bool operator==(const Uid& uid, const RdmUid& c_uid) noexcept
{
  return uid.get() == c_uid;
}

constexpr bool operator!=(const Uid& uid, const RdmUid& c_uid) noexcept
{
  return !(uid == c_uid);
}

// Standard operators

constexpr bool operator==(const Uid& a, const Uid& b) noexcept
{
  return a.get() == b.get();
}

constexpr bool operator!=(const Uid& a, const Uid& b) noexcept
{
  return !(a == b);
}

constexpr bool operator<(const Uid& a, const Uid& b) noexcept
{
  return a.get() < b.get();
}

constexpr bool operator>(const Uid& a, const Uid& b) noexcept
{
  return b < a;
}

constexpr bool operator<=(const Uid& a, const Uid& b) noexcept
{
  return !(b < a);
}

constexpr bool operator>=(const Uid& a, const Uid& b) noexcept
{
  return !(a < b);
}

/// @}
/// @}

};  // namespace rdm

#endif  // RDM_CPP_UID_H_
