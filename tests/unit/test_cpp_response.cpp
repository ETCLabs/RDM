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

#include <array>
#include <cstring>
#include "gtest/gtest.h"

TEST(CppRespHeader, DefaultConstructorWorks)
{
  rdm::ResponseHeader header;
}

TEST(CppResponse, DefaultConstructorWorks)
{
  rdm::Response response;
  EXPECT_FALSE(response.IsValid());
  EXPECT_FALSE(response.header().IsValid());
  EXPECT_FALSE(response.HasData());
}

TEST(CppResponse, HeaderConstructorWorks)
{
  rdm::ResponseHeader header(rdm::Uid(0x1234, 0x56789abc), rdm::Uid(0xcba9, 0x87654321), 0x22, kRdmResponseTypeAck, 0,
                             0, kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS);

  std::array<uint8_t, 4> resp_data{0, 1, 2, 3};
  rdm::Response response(header, resp_data.data(), resp_data.size());

  EXPECT_TRUE(response.IsValid());
  EXPECT_TRUE(response.header().IsValid());
  EXPECT_TRUE(response.HasData());

  ASSERT_EQ(response.data_len(), resp_data.size());
  EXPECT_EQ(0, std::memcmp(response.data(), resp_data.data(), resp_data.size()));

  // Test the C-style header constructor
  response = rdm::Response(header.get(), resp_data.data(), resp_data.size());
  EXPECT_TRUE(response.IsValid());
  EXPECT_TRUE(response.header().IsValid());
  EXPECT_TRUE(response.HasData());

  ASSERT_EQ(response.data_len(), resp_data.size());
  EXPECT_EQ(0, std::memcmp(response.data(), resp_data.data(), resp_data.size()));
}
