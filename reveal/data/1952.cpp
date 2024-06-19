void proto_register_usb_audio ( void ) {
 static hf_register_info hf [ ] = {
 {
 & hf_midi_cable_number , {
 "Cable Number" , "usbaudio.midi.cable_number" , FT_UINT8 , BASE_HEX , NULL , 0xF0 , NULL , HFILL }
 }
 , {
 & hf_midi_code_index , {
 "Code Index" , "usbaudio.midi.code_index" , FT_UINT8 , BASE_HEX , VALS ( code_index_vals ) , 0x0F , NULL , HFILL }
 }
 , {
 & hf_midi_event , {
 "MIDI Event" , "usbaudio.midi.event" , FT_UINT24 , BASE_HEX , NULL , 0 , NULL , HFILL }
 }
 , {
 & hf_ac_if_desc_subtype , {
 "Subtype" , "usbaudio.ac_if_subtype" , FT_UINT8 , BASE_HEX | BASE_EXT_STRING , & ac_subtype_vals_ext , 0x00 , "bDescriptorSubtype" , HFILL }
 }
 , {
 & hf_ac_if_hdr_ver , {
 "Version" , "usbaudio.ac_if_hdr.bcdADC" , FT_DOUBLE , BASE_NONE , NULL , 0 , "bcdADC" , HFILL }
 }
 , {
 & hf_ac_if_hdr_total_len , {
 "Total length" , "usbaudio.ac_if_hdr.wTotalLength" , FT_UINT16 , BASE_DEC , NULL , 0x00 , "wTotalLength" , HFILL }
 }
 , {
 & hf_ac_if_hdr_bInCollection , {
 "Total number of interfaces" , "usbaudio.ac_if_hdr.bInCollection" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bInCollection" , HFILL }
 }
 , {
 & hf_ac_if_hdr_if_num , {
 "Interface number" , "usbaudio.ac_if_hdr.baInterfaceNr" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "baInterfaceNr" , HFILL }
 }
 , {
 & hf_ac_if_input_terminalid , {
 "Terminal ID" , "usbaudio.ac_if_input.bTerminalID" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bTerminalID" , HFILL }
 }
 , {
 & hf_ac_if_input_terminaltype , {
 "Terminal Type" , "usbaudio.ac_if_input.wTerminalType" , FT_UINT16 , BASE_HEX | BASE_EXT_STRING , & terminal_types_vals_ext , 0x00 , "wTerminalType" , HFILL }
 }
 , {
 & hf_ac_if_input_assocterminal , {
 "Assoc Terminal" , "usbaudio.ac_if_input.bAssocTerminal" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bAssocTerminal" , HFILL }
 }
 , {
 & hf_ac_if_input_nrchannels , {
 "Number Channels" , "usbaudio.ac_if_input.bNrChannels" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bNrChannels" , HFILL }
 }
 , {
 & hf_ac_if_input_channelconfig , {
 "Channel Config" , "usbaudio.ac_if_input.wChannelConfig" , FT_UINT16 , BASE_HEX , NULL , 0x00 , "wChannelConfig" , HFILL }
 }
 , {
 & hf_ac_if_input_channelnames , {
 "Channel Names" , "usbaudio.ac_if_input.iChannelNames" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "iChannelNames" , HFILL }
 }
 , {
 & hf_ac_if_input_terminal , {
 "Terminal" , "usbaudio.ac_if_input.iTerminal" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "iTerminal" , HFILL }
 }
 , {
 & hf_ac_if_output_terminalid , {
 "Terminal ID" , "usbaudio.ac_if_output.bTerminalID" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bTerminalID" , HFILL }
 }
 , {
 & hf_ac_if_output_terminaltype , {
 "Terminal Type" , "usbaudio.ac_if_output.wTerminalType" , FT_UINT16 , BASE_HEX | BASE_EXT_STRING , & terminal_types_vals_ext , 0x00 , "wTerminalType" , HFILL }
 }
 , {
 & hf_ac_if_output_assocterminal , {
 "Assoc Terminal" , "usbaudio.ac_if_output.bAssocTerminal" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bAssocTerminal" , HFILL }
 }
 , {
 & hf_ac_if_output_sourceid , {
 "Source ID" , "usbaudio.ac_if_output.bSourceID" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bSourceID" , HFILL }
 }
 , {
 & hf_ac_if_output_terminal , {
 "Terminal" , "usbaudio.ac_if_output.iTerminal" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "iTerminal" , HFILL }
 }
 , {
 & hf_ac_if_fu_unitid , {
 "Unit ID" , "usbaudio.ac_if_fu.bUnitID" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bUnitID" , HFILL }
 }
 , {
 & hf_ac_if_fu_sourceid , {
 "Source ID" , "usbaudio.ac_if_fu.bSourceID" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bSourceID" , HFILL }
 }
 , {
 & hf_ac_if_fu_controlsize , {
 "Control Size" , "usbaudio.ac_if_fu.bControlSize" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bControlSize" , HFILL }
 }
 , {
 & hf_ac_if_fu_controls , {
 "Controls" , "usbaudio.ac_if_fu.bmaControls" , FT_BYTES , BASE_NONE , NULL , 0x00 , "bmaControls" , HFILL }
 }
 , {
 & hf_ac_if_fu_control , {
 "Control" , "usbaudio.ac_if_fu.bmaControl" , FT_UINT8 , BASE_HEX , NULL , 0x00 , "bmaControls" , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d0 , {
 "Mute" , "usbaudio.ac_if_fu.bmaControls.d0" , FT_BOOLEAN , 8 , NULL , 0x01 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d1 , {
 "Volume" , "usbaudio.ac_if_fu.bmaControls.d1" , FT_BOOLEAN , 8 , NULL , 0x02 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d2 , {
 "Bass" , "usbaudio.ac_if_fu.bmaControls.d2" , FT_BOOLEAN , 8 , NULL , 0x04 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d3 , {
 "Mid" , "usbaudio.ac_if_fu.bmaControls.d3" , FT_BOOLEAN , 8 , NULL , 0x08 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d4 , {
 "Treble" , "usbaudio.ac_if_fu.bmaControls.d4" , FT_BOOLEAN , 8 , NULL , 0x10 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d5 , {
 "Graphic Equalizer" , "usbaudio.ac_if_fu.bmaControls.d5" , FT_BOOLEAN , 8 , NULL , 0x20 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d6 , {
 "Automatic Gain" , "usbaudio.ac_if_fu.bmaControls.d6" , FT_BOOLEAN , 8 , NULL , 0x40 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d7 , {
 "Delay" , "usbaudio.ac_if_fu.bmaControls.d7" , FT_BOOLEAN , 8 , NULL , 0x80 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d8 , {
 "Bass Boost" , "usbaudio.ac_if_fu.bmaControls.d8" , FT_BOOLEAN , 8 , NULL , 0x01 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_d9 , {
 "Loudness" , "usbaudio.ac_if_fu.bmaControls.d9" , FT_BOOLEAN , 8 , NULL , 0x02 , NULL , HFILL }
 }
 , {
 & hf_ac_if_fu_controls_rsv , {
 "Reserved" , "usbaudio.ac_if_fu.bmaControls.rsv" , FT_UINT8 , BASE_HEX , NULL , 0xFC , "Must be zero" , HFILL }
 }
 , {
 & hf_ac_if_fu_ifeature , {
 "Feature" , "usbaudio.ac_if_fu.iFeature" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "iFeature" , HFILL }
 }
 , {
 & hf_as_if_desc_subtype , {
 "Subtype" , "usbaudio.as_if_subtype" , FT_UINT8 , BASE_HEX | BASE_EXT_STRING , & as_subtype_vals_ext , 0x00 , "bDescriptorSubtype" , HFILL }
 }
 , {
 & hf_as_if_gen_term_id , {
 "Terminal ID" , "usbaudio.as_if_gen.bTerminalLink" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bTerminalLink" , HFILL }
 }
 , {
 & hf_as_if_gen_delay , {
 "Interface delay in frames" , "usbaudio.as_if_gen.bDelay" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bDelay" , HFILL }
 }
 , {
 & hf_as_if_gen_format , {
 "Format" , "usbaudio.as_if_gen.wFormatTag" , FT_UINT16 , BASE_HEX , NULL , 0x00 , "wFormatTag" , HFILL }
 }
 , {
 & hf_as_if_ft_formattype , {
 "FormatType" , "usbaudio.as_if_ft.bFormatType" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "wFormatType" , HFILL }
 }
 , {
 & hf_as_if_ft_maxbitrate , {
 "Max Bit Rate" , "usbaudio.as_if_ft.wMaxBitRate" , FT_UINT16 , BASE_DEC , NULL , 0x00 , "wMaxBitRate" , HFILL }
 }
 , {
 & hf_as_if_ft_nrchannels , {
 "Number Channels" , "usbaudio.as_if_ft.bNrChannels" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bNrChannels" , HFILL }
 }
 , {
 & hf_as_if_ft_subframesize , {
 "Subframe Size" , "usbaudio.as_if_ft.bSubframeSize" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bSubframeSize" , HFILL }
 }
 , {
 & hf_as_if_ft_bitresolution , {
 "Bit Resolution" , "usbaudio.as_if_ft.bBitResolution" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bBitResolution" , HFILL }
 }
 , {
 & hf_as_if_ft_samplesperframe , {
 "Samples Per Frame" , "usbaudio.as_if_ft.wSamplesPerFrame" , FT_UINT16 , BASE_DEC , NULL , 0x00 , "wSamplesPerFrame" , HFILL }
 }
 , {
 & hf_as_if_ft_samfreqtype , {
 "Samples Frequence Type" , "usbaudio.as_if_ft.bSamFreqType" , FT_UINT8 , BASE_DEC , NULL , 0x00 , "bSamFreqType" , HFILL }
 }
 , {
 & hf_as_if_ft_lowersamfreq , {
 "Lower Samples Frequence" , "usbaudio.as_if_ft.tLowerSamFreq" , FT_UINT24 , BASE_DEC , NULL , 0x00 , "tLowerSamFreq" , HFILL }
 }
 , {
 & hf_as_if_ft_uppersamfreq , {
 "Upper Samples Frequence" , "usbaudio.as_if_ft.tUpperSamFreq" , FT_UINT24 , BASE_DEC , NULL , 0x00 , "tUpperSamFreq" , HFILL }
 }
 , {
 & hf_as_if_ft_samfreq , {
 "Samples Frequence" , "usbaudio.as_if_ft.tSamFreq" , FT_UINT24 , BASE_DEC , NULL , 0x00 , "tSamFreq" , HFILL }
 }
 , {
 & hf_as_ep_desc_subtype , {
 "Subtype" , "usbaudio.as_ep_subtype" , FT_UINT8 , BASE_HEX , NULL , 0x00 , "bDescriptorSubtype" , HFILL }
 }
 , {
 & hf_sysex_msg_fragments , {
 "Message fragments" , "usbaudio.sysex.fragments" , FT_NONE , BASE_NONE , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_fragment , {
 "Message fragment" , "usbaudio.sysex.fragment" , FT_FRAMENUM , BASE_NONE , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_fragment_overlap , {
 "Message fragment overlap" , "usbaudio.sysex.fragment.overlap" , FT_BOOLEAN , 0 , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_fragment_overlap_conflicts , {
 "Message fragment overlapping with conflicting data" , "usbaudio.sysex.fragment.overlap.conflicts" , FT_BOOLEAN , 0 , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_fragment_multiple_tails , {
 "Message has multiple tail fragments" , "usbaudio.sysex.fragment.multiple_tails" , FT_BOOLEAN , 0 , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_fragment_too_long_fragment , {
 "Message fragment too long" , "usbaudio.sysex.fragment.too_long_fragment" , FT_BOOLEAN , 0 , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_fragment_error , {
 "Message defragmentation error" , "usbaudio.sysex.fragment.error" , FT_FRAMENUM , BASE_NONE , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_fragment_count , {
 "Message fragment count" , "usbaudio.sysex.fragment.count" , FT_UINT32 , BASE_DEC , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_reassembled_in , {
 "Reassembled in" , "usbaudio.sysex.reassembled.in" , FT_FRAMENUM , BASE_NONE , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_reassembled_length , {
 "Reassembled length" , "usbaudio.sysex.reassembled.length" , FT_UINT32 , BASE_DEC , NULL , 0x00 , NULL , HFILL }
 }
 , {
 & hf_sysex_msg_reassembled_data , {
 "Reassembled data" , "usbaudio.sysex.reassembled.data" , FT_BYTES , BASE_NONE , NULL , 0x00 , NULL , HFILL }
 }
 }
 ;
 static gint * usb_audio_subtrees [ ] = {
 & ett_usb_audio , & ett_usb_audio_desc , & ett_sysex_msg_fragment , & ett_sysex_msg_fragments , & ett_ac_if_fu_controls , & ett_ac_if_fu_controls0 , & ett_ac_if_fu_controls1 }
 ;
 static ei_register_info ei [ ] = {
 {
 & ei_usb_audio_undecoded , {
 "usbaudio.undecoded" , PI_UNDECODED , PI_WARN , "Not dissected yet (report to wireshark.org)" , EXPFILL }
 }
 , }
 ;
 expert_module_t * expert_usb_audio ;
 proto_usb_audio = proto_register_protocol ( "USB Audio" , "USBAUDIO" , "usbaudio" ) ;
 proto_register_field_array ( proto_usb_audio , hf , array_length ( hf ) ) ;
 proto_register_subtree_array ( usb_audio_subtrees , array_length ( usb_audio_subtrees ) ) ;
 expert_usb_audio = expert_register_protocol ( proto_usb_audio ) ;
 expert_register_field_array ( expert_usb_audio , ei , array_length ( ei ) ) ;
 register_init_routine ( & midi_data_reassemble_init ) ;
 register_cleanup_routine ( & midi_data_reassemble_cleanup ) ;
 register_dissector ( "usbaudio" , dissect_usb_audio_bulk , proto_usb_audio ) ;
 }