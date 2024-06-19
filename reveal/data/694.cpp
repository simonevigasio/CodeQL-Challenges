static int allocate_buffers ( ShortenContext * s ) {
 int i , chan ;
 int * coeffs ;
 void * tmp_ptr ;
 for ( chan = 0 ;
 chan < s -> channels ;
 chan ++ ) {
 if ( FFMAX ( 1 , s -> nmean ) >= UINT_MAX / sizeof ( int32_t ) ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "nmean too large\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 if ( s -> blocksize + s -> nwrap >= UINT_MAX / sizeof ( int32_t ) || s -> blocksize + s -> nwrap <= ( unsigned ) s -> nwrap ) {
 av_log ( s -> avctx , AV_LOG_ERROR , "s->blocksize + s->nwrap too large\n" ) ;
 return AVERROR_INVALIDDATA ;
 }
 tmp_ptr = av_realloc ( s -> offset [ chan ] , sizeof ( int32_t ) * FFMAX ( 1 , s -> nmean ) ) ;
 if ( ! tmp_ptr ) return AVERROR ( ENOMEM ) ;
 s -> offset [ chan ] = tmp_ptr ;
 tmp_ptr = av_realloc ( s -> decoded_base [ chan ] , ( s -> blocksize + s -> nwrap ) * sizeof ( s -> decoded_base [ 0 ] [ 0 ] ) ) ;
 if ( ! tmp_ptr ) return AVERROR ( ENOMEM ) ;
 s -> decoded_base [ chan ] = tmp_ptr ;
 for ( i = 0 ;
 i < s -> nwrap ;
 i ++ ) s -> decoded_base [ chan ] [ i ] = 0 ;
 s -> decoded [ chan ] = s -> decoded_base [ chan ] + s -> nwrap ;
 }
 coeffs = av_realloc ( s -> coeffs , s -> nwrap * sizeof ( * s -> coeffs ) ) ;
 if ( ! coeffs ) return AVERROR ( ENOMEM ) ;
 s -> coeffs = coeffs ;
 return 0 ;
 }