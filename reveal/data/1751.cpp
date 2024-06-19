static int rc_pick_q_and_bounds_one_pass_vbr ( const VP9_COMP * cpi , int * bottom_index , int * top_index ) {
 const VP9_COMMON * const cm = & cpi -> common ;
 const RATE_CONTROL * const rc = & cpi -> rc ;
 const VP9EncoderConfig * const oxcf = & cpi -> oxcf ;
 const int cq_level = get_active_cq_level ( rc , oxcf ) ;
 int active_best_quality ;
 int active_worst_quality = calc_active_worst_quality_one_pass_vbr ( cpi ) ;
 int q ;
 int * inter_minq ;
 ASSIGN_MINQ_TABLE ( cm -> bit_depth , inter_minq ) ;
 if ( frame_is_intra_only ( cm ) ) {
 if ( rc -> this_key_frame_forced ) {
 int qindex = rc -> last_boosted_qindex ;
 double last_boosted_q = vp9_convert_qindex_to_q ( qindex , cm -> bit_depth ) ;
 int delta_qindex = vp9_compute_qdelta ( rc , last_boosted_q , last_boosted_q * 0.75 , cm -> bit_depth ) ;
 active_best_quality = MAX ( qindex + delta_qindex , rc -> best_quality ) ;
 }
 else {
 double q_adj_factor = 1.0 ;
 double q_val ;
 active_best_quality = get_kf_active_quality ( rc , rc -> avg_frame_qindex [ KEY_FRAME ] , cm -> bit_depth ) ;
 if ( ( cm -> width * cm -> height ) <= ( 352 * 288 ) ) {
 q_adj_factor -= 0.25 ;
 }
 q_val = vp9_convert_qindex_to_q ( active_best_quality , cm -> bit_depth ) ;
 active_best_quality += vp9_compute_qdelta ( rc , q_val , q_val * q_adj_factor , cm -> bit_depth ) ;
 }
 }
 else if ( ! rc -> is_src_frame_alt_ref && ( cpi -> refresh_golden_frame || cpi -> refresh_alt_ref_frame ) ) {
 if ( rc -> frames_since_key > 1 && rc -> avg_frame_qindex [ INTER_FRAME ] < active_worst_quality ) {
 q = rc -> avg_frame_qindex [ INTER_FRAME ] ;
 }
 else {
 q = rc -> avg_frame_qindex [ KEY_FRAME ] ;
 }
 if ( oxcf -> rc_mode == VPX_CQ ) {
 if ( q < cq_level ) q = cq_level ;
 active_best_quality = get_gf_active_quality ( rc , q , cm -> bit_depth ) ;
 active_best_quality = active_best_quality * 15 / 16 ;
 }
 else if ( oxcf -> rc_mode == VPX_Q ) {
 if ( ! cpi -> refresh_alt_ref_frame ) {
 active_best_quality = cq_level ;
 }
 else {
 active_best_quality = get_gf_active_quality ( rc , q , cm -> bit_depth ) ;
 }
 }
 else {
 active_best_quality = get_gf_active_quality ( rc , q , cm -> bit_depth ) ;
 }
 }
 else {
 if ( oxcf -> rc_mode == VPX_Q ) {
 active_best_quality = cq_level ;
 }
 else {
 if ( cm -> current_video_frame > 1 ) active_best_quality = inter_minq [ rc -> avg_frame_qindex [ INTER_FRAME ] ] ;
 else active_best_quality = inter_minq [ rc -> avg_frame_qindex [ KEY_FRAME ] ] ;
 if ( ( oxcf -> rc_mode == VPX_CQ ) && ( active_best_quality < cq_level ) ) {
 active_best_quality = cq_level ;
 }
 }
 }
 active_best_quality = clamp ( active_best_quality , rc -> best_quality , rc -> worst_quality ) ;
 active_worst_quality = clamp ( active_worst_quality , active_best_quality , rc -> worst_quality ) ;
 * top_index = active_worst_quality ;
 * bottom_index = active_best_quality ;
 # if LIMIT_QRANGE_FOR_ALTREF_AND_KEY {
 int qdelta = 0 ;
 vp9_clear_system_state ( ) ;
 if ( cm -> frame_type == KEY_FRAME && ! rc -> this_key_frame_forced && ! ( cm -> current_video_frame == 0 ) ) {
 qdelta = vp9_compute_qdelta_by_rate ( & cpi -> rc , cm -> frame_type , active_worst_quality , 2.0 , cm -> bit_depth ) ;
 }
 else if ( ! rc -> is_src_frame_alt_ref && ( cpi -> refresh_golden_frame || cpi -> refresh_alt_ref_frame ) ) {
 qdelta = vp9_compute_qdelta_by_rate ( & cpi -> rc , cm -> frame_type , active_worst_quality , 1.75 , cm -> bit_depth ) ;
 }
 * top_index = active_worst_quality + qdelta ;
 * top_index = ( * top_index > * bottom_index ) ? * top_index : * bottom_index ;
 }
 # endif if ( oxcf -> rc_mode == VPX_Q ) {
 q = active_best_quality ;
 }
 else if ( ( cm -> frame_type == KEY_FRAME ) && rc -> this_key_frame_forced ) {
 q = rc -> last_boosted_qindex ;
 }
 else {
 q = vp9_rc_regulate_q ( cpi , rc -> this_frame_target , active_best_quality , active_worst_quality ) ;
 if ( q > * top_index ) {
 if ( rc -> this_frame_target >= rc -> max_frame_bandwidth ) * top_index = q ;
 else q = * top_index ;
 }
 }
 assert ( * top_index <= rc -> worst_quality && * top_index >= rc -> best_quality ) ;
 assert ( * bottom_index <= rc -> worst_quality && * bottom_index >= rc -> best_quality ) ;
 assert ( q <= rc -> worst_quality && q >= rc -> best_quality ) ;
 return q ;
 }