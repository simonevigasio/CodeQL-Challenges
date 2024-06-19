void jpc_ft_fwdlift_colres ( jpc_fix_t * a , int numrows , int numcols , int stride , int parity ) {
 jpc_fix_t * lptr ;
 jpc_fix_t * hptr ;
 register jpc_fix_t * lptr2 ;
 register jpc_fix_t * hptr2 ;
 register int n ;
 register int i ;
 int llen ;
 llen = ( numrows + 1 - parity ) >> 1 ;
 if ( numrows > 1 ) {
 lptr = & a [ 0 ] ;
 hptr = & a [ llen * stride ] ;
 if ( parity ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 for ( i = 0 ;
 i < numcols ;
 ++ i ) {
 hptr2 [ 0 ] -= lptr2 [ 0 ] ;
 ++ hptr2 ;
 ++ lptr2 ;
 }
 hptr += stride ;
 }
 n = numrows - llen - parity - ( parity == ( numrows & 1 ) ) ;
 while ( n -- > 0 ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 for ( i = 0 ;
 i < numcols ;
 ++ i ) {
 hptr2 [ 0 ] -= jpc_fix_asr ( lptr2 [ 0 ] + lptr2 [ stride ] , 1 ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 }
 hptr += stride ;
 lptr += stride ;
 }
 if ( parity == ( numrows & 1 ) ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 for ( i = 0 ;
 i < numcols ;
 ++ i ) {
 hptr2 [ 0 ] -= lptr2 [ 0 ] ;
 ++ lptr2 ;
 ++ hptr2 ;
 }
 }
 lptr = & a [ 0 ] ;
 hptr = & a [ llen * stride ] ;
 if ( ! parity ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 for ( i = 0 ;
 i < numcols ;
 ++ i ) {
 lptr2 [ 0 ] += jpc_fix_asr ( hptr2 [ 0 ] + 1 , 1 ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 }
 lptr += stride ;
 }
 n = llen - ( ! parity ) - ( parity != ( numrows & 1 ) ) ;
 while ( n -- > 0 ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 for ( i = 0 ;
 i < numcols ;
 ++ i ) {
 lptr2 [ 0 ] += jpc_fix_asr ( hptr2 [ 0 ] + hptr2 [ stride ] + 2 , 2 ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 }
 lptr += stride ;
 hptr += stride ;
 }
 if ( parity != ( numrows & 1 ) ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 for ( i = 0 ;
 i < numcols ;
 ++ i ) {
 lptr2 [ 0 ] += jpc_fix_asr ( hptr2 [ 0 ] + 1 , 1 ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 }
 }
 }
 else {
 if ( parity ) {
 lptr2 = & a [ 0 ] ;
 for ( i = 0 ;
 i < numcols ;
 ++ i ) {
 lptr2 [ 0 ] = jpc_fix_asl ( lptr2 [ 0 ] , 1 ) ;
 ++ lptr2 ;
 }
 }
 }
 }