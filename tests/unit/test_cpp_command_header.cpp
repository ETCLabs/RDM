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

#include "rdm/cpp/message_types/command_header.h"

#include <array>
#include "message_test_data.h"
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

TEST(CppCmdHeader, ValueConstructorWorks)
{
  rdm::CommandHeader header({0x6574, 0x1234}, {0x6574, 0x4321}, 20, 1, 200, kRdmCCGetCommand,
                            E120_SUPPORTED_PARAMETERS);

  EXPECT_TRUE(header.IsValid());
  EXPECT_EQ(header.source_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(header.source_uid().device_id(), 0x1234u);
  EXPECT_EQ(header.dest_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(header.dest_uid().device_id(), 0x4321u);
  EXPECT_EQ(header.transaction_num(), 20u);
  EXPECT_EQ(header.port_id(), 1u);
  EXPECT_EQ(header.subdevice(), 200u);
  EXPECT_EQ(header.command_class(), kRdmCCGetCommand);
  EXPECT_EQ(header.param_id(), E120_SUPPORTED_PARAMETERS);
}

TEST(CppCmdHeader, SettersAndGettersWork)
{
  rdm::CommandHeader header;

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
  header.SetPortId(5);
  EXPECT_EQ(header.port_id(), 5u);
  header.SetSubdevice(200);
  EXPECT_EQ(header.subdevice(), 200u);
  header.SetCommandClass(kRdmCCSetCommand);
  EXPECT_EQ(header.command_class(), kRdmCCSetCommand);
  header.SetParamId(0x1000);
  EXPECT_EQ(header.param_id(), 0x1000u);
}

TEST(CppCmdHeader, IsValidWorks)
{
  // Construct a command header with valid values. Should be valid.
  rdm::CommandHeader header({0x6574, 0x1234}, {0x6574, 0x4321}, 20, 1, 0, kRdmCCGetCommand, E120_SUPPORTED_PARAMETERS);
  EXPECT_TRUE(header.IsValid());

  // Headers with response command classes should not be valid.
  header.SetCommandClass(kRdmCCDiscoveryCommandResponse);
  EXPECT_FALSE(header.IsValid());
  header.SetCommandClass(kRdmCCGetCommandResponse);
  EXPECT_FALSE(header.IsValid());
  header.SetCommandClass(kRdmCCSetCommandResponse);
  EXPECT_FALSE(header.IsValid());
}

TEST(CppCmdHeader, InspectorsWork)
{
  rdm::CommandHeader header({0x6574, 0x1234}, {0x6574, 0x4321}, 20, 1, 0, kRdmCCGetCommand, E120_SUPPORTED_PARAMETERS);
  EXPECT_TRUE(header.IsGet());

  header.SetCommandClass(kRdmCCSetCommand);
  EXPECT_TRUE(header.IsSet());

  header.SetCommandClass(kRdmCCDiscoveryCommand);
  EXPECT_TRUE(header.IsDiscovery());
}

TEST(CppCmdHeader, PackedSizeWorks)
{
  rdm::CommandHeader header;
  EXPECT_EQ(header.PackedSize(), RDM_HEADER_SIZE + 2);

  for (uint8_t i = 0; i < RDM_MAX_PDL; ++i)
  {
    EXPECT_EQ(header.PackedSize(i), RDM_HEADER_SIZE + 2 + i);
  }
}

// The serialize functions call the rdm/message.h functions that are tested more thoroughly in
// test_message.cpp. This test can therefore be more trivial.
TEST(CppCmdHeader, SerializeWorks)
{
  const auto sensor_def = rdmtest::GetSensorDefinition();

  rdm::CommandHeader header(sensor_def.cmd_header);

  // Invalid arguments
  EXPECT_EQ(header.Serialize(nullptr, 0, sensor_def.cmd_data(), sensor_def.cmd_data_size()), kEtcPalErrInvalid);

  RdmBuffer buf;
  EXPECT_TRUE(header.Serialize(buf, sensor_def.cmd_data(), sensor_def.cmd_data_size()));
  EXPECT_EQ(buf, sensor_def.cmd_buf());

  std::array<uint8_t, RDM_MAX_BYTES> raw_buf;
  EXPECT_TRUE(header.Serialize(raw_buf.data(), raw_buf.size(), sensor_def.cmd_data(), sensor_def.cmd_data_size()));
  EXPECT_EQ(std::memcmp(raw_buf.data(), buf.data, buf.data_len), 0);
}

TEST(CppCmdHeader, StaticGetWorks)
{
  const auto header =
      rdm::CommandHeader::Get(E120_SUPPORTED_PARAMETERS, {0x6574, 0x1234}, {0x6574, 0x4321}, 200, 20, 1);
  EXPECT_TRUE(header.IsValid());
  EXPECT_EQ(header.source_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(header.source_uid().device_id(), 0x1234u);
  EXPECT_EQ(header.dest_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(header.dest_uid().device_id(), 0x4321u);
  EXPECT_EQ(header.transaction_num(), 20u);
  EXPECT_EQ(header.port_id(), 1u);
  EXPECT_EQ(header.subdevice(), 200u);
  EXPECT_EQ(header.command_class(), kRdmCCGetCommand);
  EXPECT_EQ(header.param_id(), E120_SUPPORTED_PARAMETERS);
}

TEST(CppCmdHeader, StaticSetWorks)
{
  const auto header = rdm::CommandHeader::Set(E120_RESET_DEVICE, {0x6574, 0x1234}, {0x6574, 0x4321}, 200, 20, 1);
  EXPECT_TRUE(header.IsValid());
  EXPECT_EQ(header.source_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(header.source_uid().device_id(), 0x1234u);
  EXPECT_EQ(header.dest_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(header.dest_uid().device_id(), 0x4321u);
  EXPECT_EQ(header.transaction_num(), 20u);
  EXPECT_EQ(header.port_id(), 1u);
  EXPECT_EQ(header.subdevice(), 200u);
  EXPECT_EQ(header.command_class(), kRdmCCSetCommand);
  EXPECT_EQ(header.param_id(), E120_RESET_DEVICE);
}
