static int jbig2_decode_generic_template1_TPGDON ( Jbig2Ctx * ctx , Jbig2Segment * segment , const Jbig2GenericRegionParams * params , Jbig2ArithState * as , Jbig2Image * image , Jbig2ArithCx * GB_stats ) {
 const int GBW = image -> width ;
 const int GBH = image -> height ;
 uint32_t CONTEXT ;
 int x , y ;
 bool bit ;
 int LTP = 0 ;
 for ( y = 0 ;
 y < GBH ;
 y ++ ) {
 bit = jbig2_arith_decode ( as , & GB_stats [ 0x0795 ] ) ;
 if ( bit < 0 ) return - 1 ;
 LTP ^= bit ;
 if ( ! LTP ) {
 for ( x = 0 ;
 x < GBW ;
 x ++ ) {
 CONTEXT = jbig2_image_get_pixel ( image , x - 1 , y ) ;
 CONTEXT |= jbig2_image_get_pixel ( image , x - 2 , y ) << 1 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x - 3 , y ) << 2 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x + params -> gbat [ 0 ] , y + params -> gbat [ 1 ] ) << 3 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x + 2 , y - 1 ) << 4 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x + 1 , y - 1 ) << 5 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x , y - 1 ) << 6 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x - 1 , y - 1 ) << 7 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x - 2 , y - 1 ) << 8 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x + 2 , y - 2 ) << 9 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x + 1 , y - 2 ) << 10 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x , y - 2 ) << 11 ;
 CONTEXT |= jbig2_image_get_pixel ( image , x - 1 , y - 2 ) << 12 ;
 bit = jbig2_arith_decode ( as , & GB_stats [ CONTEXT ] ) ;
 if ( bit < 0 ) return - 1 ;
 jbig2_image_set_pixel ( image , x , y , bit ) ;
 }
 }
 else {
 copy_prev_row ( image , y ) ;
 }
 }
 return 0 ;
 }