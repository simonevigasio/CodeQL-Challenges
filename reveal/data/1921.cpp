void jpc_ns_invlift_col ( jpc_fix_t * a , int numrows , int stride , int parity ) {
 jpc_fix_t * lptr ;
 jpc_fix_t * hptr ;
 register jpc_fix_t * lptr2 ;
 register jpc_fix_t * hptr2 ;
 register int n ;
 int llen ;
 llen = ( numrows + 1 - parity ) >> 1 ;
 if ( numrows > 1 ) {
 # if defined ( WT_DOSCALE ) lptr = & a [ 0 ] ;
 n = llen ;
 while ( n -- > 0 ) {
 lptr2 = lptr ;
 lptr2 [ 0 ] = jpc_fix_mul ( lptr2 [ 0 ] , jpc_dbltofix ( 1.0 / LGAIN ) ) ;
 ++ lptr2 ;
 lptr += stride ;
 }
 hptr = & a [ llen * stride ] ;
 n = numrows - llen ;
 while ( n -- > 0 ) {
 hptr2 = hptr ;
 hptr2 [ 0 ] = jpc_fix_mul ( hptr2 [ 0 ] , jpc_dbltofix ( 1.0 / HGAIN ) ) ;
 ++ hptr2 ;
 hptr += stride ;
 }
 # endif lptr = & a [ 0 ] ;
 hptr = & a [ llen * stride ] ;
 if ( ! parity ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( lptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( 2.0 * DELTA ) , hptr2 [ 0 ] ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 lptr += stride ;
 }
 n = llen - ( ! parity ) - ( parity != ( numrows & 1 ) ) ;
 while ( n -- > 0 ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( lptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( DELTA ) , jpc_fix_add ( hptr2 [ 0 ] , hptr2 [ stride ] ) ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 lptr += stride ;
 hptr += stride ;
 }
 if ( parity != ( numrows & 1 ) ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( lptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( 2.0 * DELTA ) , hptr2 [ 0 ] ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 }
 lptr = & a [ 0 ] ;
 hptr = & a [ llen * stride ] ;
 if ( parity ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( hptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( 2.0 * GAMMA ) , lptr2 [ 0 ] ) ) ;
 ++ hptr2 ;
 ++ lptr2 ;
 hptr += stride ;
 }
 n = numrows - llen - parity - ( parity == ( numrows & 1 ) ) ;
 while ( n -- > 0 ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( hptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( GAMMA ) , jpc_fix_add ( lptr2 [ 0 ] , lptr2 [ stride ] ) ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 hptr += stride ;
 lptr += stride ;
 }
 if ( parity == ( numrows & 1 ) ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( hptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( 2.0 * GAMMA ) , lptr2 [ 0 ] ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 }
 lptr = & a [ 0 ] ;
 hptr = & a [ llen * stride ] ;
 if ( ! parity ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( lptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( 2.0 * BETA ) , hptr2 [ 0 ] ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 lptr += stride ;
 }
 n = llen - ( ! parity ) - ( parity != ( numrows & 1 ) ) ;
 while ( n -- > 0 ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( lptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( BETA ) , jpc_fix_add ( hptr2 [ 0 ] , hptr2 [ stride ] ) ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 lptr += stride ;
 hptr += stride ;
 }
 if ( parity != ( numrows & 1 ) ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( lptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( 2.0 * BETA ) , hptr2 [ 0 ] ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 }
 lptr = & a [ 0 ] ;
 hptr = & a [ llen * stride ] ;
 if ( parity ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( hptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( 2.0 * ALPHA ) , lptr2 [ 0 ] ) ) ;
 ++ hptr2 ;
 ++ lptr2 ;
 hptr += stride ;
 }
 n = numrows - llen - parity - ( parity == ( numrows & 1 ) ) ;
 while ( n -- > 0 ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( hptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( ALPHA ) , jpc_fix_add ( lptr2 [ 0 ] , lptr2 [ stride ] ) ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 hptr += stride ;
 lptr += stride ;
 }
 if ( parity == ( numrows & 1 ) ) {
 lptr2 = lptr ;
 hptr2 = hptr ;
 jpc_fix_minuseq ( hptr2 [ 0 ] , jpc_fix_mul ( jpc_dbltofix ( 2.0 * ALPHA ) , lptr2 [ 0 ] ) ) ;
 ++ lptr2 ;
 ++ hptr2 ;
 }
 }
 else {
 # if defined ( WT_LENONE ) if ( parity ) {
 lptr2 = & a [ 0 ] ;
 lptr2 [ 0 ] = jpc_fix_asr ( lptr2 [ 0 ] , 1 ) ;
 ++ lptr2 ;
 }
 # endif }
 }