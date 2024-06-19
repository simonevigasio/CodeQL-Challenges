static void encode_color_spec ( VC2EncContext * s ) {
 AVCodecContext * avctx = s -> avctx ;
 put_bits ( & s -> pb , 1 , ! s -> strict_compliance ) ;
 if ( ! s -> strict_compliance ) {
 int val ;
 put_vc2_ue_uint ( & s -> pb , 0 ) ;
 put_bits ( & s -> pb , 1 , 1 ) ;
 if ( avctx -> color_primaries == AVCOL_PRI_BT470BG ) val = 2 ;
 else if ( avctx -> color_primaries == AVCOL_PRI_SMPTE170M ) val = 1 ;
 else if ( avctx -> color_primaries == AVCOL_PRI_SMPTE240M ) val = 1 ;
 else val = 0 ;
 put_vc2_ue_uint ( & s -> pb , val ) ;
 put_bits ( & s -> pb , 1 , 1 ) ;
 if ( avctx -> colorspace == AVCOL_SPC_RGB ) val = 3 ;
 else if ( avctx -> colorspace == AVCOL_SPC_YCOCG ) val = 2 ;
 else if ( avctx -> colorspace == AVCOL_SPC_BT470BG ) val = 1 ;
 else val = 0 ;
 put_vc2_ue_uint ( & s -> pb , val ) ;
 put_bits ( & s -> pb , 1 , 1 ) ;
 if ( avctx -> color_trc == AVCOL_TRC_LINEAR ) val = 2 ;
 else if ( avctx -> color_trc == AVCOL_TRC_BT1361_ECG ) val = 1 ;
 else val = 0 ;
 put_vc2_ue_uint ( & s -> pb , val ) ;
 }
 }