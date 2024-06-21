static void test_bug4026 ( ) {
 MYSQL_STMT * stmt ;
 MYSQL_BIND my_bind [ 2 ] ;
 MYSQL_TIME time_in , time_out ;
 MYSQL_TIME datetime_in , datetime_out ;
 const char * stmt_text ;
 int rc ;
 myheader ( "test_bug4026" ) ;
 stmt = mysql_stmt_init ( mysql ) ;
 stmt_text = "SELECT ?, ?" ;
 rc = mysql_stmt_prepare ( stmt , stmt_text , strlen ( stmt_text ) ) ;
 check_execute ( stmt , rc ) ;
 memset ( my_bind , 0 , sizeof ( my_bind ) ) ;
 memset ( & time_in , 0 , sizeof ( time_in ) ) ;
 memset ( & time_out , 0 , sizeof ( time_out ) ) ;
 memset ( & datetime_in , 0 , sizeof ( datetime_in ) ) ;
 memset ( & datetime_out , 0 , sizeof ( datetime_out ) ) ;
 my_bind [ 0 ] . buffer_type = MYSQL_TYPE_TIME ;
 my_bind [ 0 ] . buffer = ( void * ) & time_in ;
 my_bind [ 1 ] . buffer_type = MYSQL_TYPE_DATETIME ;
 my_bind [ 1 ] . buffer = ( void * ) & datetime_in ;
 time_in . hour = 23 ;
 time_in . minute = 59 ;
 time_in . second = 59 ;
 time_in . second_part = 123456 ;
 time_in . time_type = MYSQL_TIMESTAMP_TIME ;
 datetime_in = time_in ;
 datetime_in . year = 2003 ;
 datetime_in . month = 12 ;
 datetime_in . day = 31 ;
 datetime_in . time_type = MYSQL_TIMESTAMP_DATETIME ;
 mysql_stmt_bind_param ( stmt , my_bind ) ;
 rc = mysql_stmt_execute ( stmt ) ;
 check_execute ( stmt , rc ) ;
 my_bind [ 0 ] . buffer = ( void * ) & time_out ;
 my_bind [ 1 ] . buffer = ( void * ) & datetime_out ;
 mysql_stmt_bind_result ( stmt , my_bind ) ;
 rc = mysql_stmt_fetch ( stmt ) ;
 DIE_UNLESS ( rc == 0 ) ;
 if ( ! opt_silent ) {
 printf ( "%d:%d:%d.%lu\n" , time_out . hour , time_out . minute , time_out . second , time_out . second_part ) ;
 printf ( "%d-%d-%d %d:%d:%d.%lu\n" , datetime_out . year , datetime_out . month , datetime_out . day , datetime_out . hour , datetime_out . minute , datetime_out . second , datetime_out . second_part ) ;
 }
 DIE_UNLESS ( memcmp ( & time_in , & time_out , sizeof ( time_in ) ) == 0 ) ;
 DIE_UNLESS ( memcmp ( & datetime_in , & datetime_out , sizeof ( datetime_in ) ) == 0 ) ;
 mysql_stmt_close ( stmt ) ;
 }