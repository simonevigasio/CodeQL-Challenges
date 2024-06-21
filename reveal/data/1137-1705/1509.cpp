void vp9_sad ## m ## x ## n ## x ## k ## _c ( const uint8_t * src , int src_stride , const uint8_t * ref , int ref_stride , unsigned int * sads ) {
 int i ;
 for ( i = 0 ;
 i < k ;
 ++ i ) sads [ i ] = vp9_sad ## m ## x ## n ## _c ( src , src_stride , & ref [ i ] , ref_stride ) ;
 \ }
 # define sadMxNx4D ( m , n ) void vp9_sad ## m ## x ## n ## x4d_c ( const uint8_t * src , int src_stride , const uint8_t * const refs [ ] , int ref_stride , unsigned int * sads ) {
 int i ;
 for ( i = 0 ;
 i < 4 ;
 ++ i ) sads [ i ] = vp9_sad ## m ## x ## n ## _c ( src , src_stride , refs [ i ] , ref_stride ) ;
 \ }
 sadMxN ( 64 , 64 ) sadMxNxK ( 64 , 64 , 3 ) sadMxNxK ( 64 , 64 , 8 ) sadMxNx4D ( 64 , 64 ) sadMxN ( 64 , 32 ) sadMxNx4D ( 64 , 32 ) sadMxN ( 32 , 64 ) sadMxNx4D ( 32 , 64 ) sadMxN ( 32 , 32 ) sadMxNxK ( 32 , 32 , 3 ) sadMxNxK ( 32 , 32 , 8 )