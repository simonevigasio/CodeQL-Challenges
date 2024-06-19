int chk_data_link ( MI_CHECK * param , MI_INFO * info , int extend ) {
 int error , got_error , flag ;
 uint key , UNINIT_VAR ( left_length ) , b_type , field ;
 ha_rows records , del_blocks ;
 my_off_t used , empty , pos , splits , UNINIT_VAR ( start_recpos ) , del_length , link_used , start_block ;
 uchar * record = 0 , * UNINIT_VAR ( to ) ;
 char llbuff [ 22 ] , llbuff2 [ 22 ] , llbuff3 [ 22 ] ;
 ha_checksum intern_record_checksum ;
 ha_checksum key_checksum [ HA_MAX_POSSIBLE_KEY ] ;
 my_bool static_row_size ;
 MI_KEYDEF * keyinfo ;
 MI_BLOCK_INFO block_info ;
 DBUG_ENTER ( "chk_data_link" ) ;
 if ( ! ( param -> testflag & T_SILENT ) ) {
 if ( extend ) puts ( "- check records and index references" ) ;
 else puts ( "- check record links" ) ;
 }
 if ( ! mi_alloc_rec_buff ( info , - 1 , & record ) ) {
 mi_check_print_error ( param , "Not enough memory for record" ) ;
 DBUG_RETURN ( - 1 ) ;
 }
 records = del_blocks = 0 ;
 used = link_used = splits = del_length = 0 ;
 intern_record_checksum = param -> glob_crc = 0 ;
 got_error = error = 0 ;
 empty = info -> s -> pack . header_length ;
 static_row_size = 1 ;
 if ( info -> s -> data_file_type == COMPRESSED_RECORD ) {
 for ( field = 0 ;
 field < info -> s -> base . fields ;
 field ++ ) {
 if ( info -> s -> rec [ field ] . base_type == FIELD_BLOB || info -> s -> rec [ field ] . base_type == FIELD_VARCHAR ) {
 static_row_size = 0 ;
 break ;
 }
 }
 }
 pos = my_b_tell ( & param -> read_cache ) ;
 bzero ( ( char * ) key_checksum , info -> s -> base . keys * sizeof ( key_checksum [ 0 ] ) ) ;
 while ( pos < info -> state -> data_file_length ) {
 if ( * killed_ptr ( param ) ) goto err2 ;
 switch ( info -> s -> data_file_type ) {
 case STATIC_RECORD : if ( my_b_read ( & param -> read_cache , ( uchar * ) record , info -> s -> base . pack_reclength ) ) goto err ;
 start_recpos = pos ;
 pos += info -> s -> base . pack_reclength ;
 splits ++ ;
 if ( * record == '\0' ) {
 del_blocks ++ ;
 del_length += info -> s -> base . pack_reclength ;
 continue ;
 }
 param -> glob_crc += mi_static_checksum ( info , record ) ;
 used += info -> s -> base . pack_reclength ;
 break ;
 case DYNAMIC_RECORD : flag = block_info . second_read = 0 ;
 block_info . next_filepos = pos ;
 do {
 if ( _mi_read_cache ( & param -> read_cache , ( uchar * ) block_info . header , ( start_block = block_info . next_filepos ) , sizeof ( block_info . header ) , ( flag ? 0 : READING_NEXT ) | READING_HEADER ) ) goto err ;
 if ( start_block & ( MI_DYN_ALIGN_SIZE - 1 ) ) {
 mi_check_print_error ( param , "Wrong aligned block at %s" , llstr ( start_block , llbuff ) ) ;
 goto err2 ;
 }
 b_type = _mi_get_block_info ( & block_info , - 1 , start_block ) ;
 if ( b_type & ( BLOCK_DELETED | BLOCK_ERROR | BLOCK_SYNC_ERROR | BLOCK_FATAL_ERROR ) ) {
 if ( b_type & BLOCK_SYNC_ERROR ) {
 if ( flag ) {
 mi_check_print_error ( param , "Unexpected byte: %d at link: %s" , ( int ) block_info . header [ 0 ] , llstr ( start_block , llbuff ) ) ;
 goto err2 ;
 }
 pos = block_info . filepos + block_info . block_len ;
 goto next ;
 }
 if ( b_type & BLOCK_DELETED ) {
 if ( block_info . block_len < info -> s -> base . min_block_length ) {
 mi_check_print_error ( param , "Deleted block with impossible length %lu at %s" , block_info . block_len , llstr ( pos , llbuff ) ) ;
 goto err2 ;
 }
 if ( ( block_info . next_filepos != HA_OFFSET_ERROR && block_info . next_filepos >= info -> state -> data_file_length ) || ( block_info . prev_filepos != HA_OFFSET_ERROR && block_info . prev_filepos >= info -> state -> data_file_length ) ) {
 mi_check_print_error ( param , "Delete link points outside datafile at %s" , llstr ( pos , llbuff ) ) ;
 goto err2 ;
 }
 del_blocks ++ ;
 del_length += block_info . block_len ;
 pos = block_info . filepos + block_info . block_len ;
 splits ++ ;
 goto next ;
 }
 mi_check_print_error ( param , "Wrong bytesec: %d-%d-%d at linkstart: %s" , block_info . header [ 0 ] , block_info . header [ 1 ] , block_info . header [ 2 ] , llstr ( start_block , llbuff ) ) ;
 goto err2 ;
 }
 if ( info -> state -> data_file_length < block_info . filepos + block_info . block_len ) {
 mi_check_print_error ( param , "Recordlink that points outside datafile at %s" , llstr ( pos , llbuff ) ) ;
 got_error = 1 ;
 break ;
 }
 splits ++ ;
 if ( ! flag ++ ) {
 start_recpos = pos ;
 pos = block_info . filepos + block_info . block_len ;
 if ( block_info . rec_len > ( uint ) info -> s -> base . max_pack_length ) {
 mi_check_print_error ( param , "Found too long record (%lu) at %s" , ( ulong ) block_info . rec_len , llstr ( start_recpos , llbuff ) ) ;
 got_error = 1 ;
 break ;
 }
 if ( info -> s -> base . blobs ) {
 if ( ! ( to = mi_alloc_rec_buff ( info , block_info . rec_len , & info -> rec_buff ) ) ) {
 mi_check_print_error ( param , "Not enough memory (%lu) for blob at %s" , ( ulong ) block_info . rec_len , llstr ( start_recpos , llbuff ) ) ;
 got_error = 1 ;
 break ;
 }
 }
 else to = info -> rec_buff ;
 left_length = block_info . rec_len ;
 }
 if ( left_length < block_info . data_len ) {
 mi_check_print_error ( param , "Found too long record (%lu) at %s" , ( ulong ) block_info . data_len , llstr ( start_recpos , llbuff ) ) ;
 got_error = 1 ;
 break ;
 }
 if ( _mi_read_cache ( & param -> read_cache , ( uchar * ) to , block_info . filepos , ( uint ) block_info . data_len , flag == 1 ? READING_NEXT : 0 ) ) goto err ;
 to += block_info . data_len ;
 link_used += block_info . filepos - start_block ;
 used += block_info . filepos - start_block + block_info . data_len ;
 empty += block_info . block_len - block_info . data_len ;
 left_length -= block_info . data_len ;
 if ( left_length ) {
 if ( b_type & BLOCK_LAST ) {
 mi_check_print_error ( param , "Wrong record length %s of %s at %s" , llstr ( block_info . rec_len - left_length , llbuff ) , llstr ( block_info . rec_len , llbuff2 ) , llstr ( start_recpos , llbuff3 ) ) ;
 got_error = 1 ;
 break ;
 }
 if ( info -> state -> data_file_length < block_info . next_filepos ) {
 mi_check_print_error ( param , "Found next-recordlink that points outside datafile at %s" , llstr ( block_info . filepos , llbuff ) ) ;
 got_error = 1 ;
 break ;
 }
 }
 }
 while ( left_length ) ;
 if ( ! got_error ) {
 if ( _mi_rec_unpack ( info , record , info -> rec_buff , block_info . rec_len ) == MY_FILE_ERROR ) {
 mi_check_print_error ( param , "Found wrong record at %s" , llstr ( start_recpos , llbuff ) ) ;
 got_error = 1 ;
 }
 else {
 info -> checksum = mi_checksum ( info , record ) ;
 if ( param -> testflag & ( T_EXTEND | T_MEDIUM | T_VERBOSE ) ) {
 if ( _mi_rec_check ( info , record , info -> rec_buff , block_info . rec_len , test ( info -> s -> calc_checksum ) ) ) {
 mi_check_print_error ( param , "Found wrong packed record at %s" , llstr ( start_recpos , llbuff ) ) ;
 got_error = 1 ;
 }
 }
 if ( ! got_error ) param -> glob_crc += info -> checksum ;
 }
 }
 else if ( ! flag ) pos = block_info . filepos + block_info . block_len ;
 break ;
 case COMPRESSED_RECORD : if ( _mi_read_cache ( & param -> read_cache , ( uchar * ) block_info . header , pos , info -> s -> pack . ref_length , READING_NEXT ) ) goto err ;
 start_recpos = pos ;
 splits ++ ;
 ( void ) _mi_pack_get_block_info ( info , & info -> bit_buff , & block_info , & info -> rec_buff , - 1 , start_recpos ) ;
 pos = block_info . filepos + block_info . rec_len ;
 if ( block_info . rec_len < ( uint ) info -> s -> min_pack_length || block_info . rec_len > ( uint ) info -> s -> max_pack_length ) {
 mi_check_print_error ( param , "Found block with wrong recordlength: %d at %s" , block_info . rec_len , llstr ( start_recpos , llbuff ) ) ;
 got_error = 1 ;
 break ;
 }
 if ( _mi_read_cache ( & param -> read_cache , ( uchar * ) info -> rec_buff , block_info . filepos , block_info . rec_len , READING_NEXT ) ) goto err ;
 if ( _mi_pack_rec_unpack ( info , & info -> bit_buff , record , info -> rec_buff , block_info . rec_len ) ) {
 mi_check_print_error ( param , "Found wrong record at %s" , llstr ( start_recpos , llbuff ) ) ;
 got_error = 1 ;
 }
 if ( static_row_size ) param -> glob_crc += mi_static_checksum ( info , record ) ;
 else param -> glob_crc += mi_checksum ( info , record ) ;
 link_used += ( block_info . filepos - start_recpos ) ;
 used += ( pos - start_recpos ) ;
 break ;
 case BLOCK_RECORD : assert ( 0 ) ;
 }
 if ( ! got_error ) {
 intern_record_checksum += ( ha_checksum ) start_recpos ;
 records ++ ;
 if ( param -> testflag & T_WRITE_LOOP && records % WRITE_COUNT == 0 ) {
 printf ( "%s\r" , llstr ( records , llbuff ) ) ;
 ( void ) fflush ( stdout ) ;
 }
 for ( key = 0 , keyinfo = info -> s -> keyinfo ;
 key < info -> s -> base . keys ;
 key ++ , keyinfo ++ ) {
 if ( mi_is_key_active ( info -> s -> state . key_map , key ) ) {
 if ( ! ( keyinfo -> flag & HA_FULLTEXT ) ) {
 uint key_length = _mi_make_key ( info , key , info -> lastkey , record , start_recpos ) ;
 if ( extend ) {
 int search_result = # ifdef HAVE_RTREE_KEYS ( keyinfo -> flag & HA_SPATIAL ) ? rtree_find_first ( info , key , info -> lastkey , key_length , MBR_EQUAL | MBR_DATA ) : # endif _mi_search ( info , keyinfo , info -> lastkey , key_length , SEARCH_SAME , info -> s -> state . key_root [ key ] ) ;
 if ( search_result ) {
 mi_check_print_error ( param , "Record at: %10s " "Can't find key for index: %2d" , llstr ( start_recpos , llbuff ) , key + 1 ) ;
 if ( error ++ > MAXERR || ! ( param -> testflag & T_VERBOSE ) ) goto err2 ;
 }
 }
 else key_checksum [ key ] += mi_byte_checksum ( ( uchar * ) info -> lastkey , key_length ) ;
 }
 }
 }
 }
 else {
 got_error = 0 ;
 if ( error ++ > MAXERR || ! ( param -> testflag & T_VERBOSE ) ) goto err2 ;
 }
 next : ;
 }
 if ( param -> testflag & T_WRITE_LOOP ) {
 ( void ) fputs ( " \r" , stdout ) ;
 ( void ) fflush ( stdout ) ;
 }
 if ( records != info -> state -> records ) {
 mi_check_print_error ( param , "Record-count is not ok;
 is %-10s Should be: %s" , llstr ( records , llbuff ) , llstr ( info -> state -> records , llbuff2 ) ) ;
 error = 1 ;
 }
 else if ( param -> record_checksum && param -> record_checksum != intern_record_checksum ) {
 mi_check_print_error ( param , "Keypointers and record positions doesn't match" ) ;
 error = 1 ;
 }
 else if ( param -> glob_crc != info -> state -> checksum && ( info -> s -> options & ( HA_OPTION_CHECKSUM | HA_OPTION_COMPRESS_RECORD ) ) ) {
 mi_check_print_warning ( param , "Record checksum is not the same as checksum stored in the index file\n" ) ;
 error = 1 ;
 }
 else if ( ! extend ) {
 for ( key = 0 ;
 key < info -> s -> base . keys ;
 key ++ ) {
 if ( key_checksum [ key ] != param -> key_crc [ key ] && ! ( info -> s -> keyinfo [ key ] . flag & ( HA_FULLTEXT | HA_SPATIAL ) ) ) {
 mi_check_print_error ( param , "Checksum for key: %2d doesn't match checksum for records" , key + 1 ) ;
 error = 1 ;
 }
 }
 }
 if ( del_length != info -> state -> empty ) {
 mi_check_print_warning ( param , "Found %s deleted space. Should be %s" , llstr ( del_length , llbuff2 ) , llstr ( info -> state -> empty , llbuff ) ) ;
 }
 if ( used + empty + del_length != info -> state -> data_file_length ) {
 mi_check_print_warning ( param , "Found %s record-data and %s unused data and %s deleted-data" , llstr ( used , llbuff ) , llstr ( empty , llbuff2 ) , llstr ( del_length , llbuff3 ) ) ;
 mi_check_print_warning ( param , "Total %s, Should be: %s" , llstr ( ( used + empty + del_length ) , llbuff ) , llstr ( info -> state -> data_file_length , llbuff2 ) ) ;
 }
 if ( del_blocks != info -> state -> del ) {
 mi_check_print_warning ( param , "Found %10s deleted blocks Should be: %s" , llstr ( del_blocks , llbuff ) , llstr ( info -> state -> del , llbuff2 ) ) ;
 }
 if ( splits != info -> s -> state . split ) {
 mi_check_print_warning ( param , "Found %10s key parts. Should be: %s" , llstr ( splits , llbuff ) , llstr ( info -> s -> state . split , llbuff2 ) ) ;
 }
 if ( param -> testflag & T_INFO ) {
 if ( param -> warning_printed || param -> error_printed ) puts ( "" ) ;
 if ( used != 0 && ! param -> error_printed ) {
 printf ( "Records:%18s M.recordlength:%9lu Packed:%14.0f%%\n" , llstr ( records , llbuff ) , ( long ) ( ( used - link_used ) / records ) , ( info -> s -> base . blobs ? 0.0 : ( ulonglong2double ( ( ulonglong ) info -> s -> base . reclength * records ) - my_off_t2double ( used ) ) / ulonglong2double ( ( ulonglong ) info -> s -> base . reclength * records ) * 100.0 ) ) ;
 printf ( "Recordspace used:%9.0f%% Empty space:%12d%% Blocks/Record: %6.2f\n" , ( ulonglong2double ( used - link_used ) / ulonglong2double ( used - link_used + empty ) * 100.0 ) , ( ! records ? 100 : ( int ) ( ulonglong2double ( del_length + empty ) / my_off_t2double ( used ) * 100.0 ) ) , ulonglong2double ( splits - del_blocks ) / records ) ;
 }
 printf ( "Record blocks:%12s Delete blocks:%10s\n" , llstr ( splits - del_blocks , llbuff ) , llstr ( del_blocks , llbuff2 ) ) ;
 printf ( "Record data: %12s Deleted data: %10s\n" , llstr ( used - link_used , llbuff ) , llstr ( del_length , llbuff2 ) ) ;
 printf ( "Lost space: %12s Linkdata: %10s\n" , llstr ( empty , llbuff ) , llstr ( link_used , llbuff2 ) ) ;
 }
 my_free ( mi_get_rec_buff_ptr ( info , record ) ) ;
 DBUG_RETURN ( error ) ;
 err : mi_check_print_error ( param , "got error: %d when reading datafile at record: %s" , my_errno , llstr ( records , llbuff ) ) ;
 err2 : my_free ( mi_get_rec_buff_ptr ( info , record ) ) ;
 param -> testflag |= T_RETRY_WITHOUT_QUICK ;
 DBUG_RETURN ( 1 ) ;
 }