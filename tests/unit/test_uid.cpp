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

TEST(Uid, UidToStringHandlesInvalidCalls)
{
  char str_buf[RDM_UID_STRING_BYTES];
  RdmUid uid = {0x6574u, 0x88888888u};

  EXPECT_EQ(kEtcPalErrInvalid, rdm_uid_to_string(nullptr, str_buf));
  EXPECT_EQ(kEtcPalErrInvalid, rdm_uid_to_string(&uid, nullptr));
  EXPECT_EQ(kEtcPalErrInvalid, rdm_uid_to_string(nullptr, nullptr));
}

TEST(Uid, NormalUidToStringWorks)
{
  char str_buf[RDM_UID_STRING_BYTES];
  RdmUid uid = {0x6574u, 0x88884444u};

  ASSERT_EQ(kEtcPalErrOk, rdm_uid_to_string(&uid, str_buf));
  EXPECT_STREQ(str_buf, "6574:88884444");
}

TEST(Uid, NullUidToStringWorks)
{
  char str_buf[RDM_UID_STRING_BYTES];
  ASSERT_EQ(kEtcPalErrOk, rdm_uid_to_string(&kRdmNullUid, str_buf));
  EXPECT_STREQ(str_buf, "0000:00000000");
}

TEST(Uid, StringToUidHandlesInvalidCalls)
{
  RdmUid uid;

  EXPECT_EQ(kEtcPalErrInvalid, rdm_string_to_uid(nullptr, &uid));
  EXPECT_EQ(kEtcPalErrInvalid, rdm_string_to_uid("6574:88888888", nullptr));
  EXPECT_EQ(kEtcPalErrInvalid, rdm_string_to_uid(nullptr, nullptr));
}

TEST(Uid, StringToUidConversionWorks)
{
  RdmUid uid;
  ASSERT_EQ(kEtcPalErrOk, rdm_string_to_uid("6574:0a0b0c0d", &uid));
  EXPECT_EQ(uid.manu, 0x6574u);
  EXPECT_EQ(uid.id, 0x0a0b0c0du);

  ASSERT_EQ(kEtcPalErrOk, rdm_string_to_uid("65740a0b0c0d", &uid));
  EXPECT_EQ(uid.manu, 0x6574u);
  EXPECT_EQ(uid.id, 0x0a0b0c0du);
}

TEST(Uid, StringToUidConversionRejectsBadStrings)
{
  const char* kBadUidStrings[] = {
    "0a0b:g0112233", // Bad character in the device ID
    "0a0g:00112233", // Bad character in the manufacturer ID
    "Not a UID", // Short random string
    "This is not a UID", // Long random string
  };

  for (const char* str : kBadUidStrings)
  {
    RdmUid uid;
    EXPECT_EQ(kEtcPalErrInvalid, rdm_string_to_uid(str, &uid)) << "Failed on input: " << str;
  }
}
