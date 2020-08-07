var group__message =
[
    [ "RdmBuffer", "struct_rdm_buffer.html", [
      [ "data", "struct_rdm_buffer.html#ab8d006dad5cd9f0de4302a49c69fb609", null ],
      [ "data_len", "struct_rdm_buffer.html#ace1c2e33b74df8973a7d9a19c935af80", null ]
    ] ],
    [ "RdmCommandHeader", "struct_rdm_command_header.html", [
      [ "command_class", "struct_rdm_command_header.html#a8627e24459da221d560b593ea2d9f5f6", null ],
      [ "dest_uid", "struct_rdm_command_header.html#aa338a4490fb2e2c37a819966f75cc3e9", null ],
      [ "param_id", "struct_rdm_command_header.html#a43f5e73445b99b42061429682fdc78fc", null ],
      [ "port_id", "struct_rdm_command_header.html#aae6814cc8987b1899b9d94cb621857c3", null ],
      [ "source_uid", "struct_rdm_command_header.html#a40cba7d6d1910fb740982421cf66c5d6", null ],
      [ "subdevice", "struct_rdm_command_header.html#a08af6f95b61e2695caf7b11ebb8e0953", null ],
      [ "transaction_num", "struct_rdm_command_header.html#a94aea5f05dbb840df368f865f4e57cb3", null ]
    ] ],
    [ "RdmResponseHeader", "struct_rdm_response_header.html", [
      [ "command_class", "struct_rdm_response_header.html#a8627e24459da221d560b593ea2d9f5f6", null ],
      [ "dest_uid", "struct_rdm_response_header.html#aa338a4490fb2e2c37a819966f75cc3e9", null ],
      [ "msg_count", "struct_rdm_response_header.html#a7ddcf8e4afca9fbd2a1c27aa8cfa6651", null ],
      [ "param_id", "struct_rdm_response_header.html#a43f5e73445b99b42061429682fdc78fc", null ],
      [ "resp_type", "struct_rdm_response_header.html#a9241c33b6955e810316a2764d7738d63", null ],
      [ "source_uid", "struct_rdm_response_header.html#a40cba7d6d1910fb740982421cf66c5d6", null ],
      [ "subdevice", "struct_rdm_response_header.html#a08af6f95b61e2695caf7b11ebb8e0953", null ],
      [ "transaction_num", "struct_rdm_response_header.html#a94aea5f05dbb840df368f865f4e57cb3", null ]
    ] ],
    [ "RDM_GET_COMMAND_CLASS", "group__message.html#ga27ee6d5259d609fa25b83a94014692db", null ],
    [ "RDM_GET_TRANSACTION_NUM", "group__message.html#ga1a8181950bf393e5b5287dc025dc55bb", null ],
    [ "RDM_HEADER_SIZE", "group__message.html#gaa829a9423b849e07e59d42cf07943e71", null ],
    [ "RDM_MAX_BYTES", "group__message.html#ga18306dd74c16b502547003bd8b55f6dc", null ],
    [ "RDM_MAX_PDL", "group__message.html#ga4592ed22b54c1daabb9c506b72a83756", null ],
    [ "RDM_MIN_BYTES", "group__message.html#ga49d73615b42b8b11ebf70a1570724fca", null ],
    [ "RDM_NUM_STANDARD_NR_CODES", "group__message.html#gabd23c71f2d06a164ee6a83977f6aaf15", null ],
    [ "RDM_PACKED_SIZE", "group__message.html#gabf72a7fa5dde49b4e559165a28773eac", null ],
    [ "RdmBuffer", "group__message.html#ga2315bfee492030e1219b5d8fd7ce7d96", null ],
    [ "RdmCommandHeader", "group__message.html#ga8d7171aedb6a09d1731a734709390d5b", null ],
    [ "RdmResponseHeader", "group__message.html#ga8b9f70c97f84c5d0366c1dc019f012a4", null ],
    [ "rdm_command_class_t", "group__message.html#gafda6b6d4c08788e755cb3cb6c1ca021b", [
      [ "kRdmCCDiscoveryCommand", "group__message.html#ggafda6b6d4c08788e755cb3cb6c1ca021ba0b0ef646e345b039036b1238a526dcd2", null ],
      [ "kRdmCCDiscoveryCommandResponse", "group__message.html#ggafda6b6d4c08788e755cb3cb6c1ca021ba70eaf32858798b744a732c829bf1a374", null ],
      [ "kRdmCCGetCommand", "group__message.html#ggafda6b6d4c08788e755cb3cb6c1ca021ba434701b28c3c83f9230b5da6d54e26ab", null ],
      [ "kRdmCCGetCommandResponse", "group__message.html#ggafda6b6d4c08788e755cb3cb6c1ca021ba7ecfdb6ec568395f6691948051badf3c", null ],
      [ "kRdmCCSetCommand", "group__message.html#ggafda6b6d4c08788e755cb3cb6c1ca021baf16a0108fda7d3470eb281147527ddbf", null ],
      [ "kRdmCCSetCommandResponse", "group__message.html#ggafda6b6d4c08788e755cb3cb6c1ca021ba0832d12a2fa74545475e135bef44dfe1", null ]
    ] ],
    [ "rdm_nack_reason_t", "group__message.html#gaa11fc36923818f02895f40dafd228474", [
      [ "kRdmNRUnknownPid", "group__message.html#ggaa11fc36923818f02895f40dafd228474a9ff3ed335721af7014435875e1e46e95", null ],
      [ "kRdmNRFormatError", "group__message.html#ggaa11fc36923818f02895f40dafd228474a95ce3af33094fc6a5ca33f8952c58f86", null ],
      [ "kRdmNRHardwareFault", "group__message.html#ggaa11fc36923818f02895f40dafd228474a0a7b4b20e203d6dec8142b5081e0c5ba", null ],
      [ "kRdmNRProxyReject", "group__message.html#ggaa11fc36923818f02895f40dafd228474abbce9e4deaefe0f559cc560704152cb4", null ],
      [ "kRdmNRWriteProtect", "group__message.html#ggaa11fc36923818f02895f40dafd228474a80fd723c651ecb1b2c009e2de9eb612c", null ],
      [ "kRdmNRUnsupportedCommandClass", "group__message.html#ggaa11fc36923818f02895f40dafd228474a119433ed2edf83096329be3d4e87b25f", null ],
      [ "kRdmNRDataOutOfRange", "group__message.html#ggaa11fc36923818f02895f40dafd228474a98240ffd8525a507a932d7672bc8d9f3", null ],
      [ "kRdmNRBufferFull", "group__message.html#ggaa11fc36923818f02895f40dafd228474ac3e7375e24715b7814a182f641c71b33", null ],
      [ "kRdmNRPacketSizeUnsupported", "group__message.html#ggaa11fc36923818f02895f40dafd228474ac393953c3b1859efec106c94e18b578b", null ],
      [ "kRdmNRSubDeviceOutOfRange", "group__message.html#ggaa11fc36923818f02895f40dafd228474ae32eb05eb785326d7ecdb21d0eec30cf", null ],
      [ "kRdmNRProxyBufferFull", "group__message.html#ggaa11fc36923818f02895f40dafd228474a211f60cce00bf6a4ae1a1235bb07917b", null ],
      [ "kRdmNRActionNotSupported", "group__message.html#ggaa11fc36923818f02895f40dafd228474aecb7b47ab8b5938d28703a36f44e10bd", null ],
      [ "kRdmNREndpointNumberInvalid", "group__message.html#ggaa11fc36923818f02895f40dafd228474ab9bdca77dea3f6594bd04aacc8490aa3", null ],
      [ "kRdmNRInvalidEndpointMode", "group__message.html#ggaa11fc36923818f02895f40dafd228474a021ed59452fbd2a865f5ac29c642728b", null ],
      [ "kRdmNRUnknownUid", "group__message.html#ggaa11fc36923818f02895f40dafd228474a977b4f85c3ce970389a7d7a4cac53deb", null ],
      [ "kRdmNRUnknownScope", "group__message.html#ggaa11fc36923818f02895f40dafd228474a305743a045dd3cd1e1468345b1c1efd1", null ],
      [ "kRdmNRInvalidStaticConfigType", "group__message.html#ggaa11fc36923818f02895f40dafd228474a39a5696039594168d0e294257878cdb1", null ],
      [ "kRdmNRInvalidIpv4Address", "group__message.html#ggaa11fc36923818f02895f40dafd228474ab6558933e20fb9d15b813666a228c113", null ],
      [ "kRdmNRInvalidIpv6Address", "group__message.html#ggaa11fc36923818f02895f40dafd228474a5062467354c83f47abdfbe8a1ab19df8", null ],
      [ "kRdmNRInvalidPort", "group__message.html#ggaa11fc36923818f02895f40dafd228474a6684c51d582615356397c1dbb53e676e", null ]
    ] ],
    [ "rdm_response_type_t", "group__message.html#ga6ecebd42ee8d90fe410cc4001a98e7f0", [
      [ "kRdmResponseTypeAck", "group__message.html#gga6ecebd42ee8d90fe410cc4001a98e7f0a532711bc8c0acf6c938dc2535fb223f0", null ],
      [ "kRdmResponseTypeAckTimer", "group__message.html#gga6ecebd42ee8d90fe410cc4001a98e7f0ad89f498f1c63ac3274f7547c4277631f", null ],
      [ "kRdmResponseTypeNackReason", "group__message.html#gga6ecebd42ee8d90fe410cc4001a98e7f0ab2a2bb15aa43e047c469efc21fd7fa8a", null ],
      [ "kRdmResponseTypeAckOverflow", "group__message.html#gga6ecebd42ee8d90fe410cc4001a98e7f0af7fb4563b24d5aa1bbcd9e52b6fa49ff", null ]
    ] ],
    [ "rdm_append_parameter_data", "group__message.html#gad7dc9011987f10d7bc71052de1ce748d", null ],
    [ "rdm_command_class_to_string", "group__message.html#ga781f4eced16d58caa561075a251d7274", null ],
    [ "rdm_command_header_is_valid", "group__message.html#gae0e5e85c3bc1c0554656d0c3de228b13", null ],
    [ "rdm_get_ack_timer_delay", "group__message.html#gab04c0158f2ce347f244bdf5f6f3f7a1f", null ],
    [ "rdm_get_nack_reason_code", "group__message.html#gaa7a615390f37dfeaf75a9f708ac21362", null ],
    [ "rdm_get_num_overflow_responses_needed", "group__message.html#gae120f4717f4a02470525ec7b09eee230", null ],
    [ "rdm_nack_reason_to_string", "group__message.html#ga2f2bc97e71e32fde1388c76fdcb5dd89", null ],
    [ "rdm_pack_checksum", "group__message.html#ga3dacbba8c2e5243ed7556c86d8e20303", null ],
    [ "rdm_pack_command", "group__message.html#ga15a06777f4ab333d9ae3e40b38747cd1", null ],
    [ "rdm_pack_command_with_custom_buf", "group__message.html#gaac8e834a006822657d50426cd2180e5b", null ],
    [ "rdm_pack_dub_response", "group__message.html#gabb821ac360758e1ec1d4fe2ce365dfed", null ],
    [ "rdm_pack_full_overflow_response", "group__message.html#gaffcc4b47ace27ad7c22c7790d552b29c", null ],
    [ "rdm_pack_nack_response", "group__message.html#ga555626c5ed7bcb0e7f8a904ec98f23c6", null ],
    [ "rdm_pack_overflow_response", "group__message.html#gaae46c569f61cb1818d98cbb502ffb802", null ],
    [ "rdm_pack_response", "group__message.html#gac505a929fc48db196c8cb425f70ca206", null ],
    [ "rdm_pack_timer_response", "group__message.html#gaeea1c73d228ce64c716664531f9a2f0b", null ],
    [ "rdm_response_header_is_valid", "group__message.html#ga9c4a8f96c7002f4bdb3091d0f2279f03", null ],
    [ "rdm_response_type_to_string", "group__message.html#ga8e09cc2d4d6e2c0baebe61aa0042c9b2", null ],
    [ "rdm_unpack_command", "group__message.html#ga3e884cd6d1c2354a12fdc734b8f84ccc", null ],
    [ "rdm_unpack_dub_response", "group__message.html#gae28fe93b1c4b4a422edc38adc5048f4b", null ],
    [ "rdm_unpack_response", "group__message.html#gacec20040057606b981dc197d52f8fbca", null ],
    [ "rdm_validate_msg", "group__message.html#gae584f1424d687763fd6e560f4f49583f", null ]
];