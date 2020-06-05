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
#include <numeric>
#include "message_test_data.h"
#include "gtest/gtest.h"

TEST(Message, PackChecksumWorks)
{
  // seventeen 1's, one 0xff, and two checksum placeholders at the end
  std::array<uint8_t, 20> buf{1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0xff, 0, 0};

  // This should sum slots 0 through 17
  rdm_pack_checksum(buf.data(), buf.size() - 2);
  EXPECT_EQ(buf[18], 0x01);
  EXPECT_EQ(buf[19], 0x10);
}

TEST(Message, RdmCmdHeaderIsValidWorks)
{
  RdmCommandHeader cmd_header = rdmtest::GetSupportedParams().cmd_header;

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

TEST(Message, PackCommandInvalid)
{
  RdmCommandHeader cmd_header = rdmtest::GetSupportedParams().cmd_header;
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

TEST(Message, PackCommandTooLong)
{
  RdmCommandHeader cmd_header = rdmtest::GetSupportedParams().cmd_header;
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

TEST(Message, PackCommandWithData)
{
  const auto sensor_def_pair = rdmtest::GetSensorDefinition();
  RdmBuffer cmd_buf;

  ASSERT_EQ(rdm_pack_command(&sensor_def_pair.cmd_header, sensor_def_pair.cmd_data(), sensor_def_pair.cmd_data_size(),
                             &cmd_buf),
            kEtcPalErrOk);
  ASSERT_EQ(cmd_buf.data_len, sensor_def_pair.packed_cmd_size());
  EXPECT_EQ(0, std::memcmp(sensor_def_pair.packed_cmd(), cmd_buf.data, sensor_def_pair.packed_cmd_size()));
}

// rdm_pack_command() should be usable to pack a command with zero-length data.
TEST(Message, PackCommandEmpty)
{
  const auto reset_dev_pair = rdmtest::SetResetDevice();
  RdmBuffer cmd_buf;

  ASSERT_EQ(rdm_pack_command(&reset_dev_pair.cmd_header, nullptr, 0, &cmd_buf), kEtcPalErrOk);
  ASSERT_EQ(cmd_buf.data_len, reset_dev_pair.packed_cmd_size());

  EXPECT_EQ(0, std::memcmp(reset_dev_pair.packed_cmd(), cmd_buf.data, reset_dev_pair.packed_cmd_size()));
}

TEST(Message, PackCommandWithCustomBuf)
{
  const auto sensor_def_pair = rdmtest::GetSensorDefinition();
  std::array<uint8_t, RDM_MAX_BYTES> buf;

  ASSERT_EQ(rdm_pack_command_with_custom_buf(&sensor_def_pair.cmd_header, sensor_def_pair.cmd_data(),
                                             sensor_def_pair.cmd_data_size(), buf.data(), buf.size()),
            kEtcPalErrOk);
  EXPECT_EQ(0, std::memcmp(sensor_def_pair.packed_cmd(), buf.data(), sensor_def_pair.packed_cmd_size()));
}

TEST(Message, PackCommandWithCustomBufInvalid)
{
  const auto sensor_def_pair = rdmtest::GetSensorDefinition();
  std::array<uint8_t, RDM_MAX_BYTES> buf;

  // Invalid arguments
  EXPECT_EQ(rdm_pack_command_with_custom_buf(nullptr, sensor_def_pair.cmd_data(), sensor_def_pair.cmd_data_size(),
                                             buf.data(), buf.size()),
            kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_command_with_custom_buf(&sensor_def_pair.cmd_header, sensor_def_pair.cmd_data(),
                                             sensor_def_pair.cmd_data_size(), nullptr, buf.size()),
            kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_command_with_custom_buf(&sensor_def_pair.cmd_header, sensor_def_pair.cmd_data(),
                                             sensor_def_pair.cmd_data_size(), buf.data(), 0),
            kEtcPalErrInvalid);

  // Buffer too short
  EXPECT_EQ(rdm_pack_command_with_custom_buf(&sensor_def_pair.cmd_header, sensor_def_pair.cmd_data(),
                                             sensor_def_pair.cmd_data_size(), buf.data(),
                                             sensor_def_pair.packed_cmd_size() - 1),
            kEtcPalErrBufSize);
}

TEST(Message, PackResponseInvalid)
{
  RdmCommandHeader cmd_header = rdmtest::GetSupportedParams().cmd_header;
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

TEST(Message, PackResponseTooLong)
{
  RdmCommandHeader cmd_header = rdmtest::GetSupportedParams().cmd_header;
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

TEST(Message, PackResponseWithData)
{
  const auto identify_pair = rdmtest::GetIdentifyDevice();
  RdmBuffer response_buf;

  ASSERT_EQ(rdm_pack_response(&identify_pair.cmd_header, identify_pair.RespMessageCount(), identify_pair.resp_data(),
                              identify_pair.resp_data_size(), &response_buf),
            kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, identify_pair.first_packed_resp_size());
  EXPECT_EQ(0,
            std::memcmp(identify_pair.first_packed_resp(), response_buf.data, identify_pair.first_packed_resp_size()));
}

// rdm_pack_response() should be usable to pack a response with zero-length data.
TEST(Message, PackResponseEmpty)
{
  const auto reset_dev_pair = rdmtest::SetResetDevice();
  RdmBuffer response_buf;

  ASSERT_EQ(rdm_pack_response(&reset_dev_pair.cmd_header, reset_dev_pair.RespMessageCount(), nullptr, 0, &response_buf),
            kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, reset_dev_pair.first_packed_resp_size());

  EXPECT_EQ(
      0, std::memcmp(reset_dev_pair.first_packed_resp(), response_buf.data, reset_dev_pair.first_packed_resp_size()));
}

TEST(Message, PackOverflowResponseInvalid)
{
  RdmCommandHeader cmd_header = rdmtest::GetSupportedParams().cmd_header;
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

TEST(Message, PackOverflowResponse)
{
  const auto overflow = rdmtest::GetSupportedParamsWithOverflow();
  RdmBuffer response_buf;

  // Pack the first segment of the SUPPORTED_PARAMETERS overflow response (230 bytes)
  ASSERT_EQ(rdm_pack_overflow_response(&overflow.cmd_header, overflow.resp_data(), 230, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, overflow.first_packed_resp_size());
  EXPECT_EQ(0, std::memcmp(overflow.first_packed_resp(), response_buf.data, overflow.first_packed_resp_size()));
}

TEST(Message, PackNackResponseInvalid)
{
  RdmCommandHeader cmd_header = rdmtest::GetSupportedParams().cmd_header;
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

TEST(Message, PackNackResponse)
{
  const auto nack_pair = rdmtest::SetResetDeviceNackResponse();
  RdmBuffer response_buf;

  ASSERT_EQ(rdm_pack_nack_response(&nack_pair.cmd_header, 0, nack_pair.NackReason().value(), &response_buf),
            kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, nack_pair.first_packed_resp_size());

  EXPECT_EQ(0, std::memcmp(nack_pair.first_packed_resp(), response_buf.data, nack_pair.first_packed_resp_size()));
}

TEST(Message, PackTimerResponseInvalid)
{
  RdmCommandHeader cmd_header = rdmtest::GetSupportedParams().cmd_header;
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

TEST(Message, PackTimerResponse)
{
  const auto timer_pair = rdmtest::GetLampStrikesTimerResponse();
  RdmBuffer response_buf;

  unsigned int delay_ms = timer_pair.AckTimerDelay().value();
  ASSERT_EQ(rdm_pack_timer_response(&timer_pair.cmd_header, 0, delay_ms - 50, &response_buf), kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, timer_pair.first_packed_resp_size());

  EXPECT_EQ(0, std::memcmp(timer_pair.first_packed_resp(), response_buf.data, timer_pair.first_packed_resp_size()));
}

static const RdmUid kValidDubResponseUid{2000, 400000};
static constexpr size_t kValidDubResponseSize = 24;
static const std::array<uint8_t, kValidDubResponseSize> kValidDubResponse = {
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

TEST(Message, PackDubResponseInvalid)
{
  RdmUid uid{1, 3};
  RdmBuffer buf;
  EXPECT_EQ(rdm_pack_dub_response(nullptr, nullptr), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_dub_response(&uid, nullptr), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_dub_response(nullptr, &buf), kEtcPalErrInvalid);
}

TEST(Message, PackDubResponse)
{
  RdmBuffer dub_buffer;

  EXPECT_EQ(rdm_pack_dub_response(&kValidDubResponseUid, &dub_buffer), kEtcPalErrOk);
  ASSERT_EQ(dub_buffer.data_len, kValidDubResponseSize);
  EXPECT_EQ(0, std::memcmp(kValidDubResponse.data(), dub_buffer.data, kValidDubResponse.size()));
}

// Test unpack DUB response with invalid parameters, no preamble separator or with an invalid
// checksum
TEST(Message, UnpackDubResponseInvalid)
{
  RdmUid uid;

  RdmBuffer dub_buf;
  std::memcpy(dub_buf.data, kValidDubResponse.data(), kValidDubResponseSize);
  dub_buf.data_len = kValidDubResponseSize;

  EXPECT_EQ(rdm_unpack_dub_response(nullptr, &uid), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_unpack_dub_response(&dub_buf, nullptr), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_unpack_dub_response(nullptr, nullptr), kEtcPalErrInvalid);

  // Mess with the checksum
  std::memset(&dub_buf.data[20], 0xff, 4);
  EXPECT_EQ(rdm_unpack_dub_response(&dub_buf, &uid), kEtcPalErrProtocol);

  // For no preamble separator, just use the valid one at an offset of one past the separator
  std::memcpy(dub_buf.data, &kValidDubResponse[8], kValidDubResponseSize - 8);
  dub_buf.data_len = kValidDubResponseSize - 8;

  EXPECT_EQ(rdm_unpack_dub_response(&dub_buf, &uid), kEtcPalErrProtocol);
}

TEST(Message, UnpackDubResponse)
{
  RdmBuffer dub_buf;
  RdmUid uid{};

  std::memcpy(dub_buf.data, kValidDubResponse.data(), kValidDubResponseSize);
  dub_buf.data_len = kValidDubResponseSize;

  // Progressively strip off the 0xfe padding bytes from the beginning. The function should be able
  // to validate a DUB response with anywhere from 0 to 7 0xfe bytes.
  for (size_t i = 0; i < 8; ++i)
  {
    // Validate
    EXPECT_EQ(rdm_unpack_dub_response(&dub_buf, &uid), kEtcPalErrOk) << "Failed on iteration " << i;
    EXPECT_EQ(uid, kValidDubResponseUid) << "Failed on iteration " << i;

    // Shift buffer to remove a preceding 0xfe byte
    uid = RdmUid{};
    std::memmove(dub_buf.data, &dub_buf.data[1], dub_buf.data_len - 1);
    --dub_buf.data_len;
  }
}

// This library's convention is to split ACK_OVERFLOW response data such that atomic units being
// repeated in the data are not split over message boundaries. This is not fully clear in the
// standard, but it is a strong convention and is required for some parameter messages.
TEST(Message, GetNumOverflowResponsesNeededWorks)
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

TEST(Message, PackFullOverflowResponseInvalid)
{
  const auto overflow = rdmtest::GetSupportedParamsWithOverflow();
  RdmBuffer response_buf;

  // Invalid parameters
  EXPECT_EQ(rdm_pack_full_overflow_response(nullptr, overflow.resp_data(), 200, &response_buf, 1), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_full_overflow_response(&overflow.cmd_header, nullptr, 0, &response_buf, 1), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_full_overflow_response(&overflow.cmd_header, overflow.resp_data(), 200, nullptr, 1),
            kEtcPalErrInvalid);
  EXPECT_EQ(rdm_pack_full_overflow_response(&overflow.cmd_header, overflow.resp_data(), 200, &response_buf, 0),
            kEtcPalErrInvalid);

  // Short buf
  EXPECT_EQ(rdm_pack_full_overflow_response(&overflow.cmd_header, overflow.resp_data(),
                                            overflow.overflow_resp_data_size(), &response_buf, 1),
            kEtcPalErrBufSize);
}

TEST(Message, PackFullOverflowResponse)
{
  const auto overflow = rdmtest::GetSupportedParamsWithOverflow();
  std::vector<RdmBuffer> response_bufs(overflow.packed_resps_.size());

  // We pack a response representing 150 supported parameters, 300 bytes, which should be split
  // with 230 in the first response and 70 in the second response.
  constexpr size_t kAckOverflowPdSize = 300;

  // Fill a data buffer with incrementing values
  std::array<uint8_t, kAckOverflowPdSize> pd;
  uint8_t byte = 1;
  std::generate(std::begin(pd), std::end(pd), [&]() { return byte++; });

  ASSERT_EQ(
      rdm_pack_full_overflow_response(&overflow.cmd_header, overflow.resp_data(), overflow.overflow_resp_data_size(),
                                      response_bufs.data(), response_bufs.size()),
      kEtcPalErrOk);
  for (size_t i = 0; i < overflow.packed_resps_.size(); ++i)
  {
    ASSERT_EQ(response_bufs[i].data_len, overflow.packed_resps_[i].size()) << "Failed on iteration " << i;
    EXPECT_EQ(0, std::memcmp(overflow.packed_resps_[i].data(), response_bufs[i].data, overflow.packed_resps_[i].size()))
        << "Failed on iteration " << i;
  }
}

TEST(Message, AppendParameterDataInvalid)
{
  const auto supported_params = rdmtest::GetSupportedParams();
  RdmBuffer response_buf;
  ASSERT_EQ(rdm_pack_response(&supported_params.cmd_header, 0, supported_params.resp_data(),
                              supported_params.resp_data_size(), &response_buf),
            kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, supported_params.first_packed_resp_size());

  std::array<uint8_t, 4> additional_pd = {0x00, 0x60, 0x00, 0x70};

  EXPECT_EQ(rdm_append_parameter_data(nullptr, additional_pd.data(), static_cast<uint8_t>(additional_pd.size())),
            kEtcPalErrInvalid);
  EXPECT_EQ(rdm_append_parameter_data(&response_buf, nullptr, static_cast<uint8_t>(additional_pd.size())),
            kEtcPalErrInvalid);
  EXPECT_EQ(rdm_append_parameter_data(&response_buf, additional_pd.data(), 0), kEtcPalErrInvalid);
}

TEST(Message, AppendParamaterDataTooLong)
{
  const auto supported_params = rdmtest::GetSupportedParams();

  RdmBuffer response_buf;
  ASSERT_EQ(rdm_pack_response(&supported_params.cmd_header, 0, supported_params.resp_data(),
                              supported_params.resp_data_size(), &response_buf),
            kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, supported_params.first_packed_resp_size());

  const size_t additional_pd_size = RDM_MAX_PDL - supported_params.resp_data_size() + 1;
  std::vector<uint8_t> additional_pd(additional_pd_size);
  std::iota(additional_pd.begin(), additional_pd.end(), static_cast<uint8_t>(0));

  EXPECT_EQ(rdm_append_parameter_data(&response_buf, additional_pd.data(), static_cast<uint8_t>(additional_pd.size())),
            kEtcPalErrBufSize);
}

TEST(Message, AppendParameterData)
{
  const auto supported_params = rdmtest::GetSupportedParams();
  RdmBuffer response_buf;

  ASSERT_EQ(rdm_pack_response(&supported_params.cmd_header, 0, supported_params.resp_data(),
                              supported_params.resp_data_size(), &response_buf),
            kEtcPalErrOk);
  ASSERT_EQ(response_buf.data_len, supported_params.first_packed_resp_size());
  ASSERT_EQ(
      std::memcmp(supported_params.first_packed_resp(), response_buf.data, supported_params.first_packed_resp_size()),
      0);

  constexpr uint8_t kAdditionalPdSize = 4;
  std::array<uint8_t, kAdditionalPdSize> additional_pd = {0x00, 0x60, 0x00, 0x70};

  ASSERT_EQ(rdm_append_parameter_data(&response_buf, additional_pd.data(), kAdditionalPdSize), kEtcPalErrOk);

  // TODO magic values, it would be better to integrate this into message_test_data.h somehow
  const std::vector<uint8_t> modified_response = {
      0xcc,                             // START code
      0x01,                             // Sub START code
      0x20,                             // Message Length
      0,    1,    0,    0,    0,    2,  // Destination UID
      0,    3,    0,    0,    0,    4,  // Source UID
      5,                                // Transaction Number
      0,                                // Response Type (ACK)
      0,                                // Message Count
      0,    7,                          // Subdevice
      0x21,                             // Command Class (GET Response)
      0x00, 0x50,                       // PID (E120_SUPPORTED_PARAMETERS)
      0x08,                             // PD Length

      0x00, 0x40, 0x00, 0x50, 0x00, 0x60, 0x00, 0x70,  // Parameter data

      0x02, 0xdc  // Checksum
  };
  EXPECT_EQ(0, std::memcmp(modified_response.data(), response_buf.data, modified_response.size()));
}

TEST(Message, UnpackCommand)
{
  const auto supported_params = rdmtest::GetSupportedParams();
  RdmBuffer packed_cmd = supported_params.cmd_buf();

  RdmCommandHeader cmd_header;
  const uint8_t* param_data;
  uint8_t param_data_len;

  EXPECT_EQ(rdm_unpack_command(&packed_cmd, &cmd_header, &param_data, &param_data_len), kEtcPalErrOk);
  EXPECT_EQ(cmd_header, supported_params.cmd_header);
  EXPECT_EQ(param_data, nullptr);
  EXPECT_EQ(param_data_len, 0);
}

TEST(Message, UnpackCommandWithData)
{
  const auto sensor_def = rdmtest::GetSensorDefinition();
  RdmBuffer packed_cmd = sensor_def.cmd_buf();

  RdmCommandHeader cmd_header;
  const uint8_t* param_data;
  uint8_t param_data_len;

  EXPECT_EQ(rdm_unpack_command(&packed_cmd, &cmd_header, &param_data, &param_data_len), kEtcPalErrOk);
  EXPECT_EQ(cmd_header, sensor_def.cmd_header);
  EXPECT_EQ(param_data, &packed_cmd.data[RDM_OFFSET_PARAM_DATA]);
  EXPECT_EQ(param_data_len, sensor_def.cmd_data_size());
}

TEST(Message, UnpackResponse)
{
  const auto reset_device = rdmtest::SetResetDevice();
  RdmBuffer packed_resp = reset_device.first_resp_buf();

  RdmResponseHeader resp_header;
  const uint8_t* resp_data;
  uint8_t resp_data_len;

  EXPECT_EQ(rdm_unpack_response(&packed_resp, &resp_header, &resp_data, &resp_data_len), kEtcPalErrOk);
  EXPECT_EQ(resp_header, reset_device.resp_header);
  EXPECT_EQ(resp_data, nullptr);
  EXPECT_EQ(resp_data_len, 0);
}

TEST(Message, UnpackResponseWithData)
{
  const auto supported_params = rdmtest::GetSupportedParams();
  RdmBuffer packed_resp = supported_params.first_resp_buf();

  RdmResponseHeader resp_header;
  const uint8_t* resp_data;
  uint8_t resp_data_len;

  EXPECT_EQ(rdm_unpack_response(&packed_resp, &resp_header, &resp_data, &resp_data_len), kEtcPalErrOk);
  EXPECT_EQ(resp_header, supported_params.resp_header);
  EXPECT_EQ(resp_data, &packed_resp.data[RDM_OFFSET_PARAM_DATA]);
  EXPECT_EQ(resp_data_len, supported_params.resp_data_size());
}

TEST(Message, GetAckTimerDelay)
{
  EXPECT_EQ(rdm_get_ack_timer_delay(nullptr), 0u);

  const uint8_t ack_timer_pd[2]{0x22, 0x60};
  EXPECT_EQ(rdm_get_ack_timer_delay(ack_timer_pd), 880000u);

  const auto ack_timer = rdmtest::GetLampStrikesTimerResponse();
  EXPECT_EQ(rdm_get_ack_timer_delay(&ack_timer.first_packed_resp()[RDM_OFFSET_PARAM_DATA]),
            ack_timer.AckTimerDelay().value());
}

TEST(Message, GetNackReasonCode)
{
  const uint8_t nack_reason_pd[2]{0x00, 0x07};
  EXPECT_EQ(rdm_get_nack_reason_code(nack_reason_pd), kRdmNRBufferFull);

  const auto nack = rdmtest::SetResetDeviceNackResponse();
  EXPECT_EQ(rdm_get_nack_reason_code(&nack.first_packed_resp()[RDM_OFFSET_PARAM_DATA]), nack.NackReason().value());
}

TEST(Message, CommandClassToString)
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

TEST(Message, ResponseTypeToString)
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

TEST(Message, NackReasonToString)
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
