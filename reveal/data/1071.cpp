static void hb_fir_filter ( float * out , const float fir_coef [ HB_FIR_SIZE + 1 ] , float mem [ HB_FIR_SIZE ] , const float * in ) {
 int i , j ;
 float data [ AMRWB_SFR_SIZE_16k + HB_FIR_SIZE ] ;
 memcpy ( data , mem , HB_FIR_SIZE * sizeof ( float ) ) ;
 memcpy ( data + HB_FIR_SIZE , in , AMRWB_SFR_SIZE_16k * sizeof ( float ) ) ;
 for ( i = 0 ;
 i < AMRWB_SFR_SIZE_16k ;
 i ++ ) {
 out [ i ] = 0.0 ;
 for ( j = 0 ;
 j <= HB_FIR_SIZE ;
 j ++ ) out [ i ] += data [ i + j ] * fir_coef [ j ] ;
 }
 memcpy ( mem , data + AMRWB_SFR_SIZE_16k , HB_FIR_SIZE * sizeof ( float ) ) ;
 }