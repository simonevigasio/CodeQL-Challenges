static void set_segmentation_map ( VP8_COMP * cpi , unsigned char * segmentation_map ) {
 vpx_memcpy ( cpi -> segmentation_map , segmentation_map , ( cpi -> common . mb_rows * cpi -> common . mb_cols ) ) ;
 cpi -> mb . e_mbd . update_mb_segmentation_map = 1 ;
 cpi -> mb . e_mbd . update_mb_segmentation_data = 1 ;
 }