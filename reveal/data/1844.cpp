static int rtp_packetize_mp4a_latm ( sout_stream_id_sys_t * id , block_t * in ) {
 int i_max = rtp_mtu ( id ) - 2 ;
 int latmhdrsize = in -> i_buffer / 0xff + 1 ;
 int i_count = ( in -> i_buffer + i_max - 1 ) / i_max ;
 uint8_t * p_data = in -> p_buffer , * p_header = NULL ;
 int i_data = in -> i_buffer ;
 int i ;
 for ( i = 0 ;
 i < i_count ;
 i ++ ) {
 int i_payload = __MIN ( i_max , i_data ) ;
 block_t * out ;
 if ( i != 0 ) latmhdrsize = 0 ;
 out = block_Alloc ( 12 + latmhdrsize + i_payload ) ;
 rtp_packetize_common ( id , out , ( ( i == i_count - 1 ) ? 1 : 0 ) , ( in -> i_pts > VLC_TS_INVALID ? in -> i_pts : in -> i_dts ) ) ;
 if ( i == 0 ) {
 int tmp = in -> i_buffer ;
 p_header = out -> p_buffer + 12 ;
 while ( tmp > 0xfe ) {
 * p_header = 0xff ;
 p_header ++ ;
 tmp -= 0xff ;
 }
 * p_header = tmp ;
 }
 memcpy ( & out -> p_buffer [ 12 + latmhdrsize ] , p_data , i_payload ) ;
 out -> i_dts = in -> i_dts + i * in -> i_length / i_count ;
 out -> i_length = in -> i_length / i_count ;
 rtp_packetize_send ( id , out ) ;
 p_data += i_payload ;
 i_data -= i_payload ;
 }
 block_Release ( in ) ;
 return VLC_SUCCESS ;
 }