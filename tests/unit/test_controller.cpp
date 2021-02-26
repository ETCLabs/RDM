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

#include "rdm/controller.h"

#include <array>
#include "gtest/gtest.h"
#include "message_test_data.h"

// Test unpack DUB response with invalid parameters, no preamble separator or with an invalid
// checksum
TEST(Controller, UnpackDubResponseInvalid)
{
  auto valid_dub_response = rdmtest::GetValidDubResponse();

  RdmBuffer dub_buf;
  std::memcpy(dub_buf.data, valid_dub_response.message.data(), valid_dub_response.message.size());
  dub_buf.data_len = valid_dub_response.message.size();

  RdmUid uid;
  EXPECT_EQ(rdm_controller_unpack_dub_response(nullptr, &uid), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_controller_unpack_dub_response(&dub_buf, nullptr), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_controller_unpack_dub_response(nullptr, nullptr), kEtcPalErrInvalid);

  // Mess with the checksum
  std::memset(&dub_buf.data[20], 0xff, 4);
  EXPECT_EQ(rdm_controller_unpack_dub_response(&dub_buf, &uid), kEtcPalErrProtocol);

  // For no preamble separator, just use the valid one at an offset of one past the separator
  std::memcpy(dub_buf.data, &valid_dub_response.message[8], valid_dub_response.message.size() - 8);
  dub_buf.data_len = valid_dub_response.message.size() - 8;

  EXPECT_EQ(rdm_controller_unpack_dub_response(&dub_buf, &uid), kEtcPalErrProtocol);
}

TEST(Controller, UnpackDubResponse)
{
  auto valid_dub_response = rdmtest::GetValidDubResponse();

  RdmBuffer dub_buf;
  std::memcpy(dub_buf.data, valid_dub_response.message.data(), valid_dub_response.message.size());
  dub_buf.data_len = valid_dub_response.message.size();

  // Progressively strip off the 0xfe padding bytes from the beginning. The function should be able
  // to validate a DUB response with anywhere from 0 to 7 0xfe bytes.
  for (size_t i = 0; i < 8; ++i)
  {
    // Validate
    RdmUid uid{};
    EXPECT_EQ(rdm_controller_unpack_dub_response(&dub_buf, &uid), kEtcPalErrOk) << "Failed on iteration " << i;
    EXPECT_EQ(uid, valid_dub_response.uid) << "Failed on iteration " << i;

    // Shift buffer to remove a preceding 0xfe byte
    uid = RdmUid{};
    std::memmove(dub_buf.data, &dub_buf.data[1], dub_buf.data_len - 1);
    --dub_buf.data_len;
  }
}
