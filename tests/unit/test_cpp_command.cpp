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

TEST(CppCommand, DefaultConstructorWorks)
{
  // Default constructor should set some reasonable "null" or "zero" values
  rdm::Command cmd;
  EXPECT_FALSE(cmd.IsValid());
  EXPECT_FALSE(cmd.source_uid().IsValid());
  EXPECT_FALSE(cmd.dest_uid().IsValid());
  EXPECT_EQ(cmd.transaction_num(), 0);
  EXPECT_EQ(cmd.port_id(), 0);
  EXPECT_EQ(cmd.subdevice(), 0);
  EXPECT_EQ(cmd.param_id(), 0);
  EXPECT_EQ(cmd.datalen(), 0);
  EXPECT_EQ(cmd.data(), nullptr);
}

TEST(CppCommand, UidSettersWork)
{
  rdm::Command cmd;

  RdmUid uid1 = {20, 40};
  cmd.SetSourceUid(uid1);
  EXPECT_EQ(cmd.source_uid(), uid1);

  cmd.SetDestUid(uid1);
  EXPECT_EQ(cmd.dest_uid(), uid1);

  rdm::Uid uid2(60, 80);
  cmd.SetSourceUid(uid2);
  EXPECT_EQ(cmd.source_uid(), uid2);
  cmd.SetDestUid(uid2);
  EXPECT_EQ(cmd.dest_uid(), uid2);
}
