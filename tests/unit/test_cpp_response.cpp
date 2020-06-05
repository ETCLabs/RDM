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

#include "rdm/cpp/message_types/response.h"

#include <array>
#include <cstring>
#include "gtest/gtest.h"

TEST(CppResponse, DefaultConstructorWorks)
{
  rdm::Response response;
  EXPECT_FALSE(response.IsValid());
  EXPECT_FALSE(response.header().IsValid());
  EXPECT_FALSE(response.HasData());
}

TEST(CppResponse, ValueConstructorWorks)
{
  rdm::Response resp({0x1234, 0x56789abc}, {0x1234, 0x87654321}, 0x22, kRdmResponseTypeAck, 3, 200,
                     kRdmCCSetCommandResponse, E120_DEVICE_LABEL);

  EXPECT_TRUE(resp.IsValid());
  EXPECT_EQ(resp.source_uid().manufacturer_id(), 0x1234u);
  EXPECT_EQ(resp.source_uid().device_id(), 0x56789abcu);
  EXPECT_EQ(resp.dest_uid().manufacturer_id(), 0x1234u);
  EXPECT_EQ(resp.dest_uid().device_id(), 0x87654321u);
  EXPECT_EQ(resp.transaction_num(), 0x22u);
  EXPECT_EQ(resp.response_type(), kRdmResponseTypeAck);
  EXPECT_EQ(resp.message_count(), 3u);
  EXPECT_EQ(resp.subdevice(), 200u);
  EXPECT_EQ(resp.command_class(), kRdmCCSetCommandResponse);
  EXPECT_EQ(resp.param_id(), E120_DEVICE_LABEL);

  EXPECT_FALSE(resp.HasData());
  EXPECT_EQ(resp.data(), nullptr);
  EXPECT_EQ(resp.data_len(), 0);

  // Construct a response with parameter data
  const std::array<uint8_t, 10> kSupportedParams = {0x00, 0x10, 0x00, 0x20, 0x10, 0x01, 0x10, 0x02, 0x80, 0x01};
  resp = rdm::Response({0x1234, 0x56789abc}, {0x1234, 0x87654321}, 0x22, kRdmResponseTypeAck, 3, 200,
                       kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS, kSupportedParams.data(),
                       kSupportedParams.size());

  EXPECT_TRUE(resp.HasData());
  ASSERT_EQ(resp.data_len(), kSupportedParams.size());
  EXPECT_EQ(std::memcmp(resp.data(), kSupportedParams.data(), kSupportedParams.size()), 0);
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

TEST(CppResponse, SettersAndGettersWork)
{
  // Construct a response with parameter data
  const std::array<uint8_t, 10> kSupportedParams = {0x00, 0x10, 0x00, 0x20, 0x10, 0x01, 0x10, 0x02, 0x80, 0x01};
  rdm::Response resp({0x1234, 0x56789abc}, {0x1234, 0x87654321}, 0x22, kRdmResponseTypeAck, 3, 200,
                     kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS, kSupportedParams.data(),
                     kSupportedParams.size());

  ASSERT_EQ(resp.data_len(), kSupportedParams.size());
  EXPECT_EQ(std::memcmp(resp.data(), kSupportedParams.data(), kSupportedParams.size()), 0);

  resp.ClearData();
  EXPECT_FALSE(resp.HasData());
  EXPECT_EQ(resp.data_len(), 0u);
  EXPECT_EQ(resp.data(), nullptr);

  resp.SetData(nullptr, 0);
  EXPECT_FALSE(resp.HasData());
  EXPECT_EQ(resp.data_len(), 0u);
  EXPECT_EQ(resp.data(), nullptr);

  resp.SetData(kSupportedParams.data(), kSupportedParams.size());
  EXPECT_TRUE(resp.HasData());
  ASSERT_EQ(resp.data_len(), kSupportedParams.size());
  EXPECT_EQ(std::memcmp(resp.data(), kSupportedParams.data(), kSupportedParams.size()), 0);
}

TEST(CppResponse, GetAckTimerDelayWorks)
{
  // A default constructed response should return an error for GetAckTimerDelayMs()
  rdm::Response resp;
  EXPECT_FALSE(resp.GetAckTimerDelayMs().has_value());

  // Construct an ACK_TIMER response
  const std::array<uint8_t, 2> kTimerVal{0x22, 0x60};
  resp = rdm::Response({0x1234, 0x56789abc}, {0x1234, 0x87654321}, 0x22, kRdmResponseTypeAckTimer, 3, 0,
                       kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS, kTimerVal.data(), kTimerVal.size());

  EXPECT_TRUE(resp.IsAckTimer());
  auto timer_ms = resp.GetAckTimerDelayMs();
  ASSERT_TRUE(timer_ms.has_value());
  EXPECT_EQ(*timer_ms, 880000u);
}

TEST(CppResponse, GetNackReasonWorks)
{
  // A default constructed response should return an error for GetNackReason()
  rdm::Response resp;
  EXPECT_FALSE(resp.GetNackReason().has_value());

  // Construct a NACK response
  const std::array<uint8_t, 2> kNackReason = {0x00, 0x07};
  resp = rdm::Response({0x1234, 0x56789abc}, {0x1234, 0x87654321}, 0x22, kRdmResponseTypeNackReason, 3, 0,
                       kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS, kNackReason.data(), kNackReason.size());

  EXPECT_TRUE(resp.IsNack());
  auto nack_reason = resp.GetNackReason();
  ASSERT_TRUE(nack_reason.has_value());
  EXPECT_EQ(nack_reason->code(), kRdmNRBufferFull);
}
