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

/// @file rdm/cpp/uid.h
/// @brief C++ wrapper and utilities for rdm/uid.h

#ifndef RDM_CPP_UID_H_
#define RDM_CPP_UID_H_

#include <cstdint>
#include <string>
#include "etcpal/cpp/common.h"
#include "rdm/uid.h"

namespace rdm
{
/// @defgroup cpp_uid UID
/// @ingroup rdm_cpp
/// @brief C++ utilities for RDM UIDs.
///
/// ```c
/// #include "rdm/cpp/uid.h"
/// ```
///
/// See @ref uid for more information on UIDs.

/// @ingroup cpp_uid
/// @brief A C++ wrapper class for the RDM UID type.
///
/// Provides C++ syntactic sugar for working with RDM UIDs. You can convert from the basic C type
/// implicitly, and to it explicitly:
/// @code
/// RdmUid c_uid = { 0x1111u, 0x22222222u };
/// rdm::Uid uid = c_uid;
/// c_uid = uid.get();
/// @endcode
///
/// Or construct directly from the ESTA Manufacturer ID and Device ID:
/// @code
/// rdm::Uid uid(0x1111u, 0x22222222u);
/// EXPECT_EQ(uid.manufacturer_id(), 0x1111u);
/// EXPECT_EQ(uid.device_id(), 0x22222222u);
/// @endcode
///
/// Or use one of the special UID generators:
/// @code
/// auto uid_from_str = rdm::Uid::FromString("1111:22222222");      // Create from a string representation
/// auto bcast_uid = rdm::Uid::Broadcast();                         // Creates E1.20's BROADCAST_ALL_DEVICES_ID
/// auto controller_bcast_uid = rdm::Uid::ControllerBroadcast();    // Creates E1.33's RPT_ALL_CONTROLLERS
/// auto device_bcast_uid = rdm::Uid::DeviceBroadcast();            // Creates E1.33's RPT_ALL_DEVICES
/// auto device_manu_bcast_uid = rdm::Uid::DeviceBroadcast(0x6574); // Creates E1.33's RPT_ALL_MID_DEVICES
/// auto static_uid = rdm::Uid::Static(0x1111u, 0x22222222u);       // Explicitly create an RDMnet Static UID
/// auto dynamic_req = rdm::Uid::DynamicUidRequest(0x1111u);        // Explicitly create an RDMnet Dynamic UID Request
/// @endcode
///
/// There are also inspectors to check whether a UID is any of the above special types.
///
/// Use ToString() to get a string representation:
/// @code
/// std::cout << "Discovered responder " << uid.ToString() << '\n';
/// // Or...
/// printf("Discovered responder %s\n", uid.ToString().c_str());
/// @endcode
class Uid
{
public:
  /// @brief Constructs a null UID by default.
  Uid() = default;
  constexpr Uid(uint16_t manu_val, uint32_t id_val) noexcept;

  constexpr Uid(const RdmUid& c_uid) noexcept;
  Uid& operator=(const RdmUid& c_uid) noexcept;

  constexpr const RdmUid& get() const noexcept;
  ETCPAL_CONSTEXPR_14 RdmUid& get() noexcept;
  constexpr uint16_t manufacturer_id() const noexcept;
  constexpr uint32_t device_id() const noexcept;
  std::string ToString() const;

  constexpr bool IsValid() const noexcept;
  bool IsBroadcast() const noexcept;
  bool IsControllerBroadcast() const noexcept;
  bool IsDeviceBroadcast() const noexcept;
  bool IsDeviceManufacturerBroadcast() const noexcept;
  constexpr bool BroadcastManufacturerIdMatches(uint16_t manu_val) const noexcept;
  constexpr uint16_t DeviceBroadcastManufacturerId() const noexcept;

  constexpr bool IsDynamic() const noexcept;
  constexpr bool IsDynamicUidRequest() const noexcept;
  constexpr bool IsStatic() const noexcept;

  void SetManufacturerId(uint16_t manu_val) noexcept;
  void SetDeviceId(uint32_t id_val) noexcept;

  static Uid FromString(const std::string& uid_str) noexcept;
  static Uid FromString(const char* uid_str) noexcept;
  static Uid Broadcast() noexcept;
  static Uid ControllerBroadcast() noexcept;
  static Uid DeviceBroadcast() noexcept;
  static Uid DeviceBroadcast(uint16_t manu_val) noexcept;
  static Uid Static(uint16_t manu_val, uint32_t id_val) noexcept;
  static Uid DynamicUidRequest(uint16_t manu_val) noexcept;

private:
  RdmUid uid_{};
};

/// @brief Construct a UID explicitly from an ESTA Manufacturer ID and a device ID.
constexpr Uid::Uid(uint16_t manu_val, uint32_t id_val) noexcept : uid_{manu_val, id_val}
{
}

/// @brief Construct a UID copied from an instance of the C RdmUid type.
constexpr Uid::Uid(const RdmUid& c_uid) noexcept : uid_(c_uid)
{
}

/// @brief Assign an instance of the C RdmUid type to an instance of this class.
inline Uid& Uid::operator=(const RdmUid& c_uid) noexcept
{
  uid_ = c_uid;
  return *this;
}

/// @brief Get a const reference to the underlying C type.
constexpr const RdmUid& Uid::get() const noexcept
{
  return uid_;
}

/// @brief Get a mutable reference to the underlying C type.
ETCPAL_CONSTEXPR_14_OR_INLINE RdmUid& Uid::get() noexcept
{
  return uid_;
}

/// @brief Get the ESTA Manufacturer ID portion of this UID.
constexpr uint16_t Uid::manufacturer_id() const noexcept
{
  return RDM_GET_MANUFACTURER_ID(&uid_);
}

/// @brief Get the Device ID portion of this UID.
constexpr uint32_t Uid::device_id() const noexcept
{
  return RDM_GET_DEVICE_ID(&uid_);
}

/// @brief Convert a UID to a string representation.
/// @details See rdm_uid_to_string() for more information on the format.
inline std::string Uid::ToString() const
{
  char str_buf[RDM_UID_STRING_BYTES];
  if (rdm_uid_to_string(&uid_, str_buf))
    return str_buf;
  else
    return std::string();
}

/// @brief Determine if a UID is valid. All UIDs are valid unless they are null (all 0's).
constexpr bool Uid::IsValid() const noexcept
{
  return !RDM_UID_IS_NULL(&uid_);
}

/// @brief Determine whether a UID is the E1.20 value BROADCAST_ALL_DEVICES_ID.
inline bool Uid::IsBroadcast() const noexcept
{
  return RDM_UID_IS_BROADCAST(&uid_);
}

/// @brief Determine whether a UID is the E1.33 value RPT_ALL_CONTROLLERS.
inline bool Uid::IsControllerBroadcast() const noexcept
{
  return RDMNET_UID_IS_CONTROLLER_BROADCAST(&uid_);
}

/// @brief Determine whether a UID is the E1.33 value RPT_ALL_DEVICES.
inline bool Uid::IsDeviceBroadcast() const noexcept
{
  return RDMNET_UID_IS_DEVICE_BROADCAST(&uid_);
}

/// @brief Determine whether a UID is one of the E1.33 values defined by RPT_ALL_MID_DEVICES.
/// @details See RDMNET_UID_IS_DEVICE_MANU_BROADCAST() for more information.
inline bool Uid::IsDeviceManufacturerBroadcast() const noexcept
{
  return RDMNET_UID_IS_DEVICE_MANU_BROADCAST(&uid_);
}

/// @brief Determine whether an RDMnet Device Manufacturer Broadcast UID matches a specific ESTA
///        Manufacturer ID.
constexpr bool Uid::BroadcastManufacturerIdMatches(uint16_t manu_val) const noexcept
{
  return RDMNET_DEVICE_BROADCAST_MANU_MATCHES(&uid_, manu_val);
}

/// @brief Get the ESTA Manufacturer ID from an RDMnet Device Manufacturer Broadcast UID.
constexpr uint16_t Uid::DeviceBroadcastManufacturerId() const noexcept
{
  return RDMNET_DEVICE_BROADCAST_MANU_ID(&uid_);
}

/// @brief Determine whether a UID is a Dynamic UID as defined in ANSI E1.33.
/// @details See RDMNET_UID_IS_DYNAMIC() for more information.
constexpr bool Uid::IsDynamic() const noexcept
{
  return RDMNET_UID_IS_DYNAMIC(&uid_);
}

/// @brief Determine whether a UID is a Dynamic UID Request as defined in ANSI E1.33.
constexpr bool Uid::IsDynamicUidRequest() const noexcept
{
  return RDMNET_UID_IS_DYNAMIC_UID_REQUEST(&uid_);
}

/// @brief Determine whether a UID is a Static UID as defined in ANSI E1.33.
/// @details See RDMNET_UID_IS_STATIC() for more information.
constexpr bool Uid::IsStatic() const noexcept
{
  return RDMNET_UID_IS_STATIC(&uid_);
}

/// @brief Set the ESTA Manufacturer ID portion of this UID.
/// @details The top bit of manu_val (the dynamic vs static bit in RDMnet) is disregarded and the
/// top bit of manufacturer_id() remains as it was before this function was called.
inline void Uid::SetManufacturerId(uint16_t manu_val) noexcept
{
  // Preserve the state of the top bit
  uid_.manu = (uid_.manu & 0x8000) | (manu_val & 0x7fff);
}

/// @brief Set the Device ID portion of this UID.
inline void Uid::SetDeviceId(uint32_t id_val) noexcept
{
  uid_.id = id_val;
}

/// @brief Create a UID from a string representation.
/// @details See rdm_string_to_uid() for more information.
inline Uid Uid::FromString(const std::string& uid_str) noexcept
{
  return FromString(uid_str.c_str());
}

/// @brief Create a UID from a string representation.
/// @details See rdm_string_to_uid() for more information.
inline Uid Uid::FromString(const char* uid_str) noexcept
{
  Uid uid;
  if (rdm_string_to_uid(uid_str, &uid.uid_))
    return uid;
  else
    return Uid{};
}

/// @brief Create a UID representing BROADCAST_ALL_DEVICES_ID as defined in ANSI E1.20.
inline Uid Uid::Broadcast() noexcept
{
  return kRdmBroadcastUid;
}

/// @brief Create a UID representing RPT_ALL_CONTROLLERS as defined in ANSI E1.33.
inline Uid Uid::ControllerBroadcast() noexcept
{
  return kRdmnetControllerBroadcastUid;
}

/// @brief Create a UID representing RPT_ALL_DEVICES as defined in ANSI E1.33.
inline Uid Uid::DeviceBroadcast() noexcept
{
  return kRdmnetDeviceBroadcastUid;
}

/// @brief Create a UID representing RPT_ALL_MID_DEVICES as defined in ANSI E1.33.
inline Uid Uid::DeviceBroadcast(uint16_t manu_val) noexcept
{
  Uid uid;
  RDMNET_INIT_DEVICE_MANU_BROADCAST(&uid.uid_, manu_val);
  return uid;
}

/// @brief Explicitly create an RDMnet Static UID from an ESTA Manufacturer ID and Device ID.
/// @details The top bit of manu_val is masked off and the top bit of manufacturer_id() is always 0
/// after this function is called.
inline Uid Uid::Static(uint16_t manu_val, uint32_t id_val) noexcept
{
  return Uid((manu_val & 0x7fff), id_val);
}

/// @brief Create an RDMnet Dynamic UID Request from a given ESTA Manufacturer ID.
inline Uid Uid::DynamicUidRequest(uint16_t manu_val) noexcept
{
  Uid uid;
  RDMNET_INIT_DYNAMIC_UID_REQUEST(&uid.uid_, manu_val);
  return uid;
}

/// @addtogroup cpp_uid
/// @{

/// @name UID Relational Operators
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
