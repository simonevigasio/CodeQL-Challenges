static my_bool test_if_special_chars ( const char * str ) {
 # if MYSQL_VERSION_ID >= 32300 for ( ;
 * str ;
 str ++ ) if ( ! my_isvar ( charset_info , * str ) && * str != '$' ) return 1 ;
 # endif return 0 ;
 }