bool fn_format_relative_to_data_home ( char * to , const char * name , const char * dir , const char * extension ) {
 char tmp_path [ FN_REFLEN ] ;
 if ( ! test_if_hard_path ( dir ) ) {
 strxnmov ( tmp_path , sizeof ( tmp_path ) - 1 , mysql_real_data_home , dir , NullS ) ;
 dir = tmp_path ;
 }
 return ! fn_format ( to , name , dir , extension , MY_APPEND_EXT | MY_UNPACK_FILENAME | MY_SAFE_PATH ) ;
 }