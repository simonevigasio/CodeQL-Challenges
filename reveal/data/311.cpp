static int tm2_decode_blocks ( TM2Context * ctx , AVFrame * p ) {
 int i , j ;
 int w = ctx -> avctx -> width , h = ctx -> avctx -> height , bw = w >> 2 , bh = h >> 2 , cw = w >> 1 ;
 int type ;
 int keyframe = 1 ;
 int * Y , * U , * V ;
 uint8_t * dst ;
 for ( i = 0 ;
 i < TM2_NUM_STREAMS ;
 i ++ ) ctx -> tok_ptrs [ i ] = 0 ;
 if ( ctx -> tok_lens [ TM2_TYPE ] < bw * bh ) {
 av_log ( ctx -> avctx , AV_LOG_ERROR , "Got %i tokens for %i blocks\n" , ctx -> tok_lens [ TM2_TYPE ] , bw * bh ) ;
 return AVERROR_INVALIDDATA ;
 }
 memset ( ctx -> last , 0 , 4 * bw * sizeof ( int ) ) ;
 memset ( ctx -> clast , 0 , 4 * bw * sizeof ( int ) ) ;
 for ( j = 0 ;
 j < bh ;
 j ++ ) {
 memset ( ctx -> D , 0 , 4 * sizeof ( int ) ) ;
 memset ( ctx -> CD , 0 , 4 * sizeof ( int ) ) ;
 for ( i = 0 ;
 i < bw ;
 i ++ ) {
 type = GET_TOK ( ctx , TM2_TYPE ) ;
 switch ( type ) {
 case TM2_HI_RES : tm2_hi_res_block ( ctx , p , i , j ) ;
 break ;
 case TM2_MED_RES : tm2_med_res_block ( ctx , p , i , j ) ;
 break ;
 case TM2_LOW_RES : tm2_low_res_block ( ctx , p , i , j ) ;
 break ;
 case TM2_NULL_RES : tm2_null_res_block ( ctx , p , i , j ) ;
 break ;
 case TM2_UPDATE : tm2_update_block ( ctx , p , i , j ) ;
 keyframe = 0 ;
 break ;
 case TM2_STILL : tm2_still_block ( ctx , p , i , j ) ;
 keyframe = 0 ;
 break ;
 case TM2_MOTION : tm2_motion_block ( ctx , p , i , j ) ;
 keyframe = 0 ;
 break ;
 default : av_log ( ctx -> avctx , AV_LOG_ERROR , "Skipping unknown block type %i\n" , type ) ;
 }
 }
 }
 Y = ( ctx -> cur ? ctx -> Y2 : ctx -> Y1 ) ;
 U = ( ctx -> cur ? ctx -> U2 : ctx -> U1 ) ;
 V = ( ctx -> cur ? ctx -> V2 : ctx -> V1 ) ;
 dst = p -> data [ 0 ] ;
 for ( j = 0 ;
 j < h ;
 j ++ ) {
 for ( i = 0 ;
 i < w ;
 i ++ ) {
 int y = Y [ i ] , u = U [ i >> 1 ] , v = V [ i >> 1 ] ;
 dst [ 3 * i + 0 ] = av_clip_uint8 ( y + v ) ;
 dst [ 3 * i + 1 ] = av_clip_uint8 ( y ) ;
 dst [ 3 * i + 2 ] = av_clip_uint8 ( y + u ) ;
 }
 Y [ - 4 ] = Y [ - 3 ] = Y [ - 2 ] = Y [ - 1 ] = Y [ 0 ] ;
 Y [ w + 3 ] = Y [ w + 2 ] = Y [ w + 1 ] = Y [ w ] = Y [ w - 1 ] ;
 if ( j == 0 ) {
 memcpy ( Y - 4 - 1 * ctx -> y_stride , Y - 4 , ctx -> y_stride ) ;
 memcpy ( Y - 4 - 2 * ctx -> y_stride , Y - 4 , ctx -> y_stride ) ;
 memcpy ( Y - 4 - 3 * ctx -> y_stride , Y - 4 , ctx -> y_stride ) ;
 memcpy ( Y - 4 - 4 * ctx -> y_stride , Y - 4 , ctx -> y_stride ) ;
 }
 else if ( j == h - 1 ) {
 memcpy ( Y - 4 + 1 * ctx -> y_stride , Y - 4 , ctx -> y_stride ) ;
 memcpy ( Y - 4 + 2 * ctx -> y_stride , Y - 4 , ctx -> y_stride ) ;
 memcpy ( Y - 4 + 3 * ctx -> y_stride , Y - 4 , ctx -> y_stride ) ;
 memcpy ( Y - 4 + 4 * ctx -> y_stride , Y - 4 , ctx -> y_stride ) ;
 }
 Y += ctx -> y_stride ;
 if ( j & 1 ) {
 U [ - 2 ] = U [ - 1 ] = U [ 0 ] ;
 V [ - 2 ] = V [ - 1 ] = V [ 0 ] ;
 U [ cw + 1 ] = U [ cw ] = U [ cw - 1 ] ;
 V [ cw + 1 ] = V [ cw ] = V [ cw - 1 ] ;
 if ( j == 1 ) {
 memcpy ( U - 2 - 1 * ctx -> uv_stride , U - 2 , ctx -> uv_stride ) ;
 memcpy ( V - 2 - 1 * ctx -> uv_stride , V - 2 , ctx -> uv_stride ) ;
 memcpy ( U - 2 - 2 * ctx -> uv_stride , U - 2 , ctx -> uv_stride ) ;
 memcpy ( V - 2 - 2 * ctx -> uv_stride , V - 2 , ctx -> uv_stride ) ;
 }
 else if ( j == h - 1 ) {
 memcpy ( U - 2 + 1 * ctx -> uv_stride , U - 2 , ctx -> uv_stride ) ;
 memcpy ( V - 2 + 1 * ctx -> uv_stride , V - 2 , ctx -> uv_stride ) ;
 memcpy ( U - 2 + 2 * ctx -> uv_stride , U - 2 , ctx -> uv_stride ) ;
 memcpy ( V - 2 + 2 * ctx -> uv_stride , V - 2 , ctx -> uv_stride ) ;
 }
 U += ctx -> uv_stride ;
 V += ctx -> uv_stride ;
 }
 dst += p -> linesize [ 0 ] ;
 }
 return keyframe ;
 }