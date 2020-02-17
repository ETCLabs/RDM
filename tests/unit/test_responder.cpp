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

#include "rdm/responder.h"
#include "gtest/gtest.h"
#include "etcpal/pack.h"

// Test the RDM_CREATE_NACK_* macros
TEST(Responder, CreateNack)
{
  RdmCommand test_cmd{};
  test_cmd.source_uid = {0, 1};
  test_cmd.dest_uid = {1, 0};
  test_cmd.transaction_num = 42;
  test_cmd.port_id = 1;
  test_cmd.subdevice = 50;
  test_cmd.command_class = kRdmCCGetCommand;
  test_cmd.param_id = E120_DMX_START_ADDRESS;
  test_cmd.datalen = 20;

  RdmResponse nack{};
  RDM_CREATE_NACK_FROM_COMMAND(&nack, &test_cmd, E120_NR_BUFFER_FULL);

  EXPECT_EQ(nack.dest_uid, RdmUid({0, 1}));
  EXPECT_EQ(nack.source_uid, RdmUid({1, 0}));
  EXPECT_EQ(nack.transaction_num, 42);
  EXPECT_EQ(nack.resp_type, kRdmResponseTypeNackReason);
  EXPECT_EQ(nack.msg_count, 0);
  EXPECT_EQ(nack.subdevice, 50);
  EXPECT_EQ(nack.command_class, kRdmCCGetCommandResponse);
  EXPECT_EQ(nack.param_id, E120_DMX_START_ADDRESS);
  EXPECT_EQ(nack.datalen, 2);
  EXPECT_EQ(etcpal_unpack_u16b(nack.data), E120_NR_BUFFER_FULL);

  RDM_CREATE_NACK_FROM_COMMAND_WITH_MSG_COUNT(&nack, &test_cmd, E120_NR_BUFFER_FULL, 20);

  EXPECT_EQ(nack.msg_count, 20);

  test_cmd.command_class = kRdmCCSetCommand;
  RDM_CREATE_NACK_FROM_COMMAND(&nack, &test_cmd, E120_NR_FORMAT_ERROR);

  EXPECT_EQ(nack.command_class, kRdmCCSetCommandResponse);
  EXPECT_EQ(etcpal_unpack_u16b(nack.data), E120_NR_FORMAT_ERROR);
}
