static int sort_one_index ( MI_CHECK * param , MI_INFO * info , MI_KEYDEF * keyinfo , my_off_t pagepos , File new_file ) {
 uint length , nod_flag , used_length , key_length ;
 uchar * buff , * keypos , * endpos ;
 uchar key [ HA_MAX_POSSIBLE_KEY_BUFF ] ;
 my_off_t new_page_pos , next_page ;
 char llbuff [ 22 ] ;
 DBUG_ENTER ( "sort_one_index" ) ;
 DBUG_ASSERT ( keyinfo -> key_alg != HA_KEY_ALG_RTREE ) ;
 new_page_pos = param -> new_file_pos ;
 param -> new_file_pos += keyinfo -> block_length ;
 if ( ! ( buff = ( uchar * ) my_alloca ( ( uint ) keyinfo -> block_length ) ) ) {
 mi_check_print_error ( param , "Not enough memory for key block" ) ;
 DBUG_RETURN ( - 1 ) ;
 }
 if ( ! _mi_fetch_keypage ( info , keyinfo , pagepos , DFLT_INIT_HITS , buff , 0 ) ) {
 mi_check_print_error ( param , "Can't read key block from filepos: %s" , llstr ( pagepos , llbuff ) ) ;
 goto err ;
 }
 if ( ( nod_flag = mi_test_if_nod ( buff ) ) || keyinfo -> flag & HA_FULLTEXT ) {
 used_length = mi_getint ( buff ) ;
 keypos = buff + 2 + nod_flag ;
 endpos = buff + used_length ;
 for ( ;
 ;
 ) {
 if ( nod_flag ) {
 next_page = _mi_kpos ( nod_flag , keypos ) ;
 _mi_kpointer ( info , keypos - nod_flag , param -> new_file_pos ) ;
 if ( sort_one_index ( param , info , keyinfo , next_page , new_file ) ) {
 DBUG_PRINT ( "error" , ( "From page: %ld, keyoffset: %lu used_length: %d" , ( ulong ) pagepos , ( ulong ) ( keypos - buff ) , ( int ) used_length ) ) ;
 DBUG_DUMP ( "buff" , ( uchar * ) buff , used_length ) ;
 goto err ;
 }
 }
 if ( keypos >= endpos || ( key_length = ( * keyinfo -> get_key ) ( keyinfo , nod_flag , & keypos , key ) ) == 0 ) break ;
 DBUG_ASSERT ( keypos <= endpos ) ;
 if ( keyinfo -> flag & HA_FULLTEXT ) {
 uint off ;
 int subkeys ;
 get_key_full_length_rdonly ( off , key ) ;
 subkeys = ft_sintXkorr ( key + off ) ;
 if ( subkeys < 0 ) {
 next_page = _mi_dpos ( info , 0 , key + key_length ) ;
 _mi_dpointer ( info , keypos - nod_flag - info -> s -> rec_reflength , param -> new_file_pos ) ;
 if ( sort_one_index ( param , info , & info -> s -> ft2_keyinfo , next_page , new_file ) ) goto err ;
 }
 }
 }
 }
 length = mi_getint ( buff ) ;
 bzero ( ( uchar * ) buff + length , keyinfo -> block_length - length ) ;
 if ( mysql_file_pwrite ( new_file , ( uchar * ) buff , ( uint ) keyinfo -> block_length , new_page_pos , MYF ( MY_NABP | MY_WAIT_IF_FULL ) ) ) {
 mi_check_print_error ( param , "Can't write indexblock, error: %d" , my_errno ) ;
 goto err ;
 }
 my_afree ( ( uchar * ) buff ) ;
 DBUG_RETURN ( 0 ) ;
 err : my_afree ( ( uchar * ) buff ) ;
 DBUG_RETURN ( 1 ) ;
 }