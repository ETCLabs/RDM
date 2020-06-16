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

#ifndef TEST_MESSAGE_DATA_H_
#define TEST_MESSAGE_DATA_H_

#include <array>
#include <cstdint>
#include <cstring>
#include <optional>
#include <vector>
#include "etcpal/pack.h"
#include "rdm/message.h"

// Some utilities for comparing RDM C structures

inline bool operator==(const RdmCommandHeader& a, const RdmCommandHeader& b)
{
  return (a.source_uid == b.source_uid && a.dest_uid == b.dest_uid && a.transaction_num == b.transaction_num &&
          a.port_id == b.port_id && a.subdevice == b.subdevice && a.command_class == b.command_class &&
          a.param_id == b.param_id);
}

inline bool operator==(const RdmResponseHeader& a, const RdmResponseHeader& b)
{
  return (a.source_uid == b.source_uid && a.dest_uid == b.dest_uid && a.transaction_num == b.transaction_num &&
          a.resp_type == b.resp_type && a.msg_count == b.msg_count && a.subdevice == b.subdevice &&
          a.command_class == b.command_class && a.param_id == b.param_id);
}

inline bool operator==(const RdmBuffer& a, const RdmBuffer& b)
{
  return (a.data_len == b.data_len && (std::memcmp(a.data, b.data, a.data_len) == 0));
}

// A validation structure for RDM unit testing. Contains a command-response pair with both
// serialized and non-serialized versions, plus some convenience utilities for inspecting them.
struct CmdRespPair
{
  RdmCommandHeader                  cmd_header;
  std::vector<uint8_t>              cmd_data_;
  std::vector<uint8_t>              packed_cmd_;
  RdmResponseHeader                 resp_header;
  std::vector<uint8_t>              resp_data_;
  std::vector<std::vector<uint8_t>> packed_resps_;

  const uint8_t* cmd_data() const { return cmd_data_.data(); }
  uint8_t        cmd_data_size() const { return static_cast<uint8_t>(cmd_data_.size()); }
  const uint8_t* packed_cmd() const { return packed_cmd_.data(); }
  size_t         packed_cmd_size() const { return packed_cmd_.size(); }
  const uint8_t* resp_data() const { return resp_data_.data(); }
  uint8_t        resp_data_size() const { return static_cast<uint8_t>(resp_data_.size()); }
  size_t         overflow_resp_data_size() const { return resp_data_.size(); }
  const uint8_t* first_packed_resp() const { return packed_resps_[0].data(); }
  size_t         first_packed_resp_size() const { return packed_resps_[0].size(); }

  RdmBuffer cmd_buf() const
  {
    RdmBuffer to_return{};
    std::memcpy(to_return.data, packed_cmd(), packed_cmd_size());
    to_return.data_len = packed_cmd_size();
    return to_return;
  }

  RdmBuffer first_resp_buf() const
  {
    RdmBuffer to_return{};
    std::memcpy(to_return.data, first_packed_resp(), first_packed_resp_size());
    to_return.data_len = first_packed_resp_size();
    return to_return;
  }

  std::optional<unsigned int> AckTimerDelay() const
  {
    if (resp_header.resp_type == kRdmResponseTypeAckTimer && resp_data_.size() >= 2)
      return etcpal_unpack_u16b(resp_data_.data()) * 100;
    return {};
  }

  std::optional<rdm_nack_reason_t> NackReason() const
  {
    if (resp_header.resp_type == kRdmResponseTypeNackReason && resp_data_.size() >= 2)
      return static_cast<rdm_nack_reason_t>(etcpal_unpack_u16b(resp_data_.data()));
    return {};
  }

  uint8_t RespMessageCount() const { return first_packed_resp()[RDM_OFFSET_MSGCOUNT]; }
};

namespace rdmtest
{
inline CmdRespPair GetSupportedParams()
{
  return CmdRespPair{
      {
          {1, 2},                    // Source UID
          {3, 4},                    // Dest UID
          5,                         // Transaction Number
          6,                         // Port ID
          7,                         // Subdevice
          kRdmCCGetCommand,          // Command Class
          E120_SUPPORTED_PARAMETERS  // PID
      },
      {},
      {
          0xcc,                    // START code
          0x01,                    // Sub START code
          0x18,                    // Message Length
          0,    3,    0, 0, 0, 4,  // Destination UID
          0,    1,    0, 0, 0, 2,  // Source UID
          5,                       // Transaction Number
          6,                       // Port ID
          0,                       // Message Count
          0,    7,                 // Subdevice
          0x20,                    // Command Class (GET Response)
          0x00, 0x50,              // PID (E120_SUPPORTED_PARAMETERS)
          0x00,                    // PD Length
          0x01, 0x71               // Checksum
      },
      {{3, 4}, {1, 2}, 5, kRdmResponseTypeAck, 6, 7, kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS},
      {0x00, 0x40, 0x00, 0x50},
      {{
          0xcc,                          // START code
          0x01,                          // Sub START code
          0x1c,                          // Message Length
          0,    1,    0,    0,    0, 2,  // Destination UID
          0,    3,    0,    0,    0, 4,  // Source UID
          5,                             // Transaction Number
          0,                             // Response Type (ACK)
          6,                             // Message Count
          0,    7,                       // Subdevice
          0x21,                          // Command Class (GET Response)
          0x00, 0x50,                    // PID (E120_SUPPORTED_PARAMETERS)
          0x04,                          // PD Length

          0x00, 0x40, 0x00, 0x50,  // Parameter data

          0x02, 0x0a  // Checksum
      }}};
}

inline CmdRespPair GetSupportedParamsWithOverflow()
{
  return CmdRespPair{
      {
          {20, 200},                 // Source UID
          {40, 400},                 // Dest UID
          123,                       // Transaction Number
          3,                         // Port ID
          0,                         // Subdevice
          kRdmCCGetCommand,          // Command Class
          E120_SUPPORTED_PARAMETERS  // PID
      },
      {},
      {
          0xcc,                          // START code
          0x01,                          // Sub START code
          0x18,                          // Message Length
          0,    0x28, 0, 0, 0x01, 0x90,  // Destination UID
          0,    0x14, 0, 0, 0x00, 0xc8,  // Source UID
          0x7b,                          // Transaction Number
          3,                             // Port ID
          0,                             // Message Count
          0,    0,                       // Subdevice
          0x20,                          // Command Class (GET Response)
          0x00, 0x50,                    // PID (E120_SUPPORTED_PARAMETERS)
          0x00,                          // PD Length
          0x03, 0x68                     // Checksum
      },
      {{40, 400}, {20, 200}, 123, kRdmResponseTypeAck, 0, 0, kRdmCCGetCommandResponse, E120_SUPPORTED_PARAMETERS},
      {
          1,   2,   3,   4,   5,   6,   7,   8,   9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,
          23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
          45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,  60,  61,  62,  63,  64,  65,  66,
          67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,
          89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99,  100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110,
          111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132,
          133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154,
          155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176,
          177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198,
          199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220,
          221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242,
          243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 0,   1,   2,   3,   4,   5,   6,   7,   8,
          9,   10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,
          31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,
      },
      {{
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
           1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29,
           30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56,
           57, 58, 59, 60, 61, 62, 63, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83,
           84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 94, 95, 96, 97, 98, 99, 100, 101, 102, 103, 104, 105, 106, 107, 108,
           109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119, 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130,
           131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152,
           153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174,
           175, 176, 177, 178, 179, 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196,
           197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218,
           219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230,
           //

           0x6c, 0xfa  // Checksum
       },
       {
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
           231, 232, 233, 234, 235, 236, 237, 238, 239, 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252,
           253, 254, 255, 0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25,
           26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44,
           //

           0x1f, 0x8b,  // Checksum
       }}};
}

inline CmdRespPair GetLampStrikesTimerResponse()
{
  return CmdRespPair{
      {
          {100, 2000},       // Source UID
          {200, 4000},       // Dest UID
          222,               // Transaction Number
          1,                 // Port ID
          33,                // Subdevice
          kRdmCCGetCommand,  // Command Class
          E120_LAMP_STRIKES  // PID
      },
      {},
      {

          0xcc,                          // START code
          0x01,                          // Sub START code
          0x18,                          // Message Length
          0,    0xc8, 0, 0, 0x0f, 0xa0,  // Destination UID
          0,    0x64, 0, 0, 0x07, 0xd0,  // Source UID
          0xde,                          // Transaction Number
          1,                             // Port ID
          0,                             // Reserved
          0,    0x21,                    // Subdevice
          0x20,                          // Command Class (GET Command)
          0x04, 0x02,                    // PID (E120_LAMP_STRIKES)
          0x00,                          // PD Length
          0x04, 0xbd                     // Checksum
      },
      {{200, 4000}, {100, 2000}, 222, kRdmResponseTypeAckTimer, 0, 33, kRdmCCGetCommandResponse, E120_LAMP_STRIKES},
      {0x00, 0x0b},
      {{
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
      }}};
}

inline CmdRespPair SetResetDevice()
{
  return CmdRespPair{{
                         {22, 44},          // Source UID
                         {33, 66},          // Dest UID
                         55,                // Transaction Number
                         1,                 // Port ID
                         0,                 // Subdevice
                         kRdmCCSetCommand,  // Command Class
                         E120_RESET_DEVICE  // PID
                     },
                     {},
                     {
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
                     },
                     {{33, 66}, {22, 44}, 55, kRdmResponseTypeAck, 0, 0, kRdmCCSetCommandResponse, E120_RESET_DEVICE},
                     {},
                     {{
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
                     }}};
}

inline CmdRespPair SetResetDeviceNackResponse()
{
  return CmdRespPair{
      {
          {100, 2000},       // Source UID
          {200, 4000},       // Dest UID
          222,               // Transaction Number
          3,                 // Port ID
          33,                // Subdevice
          kRdmCCSetCommand,  // Command Class
          E120_RESET_DEVICE  // PID
      },
      {},
      {
          0xcc,                          // START code
          0x01,                          // Sub START code
          0x18,                          // Message Length
          0,    0xc8, 0, 0, 0x0f, 0xa0,  // Destination UID
          0,    0x64, 0, 0, 0x07, 0xd0,  // Source UID
          0xde,                          // Transaction Number
          3,                             // Port ID
          0,                             // Reserved
          0,    0x21,                    // Subdevice
          0x30,                          // Command Class (SET Command)
          0x10, 0x01,                    // PID (E120_LAMP_STRIKES)
          0x00,                          // PD Length
          0x04, 0xda                     // Checksum
      },
      {{200, 4000}, {100, 2000}, 222, kRdmResponseTypeNackReason, 0, 33, kRdmCCSetCommandResponse, E120_RESET_DEVICE},
      {0x00, 0x09},
      {{
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
      }}};
}

inline CmdRespPair GetSensorDefinition()
{
  return CmdRespPair{
      {
          {33, 66},               // Source UID
          {22, 44},               // Dest UID
          101,                    // Transaction Number
          1,                      // Port ID
          100,                    // Subdevice
          kRdmCCGetCommand,       // Command Class
          E120_SENSOR_DEFINITION  // PID
      },
      {0x01},
      {
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
      },
      {{22, 44}, {33, 66}, 101, kRdmResponseTypeAck, 0, 100, kRdmCCGetCommandResponse, E120_SENSOR_DEFINITION},
      {
          0x01,
          E120_SENS_TEMPERATURE,
          E120_UNITS_CENTIGRADE,
          E120_PREFIX_NONE,
          0xff,
          0xd8,
          0x00,
          0x64,
          0x00,
          0x00,
          0x00,
          0x28,
          0x00,
          84,
          101,
          115,
          116,
          32,
          68,
          101,
          115,
          99,
          114,
          105,
          112,
          116,
          105,
          111,
          110,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
          0,
      },
      {{
          0xcc,                    // START code
          0x01,                    // Sub START code
          0x45,                    // Message Length
          0, 0x21, 0, 0, 0, 0x42,  // Destination UID
          0, 0x16, 0, 0, 0, 0x2c,  // Source UID
          0x65,                    // Transaction Number
          0,                       // Response Type (ACK)
          0,                       // Message Count
          0, 0x64,                 // Subdevice
          0x21,                    // Command Class (GET Response)
          0x02, 0x00,              // PID (E120_SENSOR_DEFINITION)
          0x2d,                    // PD Length

          // Parameter Data
          0x01, 0x00, 0x01, 0x00, 0xff, 0xd8, 0x00, 0x64, 0x00, 0x00, 0x00, 0x28, 0x00, 84, 101, 115, 116, 32, 68, 101,
          115, 99, 114, 105, 112, 116, 105, 111, 110, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
          //

          0x0b, 0x79  // Checksum
      }}};
}

inline CmdRespPair GetIdentifyDevice()
{
  return CmdRespPair{
      {
          {22, 44},             // Source UID
          {33, 66},             // Dest UID
          55,                   // Transaction Number
          1,                    // Port ID
          240,                  // Subdevice
          kRdmCCGetCommand,     // Command Class
          E120_IDENTIFY_DEVICE  // PID
      },
      {},
      {
          0xcc,                       // START code
          0x01,                       // Sub START code
          0x18,                       // Message Length
          0,    0x21, 0, 0, 0, 0x42,  // Destination UID
          0,    0x16, 0, 0, 0, 0x2c,  // Source UID
          55,                         // Transaction Number
          1,                          // Port ID
          0,                          // Reserved
          0,    0xf0,                 // Subdevice
          0x20,                       // Command Class (GET Command)
          0x10, 0x00,                 // PID (E120_IDENTIFY_DEVICE)
          0x00,                       // PD Length
          0x02, 0xe2                  // Checksum
      },
      {{33, 66}, {22, 44}, 55, kRdmResponseTypeAck, 2, 240, kRdmCCGetCommandResponse, E120_IDENTIFY_DEVICE},
      {0x01},
      {{
          0xcc,                       // START code
          0x01,                       // Sub START code
          0x19,                       // Message Length
          0,    0x16, 0, 0, 0, 0x2c,  // Destination UID
          0,    0x21, 0, 0, 0, 0x42,  // Source UID
          0x37,                       // Transaction Number
          0,                          // Response Type (ACK)
          2,                          // Message Count
          0,    0xf0,                 // Subdevice
          0x21,                       // Command Class (GET Response)
          0x10, 0x00,                 // PID (E120_IDENTIFY_DEVICE)
          0x01,                       // PD Length
          0x01,                       // Parameter Data
          0x02, 0xe7                  // Checksum
      }}};
}

};  // namespace rdmtest

#endif  // TEST_MESSAGE_DATA_H_
