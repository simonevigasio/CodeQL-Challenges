void vp9_ ## type ## _predictor_ ## size ## x ## size ## _c ( uint8_t * dst , ptrdiff_t stride , const uint8_t * above , const uint8_t * left ) {
 type ## _predictor ( dst , stride , size , above , left ) ;
 }
 # define intra_pred_allsizes ( type ) intra_pred_sized ( type , 4 ) intra_pred_sized ( type , 8 ) intra_pred_sized ( type , 16 ) intra_pred_sized ( type , 32 ) static INLINE void d207_predictor ( uint8_t * dst , ptrdiff_t stride , int bs , const uint8_t * above , const uint8_t * left ) {
 int r , c ;
 ( void ) above ;
 for ( r = 0 ;
 r < bs - 1 ;
 ++ r ) dst [ r * stride ] = ROUND_POWER_OF_TWO ( left [ r ] + left [ r + 1 ] , 1 ) ;
 dst [ ( bs - 1 ) * stride ] = left [ bs - 1 ] ;
 dst ++ ;
 for ( r = 0 ;
 r < bs - 2 ;
 ++ r ) dst [ r * stride ] = ROUND_POWER_OF_TWO ( left [ r ] + left [ r + 1 ] * 2 + left [ r + 2 ] , 2 ) ;
 dst [ ( bs - 2 ) * stride ] = ROUND_POWER_OF_TWO ( left [ bs - 2 ] + left [ bs - 1 ] * 3 , 2 ) ;
 dst [ ( bs - 1 ) * stride ] = left [ bs - 1 ] ;
 dst ++ ;
 for ( c = 0 ;
 c < bs - 2 ;
 ++ c ) dst [ ( bs - 1 ) * stride + c ] = left [ bs - 1 ] ;
 for ( r = bs - 2 ;
 r >= 0 ;
 -- r ) for ( c = 0 ;
 c < bs - 2 ;
 ++ c ) dst [ r * stride + c ] = dst [ ( r + 1 ) * stride + c - 2 ] ;
 }
 intra_pred_allsizes ( d207 ) static INLINE void d63_predictor ( uint8_t * dst , ptrdiff_t stride , int bs , const uint8_t * above , const uint8_t * left ) {
 int r , c ;
 ( void ) left ;
 for ( r = 0 ;
 r < bs ;
 ++ r ) {
 for ( c = 0 ;
 c < bs ;
 ++ c ) dst [ c ] = r & 1 ? ROUND_POWER_OF_TWO ( above [ r / 2 + c ] + above [ r / 2 + c + 1 ] * 2 + above [ r / 2 + c + 2 ] , 2 ) : ROUND_POWER_OF_TWO ( above [ r / 2 + c ] + above [ r / 2 + c + 1 ] , 1 ) ;
 dst += stride ;
 }
 }
 intra_pred_allsizes ( d63 )