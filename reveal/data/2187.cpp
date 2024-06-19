int ff_mpeg4_decode_picture_header ( MpegEncContext * s , GetBitContext * gb ) {
 unsigned startcode , v ;
 align_get_bits ( gb ) ;
 if ( s -> codec_tag == AV_RL32 ( "WV1F" ) && show_bits ( gb , 24 ) == 0x575630 ) {
 skip_bits ( gb , 24 ) ;
 if ( get_bits ( gb , 8 ) == 0xF0 ) goto end ;
 }
 startcode = 0xff ;
 for ( ;
 ;
 ) {
 if ( get_bits_count ( gb ) >= gb -> size_in_bits ) {
 if ( gb -> size_in_bits == 8 && ( s -> divx_version >= 0 || s -> xvid_build >= 0 ) ) {
 av_log ( s -> avctx , AV_LOG_WARNING , "frame skip %d\n" , gb -> size_in_bits ) ;
 return FRAME_SKIPPED ;
 }
 else return - 1 ;
 }
 v = get_bits ( gb , 8 ) ;
 startcode = ( ( startcode << 8 ) | v ) & 0xffffffff ;
 if ( ( startcode & 0xFFFFFF00 ) != 0x100 ) continue ;
 / o startcode if ( s -> avctx -> debug & FF_DEBUG_STARTCODE ) {
 av_log ( s -> avctx , AV_LOG_DEBUG , "startcode: %3X " , startcode ) ;
 if ( startcode <= 0x11F ) av_log ( s -> avctx , AV_LOG_DEBUG , "Video Object Start" ) ;
 else if ( startcode <= 0x12F ) av_log ( s -> avctx , AV_LOG_DEBUG , "Video Object Layer Start" ) ;
 else if ( startcode <= 0x13F ) av_log ( s -> avctx , AV_LOG_DEBUG , "Reserved" ) ;
 else if ( startcode <= 0x15F ) av_log ( s -> avctx , AV_LOG_DEBUG , "FGS bp start" ) ;
 else if ( startcode <= 0x1AF ) av_log ( s -> avctx , AV_LOG_DEBUG , "Reserved" ) ;
 else if ( startcode == 0x1B0 ) av_log ( s -> avctx , AV_LOG_DEBUG , "Visual Object Seq Start" ) ;
 else if ( startcode == 0x1B1 ) av_log ( s -> avctx , AV_LOG_DEBUG , "Visual Object Seq End" ) ;
 else if ( startcode == 0x1B2 ) av_log ( s -> avctx , AV_LOG_DEBUG , "User Data" ) ;
 else if ( startcode == 0x1B3 ) av_log ( s -> avctx , AV_LOG_DEBUG , "Group of VOP start" ) ;
 else if ( startcode == 0x1B4 ) av_log ( s -> avctx , AV_LOG_DEBUG , "Video Session Error" ) ;
 else if ( startcode == 0x1B5 ) av_log ( s -> avctx , AV_LOG_DEBUG , "Visual Object Start" ) ;
 else if ( startcode == 0x1B6 ) av_log ( s -> avctx , AV_LOG_DEBUG , "Video Object Plane start" ) ;
 else if ( startcode == 0x1B7 ) av_log ( s -> avctx , AV_LOG_DEBUG , "slice start" ) ;
 else if ( startcode == 0x1B8 ) av_log ( s -> avctx , AV_LOG_DEBUG , "extension start" ) ;
 else if ( startcode == 0x1B9 ) av_log ( s -> avctx , AV_LOG_DEBUG , "fgs start" ) ;
 else if ( startcode == 0x1BA ) av_log ( s -> avctx , AV_LOG_DEBUG , "FBA Object start" ) ;
 else if ( startcode == 0x1BB ) av_log ( s -> avctx , AV_LOG_DEBUG , "FBA Object Plane start" ) ;
 else if ( startcode == 0x1BC ) av_log ( s -> avctx , AV_LOG_DEBUG , "Mesh Object start" ) ;
 else if ( startcode == 0x1BD ) av_log ( s -> avctx , AV_LOG_DEBUG , "Mesh Object Plane start" ) ;
 else if ( startcode == 0x1BE ) av_log ( s -> avctx , AV_LOG_DEBUG , "Still Texture Object start" ) ;
 else if ( startcode == 0x1BF ) av_log ( s -> avctx , AV_LOG_DEBUG , "Texture Spatial Layer start" ) ;
 else if ( startcode == 0x1C0 ) av_log ( s -> avctx , AV_LOG_DEBUG , "Texture SNR Layer start" ) ;
 else if ( startcode == 0x1C1 ) av_log ( s -> avctx , AV_LOG_DEBUG , "Texture Tile start" ) ;
 else if ( startcode == 0x1C2 ) av_log ( s -> avctx , AV_LOG_DEBUG , "Texture Shape Layer start" ) ;
 else if ( startcode == 0x1C3 ) av_log ( s -> avctx , AV_LOG_DEBUG , "stuffing start" ) ;
 else if ( startcode <= 0x1C5 ) av_log ( s -> avctx , AV_LOG_DEBUG , "reserved" ) ;
 else if ( startcode <= 0x1FF ) av_log ( s -> avctx , AV_LOG_DEBUG , "System start" ) ;
 av_log ( s -> avctx , AV_LOG_DEBUG , " at %d\n" , get_bits_count ( gb ) ) ;
 }
 if ( startcode >= 0x120 && startcode <= 0x12F ) {
 if ( decode_vol_header ( s , gb ) < 0 ) return - 1 ;
 }
 else if ( startcode == USER_DATA_STARTCODE ) {
 decode_user_data ( s , gb ) ;
 }
 else if ( startcode == GOP_STARTCODE ) {
 mpeg4_decode_gop_header ( s , gb ) ;
 }
 else if ( startcode == VOS_STARTCODE ) {
 mpeg4_decode_profile_level ( s , gb ) ;
 }
 else if ( startcode == VOP_STARTCODE ) {
 break ;
 }
 align_get_bits ( gb ) ;
 startcode = 0xff ;
 }
 end : if ( s -> flags & CODEC_FLAG_LOW_DELAY ) s -> low_delay = 1 ;
 s -> avctx -> has_b_frames = ! s -> low_delay ;
 return decode_vop_header ( s , gb ) ;
 }