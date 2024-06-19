static int vorbis_parse_audio_packet ( vorbis_context * vc , float * * floor_ptr ) {
 GetBitContext * gb = & vc -> gb ;
 FFTContext * mdct ;
 unsigned previous_window = vc -> previous_window ;
 unsigned mode_number , blockflag , blocksize ;
 int i , j ;
 uint8_t no_residue [ 255 ] ;
 uint8_t do_not_decode [ 255 ] ;
 vorbis_mapping * mapping ;
 float * ch_res_ptr = vc -> channel_residues ;
 uint8_t res_chan [ 255 ] ;
 unsigned res_num = 0 ;
 int retlen = 0 ;
 unsigned ch_left = vc -> audio_channels ;
 unsigned vlen ;
 if ( get_bits1 ( gb ) ) {
 av_log ( vc -> avctx , AV_LOG_ERROR , "Not a Vorbis I audio packet.\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( vc -> mode_count == 1 ) {
 mode_number = 0 ;
 }
 else {
 GET_VALIDATED_INDEX ( mode_number , ilog ( vc -> mode_count - 1 ) , vc -> mode_count ) }
 vc -> mode_number = mode_number ;
 mapping = & vc -> mappings [ vc -> modes [ mode_number ] . mapping ] ;
 av_dlog ( NULL , " Mode number: %u , mapping: %d , blocktype %d\n" , mode_number , vc -> modes [ mode_number ] . mapping , vc -> modes [ mode_number ] . blockflag ) ;
 blockflag = vc -> modes [ mode_number ] . blockflag ;
 blocksize = vc -> blocksize [ blockflag ] ;
 vlen = blocksize / 2 ;
 if ( blockflag ) {
 previous_window = get_bits ( gb , 1 ) ;
 skip_bits1 ( gb ) ;
 }
 memset ( ch_res_ptr , 0 , sizeof ( float ) * vc -> audio_channels * vlen ) ;
 for ( i = 0 ;
 i < vc -> audio_channels ;
 ++ i ) memset ( floor_ptr [ i ] , 0 , vlen * sizeof ( floor_ptr [ 0 ] [ 0 ] ) ) ;
 for ( i = 0 ;
 i < vc -> audio_channels ;
 ++ i ) {
 vorbis_floor * floor ;
 int ret ;
 if ( mapping -> submaps > 1 ) {
 floor = & vc -> floors [ mapping -> submap_floor [ mapping -> mux [ i ] ] ] ;
 }
 else {
 floor = & vc -> floors [ mapping -> submap_floor [ 0 ] ] ;
 }
 ret = floor -> decode ( vc , & floor -> data , floor_ptr [ i ] ) ;
 if ( ret < 0 ) {
 av_log ( vc -> avctx , AV_LOG_ERROR , "Invalid codebook in vorbis_floor_decode.\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 no_residue [ i ] = ret ;
 }
 for ( i = mapping -> coupling_steps - 1 ;
 i >= 0 ;
 -- i ) {
 if ( ! ( no_residue [ mapping -> magnitude [ i ] ] & no_residue [ mapping -> angle [ i ] ] ) ) {
 no_residue [ mapping -> magnitude [ i ] ] = 0 ;
 no_residue [ mapping -> angle [ i ] ] = 0 ;
 }
 }
 for ( i = 0 ;
 i < mapping -> submaps ;
 ++ i ) {
 vorbis_residue * residue ;
 unsigned ch = 0 ;
 int ret ;
 for ( j = 0 ;
 j < vc -> audio_channels ;
 ++ j ) {
 if ( ( mapping -> submaps == 1 ) || ( i == mapping -> mux [ j ] ) ) {
 res_chan [ j ] = res_num ;
 if ( no_residue [ j ] ) {
 do_not_decode [ ch ] = 1 ;
 }
 else {
 do_not_decode [ ch ] = 0 ;
 }
 ++ ch ;
 ++ res_num ;
 }
 }
 residue = & vc -> residues [ mapping -> submap_residue [ i ] ] ;
 if ( ch_left < ch ) {
 av_log ( vc -> avctx , AV_LOG_ERROR , "Too many channels in vorbis_floor_decode.\n" ) ;
 return - 1 ;
 }
 if ( ch ) {
 ret = vorbis_residue_decode ( vc , residue , ch , do_not_decode , ch_res_ptr , vlen , ch_left ) ;
 if ( ret < 0 ) return ret ;
 }
 ch_res_ptr += ch * vlen ;
 ch_left -= ch ;
 }
 if ( ch_left > 0 ) return AVERROR_INVALIDDATA ;
 for ( i = mapping -> coupling_steps - 1 ;
 i >= 0 ;
 -- i ) {
 float * mag , * ang ;
 mag = vc -> channel_residues + res_chan [ mapping -> magnitude [ i ] ] * blocksize / 2 ;
 ang = vc -> channel_residues + res_chan [ mapping -> angle [ i ] ] * blocksize / 2 ;
 vc -> dsp . vorbis_inverse_coupling ( mag , ang , blocksize / 2 ) ;
 }
 mdct = & vc -> mdct [ blockflag ] ;
 for ( j = vc -> audio_channels - 1 ;
 j >= 0 ;
 j -- ) {
 ch_res_ptr = vc -> channel_residues + res_chan [ j ] * blocksize / 2 ;
 vc -> fdsp . vector_fmul ( floor_ptr [ j ] , floor_ptr [ j ] , ch_res_ptr , blocksize / 2 ) ;
 mdct -> imdct_half ( mdct , ch_res_ptr , floor_ptr [ j ] ) ;
 }
 retlen = ( blocksize + vc -> blocksize [ previous_window ] ) / 4 ;
 for ( j = 0 ;
 j < vc -> audio_channels ;
 j ++ ) {
 unsigned bs0 = vc -> blocksize [ 0 ] ;
 unsigned bs1 = vc -> blocksize [ 1 ] ;
 float * residue = vc -> channel_residues + res_chan [ j ] * blocksize / 2 ;
 float * saved = vc -> saved + j * bs1 / 4 ;
 float * ret = floor_ptr [ j ] ;
 float * buf = residue ;
 const float * win = vc -> win [ blockflag & previous_window ] ;
 if ( blockflag == previous_window ) {
 vc -> fdsp . vector_fmul_window ( ret , saved , buf , win , blocksize / 4 ) ;
 }
 else if ( blockflag > previous_window ) {
 vc -> fdsp . vector_fmul_window ( ret , saved , buf , win , bs0 / 4 ) ;
 memcpy ( ret + bs0 / 2 , buf + bs0 / 4 , ( ( bs1 - bs0 ) / 4 ) * sizeof ( float ) ) ;
 }
 else {
 memcpy ( ret , saved , ( ( bs1 - bs0 ) / 4 ) * sizeof ( float ) ) ;
 vc -> fdsp . vector_fmul_window ( ret + ( bs1 - bs0 ) / 4 , saved + ( bs1 - bs0 ) / 4 , buf , win , bs0 / 4 ) ;
 }
 memcpy ( saved , buf + blocksize / 4 , blocksize / 4 * sizeof ( float ) ) ;
 }
 vc -> previous_window = blockflag ;
 return retlen ;
 }