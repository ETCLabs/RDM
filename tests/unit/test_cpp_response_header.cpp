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

#include "rdm/cpp/message_types/response_header.h"
#include "gtest/gtest.h"

TEST(CppRespHeader, DefaultConstructorWorks)
{
  // Default constructor should set some reasonable "null" or "zero" values
  rdm::ResponseHeader header;
  EXPECT_FALSE(header.IsValid());
  EXPECT_FALSE(header.source_uid().IsValid());
  EXPECT_FALSE(header.dest_uid().IsValid());
  EXPECT_EQ(header.transaction_num(), 0);
  EXPECT_EQ(header.message_count(), 0);
  EXPECT_EQ(header.subdevice(), 0);
  EXPECT_EQ(header.param_id(), 0);
}

TEST(CppRespHeader, ValueConstructorWorks)
{
  rdm::ResponseHeader header({0x6574, 0x1234}, {0x6574, 0x4321}, 20, kRdmResponseTypeNackReason, 3, 200,
                             kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS);

  EXPECT_TRUE(header.IsValid());
  EXPECT_EQ(header.source_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(header.source_uid().device_id(), 0x1234u);
  EXPECT_EQ(header.dest_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(header.dest_uid().device_id(), 0x4321u);
  EXPECT_EQ(header.transaction_num(), 20u);
  EXPECT_EQ(header.response_type(), kRdmResponseTypeNackReason);
  EXPECT_EQ(header.message_count(), 3u);
  EXPECT_EQ(header.subdevice(), 200u);
  EXPECT_EQ(header.command_class(), kRdmCCGetCommandResponse);
  EXPECT_EQ(header.param_id(), E120_SUPPORTED_PARAMETERS);
}

TEST(CppRespHeader, SettersAndGettersWork)
{
  rdm::ResponseHeader header;

  // Use the RdmUid setters
  RdmUid uid1 = {20, 40};
  header.SetSourceUid(uid1);
  EXPECT_EQ(header.source_uid(), uid1);
  header.SetDestUid(uid1);
  EXPECT_EQ(header.dest_uid(), uid1);

  // Use the rdm::Uid setters
  rdm::Uid uid2(60, 80);
  header.SetSourceUid(uid2);
  EXPECT_EQ(header.source_uid(), uid2);
  header.SetDestUid(uid2);
  EXPECT_EQ(header.dest_uid(), uid2);

  // Other misc setters and getters
  header.SetTransactionNum(42);
  EXPECT_EQ(header.transaction_num(), 42u);
  header.SetResponseType(kRdmResponseTypeAckTimer);
  EXPECT_EQ(header.response_type(), kRdmResponseTypeAckTimer);
  header.SetMessageCount(4);
  EXPECT_EQ(header.message_count(), 4u);
  header.SetSubdevice(200);
  EXPECT_EQ(header.subdevice(), 200u);
  header.SetCommandClass(kRdmCCSetCommand);
  EXPECT_EQ(header.command_class(), kRdmCCSetCommand);
  header.SetParamId(0x1000);
  EXPECT_EQ(header.param_id(), 0x1000u);
}

TEST(CppRespHeader, IsValidWorks)
{
  // Construct a command header with valid values. Should be valid.
  rdm::ResponseHeader header({0x6574, 0x1234}, {0x6574, 0x4321}, 20, kRdmResponseTypeNackReason, 0, 0,
                             kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS);
  EXPECT_TRUE(header.IsValid());

  // Headers with command command classes should not be valid.
  header.SetCommandClass(kRdmCCDiscoveryCommand);
  EXPECT_FALSE(header.IsValid());
  header.SetCommandClass(kRdmCCGetCommand);
  EXPECT_FALSE(header.IsValid());
  header.SetCommandClass(kRdmCCSetCommand);
  EXPECT_FALSE(header.IsValid());
}

TEST(CppRespHeader, InspectorsWork)
{
  rdm::ResponseHeader header({0x6574, 0x1234}, {0x6574, 0x4321}, 20, kRdmResponseTypeNackReason, 0, 0,
                             kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS);
  EXPECT_TRUE(header.IsGetResponse());
  header.SetCommandClass(kRdmCCSetCommandResponse);
  EXPECT_TRUE(header.IsSetResponse());
  header.SetCommandClass(kRdmCCDiscoveryCommandResponse);
  EXPECT_TRUE(header.IsDiscoveryResponse());

  header.SetResponseType(kRdmResponseTypeAck);
  EXPECT_TRUE(header.IsAck());
  header.SetResponseType(kRdmResponseTypeAckTimer);
  EXPECT_TRUE(header.IsAckTimer());
  header.SetResponseType(kRdmResponseTypeAckOverflow);
  EXPECT_TRUE(header.IsAckOverflow());
  header.SetResponseType(kRdmResponseTypeNackReason);
  EXPECT_TRUE(header.IsNack());
}
