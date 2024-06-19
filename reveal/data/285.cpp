void vp9_regular_quantize_b_4x4 ( MACROBLOCK * x , int plane , int block , const int16_t * scan , const int16_t * iscan ) {
 MACROBLOCKD * const xd = & x -> e_mbd ;
 struct macroblock_plane * p = & x -> plane [ plane ] ;
 struct macroblockd_plane * pd = & xd -> plane [ plane ] ;
 # if CONFIG_VP9_HIGHBITDEPTH if ( xd -> cur_buf -> flags & YV12_FLAG_HIGHBITDEPTH ) {
 vp9_high_quantize_b ( BLOCK_OFFSET ( p -> coeff , block ) , 16 , x -> skip_block , p -> zbin , p -> round , p -> quant , p -> quant_shift , BLOCK_OFFSET ( p -> qcoeff , block ) , BLOCK_OFFSET ( pd -> dqcoeff , block ) , pd -> dequant , p -> zbin_extra , & p -> eobs [ block ] , scan , iscan ) ;
 return ;
 }
 # endif vp9_quantize_b ( BLOCK_OFFSET ( p -> coeff , block ) , 16 , x -> skip_block , p -> zbin , p -> round , p -> quant , p -> quant_shift , BLOCK_OFFSET ( p -> qcoeff , block ) , BLOCK_OFFSET ( pd -> dqcoeff , block ) , pd -> dequant , p -> zbin_extra , & p -> eobs [ block ] , scan , iscan ) ;
 }