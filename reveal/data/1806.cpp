static void real_proto_register_diameter ( void ) {
 module_t * diameter_module ;
 expert_module_t * expert_diameter ;
 guint i , ett_length ;
 hf_register_info hf_base [ ] = {
 {
 & hf_diameter_version , {
 "Version" , "diameter.version" , FT_UINT8 , BASE_HEX , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_diameter_length , {
 "Length" , "diameter.length" , FT_UINT24 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_flags , {
 "Flags" , "diameter.flags" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_flags_request , {
 "Request" , "diameter.flags.request" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , DIAM_FLAGS_R , NULL , HFILL }
 }
 , {
 & hf_diameter_flags_proxyable , {
 "Proxyable" , "diameter.flags.proxyable" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , DIAM_FLAGS_P , NULL , HFILL }
 }
 , {
 & hf_diameter_flags_error , {
 "Error" , "diameter.flags.error" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , DIAM_FLAGS_E , NULL , HFILL }
 }
 , {
 & hf_diameter_flags_T , {
 "T(Potentially re-transmitted message)" , "diameter.flags.T" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , DIAM_FLAGS_T , NULL , HFILL }
 }
 , {
 & hf_diameter_flags_reserved4 , {
 "Reserved" , "diameter.flags.reserved4" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , DIAM_FLAGS_RESERVED4 , NULL , HFILL }
 }
 , {
 & hf_diameter_flags_reserved5 , {
 "Reserved" , "diameter.flags.reserved5" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , DIAM_FLAGS_RESERVED5 , NULL , HFILL }
 }
 , {
 & hf_diameter_flags_reserved6 , {
 "Reserved" , "diameter.flags.reserved6" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , DIAM_FLAGS_RESERVED6 , NULL , HFILL }
 }
 , {
 & hf_diameter_flags_reserved7 , {
 "Reserved" , "diameter.flags.reserved7" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , DIAM_FLAGS_RESERVED7 , NULL , HFILL }
 }
 , {
 & hf_diameter_vendor_id , {
 "VendorId" , "diameter.vendorId" , FT_UINT32 , BASE_DEC | BASE_EXT_STRING , & sminmpec_values_ext , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_application_id , {
 "ApplicationId" , "diameter.applicationId" , FT_UINT32 , BASE_DEC | BASE_EXT_STRING , VALS_EXT_PTR ( dictionary . applications ) , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_hopbyhopid , {
 "Hop-by-Hop Identifier" , "diameter.hopbyhopid" , FT_UINT32 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_endtoendid , {
 "End-to-End Identifier" , "diameter.endtoendid" , FT_UINT32 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp , {
 "AVP" , "diameter.avp" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_len , {
 "AVP Length" , "diameter.avp.len" , FT_UINT24 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_code , {
 "AVP Code" , "diameter.avp.code" , FT_UINT32 , BASE_DEC , NULL , 0 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_flags , {
 "AVP Flags" , "diameter.avp.flags" , FT_UINT8 , BASE_HEX , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_flags_vendor_specific , {
 "Vendor-Specific" , "diameter.flags.vendorspecific" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , AVP_FLAGS_V , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_flags_mandatory , {
 "Mandatory" , "diameter.flags.mandatory" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , AVP_FLAGS_M , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_flags_protected , {
 "Protected" , "diameter.avp.flags.protected" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , AVP_FLAGS_P , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_flags_reserved3 , {
 "Reserved" , "diameter.avp.flags.reserved3" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , AVP_FLAGS_RESERVED3 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_flags_reserved4 , {
 "Reserved" , "diameter.avp.flags.reserved4" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , AVP_FLAGS_RESERVED4 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_flags_reserved5 , {
 "Reserved" , "diameter.avp.flags.reserved5" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , AVP_FLAGS_RESERVED5 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_flags_reserved6 , {
 "Reserved" , "diameter.avp.flags.reserved6" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , AVP_FLAGS_RESERVED6 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_flags_reserved7 , {
 "Reserved" , "diameter.avp.flags.reserved7" , FT_BOOLEAN , 8 , TFS ( & tfs_set_notset ) , AVP_FLAGS_RESERVED7 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_vendor_id , {
 "AVP Vendor Id" , "diameter.avp.vendorId" , FT_UINT32 , BASE_DEC | BASE_EXT_STRING , & sminmpec_values_ext , 0x0 , NULL , HFILL }
 }
 , {
 & ( unknown_avp . hf_value ) , {
 "Value" , "diameter.avp.unknown" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_data_wrong_length , {
 "Data" , "diameter.avp.invalid-data" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_avp_pad , {
 "Padding" , "diameter.avp.pad" , FT_BYTES , BASE_NONE , NULL , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_code , {
 "Command Code" , "diameter.cmd.code" , FT_UINT32 , BASE_DEC , NULL , 0 , NULL , HFILL }
 }
 , {
 & hf_diameter_answer_in , {
 "Answer In" , "diameter.answer_in" , FT_FRAMENUM , BASE_NONE , FRAMENUM_TYPE ( FT_FRAMENUM_RESPONSE ) , 0x0 , "The answer to this diameter request is in this frame" , HFILL }
 }
 , {
 & hf_diameter_answer_to , {
 "Request In" , "diameter.answer_to" , FT_FRAMENUM , BASE_NONE , FRAMENUM_TYPE ( FT_FRAMENUM_REQUEST ) , 0x0 , "This is an answer to the diameter request in this frame" , HFILL }
 }
 , {
 & hf_diameter_answer_time , {
 "Response Time" , "diameter.resp_time" , FT_RELATIVE_TIME , BASE_NONE , NULL , 0x0 , "The time between the request and the answer" , HFILL }
 }
 , {
 & hf_framed_ipv6_prefix_reserved , {
 "Framed IPv6 Prefix Reserved byte" , "diameter.framed_ipv6_prefix_reserved" , FT_UINT8 , BASE_HEX , NULL , 0 , NULL , HFILL }
 }
 , {
 & hf_framed_ipv6_prefix_length , {
 "Framed IPv6 Prefix length (in bits)" , "diameter.framed_ipv6_prefix_length" , FT_UINT8 , BASE_DEC , NULL , 0 , NULL , HFILL }
 }
 , {
 & hf_framed_ipv6_prefix_bytes , {
 "Framed IPv6 Prefix as a bytestring" , "diameter.framed_ipv6_prefix_bytes" , FT_BYTES , BASE_NONE , NULL , 0 , NULL , HFILL }
 }
 , {
 & hf_framed_ipv6_prefix_ipv6 , {
 "Framed IPv6 Prefix as an IPv6 address" , "diameter.framed_ipv6_prefix_ipv6" , FT_IPv6 , BASE_NONE , NULL , 0 , "This field is present only if the prefix length is 128" , HFILL }
 }
 , {
 & hf_diameter_3gpp2_exp_res , {
 "Experimental-Result-Code" , "diameter.3gpp2.exp_res" , FT_UINT32 , BASE_DEC , VALS ( diameter_3gpp2_exp_res_vals ) , 0x0 , NULL , HFILL }
 }
 , {
 & hf_diameter_other_vendor_exp_res , {
 "Experimental-Result-Code" , "diameter.other_vendor.Experimental-Result-Code" , FT_UINT32 , BASE_DEC , NULL , 0x0 , NULL , HFILL }
 }
 }
 ;
 gint * ett_base [ ] = {
 & ett_diameter , & ett_diameter_flags , & ett_diameter_avp_flags , & ett_diameter_avpinfo , & ett_unknown , & ett_err , & ( unknown_avp . ett ) }
 ;
 static ei_register_info ei [ ] = {
 {
 & ei_diameter_reserved_bit_set , {
 "diameter.reserved_bit_set" , PI_MALFORMED , PI_WARN , "Reserved bit set" , EXPFILL }
 }
 , {
 & ei_diameter_avp_code , {
 "diameter.avp.code.unknown" , PI_UNDECODED , PI_WARN , "Unknown AVP, if you know what this is you can add it to dictionary.xml" , EXPFILL }
 }
 , {
 & ei_diameter_avp_vendor_id , {
 "diameter.unknown_vendor" , PI_UNDECODED , PI_WARN , "Unknown Vendor, if you know whose this is you can add it to dictionary.xml" , EXPFILL }
 }
 , {
 & ei_diameter_avp_no_data , {
 "diameter.avp.no_data" , PI_UNDECODED , PI_WARN , "Data is empty" , EXPFILL }
 }
 , {
 & ei_diameter_avp_pad , {
 "diameter.avp.pad.non_zero" , PI_MALFORMED , PI_NOTE , "Padding is non-zero" , EXPFILL }
 }
 , {
 & ei_diameter_avp_len , {
 "diameter.avp.invalid-len" , PI_MALFORMED , PI_WARN , "Wrong length" , EXPFILL }
 }
 , {
 & ei_diameter_application_id , {
 "diameter.applicationId.unknown" , PI_UNDECODED , PI_WARN , "Unknown Application Id, if you know what this is you can add it to dictionary.xml" , EXPFILL }
 }
 , {
 & ei_diameter_version , {
 "diameter.version.unknown" , PI_UNDECODED , PI_WARN , "Unknown Diameter Version (decoding as RFC 3588)" , EXPFILL }
 }
 , {
 & ei_diameter_code , {
 "diameter.cmd.code.unknown" , PI_UNDECODED , PI_WARN , "Unknown command, if you know what this is you can add it to dictionary.xml" , EXPFILL }
 }
 , }
 ;
 wmem_array_append ( build_dict . hf , hf_base , array_length ( hf_base ) ) ;
 ett_length = array_length ( ett_base ) ;
 for ( i = 0 ;
 i < ett_length ;
 i ++ ) {
 g_ptr_array_add ( build_dict . ett , ett_base [ i ] ) ;
 }
 proto_diameter = proto_register_protocol ( "Diameter Protocol" , "DIAMETER" , "diameter" ) ;
 proto_register_field_array ( proto_diameter , ( hf_register_info * ) wmem_array_get_raw ( build_dict . hf ) , wmem_array_get_count ( build_dict . hf ) ) ;
 proto_register_subtree_array ( ( gint * * ) build_dict . ett -> pdata , build_dict . ett -> len ) ;
 expert_diameter = expert_register_protocol ( proto_diameter ) ;
 expert_register_field_array ( expert_diameter , ei , array_length ( ei ) ) ;
 g_ptr_array_free ( build_dict . ett , TRUE ) ;
 new_register_dissector ( "diameter" , dissect_diameter , proto_diameter ) ;
 diameter_dissector_table = register_dissector_table ( "diameter.base" , "DIAMETER_BASE_AVPS" , FT_UINT32 , BASE_DEC , DISSECTOR_TABLE_ALLOW_DUPLICATE ) ;
 diameter_3gpp_avp_dissector_table = register_dissector_table ( "diameter.3gpp" , "DIAMETER_3GPP_AVPS" , FT_UINT32 , BASE_DEC , DISSECTOR_TABLE_ALLOW_DUPLICATE ) ;
 diameter_ericsson_avp_dissector_table = register_dissector_table ( "diameter.ericsson" , "DIAMETER_ERICSSON_AVPS" , FT_UINT32 , BASE_DEC , DISSECTOR_TABLE_ALLOW_DUPLICATE ) ;
 diameter_expr_result_vnd_table = register_dissector_table ( "diameter.vnd_exp_res" , "DIAMETER Experimental-Result-Code" , FT_UINT32 , BASE_DEC , DISSECTOR_TABLE_ALLOW_DUPLICATE ) ;
 range_convert_str ( & global_diameter_tcp_port_range , DEFAULT_DIAMETER_PORT_RANGE , MAX_UDP_PORT ) ;
 range_convert_str ( & global_diameter_sctp_port_range , DEFAULT_DIAMETER_PORT_RANGE , MAX_SCTP_PORT ) ;
 range_convert_str ( & global_diameter_udp_port_range , "" , MAX_UDP_PORT ) ;
 diameter_module = prefs_register_protocol ( proto_diameter , proto_reg_handoff_diameter ) ;
 prefs_register_range_preference ( diameter_module , "tcp.ports" , "Diameter TCP ports" , "TCP ports to be decoded as Diameter (default: " DEFAULT_DIAMETER_PORT_RANGE ")" , & global_diameter_tcp_port_range , MAX_UDP_PORT ) ;
 prefs_register_range_preference ( diameter_module , "sctp.ports" , "Diameter SCTP Ports" , "SCTP ports to be decoded as Diameter (default: " DEFAULT_DIAMETER_PORT_RANGE ")" , & global_diameter_sctp_port_range , MAX_SCTP_PORT ) ;
 prefs_register_bool_preference ( diameter_module , "desegment" , "Reassemble Diameter messages\nspanning multiple TCP segments" , "Whether the Diameter dissector should reassemble messages spanning multiple TCP segments." " To use this option, you must also enable \"Allow subdissectors to reassemble TCP streams\" in the TCP protocol settings." , & gbl_diameter_desegment ) ;
 prefs_register_range_preference ( diameter_module , "udp.ports" , "Diameter UDP ports" , "UDP ports to be decoded as Diameter (default: 0 as Diameter over UDP is nonstandard)" , & global_diameter_udp_port_range , MAX_UDP_PORT ) ;
 prefs_register_obsolete_preference ( diameter_module , "version" ) ;
 prefs_register_obsolete_preference ( diameter_module , "tcp.port" ) ;
 prefs_register_obsolete_preference ( diameter_module , "sctp.port" ) ;
 prefs_register_obsolete_preference ( diameter_module , "command_in_header" ) ;
 prefs_register_obsolete_preference ( diameter_module , "dictionary.name" ) ;
 prefs_register_obsolete_preference ( diameter_module , "dictionary.use" ) ;
 prefs_register_obsolete_preference ( diameter_module , "allow_zero_as_app_id" ) ;
 prefs_register_obsolete_preference ( diameter_module , "suppress_console_output" ) ;
 diameter_tap = register_tap ( "diameter" ) ;
 register_srt_table ( proto_diameter , NULL , 1 , diameterstat_packet , diameterstat_init , NULL ) ;
 }