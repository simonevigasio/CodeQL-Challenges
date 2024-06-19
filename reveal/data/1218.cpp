int read_command ( struct st_command * * command_ptr ) {
 char * p = read_command_buf ;
 struct st_command * command ;
 DBUG_ENTER ( "read_command" ) ;
 if ( parser . current_line < parser . read_lines ) {
 get_dynamic ( & q_lines , ( uchar * ) command_ptr , parser . current_line ) ;
 DBUG_RETURN ( 0 ) ;
 }
 if ( ! ( * command_ptr = command = ( struct st_command * ) my_malloc ( sizeof ( * command ) , MYF ( MY_WME | MY_ZEROFILL ) ) ) || insert_dynamic ( & q_lines , ( uchar * ) & command ) ) die ( "Out of memory" ) ;
 command -> type = Q_UNKNOWN ;
 read_command_buf [ 0 ] = 0 ;
 if ( read_line ( read_command_buf , sizeof ( read_command_buf ) ) ) {
 check_eol_junk ( read_command_buf ) ;
 DBUG_RETURN ( 1 ) ;
 }
 if ( opt_result_format_version == 1 ) convert_to_format_v1 ( read_command_buf ) ;
 DBUG_PRINT ( "info" , ( "query: '%s'" , read_command_buf ) ) ;
 if ( * p == '#' ) {
 command -> type = Q_COMMENT ;
 }
 else if ( p [ 0 ] == '-' && p [ 1 ] == '-' ) {
 command -> type = Q_COMMENT_WITH_COMMAND ;
 p += 2 ;
 }
 else if ( * p == '\n' ) {
 command -> type = Q_EMPTY_LINE ;
 }
 while ( * p && my_isspace ( charset_info , * p ) ) p ++ ;
 if ( ! ( command -> query_buf = command -> query = my_strdup ( p , MYF ( MY_WME ) ) ) ) die ( "Out of memory" ) ;
 p = command -> query ;
 while ( * p && ! my_isspace ( charset_info , * p ) && * p != '(' && ! is_delimiter ( p ) ) p ++ ;
 command -> first_word_len = ( uint ) ( p - command -> query ) ;
 DBUG_PRINT ( "info" , ( "first_word: %.*s" , command -> first_word_len , command -> query ) ) ;
 while ( * p && my_isspace ( charset_info , * p ) ) p ++ ;
 command -> first_argument = p ;
 command -> end = strend ( command -> query ) ;
 command -> query_len = ( command -> end - command -> query ) ;
 parser . read_lines ++ ;
 DBUG_RETURN ( 0 ) ;
 }