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

#include "rdm/cpp/message.h"
#include "gtest/gtest.h"

TEST(CppCmdHeader, DefaultConstructorWorks)
{
  // Default constructor should set some reasonable "null" or "zero" values
  rdm::CommandHeader header;
  EXPECT_FALSE(header.IsValid());
  EXPECT_FALSE(header.source_uid().IsValid());
  EXPECT_FALSE(header.dest_uid().IsValid());
  EXPECT_EQ(header.transaction_num(), 0);
  EXPECT_EQ(header.port_id(), 0);
  EXPECT_EQ(header.subdevice(), 0);
  EXPECT_EQ(header.param_id(), 0);
}

TEST(CppCmdHeader, UidSettersWork)
{
  rdm::CommandHeader header;

  RdmUid uid1 = {20, 40};
  header.SetSourceUid(uid1);
  EXPECT_EQ(header.source_uid(), uid1);

  header.SetDestUid(uid1);
  EXPECT_EQ(header.dest_uid(), uid1);

  rdm::Uid uid2(60, 80);
  header.SetSourceUid(uid2);
  EXPECT_EQ(header.source_uid(), uid2);
  header.SetDestUid(uid2);
  EXPECT_EQ(header.dest_uid(), uid2);
}

TEST(CppCmd, DefaultConstructorWorks)
{
  rdm::Command cmd;
  EXPECT_FALSE(cmd.IsValid());
  EXPECT_FALSE(cmd.header().IsValid());
  EXPECT_FALSE(cmd.HasData());
}

TEST(CppCmd, HeaderConstructorWorks)
{
  rdm::CommandHeader header(rdm::Uid(0x1234, 0x56789abc), rdm::Uid(0xba98, 0x76543210), 0x22, 1, 0, kRdmCCSetCommand,
                            E120_IDENTIFY_DEVICE);
  uint8_t identify_val = 1u;
  rdm::Command cmd(header, &identify_val, 1);

  EXPECT_TRUE(cmd.IsValid());
  EXPECT_TRUE(cmd.header().IsValid());
  EXPECT_TRUE(cmd.HasData());

  ASSERT_EQ(cmd.data_len(), 1u);
  EXPECT_EQ(*cmd.data(), 1u);

  // Test the C-style header constructor
  cmd = rdm::Command(header.get(), &identify_val, 1);
  EXPECT_TRUE(cmd.IsValid());
  EXPECT_TRUE(cmd.header().IsValid());
  EXPECT_TRUE(cmd.HasData());

  ASSERT_EQ(cmd.data_len(), 1u);
  EXPECT_EQ(*cmd.data(), 1u);
}
