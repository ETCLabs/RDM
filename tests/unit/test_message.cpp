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

#include "rdm/message.h"

#include <algorithm>
#include <array>
#include <cstring>
#include <limits>
#include "gtest/gtest.h"

constexpr RdmCommandHeader kDefaultValidCommandHeader{
    {1, 2},                    // Source UID
    {3, 4},                    // Dest UID
    5,                         // Transaction Number
    6,                         // Port ID
    7,                         // Subdevice
    kRdmCCGetCommand,          // Command Class
    E120_SUPPORTED_PARAMETERS  // PID
};

TEST(RdmMessage, PackChecksumWorks)
{
  // seventeen 1's, one 0xff, and two checksum placeholders at the end
  std::array<uint8_t, 20> buf{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xff, 0, 0};

  // This should sum slots 0 through 17
  rdm_pack_checksum(buf.data(), buf.size() - 2);
  EXPECT_EQ(buf[18], 0x01);
  EXPECT_EQ(buf[19], 0x10);
}

TEST(RdmMessage, RdmCmdHeaderIsValidWorks)
{
  RdmCommandHeader cmd_header = kDefaultValidCommandHeader;
  EXPECT_FALSE(rdm_command_header_is_valid(nullptr));
  EXPECT_TRUE(rdm_command_header_is_valid(&cmd_header));

  // Invalid command class
  cmd_header.command_class = (rdm_command_class_t)0xff;
  EXPECT_FALSE(rdm_command_header_is_valid(&cmd_header));

  // Invalid source UID
  cmd_header.command_class = kRdmCCGetCommand;
  cmd_header.source_uid = kRdmBroadcastUid;
  EXPECT_FALSE(rdm_command_header_is_valid(&cmd_header));
}

TEST(RdmMessage, PackCommandInvalid)
{
  RdmCommandHeader cmd_header = kDefaultValidCommandHeader;
  RdmBuffer buffer{};
  uint8_t data[2]{};

  EXPECT_EQ(rdm_pack_command(nullptr, data, 2, &buffer), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_command(&cmd_header, data, 2, nullptr), kEtcPalErrInvalid);

  // Invalid command class
  cmd_header.command_class = (rdm_command_class_t)0xff;
  EXPECT_EQ(rdm_pack_command(&cmd_header, data, 2, &buffer), kEtcPalErrInvalid);

  // Invalid source UID
  cmd_header.command_class = kRdmCCGetCommand;
  cmd_header.source_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_pack_command(&cmd_header, data, 2, &buffer), kEtcPalErrInvalid);
}

TEST(RdmMessage, PackCommandTooLong)
{
  RdmCommandHeader cmd_header = kDefaultValidCommandHeader;
  RdmBuffer cmd_buf;
  uint8_t data[RDM_MAX_PDL + 1];

  // Fill the data buffer with incrementing values
  uint8_t byte = 0;
  std::generate(std::begin(data), std::end(data), [&]() { return byte++; });

  // Equal to maximum should succeed
  EXPECT_EQ(rdm_pack_command(&cmd_header, data, RDM_MAX_PDL, &cmd_buf), kEtcPalErrOk);
  EXPECT_EQ(cmd_buf.data_len, RDM_MAX_PDL + RDM_HEADER_SIZE + 2);

  // One over the maximum should fail
  EXPECT_NE(rdm_pack_command(&cmd_header, data, RDM_MAX_PDL + 1, &cmd_buf), kEtcPalErrOk);
}

TEST(RdmMessage, PackCommandWithData)
{
  RdmCommandHeader cmd_header{
      {33, 66},               // Source UID
      {22, 44},               // Dest UID
      101,                    // Transaction Number
      1,                      // Port ID
      100,                    // Subdevice
      kRdmCCGetCommand,       // Command Class
      E120_SENSOR_DEFINITION  // PID
  };
  RdmBuffer cmd_buf;
  uint8_t data = 1;
  constexpr uint8_t kCommandPackedSize = RDM_HEADER_SIZE + 3;

  ASSERT_EQ(rdm_pack_command(&cmd_header, &data, 1, &cmd_buf), kEtcPalErrOk);
  ASSERT_EQ(cmd_buf.data_len, kCommandPackedSize);
  const std::array<uint8_t, kCommandPackedSize> valid_command = {
      0xcc,                       // START code
      0x01,                       // Sub START code
      0x19,                       // Message Length
      0,    0x16, 0, 0, 0, 0x2c,  // Destination UID
      0,    0x21, 0, 0, 0, 0x42,  // Source UID
      0x65,                       // Transaction Number
      1,                          // Port ID
      0,                          // Reserved
      0,    0x64,                 // Subdevice
      0x20,                       // Command Class (GET Command)
      0x02, 0x00,                 // PID (E120_SENSOR_DEFINITION)
      0x01,                       // PD Length
      0x01,                       // Parameter Data
      0x02, 0x79                  // Checksum
  };
  EXPECT_EQ(0, std::memcmp(valid_command.data(), cmd_buf.data, valid_command.size()));
}

// rdm_pack_command() should be usable to pack a command with zero-length data.
TEST(RdmMessage, PackCommandEmpty)
{
  RdmCommandHeader cmd_header{
      {22, 44},          // Source UID
      {33, 66},          // Dest UID
      55,                // Transaction Number
      1,                 // Port ID
      0,                 // Subdevice
      kRdmCCSetCommand,  // Command Class
      E120_RESET_DEVICE  // PID
  };
  RdmBuffer cmd_buf;
  constexpr uint8_t kCommandPackedSize = RDM_HEADER_SIZE + 2;

  ASSERT_EQ(rdm_pack_command(&cmd_header, nullptr, 0, &cmd_buf), kEtcPalErrOk);
  ASSERT_EQ(cmd_buf.data_len, kCommandPackedSize);

  const std::array<uint8_t, kCommandPackedSize> valid_command = {
      0xcc,                       // START code
      0x01,                       // Sub START code
      0x18,                       // Message Length
      0,    0x21, 0, 0, 0, 0x42,  // Destination UID
      0,    0x16, 0, 0, 0, 0x2c,  // Source UID
      0x37,                       // Transaction Number
      1,                          // Port ID
      0,                          // Reserved
      0,    0,                    // Subdevice
      0x30,                       // Command Class (SET Command)
      0x10, 0x01,                 // PID (E120_RESET_DEVICE)
      0x00,                       // PD Length
      0x02, 0x03                  // Checksum
  };
  EXPECT_EQ(0, std::memcmp(valid_command.data(), cmd_buf.data, valid_command.size()));
}

TEST(RdmMessage, PackResponseInvalid)
{
  RdmCommandHeader cmd_header = kDefaultValidCommandHeader;
  RdmBuffer buffer{};
  uint8_t data[2]{};

  EXPECT_EQ(rdm_pack_response(nullptr, 0, data, 2, &buffer), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_response(&cmd_header, 0, data, 2, nullptr), kEtcPalErrInvalid);

  // Invalid command class
  cmd_header.command_class = (rdm_command_class_t)0xff;
  EXPECT_EQ(rdm_pack_response(&cmd_header, 0, data, 2, &buffer), kEtcPalErrInvalid);

  // Invalid source UID
  cmd_header.command_class = kRdmCCGetCommand;
  cmd_header.source_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_pack_response(&cmd_header, 0, data, 2, &buffer), kEtcPalErrInvalid);

  // Invalid destination UID
  cmd_header.source_uid = RdmUid{1, 2};
  cmd_header.dest_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_pack_response(&cmd_header, 0, data, 2, &buffer), kEtcPalErrInvalid);
}

TEST(RdmMessage, PackResponseTooLong)
{
  RdmCommandHeader cmd_header = kDefaultValidCommandHeader;
  RdmBuffer response_buf;
  std::array<uint8_t, RDM_MAX_PDL + 1> pd;

  // Fill the data buffer with incrementing values
  uint8_t byte = 0;
  std::generate(std::begin(pd), std::end(pd), [&]() { return byte++; });

  // Equal to maximum should succeed
  EXPECT_EQ(rdm_pack_response(&cmd_header, 0, pd.data(), RDM_MAX_PDL, &response_buf), kEtcPalErrOk);
  EXPECT_EQ(response_buf.data_len, RDM_MAX_PDL + RDM_HEADER_SIZE + 2);

  // One over the maximum should fail
  EXPECT_NE(rdm_pack_response(&cmd_header, 0, pd.data(), RDM_MAX_PDL + 1, &response_buf), kEtcPalErrOk);
}

TEST(RdmMessage, PackResponseWithData)
{
  RdmCommandHeader cmd_header{
      {22, 44},             // Source UID
      {33, 66},             // Dest UID
      55,                   // Transaction Number
      1,                    // Port ID
      240,                  // Subdevice
      kRdmCCGetCommand,     // Command Class
      E120_IDENTIFY_DEVICE  // PID
  };
  RdmBuffer response_buf;
  uint8_t data = 1;
  constexpr uint8_t kResponsepackedSize = RDM_HEADER_SIZE + 3;

  ASSERT_EQ(rdm_pack_response(&cmd_header, 2, &data, 1, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, kResponsepackedSize);
  const std::array<uint8_t, kResponsepackedSize> valid_response = {
      0xcc,                       // START code
      0x01,                       // Sub START code
      0x19,                       // Message Length
      0,    0x16, 0, 0, 0, 0x2c,  // Destination UID
      0,    0x21, 0, 0, 0, 0x42,  // Source UID
      0x37,                       // Transaction Number
      0,                          // Response Type (ACK)
      2,                          // Message Count
      0,    0xf0,                 // Subdevice
      0x21,                       // Command Class (SET Response)
      0x10, 0x00,                 // PID (E120_IDENTIFY_DEVICE)
      0x01,                       // PD Length
      0x01,                       // Parameter Data
      0x02, 0xe7                  // Checksum
  };
  EXPECT_EQ(0, std::memcmp(valid_response.data(), response_buf.data, valid_response.size()));
}

// rdm_pack_response() should be usable to pack a response with zero-length data.
TEST(RdmMessage, PackResponseEmpty)
{
  RdmCommandHeader cmd_header{
      {22, 44},          // Source UID
      {33, 66},          // Dest UID
      55,                // Transaction Number
      1,                 // Port ID
      0,                 // Subdevice
      kRdmCCSetCommand,  // Command Class
      E120_RESET_DEVICE  // PID
  };
  RdmBuffer response_buf;
  constexpr uint8_t kResponsePackedSize = RDM_HEADER_SIZE + 2;

  ASSERT_EQ(rdm_pack_response(&cmd_header, 0, nullptr, 0, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, kResponsePackedSize);

  const std::array<uint8_t, kResponsePackedSize> valid_response = {
      0xcc,                       // START code
      0x01,                       // Sub START code
      0x18,                       // Message Length
      0,    0x16, 0, 0, 0, 0x2c,  // Destination UID
      0,    0x21, 0, 0, 0, 0x42,  // Source UID
      0x37,                       // Transaction Number
      0,                          // Response Type (ACK)
      0,                          // Message Count
      0,    0,                    // Subdevice
      0x31,                       // Command Class (SET Response)
      0x10, 0x01,                 // PID (E120_RESET_DEVICE)
      0x00,                       // PD Length
      0x02, 0x03                  // Checksum
  };
  EXPECT_EQ(0, std::memcmp(valid_response.data(), response_buf.data, valid_response.size()));
}

TEST(RdmMessage, PackOverflowResponseInvalid)
{
  RdmCommandHeader cmd_header = kDefaultValidCommandHeader;
  RdmBuffer buffer{};

  EXPECT_EQ(rdm_pack_overflow_response(nullptr, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_overflow_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, nullptr), kEtcPalErrInvalid);

  // Invalid command class
  cmd_header.command_class = (rdm_command_class_t)0xff;
  EXPECT_EQ(rdm_pack_overflow_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);

  // Invalid source UID
  cmd_header.command_class = kRdmCCGetCommand;
  cmd_header.source_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_pack_overflow_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);

  // Invalid destination UID
  cmd_header.source_uid = RdmUid{1, 2};
  cmd_header.dest_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_pack_overflow_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);
}

TEST(RdmMessage, PackOverflowResponse)
{
  RdmCommandHeader cmd_header{
      {20, 200},                 // Source UID
      {40, 400},                 // Dest UID
      123,                       // Transaction Number
      3,                         // Port ID
      0,                         // Subdevice
      kRdmCCGetCommand,          // Command Class
      E120_SUPPORTED_PARAMETERS  // PID
  };
  RdmBuffer response_buf;
  constexpr uint8_t kAckOverflowPdSize = RDM_MAX_PDL - 1;
  constexpr size_t kResponsePackedSize = RDM_HEADER_SIZE + kAckOverflowPdSize + 2;

  // Fill a data buffer with incrementing values
  std::array<uint8_t, kAckOverflowPdSize> pd;
  uint8_t byte = 1;
  std::generate(std::begin(pd), std::end(pd), [&]() { return byte++; });

  ASSERT_EQ(rdm_pack_overflow_response(&cmd_header, pd.data(), kAckOverflowPdSize, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, kResponsePackedSize);

  const std::array<uint8_t, kResponsePackedSize> ack_overflow_response = {
      0xcc,                       // START code
      0x01,                       // Sub START code
      0xfe,                       // Message Length
      0, 0x14, 0, 0, 0x00, 0xc8,  // Destination UID
      0, 0x28, 0, 0, 0x01, 0x90,  // Source UID
      0x7b,                       // Transaction Number
      3,                          // Response Type (ACK_OVERFLOW)
      0,                          // Message Count
      0, 0,                       // Subdevice
      0x21,                       // Command Class (GET Response)
      0x00, 0x50,                 // PID (E120_SUPPORTED_PARAMETERS)
      0xe6,                       // PD Length

      // Parameter data
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
      32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
      60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
      88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
      113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
      136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
      159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
      182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
      205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
      228, 229, 230,
      //

      0x6c, 0xfa  // Checksum
  };
  EXPECT_EQ(0, std::memcmp(ack_overflow_response.data(), response_buf.data, ack_overflow_response.size()));
}

TEST(RdmMessage, PackNackResponseInvalid)
{
  RdmCommandHeader cmd_header = kDefaultValidCommandHeader;
  RdmBuffer buffer{};

  EXPECT_EQ(rdm_pack_nack_response(nullptr, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_nack_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, nullptr), kEtcPalErrInvalid);

  // Invalid command class
  cmd_header.command_class = (rdm_command_class_t)0xff;
  EXPECT_EQ(rdm_pack_nack_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);

  // Invalid source UID
  cmd_header.command_class = kRdmCCGetCommand;
  cmd_header.source_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_pack_nack_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);

  // Invalid destination UID
  cmd_header.source_uid = RdmUid{1, 2};
  cmd_header.dest_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_pack_nack_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);
}

TEST(RdmMessage, PackNackResponse)
{
  RdmCommandHeader cmd_header{
      {100, 2000},       // Source UID
      {200, 4000},       // Dest UID
      222,               // Transaction Number
      3,                 // Port ID
      33,                // Subdevice
      kRdmCCSetCommand,  // Command Class
      E120_RESET_DEVICE  // PID
  };
  RdmBuffer response_buf;
  constexpr uint8_t kResponsePackedSize = RDM_HEADER_SIZE + 4;

  ASSERT_EQ(rdm_pack_nack_response(&cmd_header, 0, kRdmNRSubDeviceOutOfRange, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, kResponsePackedSize);

  const std::array<uint8_t, kResponsePackedSize> nack_response = {
      0xcc,                          // START code
      0x01,                          // Sub START code
      0x1a,                          // Message Length
      0,    0x64, 0, 0, 0x07, 0xd0,  // Destination UID
      0,    0xc8, 0, 0, 0x0f, 0xa0,  // Source UID
      0xde,                          // Transaction Number
      2,                             // Response Type (NACK)
      0,                             // Message Count
      0,    0x21,                    // Subdevice
      0x31,                          // Command Class (SET Response)
      0x10, 0x01,                    // PID (E120_RESET_DEVICE)
      0x02,                          // PD Length
      0x00, 0x09,                    // NACK reason code
      0x04, 0xe7                     // Checksum
  };
  EXPECT_EQ(0, std::memcmp(nack_response.data(), response_buf.data, nack_response.size()));
}

TEST(RdmMessage, PackTimerResponseInvalid)
{
  RdmCommandHeader cmd_header = kDefaultValidCommandHeader;
  RdmBuffer buffer{};

  EXPECT_EQ(rdm_pack_timer_response(nullptr, 0, 200, &buffer), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_timer_response(&cmd_header, 0, 200, nullptr), kEtcPalErrInvalid);

  // Invalid command class
  cmd_header.command_class = (rdm_command_class_t)0xff;
  EXPECT_EQ(rdm_pack_timer_response(&cmd_header, 0, 200, &buffer), kEtcPalErrInvalid);

  // Invalid source UID
  cmd_header.command_class = kRdmCCGetCommand;
  cmd_header.source_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_pack_timer_response(&cmd_header, 0, 200, &buffer), kEtcPalErrInvalid);

  // Invalid destination UID
  cmd_header.source_uid = RdmUid{1, 2};
  cmd_header.dest_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_pack_timer_response(&cmd_header, 0, 200, &buffer), kEtcPalErrInvalid);

  // Invalid timer times
  cmd_header.dest_uid = RdmUid{3, 4};
  EXPECT_EQ(rdm_pack_timer_response(&cmd_header, 0, 0, &buffer), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_timer_response(&cmd_header, 0, std::numeric_limits<unsigned int>::max(), &buffer),
            kEtcPalErrInvalid);
}

TEST(RdmMessage, PackTimerResponse)
{
  RdmCommandHeader cmd_header{
      {100, 2000},       // Source UID
      {200, 4000},       // Dest UID
      222,               // Transaction Number
      1,                 // Port ID
      33,                // Subdevice
      kRdmCCGetCommand,  // Command Class
      E120_LAMP_STRIKES  // PID
  };
  RdmBuffer response_buf;
  constexpr uint8_t kResponsePackedSize = RDM_HEADER_SIZE + 4;

  ASSERT_EQ(rdm_pack_timer_response(&cmd_header, 0, 1020, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, kResponsePackedSize);

  const std::array<uint8_t, kResponsePackedSize> ack_timer_response = {
      0xcc,                          // START code
      0x01,                          // Sub START code
      0x1a,                          // Message Length
      0,    0x64, 0, 0, 0x07, 0xd0,  // Destination UID
      0,    0xc8, 0, 0, 0x0f, 0xa0,  // Source UID
      0xde,                          // Transaction Number
      1,                             // Response Type (ACK_TIMER)
      0,                             // Message Count
      0,    0x21,                    // Subdevice
      0x21,                          // Command Class (GET Response)
      0x04, 0x02,                    // PID (E120_LAMP_STRIKES)
      0x02,                          // PD Length
      0x00, 0x0b,                    // ACK timer delay (1100 ms)
      0x04, 0xcd                     // Checksum
  };
  EXPECT_EQ(0, std::memcmp(ack_timer_response.data(), response_buf.data, ack_timer_response.size()));
}

TEST(RdmMessage, PackDubResponseInvalid)
{
  RdmUid uid{1, 3};
  RdmBuffer buf;
  EXPECT_EQ(rdm_pack_dub_response(nullptr, nullptr), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_dub_response(&uid, nullptr), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_dub_response(nullptr, &buf), kEtcPalErrInvalid);
}

TEST(RdmMessage, PackDubResponse)
{
  RdmBuffer dub_buffer;
  RdmUid uid{2000, 400000};
  constexpr size_t kDubResponseSize = 24;

  EXPECT_EQ(rdm_pack_dub_response(&uid, &dub_buffer), kEtcPalErrOk);

  std::array<uint8_t, kDubResponseSize> dub_response = {
      0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xfe, 0xaa,
      0xaf,  // uid.manu MSB | 0xaa
      0x57,  // uid.manu MSB | 0x55
      0xfa,  // uid.manu LSB | 0xaa
      0xd5,  // uid.manu LSB | 0x55
      0xaa,  // uid.id MSB | 0xaa
      0x55,  // uid.id MSB | 0x55
      0xae,  // uid.id byte 2 | 0xaa
      0x57,  // uid.id byte 2 | 0x55
      0xba,  // uid.id byte 1 | 0xaa
      0x5f,  // uid.id byte 1 | 0x55
      0xaa,  // uid.id LSB | 0xaa
      0xd5,  // uid.id LSB | 0x55
      // Checksum = 0x0771
      0xaf,  // Checkum MSB | 0xaa
      0x57,  // Checksum MSB | 0x55
      0xfb,  // Checksum LSB | 0xaa
      0x75,  // Checksum LSB | 0x55
  };
  EXPECT_EQ(0, std::memcmp(dub_response.data(), dub_buffer.data, dub_response.size()));
}

// This library's convention is to split ACK_OVERFLOW response data such that atomic units being
// repeated in the data are not split over message boundaries. This is not fully clear in the
// standard, but it is a strong convention and is required for some parameter messages.
TEST(RdmMessage, GetNumOverflowResponsesNeededWorks)
{
  // Test some tricky boundaries, e.g. for PID data that could be fit into two messages if items
  // were split, but actually take three, etc.

  // PROXIED_DEVICES
  EXPECT_EQ(rdm_get_num_overflow_responses_needed(E120_PROXIED_DEVICES, 456), 2);
  EXPECT_EQ(rdm_get_num_overflow_responses_needed(E120_PROXIED_DEVICES, 462), 3);

  // STATUS_MESSAGES
  EXPECT_EQ(rdm_get_num_overflow_responses_needed(E120_STATUS_MESSAGES, 450), 2);
  EXPECT_EQ(rdm_get_num_overflow_responses_needed(E120_STATUS_MESSAGES, 459), 3);

  // TCP_COMMS_STATUS
  EXPECT_EQ(rdm_get_num_overflow_responses_needed(E133_TCP_COMMS_STATUS, 87), 1);
  EXPECT_EQ(rdm_get_num_overflow_responses_needed(E133_TCP_COMMS_STATUS, 174), 2);
  EXPECT_EQ(rdm_get_num_overflow_responses_needed(E133_TCP_COMMS_STATUS, 261), 3);
}

TEST(RdmMessage, PackFullOverflowResponseInvalid)
{
  RdmCommandHeader cmd_header = kDefaultValidCommandHeader;
  RdmBuffer response_buf;
  const std::array<uint8_t, 300> param_data{};

  // Invalid parameters
  EXPECT_EQ(rdm_pack_full_overflow_response(nullptr, param_data.data(), 200, &response_buf, 1), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_full_overflow_response(&cmd_header, nullptr, 0, &response_buf, 1), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_full_overflow_response(&cmd_header, param_data.data(), 200, nullptr, 1), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_full_overflow_response(&cmd_header, param_data.data(), 200, &response_buf, 0), kEtcPalErrInvalid);

  // Short buf
  EXPECT_EQ(rdm_pack_full_overflow_response(&cmd_header, param_data.data(), param_data.size(), &response_buf, 1),
            kEtcPalErrBufSize);
}

TEST(RdmMessage, PackFullOverflowResponse)
{
  RdmCommandHeader cmd_header{
      {20, 200},                 // Source UID
      {40, 400},                 // Dest UID
      123,                       // Transaction Number
      3,                         // Port ID
      0,                         // Subdevice
      kRdmCCGetCommand,          // Command Class
      E120_SUPPORTED_PARAMETERS  // PID
  };
  RdmBuffer response_bufs[2];

  // We pack a response representing 150 supported parameters, 300 bytes, which should be split
  // with 230 in the first response and 70 in the second response.
  constexpr size_t kAckOverflowPdSize = 300;
  constexpr size_t kResponse1PackedSize = RDM_HEADER_SIZE + 230 + 2;
  constexpr size_t kResponse2PackedSize = RDM_HEADER_SIZE + 70 + 2;

  // Fill a data buffer with incrementing values
  std::array<uint8_t, kAckOverflowPdSize> pd;
  uint8_t byte = 1;
  std::generate(std::begin(pd), std::end(pd), [&]() { return byte++; });

  ASSERT_EQ(rdm_pack_full_overflow_response(&cmd_header, pd.data(), kAckOverflowPdSize, response_bufs, 2),
            kEtcPalErrOk);
  ASSERT_EQ(response_bufs[0].data_len, kResponse1PackedSize);
  ASSERT_EQ(response_bufs[1].data_len, kResponse2PackedSize);

  const std::array<uint8_t, kResponse1PackedSize> ack_overflow_response_1 = {
      0xcc,                       // START code
      0x01,                       // Sub START code
      0xfe,                       // Message Length
      0, 0x14, 0, 0, 0x00, 0xc8,  // Destination UID
      0, 0x28, 0, 0, 0x01, 0x90,  // Source UID
      0x7b,                       // Transaction Number
      3,                          // Response Type (ACK_OVERFLOW)
      0,                          // Message Count
      0, 0,                       // Subdevice
      0x21,                       // Command Class (GET Response)
      0x00, 0x50,                 // PID (E120_SUPPORTED_PARAMETERS)
      0xe6,                       // PD Length

      // Parameter data
      1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31,
      32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59,
      60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87,
      88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112,
      113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135,
      136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158,
      159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181,
      182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204,
      205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227,
      228, 229, 230,
      //

      0x6c, 0xfa  // Checksum
  };
  const std::array<uint8_t, kResponse2PackedSize> ack_overflow_response_2 = {
      0xcc,                       // START code
      0x01,                       // Sub START code
      0x5e,                       // Message Length
      0, 0x14, 0, 0, 0x00, 0xc8,  // Destination UID
      0, 0x28, 0, 0, 0x01, 0x90,  // Source UID
      0x7b,                       // Transaction Number
      0,                          // Response Type (ACK)
      0,                          // Message Count
      0, 0,                       // Subdevice
      0x21,                       // Command Class (GET Response)
      0x00, 0x50,                 // PID (E120_SUPPORTED_PARAMETERS)
      0x46,                       // PD Length

      // Parameter data
      231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253,
      254, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27,
      28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
      //

      0x1f, 0x8b,  // Checksum
  };
  EXPECT_EQ(0, std::memcmp(ack_overflow_response_1.data(), response_bufs[0].data, ack_overflow_response_1.size()));
  EXPECT_EQ(0, std::memcmp(ack_overflow_response_2.data(), response_bufs[1].data, ack_overflow_response_2.size()));
}

TEST(RdmMessage, AppendParameterData)
{
  RdmCommandHeader cmd_header = {
      {33, 66},               // Source UID
      {22, 44},               // Dest UID
      101,                    // Transaction Number
      1,                      // Port ID
      100,                    // Subdevice
      kRdmCCGetCommand,       // Command Class
      E120_SENSOR_DEFINITION  // PID
  };
  RdmBuffer cmd_buf;
  uint8_t data = 1;
  constexpr uint8_t kCommandPackedSize = RDM_HEADER_SIZE + 3;

  ASSERT_EQ(rdm_pack_command(&cmd_header, &data, 1, &cmd_buf), kEtcPalErrOk);
  ASSERT_EQ(cmd_buf.data_len, kCommandPackedSize);
  const std::array<uint8_t, kCommandPackedSize> valid_command = {
      0xcc,                       // START code
      0x01,                       // Sub START code
      0x19,                       // Message Length
      0,    0x16, 0, 0, 0, 0x2c,  // Destination UID
      0,    0x21, 0, 0, 0, 0x42,  // Source UID
      0x65,                       // Transaction Number
      1,                          // Port ID
      0,                          // Reserved
      0,    0x64,                 // Subdevice
      0x20,                       // Command Class (GET Command)
      0x02, 0x00,                 // PID (E120_SENSOR_DEFINITION)
      0x01,                       // PD Length
      0x01,                       // Parameter Data
      0x02, 0x79                  // Checksum
  };
  EXPECT_EQ(0, std::memcmp(valid_command.data(), cmd_buf.data, valid_command.size()));
}

TEST(RdmMessage, CommandClassToString)
{
  ASSERT_NE(rdm_command_class_to_string(kRdmCCDiscoveryCommand), nullptr);
  ASSERT_NE(rdm_command_class_to_string(kRdmCCDiscoveryCommandResponse), nullptr);
  ASSERT_NE(rdm_command_class_to_string(kRdmCCGetCommand), nullptr);
  ASSERT_NE(rdm_command_class_to_string(kRdmCCGetCommandResponse), nullptr);
  ASSERT_NE(rdm_command_class_to_string(kRdmCCSetCommand), nullptr);
  ASSERT_NE(rdm_command_class_to_string(kRdmCCSetCommandResponse), nullptr);

  EXPECT_GT(std::strlen(rdm_command_class_to_string(kRdmCCDiscoveryCommand)), 0u);
  EXPECT_GT(std::strlen(rdm_command_class_to_string(kRdmCCDiscoveryCommandResponse)), 0u);
  EXPECT_GT(std::strlen(rdm_command_class_to_string(kRdmCCGetCommand)), 0u);
  EXPECT_GT(std::strlen(rdm_command_class_to_string(kRdmCCGetCommandResponse)), 0u);
  EXPECT_GT(std::strlen(rdm_command_class_to_string(kRdmCCSetCommand)), 0u);
  EXPECT_GT(std::strlen(rdm_command_class_to_string(kRdmCCSetCommandResponse)), 0u);

  // out-of-range values should still return a valid "unknown" string
  EXPECT_NE(rdm_command_class_to_string(static_cast<rdm_command_class_t>(0xffu)), nullptr);
}

TEST(RdmMessage, ResponseTypeToString)
{
  ASSERT_NE(rdm_response_type_to_string(kRdmResponseTypeAck), nullptr);
  ASSERT_NE(rdm_response_type_to_string(kRdmResponseTypeAckTimer), nullptr);
  ASSERT_NE(rdm_response_type_to_string(kRdmResponseTypeNackReason), nullptr);
  ASSERT_NE(rdm_response_type_to_string(kRdmResponseTypeAckOverflow), nullptr);

  EXPECT_GT(std::strlen(rdm_response_type_to_string(kRdmResponseTypeAck)), 0u);
  EXPECT_GT(std::strlen(rdm_response_type_to_string(kRdmResponseTypeAckTimer)), 0u);
  EXPECT_GT(std::strlen(rdm_response_type_to_string(kRdmResponseTypeNackReason)), 0u);
  EXPECT_GT(std::strlen(rdm_response_type_to_string(kRdmResponseTypeAckOverflow)), 0u);

  // out-of-range values should still return a valid "unknown" string
  EXPECT_NE(rdm_response_type_to_string(static_cast<rdm_response_type_t>(0xffu)), nullptr);
}

TEST(RdmMessage, NackReasonToString)
{
  for (size_t i = 0; i < RDM_NUM_STANDARD_NR_CODES; ++i)
  {
    ASSERT_NE(rdm_nack_reason_to_string(static_cast<rdm_nack_reason_t>(i)), nullptr) << "Failed on iteration " << i;
    EXPECT_GT(std::strlen(rdm_nack_reason_to_string(static_cast<rdm_nack_reason_t>(i))), 0u)
        << "Failed on iteration " << i;
  }

  // out-of-range values should still return a valid "unknown" string
  EXPECT_NE(rdm_nack_reason_to_string(static_cast<rdm_nack_reason_t>(RDM_NUM_STANDARD_NR_CODES)), nullptr);
  EXPECT_NE(rdm_nack_reason_to_string(static_cast<rdm_nack_reason_t>(0xffffu)), nullptr);
}
