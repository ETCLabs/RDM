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

#include "rdm/cpp/message_types/command.h"

#include <array>
#include "message_test_data.h"
#include "gtest/gtest.h"

TEST(CppCmd, DefaultConstructorWorks)
{
  rdm::Command cmd;
  EXPECT_FALSE(cmd.IsValid());
  EXPECT_FALSE(cmd.header().IsValid());
  EXPECT_FALSE(cmd.HasData());
}

TEST(CppCmd, ValueConstructorWorks)
{
  rdm::Command cmd({0x6574, 0x1234}, {0x6574, 0x4321}, 20, 1, 200, kRdmCCGetCommand, E120_SUPPORTED_PARAMETERS);

  EXPECT_TRUE(cmd.IsValid());
  EXPECT_EQ(cmd.source_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(cmd.source_uid().device_id(), 0x1234u);
  EXPECT_EQ(cmd.dest_uid().manufacturer_id(), 0x6574u);
  EXPECT_EQ(cmd.dest_uid().device_id(), 0x4321u);
  EXPECT_EQ(cmd.transaction_num(), 20u);
  EXPECT_EQ(cmd.port_id(), 1u);
  EXPECT_EQ(cmd.subdevice(), 200u);
  EXPECT_EQ(cmd.command_class(), kRdmCCGetCommand);
  EXPECT_EQ(cmd.param_id(), E120_SUPPORTED_PARAMETERS);

  EXPECT_FALSE(cmd.HasData());
  EXPECT_EQ(cmd.data(), nullptr);
  EXPECT_EQ(cmd.data_len(), 0);

  // Construct a command with parameter data.
  const uint8_t sens_num = 4;
  cmd = rdm::Command({0x6574, 0x1234}, {0x6574, 0x4321}, 20, 1, 200, kRdmCCGetCommand, E120_SENSOR_DEFINITION,
                     &sens_num, 1);

  EXPECT_TRUE(cmd.HasData());
  ASSERT_EQ(cmd.data_len(), 1);
  EXPECT_EQ(cmd.data()[0], sens_num);
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
  EXPECT_EQ(cmd.data()[0], 1u);

  // Test the C-style header constructor
  cmd = rdm::Command(header.get(), &identify_val, 1);
  EXPECT_TRUE(cmd.IsValid());
  EXPECT_TRUE(cmd.header().IsValid());
  EXPECT_TRUE(cmd.HasData());

  ASSERT_EQ(cmd.data_len(), 1u);
  EXPECT_EQ(cmd.data()[0], 1u);
}

TEST(CppCmd, SettersAndGettersWork)
{
  // Construct a command with parameter data.
  constexpr char kTestDeviceLabel[] = "Test Device Label";
  auto cmd = rdm::Command({0x6574, 0x1234}, {0x6574, 0x4321}, 20, 1, 0, kRdmCCSetCommand, E120_DEVICE_LABEL,
                          reinterpret_cast<const uint8_t*>(kTestDeviceLabel), sizeof(kTestDeviceLabel));

  ASSERT_EQ(cmd.data_len(), sizeof(kTestDeviceLabel));
  EXPECT_STREQ(reinterpret_cast<const char*>(cmd.data()), kTestDeviceLabel);

  cmd.ClearData();
  EXPECT_FALSE(cmd.HasData());
  EXPECT_EQ(cmd.data_len(), 0u);
  EXPECT_EQ(cmd.data(), nullptr);

  cmd.SetData(nullptr, 0);
  EXPECT_FALSE(cmd.HasData());
  EXPECT_EQ(cmd.data_len(), 0u);
  EXPECT_EQ(cmd.data(), nullptr);

  cmd.SetData(reinterpret_cast<const uint8_t*>(kTestDeviceLabel), sizeof(kTestDeviceLabel));
  EXPECT_TRUE(cmd.HasData());
  ASSERT_EQ(cmd.data_len(), sizeof(kTestDeviceLabel));
  EXPECT_STREQ(reinterpret_cast<const char*>(cmd.data()), kTestDeviceLabel);
}

TEST(CppCmd, SerializeWorks)
{
  const auto sensor_def = rdmtest::GetSensorDefinition();

  rdm::Command cmd(sensor_def.cmd_header, sensor_def.cmd_data(), sensor_def.cmd_data_size());
  EXPECT_TRUE(cmd.HasData());
  EXPECT_EQ(cmd.PackedSize(), sensor_def.packed_cmd_size());

  RdmBuffer buf;
  EXPECT_TRUE(cmd.Serialize(buf));
  EXPECT_EQ(buf, sensor_def.cmd_buf());

  std::array<uint8_t, RDM_MAX_BYTES> raw_buf;
  EXPECT_TRUE(cmd.Serialize(raw_buf.data(), raw_buf.size()));
  EXPECT_EQ(std::memcmp(raw_buf.data(), buf.data, buf.data_len), 0);
}
