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

/*! \file rdm/uid.h
 *  \brief A type and helper functions for the Unique ID (UID) used in the RDM family of protocols.
 *  \author Sam Kearney*/
#ifndef _RDM_UID_H_
#define _RDM_UID_H_

#include "lwpa/int.h"

/*! \defgroup uid UID
 *  \ingroup rdm
 *  \brief Type and helper functions for the Unique ID (UID) used in the RDM family of protocols.
 *
 *  RDM-based protocols use an identifier called a UID to identify an entity that generates or
 *  consumes RDM messages.
 *
 *  The top bit of the UID is reserved for indicating broadcast values, or indicating Dynamic UIDs
 *  in ANSI E1.33.
 *
 *  The next 15 bits is the ESTA Manufacturer ID; each manufacturer of RDM equipment has a unique
 *  Manufacturer ID registered with ESTA. The MSB of the UID is reserved for indicating broadcast
 *  values, or indicating Dynamic UIDs in ANSI E1.33.
 *
 *  The remaining 32 bits is the Device ID. The Device ID is unique within a given scope; in the
 *  case of a Static UID, it is unique across all products made by the manufacturer indicated by
 *  the ESTA Manufacturer ID. In the case of a Dynamic UID, it is unique within its current E1.33
 *  Scope.
 *
 *  @{
 */

/*! The UID type. Don't forget that the top bit of the manu field has special meaning. The helper
 *  macros are your friend! */
typedef struct RdmUid
{
  uint16_t manu;
  uint32_t id;
} RdmUid;

/*************************** UID Comparison Macros ***************************/

/*! \brief Compare two UIDs.
 *  \param uidptr1 Pointer to first RdmUid to compare.
 *  \param uidptr2 Pointer to second RdmUid to compare.
 *  \return < 0 (uidptr1 is less than uidptr2)\n
 *            0 (uidptr1 is equal to uidptr2)\n
 *          > 0 (uidptr1 is greater than uidptr2)
 */
#define rdm_uid_cmp(uidptr1, uidptr2)                                               \
  (((uidptr1)->manu == (uidptr2)->manu) ? ((int)(uidptr1)->id - (int)(uidptr2)->id) \
                                        : ((int)(uidptr1)->manu - (int)(uidptr2)->manu))

/*! \brief Determine if two UIDs are equal.
 *  \param uidptr1 Pointer to first RdmUid.
 *  \param uidptr2 Pointer to second RdmUid.
 *  \return true (UIDs are equal) or false (UIDs are not equal).
 */
#define rdm_uid_equal(uidptr1, uidptr2) ((uidptr1)->manu == (uidptr2)->manu && (uidptr1)->id == (uidptr2)->id)

/************************* UID Initialization Macros *************************/

/*! Initialize a Static UID with a Manufacturer ID and Device ID.
 *  \param uidptr Pointer to RdmUid to initialize.
 *  \param manu_val ESTA Manufacturer ID.
 *  \param id_val Device ID.
 */
#define rdm_init_static_uid(uidptr, manu_val, id_val) \
  do                                                  \
  {                                                   \
    (uidptr)->manu = (manu_val);                      \
    (uidptr)->id = (id_val);                          \
  } while (0)

/*! Initialize an RDMnet Dynamic UID Request with a Manufacturer ID.
 *  \param uidptr Pointer to RdmUid to initialize.
 *  \param manu_val ESTA Manufacturer ID.
 */
#define rdmnet_init_dynamic_uid_request(uidptr, manu_val) \
  do                                                      \
  {                                                       \
    (uidptr)->manu = (0x8000u | (manu_val));              \
    (uidptr)->id = 0;                                     \
  } while (0)

/**************************** UID Broadcast Macros ***************************/

/*! \brief Determine whether a UID is the E1.20 value BROADCAST_ALL_DEVICES_ID.
 *  \param uidptr Pointer to RdmUid to check.
 *  \return true (uidptr is equal to BROADCAST_ALL_DEVICES_ID) or false (uidptr is not equal to
 *          BROADCAST_ALL_DEVICES_ID).
 */
#define rdm_uid_is_broadcast(uidptr) ((uidptr)->manu == kBroadcastUid.manu && (uidptr)->id == kBroadcastUid.id)

/*! \brief Determine whether a UID is the E1.33 value RPT_ALL_CONTROLLERS.
 *  \param uidptr Pointer to RdmUid to check.
 *  \return true (uidptr is equal to RPT_ALL_CONTROLLERS) or false (uidptr is not equal to
 *          RPT_ALL_CONTROLLERS).
 */
#define rdmnet_uid_is_controller_broadcast(uidptr) \
  ((uidptr)->manu == kRdmnetControllerBroadcastUid.manu && (uidptr)->id == kRdmnetControllerBroadcastUid.id)

/*! \brief Determine whether a UID is the E1.33 value RPT_ALL_DEVICES.
 *  \param uidptr Pointer to RdmUid to check.
 *  \return true (uidptr is equal to RPT_ALL_DEVICES) or false (uidptr is not equal to
 *          RPT_ALL_DEVICES).
 */
#define rdmnet_uid_is_device_broadcast(uidptr) \
  ((uidptr)->manu == kRdmnetDeviceBroadcastUid.manu && (uidptr)->id == kRdmnetDeviceBroadcastUid.id)

/*! \brief Determine whether a UID is one of the E1.33 values defined by RPT_ALL_MID_DEVICES.
 *
 *  RDMnet Device Manufacturer Broadcasts are directed to all Devices with a specific ESTA
 *  Manufacturer ID. If this check returns true, use rdmnet_device_broadcast_manu_id() to determine
 *  which Manufacturer ID this broadcast is directed to, or rdmnet_device_broadcast_manu_matches()
 *  to determine whether it matches a specific Manufacturer ID.
 *
 *  \param uidptr Pointer to RdmUid to check.
 *  \return true (uidptr is one of the values defined by RPT_ALL_MID_DEVICES) or false (uidptr is
 *          not one of the values defined by RPT_ALL_MID_DEVICES).
 */
#define rdmnet_uid_is_device_manu_broadcast(uidptr) \
  ((uidptr)->manu == kRdmnetControllerBroadcastUid.manu && (((uidptr)->id & 0xffffu) == 0xffffu))

/*! \brief Determine whether an RDMnet Device Manufacturer Broadcast UID
 *         matches a specific ESTA Manufacturer ID.
 *
 *  Use uid_is_rdmnet_device_manu_broadcast() first to determine whether this UID is an RDMnet Device Manufacturer
 *  Broadcast.
 *
 *  \param uidptr Pointer to RdmUid to check.
 *  \param manu_val ESTA Manufacturer ID to check against the Broadcast UID.
 *  \return true (RDMnet Device Manufacturer Broadcast matches this Manufacturer ID) or false
 *          (RDMnet Device Manufacturer Broadcast does not match this Manufacturer ID).
 */
#define rdmnet_device_broadcast_manu_matches(uidptr, manu_val) (rdmnet_device_broadcast_manu_id(uidptr) == manu_val)

/*! \brief Get the ESTA Manufacturer ID from an RDMnet Device Manufacturer
 *         Broadcast UID.
 *
 *  Use uid_is_rdmnet_device_manu_broadcast() first to determine whether this UID is an RDMnet
 *  Device Manufacturer Broadcast.
 *
 *  \param uidptr Pointer to RdmUid from which to get the Broadcast Manufacturer ID.
 *  \return ESTA Manufacturer ID.
 */
#define rdmnet_device_broadcast_manu_id(uidptr) ((uint16_t)((uidptr)->id >> 16))

/******************************* UID Inspection ******************************/

/*! \brief Determine whether a UID is a Dynamic UID as defined in ANSI E1.33.
 *
 *  Note that !rdmnet_uid_is_dynamic() does not imply rdmnet_uid_is_static(), because broadcast UID
 *  values are neither dynamic nor static UIDs.
 *
 *  \param uidptr Pointer to RdmUid to check.
 *  \return true (UID is an E1.33 Dynamic UID) or false (UID is not an E1.33 Dynamic UID).
 */
#define rdmnet_uid_is_dynamic(uidptr)                                                  \
  ((((uidptr)->manu & 0x8000u) != 0) && !rdmnet_uid_is_controller_broadcast(uidptr) && \
   !rdmnet_uid_is_device_manu_broadcast(uidptr) && !rdm_uid_is_broadcast(uidptr))

/*! \brief Determine whether a UID is a Dynamic UID Request as defined in ANSI E1.33.
 *  \param uidptr Pointer to RdmUid to check.
 *  \return true (UID is an E1.33 Dynamic UID Request) or false (UID is not an E1.33 Dynamic UID
 *          Request).
 */
#define rdmnet_uid_is_dynamic_uid_request(uidptr) (rdmnet_uid_is_dynamic(uidptr) && (uidptr)->id == 0u)

/*! \brief Determine whether a UID is a Static UID as defined in ANSI E1.33.
 *
 *  Note that !rdmnet_uid_is_static() does not imply rdmnet_uid_is_dynamic(), because broadcast UID
 *  values are neither dynamic nor static UIDs.
 *
 *  \param uidptr Pointer to RdmUid to check.
 *  \return true (UID is an E1.33 Static UID) or false (UID is not an E1.33 Static UID).
 */
#define rdmnet_uid_is_static(uidptr) (((uidptr)->manu & 0x8000u) == 0)

/*! \brief Get the ESTA Manufacturer ID from a UID.
 *  \param uidptr Pointer to RdmUid from which to get the ESTA Manufacturer ID.
 *  \return ESTA Manufacturer ID.
 */
#define rdm_get_manufacturer_id(uidptr) ((uidptr)->manu & 0x7fffu)

/*! \brief Get the Device ID from a UID.
 *  \param uidptr Pointer to RdmUid from which to get the Device ID.
 *  \return Device ID.
 */
#define rdm_get_device_id(uidptr) ((uidptr)->id)

#ifdef __cplusplus
/* C++ utilities */

/* Comparison operators for UIDs */

inline bool operator<(const RdmUid &a, const RdmUid &b)
{
  return ((a.manu == b.manu) ? (a.id < b.id) : (a.manu < b.manu));
}

inline bool operator==(const RdmUid &a, const RdmUid &b)
{
  return ((a.manu == b.manu && a.id == b.id));
}

inline bool operator!=(const RdmUid &a, const RdmUid &b)
{
  return ((a.manu != b.manu || a.id != b.id));
}

#endif /* __cplusplus */

#ifdef __cplusplus
extern "C" {
#endif

/*! A UID that is equal to BROADCAST_ALL_DEVICES_ID as defined in ANSI E1.20. */
extern const RdmUid kBroadcastUid;

/*! A UID that is equal to RPT_ALL_CONTROLLERS as defined in ANSI E1.33. */
extern const RdmUid kRdmnetControllerBroadcastUid;

/*! A UID that is equal to RPT_ALL_DEVICES as defined in ANSI E1.33. */
extern const RdmUid kRdmnetDeviceBroadcastUid;

#ifdef __cplusplus
}
#endif

/*!@}*/

#endif /* _LWPA_UID_H_ */
