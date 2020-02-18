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

#include "rdm/uid.h"
#include "gtest/gtest.h"

static int UidCompareHelper(uint16_t manu_a, uint32_t id_a, uint16_t manu_b, uint32_t id_b)
{
  RdmUid a{manu_a, id_a};
  RdmUid b{manu_b, id_b};
  return rdm_uid_compare(&a, &b);
}

TEST(Uid, UidComparisonIsCorrect)
{
  EXPECT_EQ(UidCompareHelper(1, 1, 1, 1), 0);
  EXPECT_LT(UidCompareHelper(1, 1, 2, 1), 0);
  EXPECT_GT(UidCompareHelper(2, 1, 1, 1), 0);

  EXPECT_EQ(UidCompareHelper(0xffff, 0xffffffff, 0xffff, 0xffffffff), 0);
  EXPECT_LT(UidCompareHelper(0, 0, 0xffff, 0xffffffff), 0);
  EXPECT_GT(UidCompareHelper(0xffff, 0xffffffff, 0, 0), 0);

  // Test some common boundary comparison issues
  EXPECT_EQ(UidCompareHelper(0, 0x7fffffff, 0, 0x7fffffff), 0);
  EXPECT_LT(UidCompareHelper(0, 0x7fffffff, 0, 0x80000000), 0);
  EXPECT_GT(UidCompareHelper(0, 0x80000000, 0, 0x7fffffff), 0);

  EXPECT_EQ(UidCompareHelper(0, 0x80000000, 0, 0x80000000), 0);
  EXPECT_LT(UidCompareHelper(0, 0, 0, 0x80000000), 0);
  EXPECT_GT(UidCompareHelper(0, 0x80000000, 0, 0), 0);

  EXPECT_EQ(UidCompareHelper(0x8000, 0, 0x8000, 0), 0);
  EXPECT_LT(UidCompareHelper(0, 0, 0x8000, 0), 0);
  EXPECT_GT(UidCompareHelper(0x8000, 0, 0, 0), 0);
}

TEST(Uid, UidEqualIsCorrect)
{
  RdmUid uid_1 = {1, 1};
  RdmUid uid_1_dup = {1, 1};
  RdmUid uid_2 = {2, 1};

  EXPECT_TRUE(RDM_UID_EQUAL(&uid_1, &uid_1_dup));
  EXPECT_FALSE(RDM_UID_EQUAL(&uid_1, &uid_2));
}

TEST(Uid, UidInitMacrosWork)
{
  RdmUid static_uid;
  RDM_INIT_STATIC_UID(&static_uid, 0x1111u, 0x22222222u);
  EXPECT_EQ(static_uid.manu, 0x1111u);
  EXPECT_EQ(static_uid.id, 0x22222222u);

  RdmUid dynamic_req;
  RDMNET_INIT_DYNAMIC_UID_REQUEST(&dynamic_req, 0x4888u);
  EXPECT_EQ(dynamic_req.manu, 0xc888u);
  EXPECT_EQ(dynamic_req.id, 0u);

  RdmUid dev_manu_bc;
  RDMNET_INIT_DEVICE_MANU_BROADCAST(&dev_manu_bc, 0x2222u);
  EXPECT_EQ(dev_manu_bc.manu, kRdmnetDeviceBroadcastUid.manu);
  EXPECT_EQ(dev_manu_bc.id, 0x2222ffffu);
}

TEST(Uid, BroadcastIsCorrect)
{
  RdmUid bc = kRdmBroadcastUid;
  EXPECT_TRUE(RDM_UID_IS_BROADCAST(&bc));
  EXPECT_FALSE(RDMNET_UID_IS_CONTROLLER_BROADCAST(&bc));
  EXPECT_FALSE(RDMNET_UID_IS_DEVICE_BROADCAST(&bc));
}

TEST(Uid, ControllerBroadcastIsCorrect)
{
  RdmUid cb = kRdmnetControllerBroadcastUid;
  EXPECT_TRUE(RDMNET_UID_IS_CONTROLLER_BROADCAST(&cb));
  EXPECT_FALSE(RDM_UID_IS_BROADCAST(&cb));
  EXPECT_FALSE(RDMNET_UID_IS_DEVICE_BROADCAST(&cb));
}

TEST(Uid, DeviceBroadcastIsCorrect)
{
  RdmUid db = kRdmnetDeviceBroadcastUid;
  EXPECT_TRUE(RDMNET_UID_IS_DEVICE_BROADCAST(&db));
  EXPECT_FALSE(RDM_UID_IS_BROADCAST(&db));
  EXPECT_FALSE(RDMNET_UID_IS_CONTROLLER_BROADCAST(&db));
}

TEST(Uid, DeviceManuBroadcastIsCorrect)
{
  RdmUid dmb;
  RDMNET_INIT_DEVICE_MANU_BROADCAST(&dmb, 0x5555);
  EXPECT_TRUE(RDMNET_UID_IS_DEVICE_MANU_BROADCAST(&dmb));
  EXPECT_FALSE(RDM_UID_IS_BROADCAST(&dmb));
  EXPECT_FALSE(RDMNET_UID_IS_CONTROLLER_BROADCAST(&dmb));
  EXPECT_FALSE(RDMNET_UID_IS_DEVICE_BROADCAST(&dmb));

  EXPECT_EQ(RDMNET_DEVICE_BROADCAST_MANU_ID(&dmb), 0x5555);
  EXPECT_TRUE(RDMNET_DEVICE_BROADCAST_MANU_MATCHES(&dmb, 0x5555));
}

TEST(Uid, NullUidWorks)
{
  RdmUid null{};
  EXPECT_TRUE(RDM_UID_IS_NULL(&null));
}

TEST(Uid, IsDynamicWorks)
{
  RdmUid dynamic_uid = {0x8111u, 0x12345678u};
  EXPECT_TRUE(RDMNET_UID_IS_DYNAMIC(&dynamic_uid));

  RdmUid static_uid = {0x0111u, 0x12345678u};
  EXPECT_FALSE(RDMNET_UID_IS_DYNAMIC(&static_uid));
}

TEST(Uid, IsStaticWorks)
{
  RdmUid dynamic_uid = {0x8111u, 0x12345678u};
  EXPECT_FALSE(RDMNET_UID_IS_STATIC(&dynamic_uid));

  RdmUid static_uid = {0x0111u, 0x12345678u};
  EXPECT_TRUE(RDMNET_UID_IS_STATIC(&static_uid));

  RdmUid null_uid{};
  EXPECT_FALSE(RDMNET_UID_IS_STATIC(&null_uid));
}

TEST(Uid, GettersWork)
{
  RdmUid uid = {0x8003u, 0x33333333u};
  EXPECT_EQ(RDM_GET_MANUFACTURER_ID(&uid), 0x0003u);
  EXPECT_EQ(RDM_GET_DEVICE_ID(&uid), 0x33333333u);
}

TEST(Uid, UidToStringHandlesInvalidCalls)
{
  char str_buf[RDM_UID_STRING_BYTES];
  RdmUid uid = {0x6574u, 0x88888888u};

  EXPECT_FALSE(rdm_uid_to_string(nullptr, str_buf));
  EXPECT_FALSE(rdm_uid_to_string(&uid, nullptr));
  EXPECT_FALSE(rdm_uid_to_string(nullptr, nullptr));
}

TEST(Uid, NormalUidToStringWorks)
{
  char str_buf[RDM_UID_STRING_BYTES];
  RdmUid uid = {0x6574u, 0x88884444u};

  ASSERT_TRUE(rdm_uid_to_string(&uid, str_buf));
  EXPECT_STREQ(str_buf, "6574:88884444");
}

TEST(Uid, NullUidToStringWorks)
{
  RdmUid null_uid{};
  char str_buf[RDM_UID_STRING_BYTES];
  ASSERT_TRUE(rdm_uid_to_string(&null_uid, str_buf));
  EXPECT_STREQ(str_buf, "0000:00000000");
}

TEST(Uid, StringToUidHandlesInvalidCalls)
{
  RdmUid uid;

  EXPECT_FALSE(rdm_string_to_uid(nullptr, &uid));
  EXPECT_FALSE(rdm_string_to_uid("6574:88888888", nullptr));
  EXPECT_FALSE(rdm_string_to_uid(nullptr, nullptr));
}

TEST(Uid, StringToUidConversionWorks)
{
  RdmUid uid;
  ASSERT_TRUE(rdm_string_to_uid("6574:0a0b0c0d", &uid));
  EXPECT_EQ(uid.manu, 0x6574u);
  EXPECT_EQ(uid.id, 0x0a0b0c0du);

  ASSERT_TRUE(rdm_string_to_uid("65740a0b0c0d", &uid));
  EXPECT_EQ(uid.manu, 0x6574u);
  EXPECT_EQ(uid.id, 0x0a0b0c0du);
}

TEST(Uid, StringToUidConversionRejectsBadStrings)
{
  const char* kBadUidStrings[] = {
      "0a0b:g0112233",      // Bad character in the device ID
      "0a0g:00112233",      // Bad character in the manufacturer ID
      "Not a UID",          // Short random string
      "This is not a UID",  // Long random string
  };

  for (const char* str : kBadUidStrings)
  {
    RdmUid uid;
    EXPECT_FALSE(rdm_string_to_uid(str, &uid)) << "Failed on input: " << str;
  }
}
