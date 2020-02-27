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
#include "gtest/gtest.h"

TEST(RdmMessage, CreateResponseInvalid)
{
  RdmCommandHeader cmd_header{
      {1, 2},                    // Source UID
      {3, 4},                    // Dest UID
      5,                         // Transaction Number
      6,                         // Port ID
      7,                         // Subdevice
      kRdmCCGetCommand,          // Command Class
      E120_SUPPORTED_PARAMETERS  // PID
  };
  RdmBuffer buffer{};
  uint8_t data[2]{};

  EXPECT_EQ(rdm_create_response(nullptr, 0, data, 2, &buffer), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_create_response(&cmd_header, 0, data, 2, nullptr), kEtcPalErrInvalid);

  // Invalid command class
  cmd_header.command_class = (rdm_command_class_t)0xff;
  EXPECT_EQ(rdm_create_response(&cmd_header, 0, data, 2, &buffer), kEtcPalErrInvalid);

  // Invalid source UID
  cmd_header.command_class = kRdmCCGetCommand;
  cmd_header.source_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_create_response(&cmd_header, 0, data, 2, &buffer), kEtcPalErrInvalid);

  // Invalid destination UID
  cmd_header.source_uid = RdmUid{1, 2};
  cmd_header.dest_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_create_response(&cmd_header, 0, data, 2, &buffer), kEtcPalErrInvalid);
}

TEST(RdmMessage, CreateResponseTooLong)
{
  RdmCommandHeader cmd_header{
      {1, 2},                    // Source UID
      {3, 4},                    // Dest UID
      5,                         // Transaction Number
      6,                         // Port ID
      7,                         // Subdevice
      kRdmCCGetCommand,          // Command Class
      E120_SUPPORTED_PARAMETERS  // PID
  };
  RdmBuffer response_buf;
  uint8_t data[RDM_MAX_PDL + 1];

  // Fill the data buffer with incrementing values
  uint8_t byte = 0;
  std::generate(std::begin(data), std::end(data), [&]() { return byte++; });

  // Equal to maximum should succeed
  EXPECT_EQ(rdm_create_response(&cmd_header, 0, data, RDM_MAX_PDL, &response_buf), kEtcPalErrOk);
  EXPECT_EQ(response_buf.data_len, RDM_MAX_PDL + RDM_HEADER_SIZE + 2);

  // One over the maximum should fail
  EXPECT_NE(rdm_create_response(&cmd_header, 0, data, RDM_MAX_PDL + 1, &response_buf), kEtcPalErrOk);
}

TEST(RdmMessage, CreateResponseWithData)
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

  ASSERT_EQ(rdm_create_response(&cmd_header, 2, &data, 1, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, kResponsepackedSize);
  std::array<uint8_t, kResponsepackedSize> valid_response = {
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

// rdm_create_response() should be usable to create a response with zero-length data.
TEST(RdmMessage, CreateResponseEmpty)
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

  ASSERT_EQ(rdm_create_response(&cmd_header, 0, nullptr, 0, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, kResponsePackedSize);

  std::array<uint8_t, kResponsePackedSize> valid_response = {
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

TEST(RdmMessage, CreateNackResponseInvalid)
{
  RdmCommandHeader cmd_header{
      {1, 2},                    // Source UID
      {3, 4},                    // Dest UID
      5,                         // Transaction Number
      6,                         // Port ID
      7,                         // Subdevice
      kRdmCCSetCommand,          // Command Class
      E120_SUPPORTED_PARAMETERS  // PID
  };
  RdmBuffer buffer{};

  EXPECT_EQ(rdm_create_nack_response(nullptr, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_create_nack_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, nullptr), kEtcPalErrInvalid);

  // Invalid command class
  cmd_header.command_class = (rdm_command_class_t)0xff;
  EXPECT_EQ(rdm_create_nack_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);

  // Invalid source UID
  cmd_header.command_class = kRdmCCGetCommand;
  cmd_header.source_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_create_nack_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);

  // Invalid destination UID
  cmd_header.source_uid = RdmUid{1, 2};
  cmd_header.dest_uid = kRdmBroadcastUid;
  EXPECT_EQ(rdm_create_nack_response(&cmd_header, 0, kRdmNRUnsupportedCommandClass, &buffer), kEtcPalErrInvalid);
}

TEST(RdmMessage, CreateNackResponse)
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

  ASSERT_EQ(rdm_create_nack_response(&cmd_header, 0, kRdmNRSubDeviceOutOfRange, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, kResponsePackedSize);

  std::array<uint8_t, kResponsePackedSize> nack_response = {
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
