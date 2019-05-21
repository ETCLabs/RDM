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
#include "gtest/gtest.h"
#include "rdm/uid.h"

class UidTest : public ::testing::Test
{
};

// Test the operator== and operator!= functions for RdmUid structs
TEST_F(UidTest, operator_equal)
{
  RdmUid uid_1{0, 0};
  RdmUid uid_2{0, 0};

  EXPECT_EQ(uid_1, uid_2);
  EXPECT_EQ(uid_2, uid_1);

  uid_1.manu = 1;
  EXPECT_NE(uid_1, uid_2);
  EXPECT_NE(uid_2, uid_1);

  uid_1.manu = 0;
  uid_1.id = 1;
  EXPECT_NE(uid_1, uid_2);
  EXPECT_NE(uid_2, uid_1);
}

TEST_F(UidTest, operator_less)
{
  RdmUid uid_1{0, 0};
  RdmUid uid_2{0, 0};

  EXPECT_FALSE(uid_1 < uid_2);

  uid_2.manu = 1;
  EXPECT_TRUE(uid_1 < uid_2);
  EXPECT_FALSE(uid_2 < uid_1);

  uid_2.manu = 0;
  uid_2.id = 1;
  EXPECT_TRUE(uid_1 < uid_2);
  EXPECT_FALSE(uid_2 < uid_1);

  uid_2.manu = 1;
  EXPECT_TRUE(uid_1 < uid_2);
  EXPECT_FALSE(uid_2 < uid_1);
}
