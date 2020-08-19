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

#include "rdm/cpp/message_types/nack_reason.h"

#include <cstring>
#include <type_traits>
#include "gtest/gtest.h"

static_assert(!std::is_default_constructible<rdm::NackReason>::value, "NackReason should not be default-constructible");

TEST(CppNackReason, ValueConstructorWorks)
{
  rdm::NackReason nack_reason(kRdmNRDataOutOfRange);
  EXPECT_EQ(nack_reason.code(), kRdmNRDataOutOfRange);
  EXPECT_EQ(nack_reason.raw_code(), static_cast<uint16_t>(kRdmNRDataOutOfRange));
  EXPECT_TRUE(nack_reason.IsStandard());
}

TEST(CppNackReason, ImplicitConstructionWorks)
{
  rdm::NackReason nack_reason = kRdmNRDataOutOfRange;
  EXPECT_EQ(nack_reason.code(), kRdmNRDataOutOfRange);
  EXPECT_EQ(nack_reason.raw_code(), static_cast<uint16_t>(kRdmNRDataOutOfRange));
  EXPECT_TRUE(nack_reason.IsStandard());
}

TEST(CppNackReason, AssignmentOperatorWorks)
{
  rdm::NackReason nack_reason(kRdmNRDataOutOfRange);

  // Reassign
  nack_reason = kRdmNRFormatError;
  EXPECT_EQ(nack_reason.code(), kRdmNRFormatError);
  EXPECT_EQ(nack_reason.raw_code(), static_cast<uint16_t>(kRdmNRFormatError));
}

TEST(CppNackReason, RawCodeConstructorWorks)
{
  rdm::NackReason nack_reason(0xffffu);
  EXPECT_EQ(nack_reason.raw_code(), 0xffffu);
  EXPECT_FALSE(nack_reason.IsStandard());
}

TEST(CppNackReason, RawCodeAssignmentOperatorWorks)
{
  rdm::NackReason nack_reason(0xffffu);

  // Reassign
  nack_reason = 0x8000u;
  EXPECT_EQ(nack_reason.raw_code(), 0x8000u);
  EXPECT_FALSE(nack_reason.IsStandard());
}

TEST(CppNackReason, ToStringWorks)
{
  rdm::NackReason nack_reason(kRdmNRHardwareFault);
  auto            str = nack_reason.ToString();
  EXPECT_FALSE(str.empty());
}

TEST(CppNackReason, ToCStringWorks)
{
  rdm::NackReason nack_reason(kRdmNRInvalidIpv4Address);
  const char*     str = nack_reason.ToCString();
  ASSERT_NE(str, nullptr);
  EXPECT_GT(std::strlen(str), 0u);
}
