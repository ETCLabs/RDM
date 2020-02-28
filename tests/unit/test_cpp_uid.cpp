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

#include "rdm/cpp/uid.h"
#include "gtest/gtest.h"

TEST(CppUid, DefaultConstructorWorks)
{
  const rdm::Uid uid;
  EXPECT_FALSE(uid.IsValid());
  EXPECT_FALSE(uid.IsBroadcast());
  EXPECT_FALSE(uid.IsControllerBroadcast());
  EXPECT_FALSE(uid.IsDeviceBroadcast());
  EXPECT_FALSE(uid.IsDeviceManufacturerBroadcast());
  EXPECT_FALSE(uid.IsDynamic());
  EXPECT_FALSE(uid.IsDynamicUidRequest());
  EXPECT_FALSE(uid.IsStatic());
}

TEST(CppUid, ValueConstructorWorks)
{
  const rdm::Uid uid(0x5555u, 0x88888888u);
  EXPECT_TRUE(uid.IsValid());
  EXPECT_EQ(uid.manufacturer_id(), 0x5555u);
  EXPECT_EQ(uid.device_id(), 0x88888888u);
}

TEST(CppUid, CopyConstructorsWork)
{
  // Copy from an RdmUid
  const RdmUid c_uid{0x5555u, 0x88888888u};
  const rdm::Uid uid(c_uid);
  EXPECT_TRUE(uid.IsValid());
  EXPECT_EQ(uid.get(), c_uid);
  EXPECT_EQ(uid.manufacturer_id(), 0x5555u);
  EXPECT_EQ(uid.device_id(), 0x88888888u);

  // Use the default copy constructor
  const rdm::Uid uid2(uid);
  EXPECT_TRUE(uid2.IsValid());
  EXPECT_EQ(uid2, uid);
  EXPECT_EQ(uid2.get(), c_uid);
  EXPECT_EQ(uid2.manufacturer_id(), 0x5555u);
  EXPECT_EQ(uid2.device_id(), 0x88888888u);
}

TEST(CppUid, AssignmentOperatorsWork)
{
  const RdmUid c_uid{0x5555u, 0x88888888u};
  rdm::Uid uid;

  // Assign to an RdmUid
  uid = c_uid;
  EXPECT_TRUE(uid.IsValid());
  EXPECT_EQ(uid.get(), c_uid);
  EXPECT_EQ(uid.manufacturer_id(), 0x5555u);
  EXPECT_EQ(uid.device_id(), 0x88888888u);

  // Test the default assignment operator
  const rdm::Uid null_uid;
  uid = null_uid;
  EXPECT_EQ(uid, null_uid);
  EXPECT_EQ(uid.get(), null_uid.get());
  EXPECT_FALSE(uid.IsValid());
}

TEST(CppUid, SettersWork)
{
  rdm::Uid uid;
  uid.SetManufacturerId(0x5555u);
  EXPECT_TRUE(uid.IsValid());
  EXPECT_EQ(uid.manufacturer_id(), 0x5555u);
  EXPECT_TRUE(uid.IsStatic());

  uid.SetDeviceId(0x88888888u);
  EXPECT_TRUE(uid.IsValid());
  EXPECT_EQ(uid.device_id(), 0x88888888u);
}

TEST(CppUid, ToStringWorks)
{
  const rdm::Uid uid(0x5555u, 0x88888888u);
  EXPECT_EQ(uid.ToString(), "5555:88888888");

  const rdm::Uid null_uid;
  EXPECT_EQ(null_uid.ToString(), "0000:00000000");
}

TEST(CppUid, FromStringWorks)
{
  const auto uid = rdm::Uid::FromString("6574:01234567");
  EXPECT_TRUE(uid.IsValid());
  EXPECT_EQ(uid.manufacturer_id(), 0x6574u);
  EXPECT_EQ(uid.device_id(), 0x01234567u);

  const auto bad_uid = rdm::Uid::FromString("Bad string");
  EXPECT_FALSE(bad_uid.IsValid());
}

TEST(CppUid, BroadcastWorks)
{
  const auto broadcast = rdm::Uid::Broadcast();
  EXPECT_TRUE(broadcast.IsValid());
  EXPECT_TRUE(broadcast.IsBroadcast());
  EXPECT_FALSE(broadcast.IsControllerBroadcast());
  EXPECT_FALSE(broadcast.IsDeviceBroadcast());
  EXPECT_FALSE(broadcast.IsDeviceManufacturerBroadcast());
  EXPECT_FALSE(broadcast.IsDynamic());
  EXPECT_FALSE(broadcast.IsDynamicUidRequest());
  EXPECT_FALSE(broadcast.IsStatic());
}

TEST(CppUid, ControllerBroadcastWorks)
{
  const auto cb = rdm::Uid::ControllerBroadcast();
  EXPECT_TRUE(cb.IsValid());
  EXPECT_FALSE(cb.IsBroadcast());
  EXPECT_TRUE(cb.IsControllerBroadcast());
  EXPECT_FALSE(cb.IsDeviceBroadcast());
  EXPECT_FALSE(cb.IsDeviceManufacturerBroadcast());
  EXPECT_FALSE(cb.IsDynamic());
  EXPECT_FALSE(cb.IsDynamicUidRequest());
  EXPECT_FALSE(cb.IsStatic());
}

TEST(CppUid, DeviceBroadcastWorks)
{
  const auto db = rdm::Uid::DeviceBroadcast();
  EXPECT_TRUE(db.IsValid());
  EXPECT_FALSE(db.IsBroadcast());
  EXPECT_FALSE(db.IsControllerBroadcast());
  EXPECT_TRUE(db.IsDeviceBroadcast());
  EXPECT_TRUE(db.IsDeviceManufacturerBroadcast());
  EXPECT_FALSE(db.IsDynamic());
  EXPECT_FALSE(db.IsDynamicUidRequest());
  EXPECT_FALSE(db.IsStatic());
}

TEST(CppUid, DeviceManufacturerBroadcastWorks)
{
  const auto db = rdm::Uid::DeviceBroadcast(0x6574);
  EXPECT_TRUE(db.IsValid());
  EXPECT_FALSE(db.IsBroadcast());
  EXPECT_FALSE(db.IsControllerBroadcast());
  EXPECT_FALSE(db.IsDeviceBroadcast());
  EXPECT_TRUE(db.IsDeviceManufacturerBroadcast());
  EXPECT_FALSE(db.IsDynamic());
  EXPECT_FALSE(db.IsDynamicUidRequest());
  EXPECT_FALSE(db.IsStatic());
  EXPECT_EQ(db.DeviceBroadcastManufacturerId(), 0x6574u);
}

TEST(CppUid, StaticGeneratorWorks)
{
  const auto stat_uid = rdm::Uid::Static(0x5555, 0x88888888);
  EXPECT_TRUE(stat_uid.IsValid());
  EXPECT_TRUE(stat_uid.IsStatic());
  EXPECT_FALSE(stat_uid.IsDynamic());
  EXPECT_EQ(stat_uid.manufacturer_id(), 0x5555u);
  EXPECT_EQ(stat_uid.device_id(), 0x88888888u);
}

TEST(CppUid, DynamicRequestGeneratorWorks)
{
  const auto dynamic_uid = rdm::Uid::DynamicUidRequest(0x5555);
  EXPECT_TRUE(dynamic_uid.IsValid());
  EXPECT_TRUE(dynamic_uid.IsDynamicUidRequest());
  EXPECT_EQ(dynamic_uid.manufacturer_id(), 0x5555u);
}

TEST(CppUid, ComparisonOperatorsWork)
{
  rdm::Uid uid_1;
  rdm::Uid uid_1_dup;
  rdm::Uid uid_2(0, 1);
  RdmUid uid_3{1, 0};

  // uid_1 should be equal to uid_1_dup.
  EXPECT_EQ(uid_1, uid_1_dup);       // ==
  EXPECT_FALSE(uid_1 != uid_1_dup);  // !=
  EXPECT_FALSE(uid_1 < uid_1_dup);   // <
  EXPECT_FALSE(uid_1 > uid_1_dup);   // >
  EXPECT_LE(uid_1, uid_1_dup);       // <=
  EXPECT_GE(uid_1, uid_1_dup);       // >=

  // uid_1 should be less than uid_2.
  EXPECT_FALSE(uid_1 == uid_2);  // ==
  EXPECT_NE(uid_1, uid_2);       // !=
  EXPECT_LT(uid_1, uid_2);       // <
  EXPECT_FALSE(uid_1 > uid_2);   // >
  EXPECT_LE(uid_1, uid_2);       // <=
  EXPECT_FALSE(uid_1 >= uid_2);  // >=

  // uid_1 should be less than uid_3
  EXPECT_FALSE(uid_1 == uid_3);  // ==
  EXPECT_NE(uid_1, uid_3);       // !=
  EXPECT_LT(uid_1, uid_3);       // <
  EXPECT_FALSE(uid_1 > uid_3);   // >
  EXPECT_LE(uid_1, uid_3);       // <=
  EXPECT_FALSE(uid_1 >= uid_3);  // >=

  // uid_2 should be less than uid_3
  // This also tests the RdmUid <=> rdm::Uid comparison, so we test each one on both sides
  EXPECT_FALSE(uid_2 == uid_3);  // ==
  EXPECT_FALSE(uid_3 == uid_2);  // ==
  EXPECT_NE(uid_2, uid_3);       // !=
  EXPECT_NE(uid_3, uid_2);       // !=
  EXPECT_LT(uid_2, uid_3);       // <
  EXPECT_FALSE(uid_3 < uid_2);   // <
  EXPECT_FALSE(uid_2 > uid_3);   // >
  EXPECT_GT(uid_3, uid_2);       // >
  EXPECT_LE(uid_2, uid_3);       // <=
  EXPECT_FALSE(uid_3 <= uid_2);  // <=
  EXPECT_FALSE(uid_2 >= uid_3);  // >=
  EXPECT_GE(uid_3, uid_2);       // >=
}

// Test the comparison operators for RdmUid structs
TEST(CppUid, RdmUidComparisonOperatorsWork)
{
  RdmUid uid_1{0, 0};
  RdmUid uid_1_dup = uid_1;
  RdmUid uid_2{0, 1};
  RdmUid uid_3{1, 0};

  // uid_1 should be equal to uid_1_dup.
  EXPECT_EQ(uid_1, uid_1_dup);       // ==
  EXPECT_FALSE(uid_1 != uid_1_dup);  // !=
  EXPECT_FALSE(uid_1 < uid_1_dup);   // <
  EXPECT_FALSE(uid_1 > uid_1_dup);   // >
  EXPECT_LE(uid_1, uid_1_dup);       // <=
  EXPECT_GE(uid_1, uid_1_dup);       // >=

  // uid_1 should be less than uid_2.
  EXPECT_FALSE(uid_1 == uid_2);  // ==
  EXPECT_NE(uid_1, uid_2);       // !=
  EXPECT_LT(uid_1, uid_2);       // <
  EXPECT_FALSE(uid_1 > uid_2);   // >
  EXPECT_LE(uid_1, uid_2);       // <=
  EXPECT_FALSE(uid_1 >= uid_2);  // >=

  // uid_1 should be less than uid_3
  EXPECT_FALSE(uid_1 == uid_3);  // ==
  EXPECT_NE(uid_1, uid_3);       // !=
  EXPECT_LT(uid_1, uid_3);       // <
  EXPECT_FALSE(uid_1 > uid_3);   // >
  EXPECT_LE(uid_1, uid_3);       // <=
  EXPECT_FALSE(uid_1 >= uid_3);  // >=

  // uid_2 should be less than uid_3
  EXPECT_FALSE(uid_2 == uid_3);  // ==
  EXPECT_NE(uid_2, uid_3);       // !=
  EXPECT_LT(uid_2, uid_3);       // <
  EXPECT_FALSE(uid_2 > uid_3);   // >
  EXPECT_LE(uid_2, uid_3);       // <=
  EXPECT_FALSE(uid_2 >= uid_3);  // >=
}
