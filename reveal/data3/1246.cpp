void vp9_apply_encoding_flags ( VP9_COMP * cpi , vpx_enc_frame_flags_t flags ) {
 if ( flags & ( VP8_EFLAG_NO_REF_LAST | VP8_EFLAG_NO_REF_GF | VP8_EFLAG_NO_REF_ARF ) ) {
 int ref = 7 ;
 if ( flags & VP8_EFLAG_NO_REF_LAST ) ref ^= VP9_LAST_FLAG ;
 if ( flags & VP8_EFLAG_NO_REF_GF ) ref ^= VP9_GOLD_FLAG ;
 if ( flags & VP8_EFLAG_NO_REF_ARF ) ref ^= VP9_ALT_FLAG ;
 vp9_use_as_reference ( cpi , ref ) ;
 }
 if ( flags & ( VP8_EFLAG_NO_UPD_LAST | VP8_EFLAG_NO_UPD_GF | VP8_EFLAG_NO_UPD_ARF | VP8_EFLAG_FORCE_GF | VP8_EFLAG_FORCE_ARF ) ) {
 int upd = 7 ;
 if ( flags & VP8_EFLAG_NO_UPD_LAST ) upd ^= VP9_LAST_FLAG ;
 if ( flags & VP8_EFLAG_NO_UPD_GF ) upd ^= VP9_GOLD_FLAG ;
 if ( flags & VP8_EFLAG_NO_UPD_ARF ) upd ^= VP9_ALT_FLAG ;
 vp9_update_reference ( cpi , upd ) ;
 }
 if ( flags & VP8_EFLAG_NO_UPD_ENTROPY ) {
 vp9_update_entropy ( cpi , 0 ) ;
 }
 }