static int ref_frame ( Vp3DecodeContext * s , ThreadFrame * dst , ThreadFrame * src ) {
 ff_thread_release_buffer ( s -> avctx , dst ) ;
 if ( src -> f -> data [ 0 ] ) return ff_thread_ref_frame ( dst , src ) ;
 return 0 ;
 }