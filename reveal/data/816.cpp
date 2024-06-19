void vp9_update_reference ( VP9_COMP * cpi , int ref_frame_flags ) {
 cpi -> ext_refresh_golden_frame = ( ref_frame_flags & VP9_GOLD_FLAG ) != 0 ;
 cpi -> ext_refresh_alt_ref_frame = ( ref_frame_flags & VP9_ALT_FLAG ) != 0 ;
 cpi -> ext_refresh_last_frame = ( ref_frame_flags & VP9_LAST_FLAG ) != 0 ;
 cpi -> ext_refresh_frame_flags_pending = 1 ;
 }