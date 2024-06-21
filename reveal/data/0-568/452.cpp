static void cirrus_reset ( void * opaque ) {
 CirrusVGAState * s = opaque ;
 vga_common_reset ( & s -> vga ) ;
 unmap_linear_vram ( s ) ;
 s -> vga . sr [ 0x06 ] = 0x0f ;
 if ( s -> device_id == CIRRUS_ID_CLGD5446 ) {
 s -> vga . sr [ 0x1F ] = 0x2d ;
 s -> vga . gr [ 0x18 ] = 0x0f ;
 s -> vga . sr [ 0x0f ] = 0x98 ;
 s -> vga . sr [ 0x17 ] = 0x20 ;
 s -> vga . sr [ 0x15 ] = 0x04 ;
 }
 else {
 s -> vga . sr [ 0x1F ] = 0x22 ;
 s -> vga . sr [ 0x0F ] = CIRRUS_MEMSIZE_2M ;
 s -> vga . sr [ 0x17 ] = s -> bustype ;
 s -> vga . sr [ 0x15 ] = 0x03 ;
 }
 s -> vga . cr [ 0x27 ] = s -> device_id ;
 s -> cirrus_hidden_dac_lockindex = 5 ;
 s -> cirrus_hidden_dac_data = 0 ;
 }