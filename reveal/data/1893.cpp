static void vga_draw_line4d2 ( VGACommonState * s1 , uint8_t * d , const uint8_t * s , int width ) {
 uint32_t plane_mask , data , v , * palette ;
 int x ;
 palette = s1 -> last_palette ;
 plane_mask = mask16 [ s1 -> ar [ VGA_ATC_PLANE_ENABLE ] & 0xf ] ;
 width >>= 3 ;
 for ( x = 0 ;
 x < width ;
 x ++ ) {
 data = ( ( uint32_t * ) s ) [ 0 ] ;
 data &= plane_mask ;
 v = expand4 [ GET_PLANE ( data , 0 ) ] ;
 v |= expand4 [ GET_PLANE ( data , 1 ) ] << 1 ;
 v |= expand4 [ GET_PLANE ( data , 2 ) ] << 2 ;
 v |= expand4 [ GET_PLANE ( data , 3 ) ] << 3 ;
 PUT_PIXEL2 ( d , 0 , palette [ v >> 28 ] ) ;
 PUT_PIXEL2 ( d , 1 , palette [ ( v >> 24 ) & 0xf ] ) ;
 PUT_PIXEL2 ( d , 2 , palette [ ( v >> 20 ) & 0xf ] ) ;
 PUT_PIXEL2 ( d , 3 , palette [ ( v >> 16 ) & 0xf ] ) ;
 PUT_PIXEL2 ( d , 4 , palette [ ( v >> 12 ) & 0xf ] ) ;
 PUT_PIXEL2 ( d , 5 , palette [ ( v >> 8 ) & 0xf ] ) ;
 PUT_PIXEL2 ( d , 6 , palette [ ( v >> 4 ) & 0xf ] ) ;
 PUT_PIXEL2 ( d , 7 , palette [ ( v >> 0 ) & 0xf ] ) ;
 d += 64 ;
 s += 4 ;
 }
 }