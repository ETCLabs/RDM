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

/*!
 * \file rdm/uid.h
 * \brief A type and helper functions for the Unique ID (UID) used in the RDM family of protocols.
 */

#ifndef RDM_UID_H_
#define RDM_UID_H_

#include <stdbool.h>
#include <stdint.h>

/*!
 * \defgroup uid UID
 * \ingroup rdm
 * \brief Type and helper functions for the Unique ID (UID) used in the RDM family of protocols.
 *
 * ```c
 * #include "rdm/uid.h"
 * ```
 *
 * RDM-based protocols use an identifier called a UID to identify an entity that generates or
 * consumes RDM messages.
 *
 * The top bit of the UID is reserved for indicating broadcast values, or indicating Dynamic UIDs
 * in ANSI E1.33.
 *
 * The next 15 bits is the ESTA Manufacturer ID; each manufacturer of RDM equipment has a unique
 * Manufacturer ID registered with ESTA. The MSB of the UID is reserved for indicating broadcast
 * values, or indicating Dynamic UIDs in ANSI E1.33.
 *
 * The remaining 32 bits is the Device ID. The Device ID is unique within a given scope; in the
 * case of a Static UID, it is unique across all products made by the manufacturer indicated by
 * the ESTA Manufacturer ID. In the case of a Dynamic UID, it is unique within its current E1.33
 * Scope.
 *
 * @{
 */

#ifdef __cplusplus
extern "C" {
#endif

/*! The UID type. Don't forget that the top bit of the manu field has special meaning. The helper
 *  macros are your friend! */
typedef struct RdmUid
{
  uint16_t manu; /*!< The ESTA Manufacturer ID. */
  uint32_t id;   /*!< The Device ID. */
} RdmUid;

/*************************** UID Comparison Macros ***************************/

/*!
 * \brief Determine if two UIDs are equal.
 * \param uidptr1 Pointer to first RdmUid.
 * \param uidptr2 Pointer to second RdmUid.
 * \return true (UIDs are equal) or false (UIDs are not equal).
 */
#define RDM_UID_EQUAL(uidptr1, uidptr2) ((uidptr1)->manu == (uidptr2)->manu && (uidptr1)->id == (uidptr2)->id)

/************************* UID Initialization Macros *************************/

/*!
 * Initialize a Static UID with a Manufacturer ID and Device ID.
 * \param uidptr Pointer to RdmUid to initialize.
 * \param manu_val ESTA Manufacturer ID.
 * \param id_val Device ID.
 */
#define RDM_INIT_STATIC_UID(uidptr, manu_val, id_val) \
  do                                                  \
  {                                                   \
    (uidptr)->manu = (manu_val);                      \
    (uidptr)->id = (id_val);                          \
  } while (0)

/*!
 * Initialize an RDMnet Dynamic UID Request with a Manufacturer ID.
 * \param uidptr Pointer to RdmUid to initialize.
 * \param manu_val ESTA Manufacturer ID.
 */
#define RDMNET_INIT_DYNAMIC_UID_REQUEST(uidptr, manu_val) \
  do                                                      \
  {                                                       \
    (uidptr)->manu = (0x8000u | (manu_val));              \
    (uidptr)->id = 0;                                     \
  } while (0)

/*!
 * \brief Initialize an RDMnet Device Manufacturer Broadcast UID with a Manufacturer ID.
 * \param uidptr Pointer to RdmUid to initialize.
 * \param manu_val ESTA Manufacturer ID.
 */
#define RDMNET_INIT_DEVICE_MANU_BROADCAST(uidptr, manu_val)                       \
  do                                                                              \
  {                                                                               \
    (uidptr)->manu = kRdmnetDeviceBroadcastUid.manu;                              \
    (uidptr)->id = (kRdmnetDeviceBroadcastUid.id & ((manu_val << 16) | 0xffffu)); \
  } while (0)

/**************************** UID Broadcast Macros ***************************/

/*!
 * \brief Determine whether a UID is the E1.20 value BROADCAST_ALL_DEVICES_ID.
 * \param uidptr Pointer to RdmUid to check.
 * \return true (uidptr is equal to BROADCAST_ALL_DEVICES_ID) or false (uidptr is not equal to
 *         BROADCAST_ALL_DEVICES_ID).
 */
#define RDM_UID_IS_BROADCAST(uidptr) ((uidptr)->manu == kRdmBroadcastUid.manu && (uidptr)->id == kRdmBroadcastUid.id)

/*!
 * \brief Determine whether a UID is the E1.33 value RPT_ALL_CONTROLLERS.
 * \param uidptr Pointer to RdmUid to check.
 * \return true (uidptr is equal to RPT_ALL_CONTROLLERS) or false (uidptr is not equal to
 *         RPT_ALL_CONTROLLERS).
 */
#define RDMNET_UID_IS_CONTROLLER_BROADCAST(uidptr) \
  ((uidptr)->manu == kRdmnetControllerBroadcastUid.manu && (uidptr)->id == kRdmnetControllerBroadcastUid.id)

/*!
 * \brief Determine whether a UID is the E1.33 value RPT_ALL_DEVICES.
 * \param uidptr Pointer to RdmUid to check.
 * \return true (uidptr is equal to RPT_ALL_DEVICES) or false (uidptr is not equal to
 *         RPT_ALL_DEVICES).
 */
#define RDMNET_UID_IS_DEVICE_BROADCAST(uidptr) \
  ((uidptr)->manu == kRdmnetDeviceBroadcastUid.manu && (uidptr)->id == kRdmnetDeviceBroadcastUid.id)

/*!
 * \brief Determine whether a UID is one of the E1.33 values defined by RPT_ALL_MID_DEVICES.
 *
 * RDMnet Device Manufacturer Broadcasts are directed to all Devices with a specific ESTA
 * Manufacturer ID. If this check returns true, use RDMNET_DEVICE_BROADCAST_MANU_ID() to determine
 * which Manufacturer ID this broadcast is directed to, or RDMNET_DEVICE_BROADCAST_MANU_MATCHES()
 * to determine whether it matches a specific Manufacturer ID.
 *
 * \param uidptr Pointer to RdmUid to check.
 * \return true (uidptr is one of the values defined by RPT_ALL_MID_DEVICES) or false (uidptr is
 *         not one of the values defined by RPT_ALL_MID_DEVICES).
 */
#define RDMNET_UID_IS_DEVICE_MANU_BROADCAST(uidptr) \
  ((uidptr)->manu == kRdmnetDeviceBroadcastUid.manu && (((uidptr)->id & 0xffffu) == 0xffffu))

/*!
 * \brief Determine whether an RDMnet Device Manufacturer Broadcast UID matches a specific ESTA
 *        Manufacturer ID.
 *
 * Use RDMNET_UID_IS_DEVICE_MANU_BROADCAST() first to determine whether this UID is an RDMnet
 * Device Manufacturer Broadcast.
 *
 * \param uidptr Pointer to RdmUid to check.
 * \param manu_val ESTA Manufacturer ID to check against the Broadcast UID.
 * \return true (RDMnet Device Manufacturer Broadcast matches this Manufacturer ID) or false
 *         (RDMnet Device Manufacturer Broadcast does not match this Manufacturer ID).
 */
#define RDMNET_DEVICE_BROADCAST_MANU_MATCHES(uidptr, manu_val) (RDMNET_DEVICE_BROADCAST_MANU_ID(uidptr) == manu_val)

/*!
 * \brief Get the ESTA Manufacturer ID from an RDMnet Device Manufacturer Broadcast UID.
 *
 * Use RDMNET_UID_IS_DEVICE_MANU_BROADCAST() first to determine whether this UID is an RDMnet
 * Device Manufacturer Broadcast.
 *
 * \param uidptr Pointer to RdmUid from which to get the Broadcast Manufacturer ID.
 * \return ESTA Manufacturer ID.
 */
#define RDMNET_DEVICE_BROADCAST_MANU_ID(uidptr) ((uint16_t)((uidptr)->id >> 16))

/******************************* UID Inspection ******************************/

/*!
 * \brief Determine if a UID is null.
 *
 * A UID is said to be 'null' when it is made up of all 0's.
 *
 * \param uidptr Pointer to UID to null-check.
 * \return true (UID is null) or false (UID is not null).
 */
#define RDM_UID_IS_NULL(uidptr) ((uidptr)->manu == 0 && (uidptr)->id == 0)

/*!
 * \brief Determine whether a UID is a Dynamic UID as defined in ANSI E1.33.
 *
 * Note that !RDMNET_UID_IS_DYNAMIC() does not imply RDMNET_UID_IS_STATIC(), because broadcast and
 * null UID values are neither dynamic nor static UIDs.
 *
 * \param uidptr Pointer to RdmUid to check.
 * \return true (UID is an E1.33 Dynamic UID) or false (UID is not an E1.33 Dynamic UID).
 */
#define RDMNET_UID_IS_DYNAMIC(uidptr)                                                  \
  ((((uidptr)->manu & 0x8000u) != 0) && !RDMNET_UID_IS_CONTROLLER_BROADCAST(uidptr) && \
   !RDMNET_UID_IS_DEVICE_MANU_BROADCAST(uidptr) && !RDM_UID_IS_BROADCAST(uidptr))

/*!
 * \brief Determine whether a UID is a Dynamic UID Request as defined in ANSI E1.33.
 * \param uidptr Pointer to RdmUid to check.
 * \return true (UID is an E1.33 Dynamic UID Request) or false (UID is not an E1.33 Dynamic UID
 *         Request).
 */
#define RDMNET_UID_IS_DYNAMIC_UID_REQUEST(uidptr) (RDMNET_UID_IS_DYNAMIC(uidptr) && (uidptr)->id == 0u)

/*!
 * \brief Determine whether a UID is a Static UID as defined in ANSI E1.33.
 *
 * Note that !RDMNET_UID_IS_STATIC() does not imply RDMNET_UID_IS_DYNAMIC(), because broadcast and
 * null UID values are neither dynamic nor static UIDs.
 *
 * \param uidptr Pointer to RdmUid to check.
 * \return true (UID is an E1.33 Static UID) or false (UID is not an E1.33 Static UID).
 */
#define RDMNET_UID_IS_STATIC(uidptr) (!RDM_UID_IS_NULL(uidptr) && ((uidptr)->manu & 0x8000u) == 0)

/*!
 * \brief Get the ESTA Manufacturer ID from a UID.
 * \param uidptr Pointer to RdmUid from which to get the ESTA Manufacturer ID.
 * \return ESTA Manufacturer ID.
 */
#define RDM_GET_MANUFACTURER_ID(uidptr) ((uidptr)->manu & 0x7fffu)

/*!
 * \brief Get the Device ID from a UID.
 * \param uidptr Pointer to RdmUid from which to get the Device ID.
 * \return Device ID.
 */
#define RDM_GET_DEVICE_ID(uidptr) ((uidptr)->id)

/*! A UID that is equal to BROADCAST_ALL_DEVICES_ID as defined in ANSI E1.20. */
extern const RdmUid kRdmBroadcastUid;

/*! A UID that is equal to RPT_ALL_CONTROLLERS as defined in ANSI E1.33. */
extern const RdmUid kRdmnetControllerBroadcastUid;

/*! A UID that is equal to RPT_ALL_DEVICES as defined in ANSI E1.33. */
extern const RdmUid kRdmnetDeviceBroadcastUid;

/*! The maximum number of bytes for a buffer that can hold a UID string representation. */
#define RDM_UID_STRING_BYTES 14

int rdm_uid_compare(const RdmUid* a, const RdmUid* b);
bool rdm_uid_to_string(const RdmUid* uid, char* buf);
bool rdm_string_to_uid(const char* str, RdmUid* uid);

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus
/* C++ utilities */

/* Comparison operators for UIDs */

constexpr bool operator==(const RdmUid& a, const RdmUid& b) noexcept
{
  return ((a.manu == b.manu && a.id == b.id));
}

constexpr bool operator!=(const RdmUid& a, const RdmUid& b) noexcept
{
  return !(a == b);
}

constexpr bool operator<(const RdmUid& a, const RdmUid& b) noexcept
{
  return ((a.manu == b.manu) ? (a.id < b.id) : (a.manu < b.manu));
}

constexpr bool operator>(const RdmUid& a, const RdmUid& b) noexcept
{
  return b < a;
}

constexpr bool operator<=(const RdmUid& a, const RdmUid& b) noexcept
{
  return !(b < a);
}

constexpr bool operator>=(const RdmUid& a, const RdmUid& b) noexcept
{
  return !(a < b);
}

#endif /* __cplusplus */
/*!
 * @}
 */

#endif /* RDM_UID_H_ */
