static int ipvideo_decode_block_opcode_0x8_16 ( IpvideoContext * s ) {
 int x , y ;
 uint16_t P [ 4 ] ;
 unsigned int flags = 0 ;
 uint16_t * pixel_ptr = ( uint16_t * ) s -> pixel_ptr ;
 P [ 0 ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 P [ 1 ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 if ( ! ( P [ 0 ] & 0x8000 ) ) {
 for ( y = 0 ;
 y < 16 ;
 y ++ ) {
 if ( ! ( y & 3 ) ) {
 if ( y ) {
 P [ 0 ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 P [ 1 ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 }
 flags = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 }
 for ( x = 0 ;
 x < 4 ;
 x ++ , flags >>= 1 ) * pixel_ptr ++ = P [ flags & 1 ] ;
 pixel_ptr += s -> stride - 4 ;
 if ( y == 7 ) pixel_ptr -= 8 * s -> stride - 4 ;
 }
 }
 else {
 flags = bytestream2_get_le32 ( & s -> stream_ptr ) ;
 P [ 2 ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 P [ 3 ] = bytestream2_get_le16 ( & s -> stream_ptr ) ;
 if ( ! ( P [ 2 ] & 0x8000 ) ) {
 for ( y = 0 ;
 y < 16 ;
 y ++ ) {
 for ( x = 0 ;
 x < 4 ;
 x ++ , flags >>= 1 ) * pixel_ptr ++ = P [ flags & 1 ] ;
 pixel_ptr += s -> stride - 4 ;
 if ( y == 7 ) {
 pixel_ptr -= 8 * s -> stride - 4 ;
 P [ 0 ] = P [ 2 ] ;
 P [ 1 ] = P [ 3 ] ;
 flags = bytestream2_get_le32 ( & s -> stream_ptr ) ;
 }
 }
 }
 else {
 for ( y = 0 ;
 y < 8 ;
 y ++ ) {
 if ( y == 4 ) {
 P [ 0 ] = P [ 2 ] ;
 P [ 1 ] = P [ 3 ] ;
 flags = bytestream2_get_le32 ( & s -> stream_ptr ) ;
 }
 for ( x = 0 ;
 x < 8 ;
 x ++ , flags >>= 1 ) * pixel_ptr ++ = P [ flags & 1 ] ;
 pixel_ptr += s -> line_inc ;
 }
 }
 }
 return 0 ;
 }