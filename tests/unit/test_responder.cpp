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

#include "rdm/responder.h"

#include "gtest/gtest.h"
#include "message_test_data.h"

TEST(Responder, PackDubResponseInvalid)
{
  RdmUid    uid{1, 3};
  RdmBuffer buf;
  EXPECT_EQ(rdm_responder_pack_dub_response(nullptr, nullptr), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_responder_pack_dub_response(&uid, nullptr), kEtcPalErrInvalid);
  EXPECT_EQ(rdm_responder_pack_dub_response(nullptr, &buf), kEtcPalErrInvalid);
}

TEST(Responder, PackDubResponse)
{
  auto valid_dub_response = rdmtest::GetValidDubResponse();

  RdmBuffer dub_buffer;
  EXPECT_EQ(rdm_responder_pack_dub_response(&valid_dub_response.uid, &dub_buffer), kEtcPalErrOk);
  ASSERT_EQ(dub_buffer.data_len, valid_dub_response.message.size());
  EXPECT_EQ(0, std::memcmp(valid_dub_response.message.data(), dub_buffer.data, valid_dub_response.message.size()));
}
