<?xml version='1.0' encoding='UTF-8' standalone='yes' ?>
<tagfile>
  <compound kind="file">
    <name>controller.h</name>
    <path>D:/git/ETCLabs/RDMnet/external/RDM/include/rdm/</path>
    <filename>controller_8h</filename>
    <includes id="message_8h" name="message.h" local="yes" imported="no">rdm/message.h</includes>
    <member kind="function">
      <type>bool</type>
      <name>rdmctl_is_non_disc_response</name>
      <anchorfile>group__controller.html</anchorfile>
      <anchor>ga62a94b697a099eea6bad03f10a592277</anchor>
      <arglist>(const RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>message.h</name>
    <path>D:/git/ETCLabs/RDMnet/external/RDM/include/rdm/cpp/</path>
    <filename>cpp_2message_8h</filename>
    <includes id="message_8h" name="message.h" local="yes" imported="no">rdm/message.h</includes>
    <includes id="cpp_2uid_8h" name="uid.h" local="yes" imported="no">rdm/cpp/uid.h</includes>
    <class kind="class">rdm::NackReason</class>
    <class kind="class">rdm::CommandHeader</class>
    <class kind="class">rdm::Command</class>
    <class kind="class">rdm::ResponseHeader</class>
    <class kind="class">rdm::Response</class>
  </compound>
  <compound kind="file">
    <name>message.h</name>
    <path>D:/git/ETCLabs/RDMnet/external/RDM/include/rdm/</path>
    <filename>message_8h</filename>
    <includes id="uid_8h" name="uid.h" local="yes" imported="no">rdm/uid.h</includes>
    <class kind="struct">RdmBuffer</class>
    <class kind="struct">RdmCommandHeader</class>
    <class kind="struct">RdmResponseHeader</class>
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
      <name>RDM_HEADER_SIZE</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaa829a9423b849e07e59d42cf07943e71</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_GET_COMMAND_CLASS</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga27ee6d5259d609fa25b83a94014692db</anchor>
      <arglist>(rdmbufptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_GET_TRANSACTION_NUM</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga1a8181950bf393e5b5287dc025dc55bb</anchor>
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
      <type>struct RdmCommandHeader</type>
      <name>RdmCommandHeader</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga8d7171aedb6a09d1731a734709390d5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmResponseHeader</type>
      <name>RdmResponseHeader</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga8b9f70c97f84c5d0366c1dc019f012a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>rdm_command_class_t</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gafda6b6d4c08788e755cb3cb6c1ca021b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>rdm_response_type_t</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga6ecebd42ee8d90fe410cc4001a98e7f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>rdm_nack_reason_t</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaa11fc36923818f02895f40dafd228474</anchor>
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
    <member kind="function">
      <type>etcpal_error_t</type>
      <name>rdm_create_response</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga2ea6c686aa76d05994796d41e7bcf7d5</anchor>
      <arglist>(const RdmCommandHeader *cmd_header, uint8_t msg_count, const uint8_t *response_data, uint8_t response_data_len, RdmBuffer *buffer)</arglist>
    </member>
    <member kind="function">
      <type>etcpal_error_t</type>
      <name>rdm_create_nack_response</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga49f59fd8adcf7c7f396760db14bd1f9f</anchor>
      <arglist>(const RdmCommandHeader *cmd_header, uint8_t msg_count, rdm_nack_reason_t nack_reason, RdmBuffer *buffer)</arglist>
    </member>
    <member kind="function">
      <type>etcpal_error_t</type>
      <name>rdm_create_timer_response</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaffcdf5fc31e74fea4b9eee58541b6aaf</anchor>
      <arglist>(const RdmCommandHeader *cmd_header, uint8_t msg_count, unsigned int delay_time_ms, RdmBuffer *buffer)</arglist>
    </member>
    <member kind="function">
      <type>etcpal_error_t</type>
      <name>rdm_create_dub_response</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaa016ed3a9b2ac887b22cf9ac93b93f7a</anchor>
      <arglist>(const RdmUid *responder_uid, RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>uid.h</name>
    <path>D:/git/ETCLabs/RDMnet/external/RDM/include/rdm/cpp/</path>
    <filename>cpp_2uid_8h</filename>
    <includes id="uid_8h" name="uid.h" local="yes" imported="no">rdm/uid.h</includes>
    <class kind="class">rdm::Uid</class>
  </compound>
  <compound kind="file">
    <name>uid.h</name>
    <path>D:/git/ETCLabs/RDMnet/external/RDM/include/rdm/</path>
    <filename>uid_8h</filename>
    <class kind="struct">RdmUid</class>
    <member kind="define">
      <type>#define</type>
      <name>RDM_UID_EQUAL</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gae538a395a7b22c4fcac46fa0f5554b78</anchor>
      <arglist>(uidptr1, uidptr2)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_INIT_STATIC_UID</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga0daf30670c9321b7a3d0d7a69dd6b4e5</anchor>
      <arglist>(uidptr, manu_val, id_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_INIT_DYNAMIC_UID_REQUEST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaad2d26ff7d08e1fcc134c1be979fcdc8</anchor>
      <arglist>(uidptr, manu_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_INIT_DEVICE_MANU_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaa32bb9da2292c2ad817b97919f6a02c5</anchor>
      <arglist>(uidptr, manu_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_UID_IS_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga65209ad8850eef83a6249f93bd215da6</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_CONTROLLER_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga53c88cbe60ef6f037d8c2a9fdff9894a</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_DEVICE_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga6f623f06ac3b14f1becf06ec4faf65f6</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_DEVICE_MANU_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga224755484593f73ac68fe0ad974c192f</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_DEVICE_BROADCAST_MANU_MATCHES</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaeaca39dc0f8d229d9966652e53be634f</anchor>
      <arglist>(uidptr, manu_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_DEVICE_BROADCAST_MANU_ID</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaa0bde1cfb88ecf6741ab5b088c79d107</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_UID_IS_NULL</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga1b5ffea9d8b7ae8f0f4cdf5560901254</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_DYNAMIC</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga6513e844d7b58f9cdee40244fe5a682f</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_DYNAMIC_UID_REQUEST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga7309e3ce684e0d96d6a5ac019d6b8892</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_STATIC</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaaaf8226917bc3e83e12f8b4d51d11ac8</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_GET_MANUFACTURER_ID</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga0918dda49918646bc3e77c82f4c04221</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_GET_DEVICE_ID</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaa014f2b3e799431e08b951b3edb794f8</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_UID_STRING_BYTES</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaca68c70b4aca047a5b8ff73e9d73b492</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmUid</type>
      <name>RdmUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gad467bfae3ada73c70aaeb29b192ee351</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>rdm_uid_compare</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga43e676dfc5272c40015061109ea6ed7f</anchor>
      <arglist>(const RdmUid *a, const RdmUid *b)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>rdm_uid_to_string</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga90190789ec0a4c7262c47e2d7a1fdf43</anchor>
      <arglist>(const RdmUid *uid, char *buf)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>rdm_string_to_uid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gae4c4bb5385d1982c9b8ed05323a25b2b</anchor>
      <arglist>(const char *str, RdmUid *uid)</arglist>
    </member>
    <member kind="variable">
      <type>const RdmUid</type>
      <name>kRdmBroadcastUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gae90bca840cb37b2833f202983f01aa4d</anchor>
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
    <name>responder.h</name>
    <path>D:/git/ETCLabs/RDMnet/external/RDM/include/rdm/</path>
    <filename>responder_8h</filename>
    <includes id="message_8h" name="message.h" local="yes" imported="no">rdm/message.h</includes>
    <member kind="function">
      <type>bool</type>
      <name>rdmresp_is_non_disc_command</name>
      <anchorfile>group__responder.html</anchorfile>
      <anchor>ga4f3587c5330568a286873f8d392d8233</anchor>
      <arglist>(const RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="file">
    <name>version.h</name>
    <path>D:/git/ETCLabs/RDMnet/external/RDM/include/rdm/</path>
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
  <compound kind="class">
    <name>rdm::Command</name>
    <filename>classrdm_1_1_command.html</filename>
  </compound>
  <compound kind="class">
    <name>rdm::CommandHeader</name>
    <filename>classrdm_1_1_command_header.html</filename>
    <member kind="function">
      <type></type>
      <name>CommandHeader</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>a657ab95acedf39de9ee64e0f6aa227e5</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>CommandHeader</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>a3844e910fcc763e4b6117bc94e23ec21</anchor>
      <arglist>(rdm_command_class_t command_class, uint16_t param_id, const Uid &amp;source_uid, const Uid &amp;dest_uid, uint8_t transaction_num, uint8_t port_id, uint16_t subdevice=0)</arglist>
    </member>
    <member kind="function">
      <type>constexpr Uid</type>
      <name>source_uid</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>a682c0a8d49eb0d987c57655bce549a52</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr Uid</type>
      <name>dest_uid</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>adae90f696c7c8e936f66772609da8354</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr uint8_t</type>
      <name>transaction_num</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>a2e59122799c69eaaeaff36fd65f20266</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr uint8_t</type>
      <name>port_id</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>ae572d68bc9ea407bff471326407839e7</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr uint16_t</type>
      <name>subdevice</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>ace76a3360a4ca27564b266d231844fe2</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr rdm_command_class_t</type>
      <name>command_class</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>a59f11046e01fbfe627604b2eb966e5d6</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr uint16_t</type>
      <name>param_id</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>aeb06b87b7d84f9dcb290a7ac38a9f326</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexprconst ::RdmCommandHeader &amp;</type>
      <name>get</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>a266dfbd22c2e32c1267ee9b288919c41</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>ETCPAL_CONSTEXPR_14 ::RdmCommandHeader &amp;</type>
      <name>get</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>aff7da1445b9f473d10a83d61c33f5970</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsValid</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>a472f705194218179e9cf26ffb288da6c</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>IsGet</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>a24e38683dabb26fcffff5ff569c24451</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>IsSet</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>ac07548ce2bdda1e5eaa95c647aadd52f</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>IsDiscovery</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>a920e16df3ae45e4ab784247198dd2821</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>etcpal::Error</type>
      <name>Serialize</name>
      <anchorfile>classrdm_1_1_command_header.html</anchorfile>
      <anchor>ab8c8dc70191aaba2ab8e7248667093a4</anchor>
      <arglist>(RdmBuffer &amp;buffer, const uint8_t *data=nullptr, uint8_t datalen=0) const noexcept</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>rdm::NackReason</name>
    <filename>classrdm_1_1_nack_reason.html</filename>
    <member kind="function">
      <type>constexpr</type>
      <name>NackReason</name>
      <anchorfile>classrdm_1_1_nack_reason.html</anchorfile>
      <anchor>a03d973194be2b2fd38d6c57255892c35</anchor>
      <arglist>(rdm_nack_reason_t code) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NackReason &amp;</type>
      <name>operator=</name>
      <anchorfile>classrdm_1_1_nack_reason.html</anchorfile>
      <anchor>a3265a3906491690c17286ea8779248e9</anchor>
      <arglist>(rdm_nack_reason_t code) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>NackReason</name>
      <anchorfile>classrdm_1_1_nack_reason.html</anchorfile>
      <anchor>a0103bd18e1a2247bdb9b9553132dcf66</anchor>
      <arglist>(uint16_t code) noexcept</arglist>
    </member>
    <member kind="function">
      <type>NackReason &amp;</type>
      <name>operator=</name>
      <anchorfile>classrdm_1_1_nack_reason.html</anchorfile>
      <anchor>a81b3dce4a2c5ab1933cd095da962637c</anchor>
      <arglist>(uint16_t code) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr rdm_nack_reason_t</type>
      <name>code</name>
      <anchorfile>classrdm_1_1_nack_reason.html</anchorfile>
      <anchor>a5ecf10f675fe91f1f30f82a89c8b87e5</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr uint16_t</type>
      <name>raw_code</name>
      <anchorfile>classrdm_1_1_nack_reason.html</anchorfile>
      <anchor>aa6afaf361808d30c5ab43bca685b57b2</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>ToString</name>
      <anchorfile>classrdm_1_1_nack_reason.html</anchorfile>
      <anchor>a19c380b03cea21d7ac7325136a131ff0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>const char *</type>
      <name>ToCString</name>
      <anchorfile>classrdm_1_1_nack_reason.html</anchorfile>
      <anchor>aa4d860f883bc4c6bc997eb61f218a27e</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>IsStandard</name>
      <anchorfile>classrdm_1_1_nack_reason.html</anchorfile>
      <anchor>ae4feff731acc0f4ce9418bbfd486662d</anchor>
      <arglist>() const noexcept</arglist>
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
    <name>RdmCommandHeader</name>
    <filename>struct_rdm_command_header.html</filename>
    <member kind="variable">
      <type>RdmUid</type>
      <name>source_uid</name>
      <anchorfile>struct_rdm_command_header.html</anchorfile>
      <anchor>a40cba7d6d1910fb740982421cf66c5d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RdmUid</type>
      <name>dest_uid</name>
      <anchorfile>struct_rdm_command_header.html</anchorfile>
      <anchor>aa338a4490fb2e2c37a819966f75cc3e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>transaction_num</name>
      <anchorfile>struct_rdm_command_header.html</anchorfile>
      <anchor>a94aea5f05dbb840df368f865f4e57cb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>port_id</name>
      <anchorfile>struct_rdm_command_header.html</anchorfile>
      <anchor>aae6814cc8987b1899b9d94cb621857c3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>subdevice</name>
      <anchorfile>struct_rdm_command_header.html</anchorfile>
      <anchor>a08af6f95b61e2695caf7b11ebb8e0953</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>rdm_command_class_t</type>
      <name>command_class</name>
      <anchorfile>struct_rdm_command_header.html</anchorfile>
      <anchor>a8627e24459da221d560b593ea2d9f5f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>param_id</name>
      <anchorfile>struct_rdm_command_header.html</anchorfile>
      <anchor>a43f5e73445b99b42061429682fdc78fc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RdmResponseHeader</name>
    <filename>struct_rdm_response_header.html</filename>
    <member kind="variable">
      <type>RdmUid</type>
      <name>source_uid</name>
      <anchorfile>struct_rdm_response_header.html</anchorfile>
      <anchor>a40cba7d6d1910fb740982421cf66c5d6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>RdmUid</type>
      <name>dest_uid</name>
      <anchorfile>struct_rdm_response_header.html</anchorfile>
      <anchor>aa338a4490fb2e2c37a819966f75cc3e9</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>transaction_num</name>
      <anchorfile>struct_rdm_response_header.html</anchorfile>
      <anchor>a94aea5f05dbb840df368f865f4e57cb3</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>rdm_response_type_t</type>
      <name>resp_type</name>
      <anchorfile>struct_rdm_response_header.html</anchorfile>
      <anchor>a9241c33b6955e810316a2764d7738d63</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint8_t</type>
      <name>msg_count</name>
      <anchorfile>struct_rdm_response_header.html</anchorfile>
      <anchor>a7ddcf8e4afca9fbd2a1c27aa8cfa6651</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>subdevice</name>
      <anchorfile>struct_rdm_response_header.html</anchorfile>
      <anchor>a08af6f95b61e2695caf7b11ebb8e0953</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>rdm_command_class_t</type>
      <name>command_class</name>
      <anchorfile>struct_rdm_response_header.html</anchorfile>
      <anchor>a8627e24459da221d560b593ea2d9f5f6</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint16_t</type>
      <name>param_id</name>
      <anchorfile>struct_rdm_response_header.html</anchorfile>
      <anchor>a43f5e73445b99b42061429682fdc78fc</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="struct">
    <name>RdmUid</name>
    <filename>struct_rdm_uid.html</filename>
    <member kind="variable">
      <type>uint16_t</type>
      <name>manu</name>
      <anchorfile>struct_rdm_uid.html</anchorfile>
      <anchor>a29d588ede91f0ca7c2cf0105990b338a</anchor>
      <arglist></arglist>
    </member>
    <member kind="variable">
      <type>uint32_t</type>
      <name>id</name>
      <anchorfile>struct_rdm_uid.html</anchorfile>
      <anchor>abaabdc509cdaba7df9f56c6c76f3ae19</anchor>
      <arglist></arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>rdm::Response</name>
    <filename>classrdm_1_1_response.html</filename>
    <member kind="function">
      <type></type>
      <name>Response</name>
      <anchorfile>classrdm_1_1_response.html</anchorfile>
      <anchor>ab266be0601d388ae0ba88b69a43c4d68</anchor>
      <arglist>()=default</arglist>
    </member>
  </compound>
  <compound kind="class">
    <name>rdm::ResponseHeader</name>
    <filename>classrdm_1_1_response_header.html</filename>
  </compound>
  <compound kind="class">
    <name>rdm::Uid</name>
    <filename>classrdm_1_1_uid.html</filename>
    <member kind="function">
      <type></type>
      <name>Uid</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a26e08027dc178237dfa2cc907e2888b4</anchor>
      <arglist>()=default</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>Uid</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a0c25f39dfd1f6bd2502ebfab4c022825</anchor>
      <arglist>(uint16_t manu_val, uint32_t id_val) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr</type>
      <name>Uid</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a301d31687b418ca366af9981c99ee5ce</anchor>
      <arglist>(const RdmUid &amp;c_uid) noexcept</arglist>
    </member>
    <member kind="function">
      <type>Uid &amp;</type>
      <name>operator=</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a1abd1d0fb4cd81bb0855276aa72a2d1f</anchor>
      <arglist>(const RdmUid &amp;c_uid) noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr const RdmUid &amp;</type>
      <name>get</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>ac2013ef417deb531e6d9427f5edc2e6d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>ETCPAL_CONSTEXPR_14 RdmUid &amp;</type>
      <name>get</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a76b39145df4e82ac23529fa2bcde4f59</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr uint16_t</type>
      <name>manufacturer_id</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>ad82230ded92e64886e7a5b2ca8f11fca</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr uint32_t</type>
      <name>device_id</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a798f537a05ad86e66684f90ba57aca3d</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>std::string</type>
      <name>ToString</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a19c380b03cea21d7ac7325136a131ff0</anchor>
      <arglist>() const</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>IsValid</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a6d02bb05c3ab899ab6752bbab4e3dbcf</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsBroadcast</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>ac796dce1a5bc8b5390b9d38a030a53f8</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsControllerBroadcast</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a2eef117170c28c8ffd858a0bab410ad2</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsDeviceBroadcast</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a86b0bf5189cf77fef12776321c70a175</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>IsDeviceManufacturerBroadcast</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a0e287069583793b672ce63b9c9748c6f</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>BroadcastManufacturerIdMatches</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a13cfa5164be242ab23c7b7ce483cff6e</anchor>
      <arglist>(uint16_t manu_val) const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr uint16_t</type>
      <name>DeviceBroadcastManufacturerId</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>acc06890f99b0c4318ada4bdabb8e8a98</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>IsDynamic</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>ab1affd5b2613fc51ad89a38272bff137</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>IsDynamicUidRequest</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a3d5604d188fdfc7e8e755a9ecb27e1f1</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>constexpr bool</type>
      <name>IsStatic</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a9354e8475205938f41c6b54378c4add1</anchor>
      <arglist>() const noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetManufacturerId</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>ad3d0b62ea5b7718b0f0b398526d688b2</anchor>
      <arglist>(uint16_t manu_val) noexcept</arglist>
    </member>
    <member kind="function">
      <type>void</type>
      <name>SetDeviceId</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a14e720c918cbee08c3246eafa443eb57</anchor>
      <arglist>(uint32_t id_val) noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Uid</type>
      <name>FromString</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a6010a34929ae98e614ca8674a2518a1a</anchor>
      <arglist>(const std::string &amp;uid_str) noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Uid</type>
      <name>Broadcast</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>acfa1e52908435eb6a8687580f0a0b9f8</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Uid</type>
      <name>ControllerBroadcast</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>aa58c75a03749f4c8f00f32b2f5015d21</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Uid</type>
      <name>DeviceBroadcast</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>ac7c155552fc201651f6dd2161321143c</anchor>
      <arglist>() noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Uid</type>
      <name>DeviceBroadcast</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a09b552d8b9b58533dbd1d4669405abba</anchor>
      <arglist>(uint16_t manu_val) noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Uid</type>
      <name>Static</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>ac4b5baabd16d695d769d503babee2ae1</anchor>
      <arglist>(uint16_t manu_val, uint32_t id_val) noexcept</arglist>
    </member>
    <member kind="function" static="yes">
      <type>static Uid</type>
      <name>DynamicUidRequest</name>
      <anchorfile>classrdm_1_1_uid.html</anchorfile>
      <anchor>a79e484fe4ffbb0f40cdb57e62f273f3f</anchor>
      <arglist>(uint16_t manu_val) noexcept</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>controller</name>
    <title>Controller</title>
    <filename>group__controller.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>rdmctl_is_non_disc_response</name>
      <anchorfile>group__controller.html</anchorfile>
      <anchor>ga62a94b697a099eea6bad03f10a592277</anchor>
      <arglist>(const RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>cpp_message</name>
    <title>Message</title>
    <filename>group__cpp__message.html</filename>
    <class kind="class">rdm::NackReason</class>
    <class kind="class">rdm::CommandHeader</class>
    <class kind="class">rdm::Command</class>
    <class kind="class">rdm::Response</class>
  </compound>
  <compound kind="group">
    <name>cpp_uid</name>
    <title>UID</title>
    <filename>group__cpp__uid.html</filename>
    <class kind="class">rdm::Uid</class>
  </compound>
  <compound kind="group">
    <name>message</name>
    <title>Message</title>
    <filename>group__message.html</filename>
    <class kind="struct">RdmBuffer</class>
    <class kind="struct">RdmCommandHeader</class>
    <class kind="struct">RdmResponseHeader</class>
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
      <name>RDM_HEADER_SIZE</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaa829a9423b849e07e59d42cf07943e71</anchor>
      <arglist></arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_GET_COMMAND_CLASS</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga27ee6d5259d609fa25b83a94014692db</anchor>
      <arglist>(rdmbufptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_GET_TRANSACTION_NUM</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga1a8181950bf393e5b5287dc025dc55bb</anchor>
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
      <type>struct RdmCommandHeader</type>
      <name>RdmCommandHeader</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga8d7171aedb6a09d1731a734709390d5b</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmResponseHeader</type>
      <name>RdmResponseHeader</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga8b9f70c97f84c5d0366c1dc019f012a4</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>rdm_command_class_t</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gafda6b6d4c08788e755cb3cb6c1ca021b</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>rdm_response_type_t</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga6ecebd42ee8d90fe410cc4001a98e7f0</anchor>
      <arglist></arglist>
    </member>
    <member kind="enumeration">
      <type></type>
      <name>rdm_nack_reason_t</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaa11fc36923818f02895f40dafd228474</anchor>
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
    <member kind="function">
      <type>etcpal_error_t</type>
      <name>rdm_create_response</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga2ea6c686aa76d05994796d41e7bcf7d5</anchor>
      <arglist>(const RdmCommandHeader *cmd_header, uint8_t msg_count, const uint8_t *response_data, uint8_t response_data_len, RdmBuffer *buffer)</arglist>
    </member>
    <member kind="function">
      <type>etcpal_error_t</type>
      <name>rdm_create_nack_response</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>ga49f59fd8adcf7c7f396760db14bd1f9f</anchor>
      <arglist>(const RdmCommandHeader *cmd_header, uint8_t msg_count, rdm_nack_reason_t nack_reason, RdmBuffer *buffer)</arglist>
    </member>
    <member kind="function">
      <type>etcpal_error_t</type>
      <name>rdm_create_timer_response</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaffcdf5fc31e74fea4b9eee58541b6aaf</anchor>
      <arglist>(const RdmCommandHeader *cmd_header, uint8_t msg_count, unsigned int delay_time_ms, RdmBuffer *buffer)</arglist>
    </member>
    <member kind="function">
      <type>etcpal_error_t</type>
      <name>rdm_create_dub_response</name>
      <anchorfile>group__message.html</anchorfile>
      <anchor>gaa016ed3a9b2ac887b22cf9ac93b93f7a</anchor>
      <arglist>(const RdmUid *responder_uid, RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>responder</name>
    <title>Responder</title>
    <filename>group__responder.html</filename>
    <member kind="function">
      <type>bool</type>
      <name>rdmresp_is_non_disc_command</name>
      <anchorfile>group__responder.html</anchorfile>
      <anchor>ga4f3587c5330568a286873f8d392d8233</anchor>
      <arglist>(const RdmBuffer *buffer)</arglist>
    </member>
  </compound>
  <compound kind="group">
    <name>uid</name>
    <title>UID</title>
    <filename>group__uid.html</filename>
    <class kind="struct">RdmUid</class>
    <member kind="define">
      <type>#define</type>
      <name>RDM_UID_EQUAL</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gae538a395a7b22c4fcac46fa0f5554b78</anchor>
      <arglist>(uidptr1, uidptr2)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_INIT_STATIC_UID</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga0daf30670c9321b7a3d0d7a69dd6b4e5</anchor>
      <arglist>(uidptr, manu_val, id_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_INIT_DYNAMIC_UID_REQUEST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaad2d26ff7d08e1fcc134c1be979fcdc8</anchor>
      <arglist>(uidptr, manu_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_INIT_DEVICE_MANU_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaa32bb9da2292c2ad817b97919f6a02c5</anchor>
      <arglist>(uidptr, manu_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_UID_IS_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga65209ad8850eef83a6249f93bd215da6</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_CONTROLLER_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga53c88cbe60ef6f037d8c2a9fdff9894a</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_DEVICE_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga6f623f06ac3b14f1becf06ec4faf65f6</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_DEVICE_MANU_BROADCAST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga224755484593f73ac68fe0ad974c192f</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_DEVICE_BROADCAST_MANU_MATCHES</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaeaca39dc0f8d229d9966652e53be634f</anchor>
      <arglist>(uidptr, manu_val)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_DEVICE_BROADCAST_MANU_ID</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaa0bde1cfb88ecf6741ab5b088c79d107</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_UID_IS_NULL</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga1b5ffea9d8b7ae8f0f4cdf5560901254</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_DYNAMIC</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga6513e844d7b58f9cdee40244fe5a682f</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_DYNAMIC_UID_REQUEST</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga7309e3ce684e0d96d6a5ac019d6b8892</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDMNET_UID_IS_STATIC</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaaaf8226917bc3e83e12f8b4d51d11ac8</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_GET_MANUFACTURER_ID</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga0918dda49918646bc3e77c82f4c04221</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_GET_DEVICE_ID</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaa014f2b3e799431e08b951b3edb794f8</anchor>
      <arglist>(uidptr)</arglist>
    </member>
    <member kind="define">
      <type>#define</type>
      <name>RDM_UID_STRING_BYTES</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gaca68c70b4aca047a5b8ff73e9d73b492</anchor>
      <arglist></arglist>
    </member>
    <member kind="typedef">
      <type>struct RdmUid</type>
      <name>RdmUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gad467bfae3ada73c70aaeb29b192ee351</anchor>
      <arglist></arglist>
    </member>
    <member kind="function">
      <type>int</type>
      <name>rdm_uid_compare</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga43e676dfc5272c40015061109ea6ed7f</anchor>
      <arglist>(const RdmUid *a, const RdmUid *b)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>rdm_uid_to_string</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>ga90190789ec0a4c7262c47e2d7a1fdf43</anchor>
      <arglist>(const RdmUid *uid, char *buf)</arglist>
    </member>
    <member kind="function">
      <type>bool</type>
      <name>rdm_string_to_uid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gae4c4bb5385d1982c9b8ed05323a25b2b</anchor>
      <arglist>(const char *str, RdmUid *uid)</arglist>
    </member>
    <member kind="variable">
      <type>const RdmUid</type>
      <name>kRdmBroadcastUid</name>
      <anchorfile>group__uid.html</anchorfile>
      <anchor>gae90bca840cb37b2833f202983f01aa4d</anchor>
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
    <subgroup>rdm_c</subgroup>
    <subgroup>rdm_cpp</subgroup>
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
  <compound kind="group">
    <name>rdm_c</name>
    <title>Core API</title>
    <filename>group__rdm__c.html</filename>
    <subgroup>controller</subgroup>
    <subgroup>message</subgroup>
    <subgroup>responder</subgroup>
    <subgroup>uid</subgroup>
  </compound>
  <compound kind="group">
    <name>rdm_cpp</name>
    <title>C++ API</title>
    <filename>group__rdm__cpp.html</filename>
    <subgroup>cpp_message</subgroup>
    <subgroup>cpp_uid</subgroup>
  </compound>
  <compound kind="page">
    <name>index</name>
    <title>RDM</title>
    <filename>index</filename>
  </compound>
</tagfile>
