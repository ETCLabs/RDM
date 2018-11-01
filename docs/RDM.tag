<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>controller.h</name>
    <path>D:/git/ETCLabs/RDM/include/rdm/</path>
    <filename>controller_8h</filename>
    <includes id="message_8h" name="message.h" local="yes" imported="no">rdm/message.h</includes>
    <member kind="function">
      <type>lwpa_error_t</type>
      <name>rdmctl_create_command</name>
      <anchorfile>group__controller.html</anchorfile>
      <anchor>ga8dd7e45b8f81aee38b235c2fb2152f0a</anchor>
      <arglist>(const RdmCommand *cmd_data, RdmBuffer *buffer)</arglist>
    </member>
    <member kind="function">
      <type>lwpa_error_t</type>
      <name>rdmctl_unpack_response</name>
      <anchorfile>group__controller.html</anchorfile>
      <anchor>gaa38b3690b6efebbc86cd2a6d51bbe2ad</anchor>
      <arglist>(const RdmBuffer *buffer, RdmResponse *resp)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>message.h</name>
    <path>D:/git/ETCLabs/RDM/include/rdm/</path>
    <filename>message_8h</filename>
    <includes id="uid_8h" name="uid.h" local="yes" imported="no">rdm/uid.h</includes>
    <class kind="struct">RdmBuffer</class>
    <class kind="struct">RdmCommand</class>
    <class kind="struct">RdmResponse</class>
    <member kind="define">
      <type>#define</type>
      <name>RDM_MAX_BYTES</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga18306dd74c16b502547003bd8b55f6dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_MIN_BYTES</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga49d73615b42b8b11ebf70a1570724fca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_MAX_PDL</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga4592ed22b54c1daabb9c506b72a83756</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>get_command_class</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gabc8df9e7d72916d85184b6ac2880549e</anchor>
      <arglist>(rdmbufptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>get_transaction_num</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga60cd644214c2cae1e83cbcaef4ba3aea</anchor>
      <arglist>(rdmbufptr)</arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmBuffer</type>
      <name>RdmBuffer</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga2315bfee492030e1219b5d8fd7ce7d96</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmCommand</type>
      <name>RdmCommand</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga9654853f19d0bb92aba656165221c29c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmResponse</type>
      <name>RdmResponse</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaa98249dee1f0de42cd77642fa18e0517</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rdm_pack_checksum</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gabbcd7273b516c2a63692e038698856fd</anchor>
      <arglist>(uint8_t *buffer, size_t datalen_without_checksum)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>rdm_validate_msg</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gae584f1424d687763fd6e560f4f49583f</anchor>
      <arglist>(const RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>responder.h</name>
    <path>D:/git/ETCLabs/RDM/include/rdm/</path>
    <filename>responder_8h</filename>
    <includes id="message_8h" name="message.h" local="yes" imported="no">rdm/message.h</includes>
    <member kind="function">
      <type>lwpa_error_t</type>
      <name>rdmresp_unpack_command</name>
      <anchorfile>group__responder.html</anchorfile>
      <anchor>ga712f0a7856b5916073ac5efda65f0683</anchor>
      <arglist>(const RdmBuffer *buffer, RdmCommand *cmd)</arglist>
    </member>
    <member kind="function">
      <type>lwpa_error_t</type>
      <name>rdmresp_create_response</name>
      <anchorfile>group__responder.html</anchorfile>
      <anchor>gac086fc597854cdb099737a66dc457344</anchor>
      <arglist>(const RdmResponse *resp_data, RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>uid.h</name>
    <path>D:/git/ETCLabs/RDM/include/rdm/</path>
    <filename>uid_8h</filename>
    <class kind="struct">RdmUid</class>
    <member kind="define">
      <type>#define</type>
      <name>uid_cmp</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga1f0e587d7bc26d1adab103e5ed6db9d7</anchor>
      <arglist>(uidptr1, uidptr2)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_equal</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gab57e197a69d465b71089c5f36e5a510f</anchor>
      <arglist>(uidptr1, uidptr2)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>init_static_uid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga51b8dcb487d19cc07d3cd926a017b698</anchor>
      <arglist>(uidptr, manu_val, id_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>init_dynamic_uid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga7f7bee2337e45eb27c8180e8b1178046</anchor>
      <arglist>(uidptr, manu_val, id_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_broadcast</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga2fc7f8e2cea15e4fd1a2e903d0ad23ea</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_rdmnet_controller_broadcast</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaabd65340080b6d7d083b9719b2d9723e</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_rdmnet_device_broadcast</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga51f99ed4b8f1edddf2068ee8c68e8e11</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_rdmnet_device_manu_broadcast</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga8ef4f2936b3b944c3cceca1d7ebf1801</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>rdmnet_device_broadcast_manu_matches</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaa173f304e2e1c469670cef664260f543</anchor>
      <arglist>(uidptr, manu_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>rdmnet_device_broadcast_manu_id</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga98d12a84da72b76c2173a46fdc25027b</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_dynamic</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga00667ebd9ba8321637134546cfb3b69f</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>get_manufacturer_id</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga8b42d44d069f01298595e0bd2b52096f</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>get_device_id</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga5826ef4fbeab6938db9c83e396a616aa</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmUid</type>
      <name>RdmUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gad467bfae3ada73c70aaeb29b192ee351</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const RdmUid</type>
      <name>kBroadcastUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga5693deeccc02604dcc7dce00f00daeea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const RdmUid</type>
      <name>kRdmnetControllerBroadcastUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga2440a6426ac165d97dc206069060fd9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const RdmUid</type>
      <name>kRdmnetDeviceBroadcastUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga9a8e7cd056b52e54e8a66d84908b82fe</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>version.h</name>
    <path>D:/git/ETCLabs/RDM/include/rdm/</path>
    <filename>version_8h</filename>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_MAJOR</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>gaff7817d7e723e0c9224c5a1402c73005</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_MINOR</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>ga28764f341874f72cf8204e0f435f4162</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_PATCH</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>ga378fba3384cafec2bc68315d32879130</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_BUILD</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>gad253ce0c4574d2cbfefaabf507aab45c</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_MAJOR</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>gaff7817d7e723e0c9224c5a1402c73005</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_MINOR</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>ga28764f341874f72cf8204e0f435f4162</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_PATCH</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>ga378fba3384cafec2bc68315d32879130</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_BUILD</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>gad253ce0c4574d2cbfefaabf507aab45c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RdmBuffer</name>
    <filename>struct_rdm_buffer.html</filename>
    <member kind="variable">
      <type>uint8_t</type>
      <name>data</name>
      <anchorfile>struct_rdm_buffer.html</anchorfile>
      <anchor>ab8d006dad5cd9f0de4302a49c69fb609</anchor>
      <arglist>[RDM_MAX_BYTES]</arglist>
    </member>
    <member kind="variable">
      <type>size_t</type>
      <name>datalen</name>
      <anchorfile>struct_rdm_buffer.html</anchorfile>
      <anchor>a3d12e161726566d24a1d0cf6c6b416bb</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RdmCommand</name>
    <filename>struct_rdm_command.html</filename>
    <member kind="variable">
      <type>RdmUid</type>
      <name>src_uid</name>
      <anchorfile>struct_rdm_command.html</anchorfile>
      <anchor>a6c99d53ad4583f3f5dc719f57d6f0714</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RdmUid</type>
      <name>dest_uid</name>
      <anchorfile>struct_rdm_command.html</anchorfile>
      <anchor>aa338a4490fb2e2c37a819966f75cc3e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>transaction_num</name>
      <anchorfile>struct_rdm_command.html</anchorfile>
      <anchor>a94aea5f05dbb840df368f865f4e57cb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>port_id</name>
      <anchorfile>struct_rdm_command.html</anchorfile>
      <anchor>aae6814cc8987b1899b9d94cb621857c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>subdevice</name>
      <anchorfile>struct_rdm_command.html</anchorfile>
      <anchor>a08af6f95b61e2695caf7b11ebb8e0953</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>command_class</name>
      <anchorfile>struct_rdm_command.html</anchorfile>
      <anchor>a4420a5c558963cb84d88b6f6ae4fdaec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>param_id</name>
      <anchorfile>struct_rdm_command.html</anchorfile>
      <anchor>a43f5e73445b99b42061429682fdc78fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>datalen</name>
      <anchorfile>struct_rdm_command.html</anchorfile>
      <anchor>ace5c23e2acbfc3932b3b282d5bbf59f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>data</name>
      <anchorfile>struct_rdm_command.html</anchorfile>
      <anchor>a1f9dbe5a4390d94cb28b7b21d2b656d5</anchor>
      <arglist>[RDM_MAX_PDL]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RdmResponse</name>
    <filename>struct_rdm_response.html</filename>
    <member kind="variable">
      <type>RdmUid</type>
      <name>src_uid</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>a6c99d53ad4583f3f5dc719f57d6f0714</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RdmUid</type>
      <name>dest_uid</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>aa338a4490fb2e2c37a819966f75cc3e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>transaction_num</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>a94aea5f05dbb840df368f865f4e57cb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>resp_type</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>a63b9744e96ce785c7a108904e1408c23</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>msg_count</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>a7ddcf8e4afca9fbd2a1c27aa8cfa6651</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>subdevice</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>a08af6f95b61e2695caf7b11ebb8e0953</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>command_class</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>a4420a5c558963cb84d88b6f6ae4fdaec</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>param_id</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>a43f5e73445b99b42061429682fdc78fc</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>datalen</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>ace5c23e2acbfc3932b3b282d5bbf59f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>data</name>
      <anchorfile>struct_rdm_response.html</anchorfile>
      <anchor>a1f9dbe5a4390d94cb28b7b21d2b656d5</anchor>
      <arglist>[RDM_MAX_PDL]</arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RdmUid</name>
    <filename>struct_rdm_uid.html</filename>
  </compound>
  <compound kind="group">
    <name>controller</name>
    <title>Controller</title>
    <filename>group__controller.html</filename>
    <member kind="function">
      <type>lwpa_error_t</type>
      <name>rdmctl_create_command</name>
      <anchorfile>group__controller.html</anchorfile>
      <anchor>ga8dd7e45b8f81aee38b235c2fb2152f0a</anchor>
      <arglist>(const RdmCommand *cmd_data, RdmBuffer *buffer)</arglist>
    </member>
    <member kind="function">
      <type>lwpa_error_t</type>
      <name>rdmctl_unpack_response</name>
      <anchorfile>group__controller.html</anchorfile>
      <anchor>gaa38b3690b6efebbc86cd2a6d51bbe2ad</anchor>
      <arglist>(const RdmBuffer *buffer, RdmResponse *resp)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>message</name>
    <title>Message</title>
    <filename>group__message.html</filename>
    <class kind="struct">RdmBuffer</class>
    <class kind="struct">RdmCommand</class>
    <class kind="struct">RdmResponse</class>
    <member kind="define">
      <type>#define</type>
      <name>RDM_MAX_BYTES</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga18306dd74c16b502547003bd8b55f6dc</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_MIN_BYTES</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga49d73615b42b8b11ebf70a1570724fca</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_MAX_PDL</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga4592ed22b54c1daabb9c506b72a83756</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>get_command_class</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gabc8df9e7d72916d85184b6ac2880549e</anchor>
      <arglist>(rdmbufptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>get_transaction_num</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga60cd644214c2cae1e83cbcaef4ba3aea</anchor>
      <arglist>(rdmbufptr)</arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmBuffer</type>
      <name>RdmBuffer</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga2315bfee492030e1219b5d8fd7ce7d96</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmCommand</type>
      <name>RdmCommand</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga9654853f19d0bb92aba656165221c29c</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmResponse</type>
      <name>RdmResponse</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaa98249dee1f0de42cd77642fa18e0517</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>rdm_pack_checksum</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gabbcd7273b516c2a63692e038698856fd</anchor>
      <arglist>(uint8_t *buffer, size_t datalen_without_checksum)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>rdm_validate_msg</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gae584f1424d687763fd6e560f4f49583f</anchor>
      <arglist>(const RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>responder</name>
    <title>Responder</title>
    <filename>group__responder.html</filename>
    <member kind="function">
      <type>lwpa_error_t</type>
      <name>rdmresp_unpack_command</name>
      <anchorfile>group__responder.html</anchorfile>
      <anchor>ga712f0a7856b5916073ac5efda65f0683</anchor>
      <arglist>(const RdmBuffer *buffer, RdmCommand *cmd)</arglist>
    </member>
    <member kind="function">
      <type>lwpa_error_t</type>
      <name>rdmresp_create_response</name>
      <anchorfile>group__responder.html</anchorfile>
      <anchor>gac086fc597854cdb099737a66dc457344</anchor>
      <arglist>(const RdmResponse *resp_data, RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>uid</name>
    <title>UID</title>
    <filename>group__uid.html</filename>
    <class kind="struct">RdmUid</class>
    <member kind="define">
      <type>#define</type>
      <name>uid_cmp</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga1f0e587d7bc26d1adab103e5ed6db9d7</anchor>
      <arglist>(uidptr1, uidptr2)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_equal</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gab57e197a69d465b71089c5f36e5a510f</anchor>
      <arglist>(uidptr1, uidptr2)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>init_static_uid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga51b8dcb487d19cc07d3cd926a017b698</anchor>
      <arglist>(uidptr, manu_val, id_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>init_dynamic_uid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga7f7bee2337e45eb27c8180e8b1178046</anchor>
      <arglist>(uidptr, manu_val, id_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_broadcast</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga2fc7f8e2cea15e4fd1a2e903d0ad23ea</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_rdmnet_controller_broadcast</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaabd65340080b6d7d083b9719b2d9723e</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_rdmnet_device_broadcast</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga51f99ed4b8f1edddf2068ee8c68e8e11</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_rdmnet_device_manu_broadcast</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga8ef4f2936b3b944c3cceca1d7ebf1801</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>rdmnet_device_broadcast_manu_matches</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaa173f304e2e1c469670cef664260f543</anchor>
      <arglist>(uidptr, manu_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>rdmnet_device_broadcast_manu_id</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga98d12a84da72b76c2173a46fdc25027b</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>uid_is_dynamic</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga00667ebd9ba8321637134546cfb3b69f</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>get_manufacturer_id</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga8b42d44d069f01298595e0bd2b52096f</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>get_device_id</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga5826ef4fbeab6938db9c83e396a616aa</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmUid</type>
      <name>RdmUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gad467bfae3ada73c70aaeb29b192ee351</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const RdmUid</type>
      <name>kBroadcastUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga5693deeccc02604dcc7dce00f00daeea</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const RdmUid</type>
      <name>kRdmnetControllerBroadcastUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga2440a6426ac165d97dc206069060fd9e</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>const RdmUid</type>
      <name>kRdmnetDeviceBroadcastUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga9a8e7cd056b52e54e8a66d84908b82fe</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>rdm</name>
    <title>RDM</title>
    <filename>group__rdm.html</filename>
    <subgroup>controller</subgroup>
    <subgroup>message</subgroup>
    <subgroup>responder</subgroup>
    <subgroup>uid</subgroup>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_MAJOR</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>gaff7817d7e723e0c9224c5a1402c73005</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_MINOR</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>ga28764f341874f72cf8204e0f435f4162</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_PATCH</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>ga378fba3384cafec2bc68315d32879130</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_VERSION_BUILD</name>
      <anchorfile>group__rdm.html</anchorfile>
      <anchor>gad253ce0c4574d2cbfefaabf507aab45c</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>RDM</title>
    <filename>index</filename>
  </compound>
</tagfile>
