static void Pass2Encode ( VP8_COMP * cpi , unsigned long * size , unsigned char * dest , unsigned char * dest_end , unsigned int * frame_flags ) {
 if ( ! cpi -> common . refresh_alt_ref_frame ) vp8_second_pass ( cpi ) ;
 encode_frame_to_data_rate ( cpi , size , dest , dest_end , frame_flags ) ;
 cpi -> twopass . bits_left -= 8 * * size ;
 if ( ! cpi -> common . refresh_alt_ref_frame ) {
 double two_pass_min_rate = ( double ) ( cpi -> oxcf . target_bandwidth * cpi -> oxcf . two_pass_vbrmin_section / 100 ) ;
 cpi -> twopass . bits_left += ( int64_t ) ( two_pass_min_rate / cpi -> framerate ) ;
 }
 }