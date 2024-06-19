static int sort_record_index ( MI_SORT_PARAM * sort_param , MI_INFO * info , MI_KEYDEF * keyinfo , my_off_t page , uchar * buff , uint sort_key , File new_file , my_bool update_index ) {
 uint nod_flag , used_length , key_length ;
 uchar * temp_buff , * keypos , * endpos ;
 my_off_t next_page , rec_pos ;
 uchar lastkey [ MI_MAX_KEY_BUFF ] ;
 char llbuff [ 22 ] ;
 SORT_INFO * sort_info = sort_param -> sort_info ;
 MI_CHECK * param = sort_info -> param ;
 DBUG_ENTER ( "sort_record_index" ) ;
 nod_flag = mi_test_if_nod ( buff ) ;
 temp_buff = 0 ;
 if ( nod_flag ) {
 if ( ! ( temp_buff = ( uchar * ) my_alloca ( ( uint ) keyinfo -> block_length ) ) ) {
 mi_check_print_error ( param , "Not Enough memory" ) ;
 DBUG_RETURN ( - 1 ) ;
 }
 }
 used_length = mi_getint ( buff ) ;
 keypos = buff + 2 + nod_flag ;
 endpos = buff + used_length ;
 for ( ;
 ;
 ) {
 if ( nod_flag ) {
 next_page = _mi_kpos ( nod_flag , keypos ) ;
 if ( my_pread ( info -> s -> kfile , ( uchar * ) temp_buff , ( uint ) keyinfo -> block_length , next_page , MYF ( MY_NABP + MY_WME ) ) ) {
 mi_check_print_error ( param , "Can't read keys from filepos: %s" , llstr ( next_page , llbuff ) ) ;
 goto err ;
 }
 if ( sort_record_index ( sort_param , info , keyinfo , next_page , temp_buff , sort_key , new_file , update_index ) ) goto err ;
 }
 if ( keypos >= endpos || ( key_length = ( * keyinfo -> get_key ) ( keyinfo , nod_flag , & keypos , lastkey ) ) == 0 ) break ;
 rec_pos = _mi_dpos ( info , 0 , lastkey + key_length ) ;
 if ( ( * info -> s -> read_rnd ) ( info , sort_param -> record , rec_pos , 0 ) ) {
 mi_check_print_error ( param , "%d when reading datafile" , my_errno ) ;
 goto err ;
 }
 if ( rec_pos != sort_param -> filepos && update_index ) {
 _mi_dpointer ( info , keypos - nod_flag - info -> s -> rec_reflength , sort_param -> filepos ) ;
 if ( movepoint ( info , sort_param -> record , rec_pos , sort_param -> filepos , sort_key ) ) {
 mi_check_print_error ( param , "%d when updating key-pointers" , my_errno ) ;
 goto err ;
 }
 }
 if ( sort_write_record ( sort_param ) ) goto err ;
 }
 bzero ( ( uchar * ) buff + used_length , keyinfo -> block_length - used_length ) ;
 if ( my_pwrite ( info -> s -> kfile , ( uchar * ) buff , ( uint ) keyinfo -> block_length , page , param -> myf_rw ) ) {
 mi_check_print_error ( param , "%d when updating keyblock" , my_errno ) ;
 goto err ;
 }
 if ( temp_buff ) my_afree ( ( uchar * ) temp_buff ) ;
 DBUG_RETURN ( 0 ) ;
 err : if ( temp_buff ) my_afree ( ( uchar * ) temp_buff ) ;
 DBUG_RETURN ( 1 ) ;
 }