static inline int32_t calculate_sign_huff ( MLPDecodeContext * m , unsigned int substr , unsigned int ch ) {
 SubStream * s = & m -> substream [ substr ] ;
 ChannelParams * cp = & s -> channel_params [ ch ] ;
 int lsb_bits = cp -> huff_lsbs - s -> quant_step_size [ ch ] ;
 int sign_shift = lsb_bits + ( cp -> codebook ? 2 - cp -> codebook : - 1 ) ;
 int32_t sign_huff_offset = cp -> huff_offset ;
 if ( cp -> codebook > 0 ) sign_huff_offset -= 7 << lsb_bits ;
 if ( sign_shift >= 0 ) sign_huff_offset -= 1 << sign_shift ;
 return sign_huff_offset ;
 }