int write_data_suffix ( SORT_INFO * sort_info , my_bool fix_datafile ) {
 MI_INFO * info = sort_info -> info ;
 if ( info -> s -> options & HA_OPTION_COMPRESS_RECORD && fix_datafile ) {
 uchar buff [ MEMMAP_EXTRA_MARGIN ] ;
 bzero ( buff , sizeof ( buff ) ) ;
 if ( my_b_write ( & info -> rec_cache , buff , sizeof ( buff ) ) ) {
 mi_check_print_error ( sort_info -> param , "%d when writing to datafile" , my_errno ) ;
 return 1 ;
 }
 sort_info -> param -> read_cache . end_of_file += sizeof ( buff ) ;
 }
 return 0 ;
 }