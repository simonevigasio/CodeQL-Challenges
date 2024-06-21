static int vp3_decode_frame ( AVCodecContext * avctx , void * data , int * got_frame , AVPacket * avpkt ) {
 const uint8_t * buf = avpkt -> data ;
 int buf_size = avpkt -> size ;
 Vp3DecodeContext * s = avctx -> priv_data ;
 GetBitContext gb ;
 int i ;
 init_get_bits ( & gb , buf , buf_size * 8 ) ;
 if ( s -> theora && get_bits1 ( & gb ) ) {
 av_log ( avctx , AV_LOG_ERROR , "Header packet passed to frame decoder, skipping\n" ) ;
 return - 1 ;
 }
 s -> keyframe = ! get_bits1 ( & gb ) ;
 if ( ! s -> theora ) skip_bits ( & gb , 1 ) ;
 for ( i = 0 ;
 i < 3 ;
 i ++ ) s -> last_qps [ i ] = s -> qps [ i ] ;
 s -> nqps = 0 ;
 do {
 s -> qps [ s -> nqps ++ ] = get_bits ( & gb , 6 ) ;
 }
 while ( s -> theora >= 0x030200 && s -> nqps < 3 && get_bits1 ( & gb ) ) ;
 for ( i = s -> nqps ;
 i < 3 ;
 i ++ ) s -> qps [ i ] = - 1 ;
 if ( s -> avctx -> debug & FF_DEBUG_PICT_INFO ) av_log ( s -> avctx , AV_LOG_INFO , " VP3 %sframe #%d: Q index = %d\n" , s -> keyframe ? "key" : "" , avctx -> frame_number + 1 , s -> qps [ 0 ] ) ;
 s -> skip_loop_filter = ! s -> filter_limit_values [ s -> qps [ 0 ] ] || avctx -> skip_loop_filter >= ( s -> keyframe ? AVDISCARD_ALL : AVDISCARD_NONKEY ) ;
 if ( s -> qps [ 0 ] != s -> last_qps [ 0 ] ) init_loop_filter ( s ) ;
 for ( i = 0 ;
 i < s -> nqps ;
 i ++ ) if ( s -> qps [ i ] != s -> last_qps [ i ] || s -> qps [ 0 ] != s -> last_qps [ 0 ] ) init_dequantizer ( s , i ) ;
 if ( avctx -> skip_frame >= AVDISCARD_NONKEY && ! s -> keyframe ) return buf_size ;
 s -> current_frame . reference = 3 ;
 s -> current_frame . pict_type = s -> keyframe ? AV_PICTURE_TYPE_I : AV_PICTURE_TYPE_P ;
 if ( ff_thread_get_buffer ( avctx , & s -> current_frame ) < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 goto error ;
 }
 if ( ! s -> edge_emu_buffer ) s -> edge_emu_buffer = av_malloc ( 9 * FFABS ( s -> current_frame . linesize [ 0 ] ) ) ;
 if ( s -> keyframe ) {
 if ( ! s -> theora ) {
 skip_bits ( & gb , 4 ) ;
 skip_bits ( & gb , 4 ) ;
 if ( s -> version ) {
 s -> version = get_bits ( & gb , 5 ) ;
 if ( avctx -> frame_number == 0 ) av_log ( s -> avctx , AV_LOG_DEBUG , "VP version: %d\n" , s -> version ) ;
 }
 }
 if ( s -> version || s -> theora ) {
 if ( get_bits1 ( & gb ) ) av_log ( s -> avctx , AV_LOG_ERROR , "Warning, unsupported keyframe coding type?!\n" ) ;
 skip_bits ( & gb , 2 ) ;
 }
 }
 else {
 if ( ! s -> golden_frame . data [ 0 ] ) {
 av_log ( s -> avctx , AV_LOG_WARNING , "vp3: first frame not a keyframe\n" ) ;
 s -> golden_frame . reference = 3 ;
 s -> golden_frame . pict_type = AV_PICTURE_TYPE_I ;
 if ( ff_thread_get_buffer ( avctx , & s -> golden_frame ) < 0 ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "get_buffer() failed\n" ) ;
 goto error ;
 }
 s -> last_frame = s -> golden_frame ;
 s -> last_frame . type = FF_BUFFER_TYPE_COPY ;
 ff_thread_report_progress ( & s -> last_frame , INT_MAX , 0 ) ;
 }
 }
 memset ( s -> all_fragments , 0 , s -> fragment_count * sizeof ( Vp3Fragment ) ) ;
 ff_thread_finish_setup ( avctx ) ;
 if ( unpack_superblocks ( s , & gb ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "error in unpack_superblocks\n" ) ;
 goto error ;
 }
 if ( unpack_modes ( s , & gb ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "error in unpack_modes\n" ) ;
 goto error ;
 }
 if ( unpack_vectors ( s , & gb ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "error in unpack_vectors\n" ) ;
 goto error ;
 }
 if ( unpack_block_qpis ( s , & gb ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "error in unpack_block_qpis\n" ) ;
 goto error ;
 }
 if ( unpack_dct_coeffs ( s , & gb ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "error in unpack_dct_coeffs\n" ) ;
 goto error ;
 }
 for ( i = 0 ;
 i < 3 ;
 i ++ ) {
 int height = s -> height >> ( i && s -> chroma_y_shift ) ;
 if ( s -> flipped_image ) s -> data_offset [ i ] = 0 ;
 else s -> data_offset [ i ] = ( height - 1 ) * s -> current_frame . linesize [ i ] ;
 }
 s -> last_slice_end = 0 ;
 for ( i = 0 ;
 i < s -> c_superblock_height ;
 i ++ ) render_slice ( s , i ) ;
 for ( i = 0 ;
 i < 3 ;
 i ++ ) {
 int row = ( s -> height >> ( 3 + ( i && s -> chroma_y_shift ) ) ) - 1 ;
 apply_loop_filter ( s , i , row , row + 1 ) ;
 }
 vp3_draw_horiz_band ( s , s -> avctx -> height ) ;
 * got_frame = 1 ;
 * ( AVFrame * ) data = s -> current_frame ;
 if ( ! HAVE_THREADS || ! ( s -> avctx -> active_thread_type & FF_THREAD_FRAME ) ) update_frames ( avctx ) ;
 return buf_size ;
 error : ff_thread_report_progress ( & s -> current_frame , INT_MAX , 0 ) ;
 if ( ! HAVE_THREADS || ! ( s -> avctx -> active_thread_type & FF_THREAD_FRAME ) ) avctx -> release_buffer ( avctx , & s -> current_frame ) ;
 return - 1 ;
 }