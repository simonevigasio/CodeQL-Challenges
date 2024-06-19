static void test_insert_select ( ) {
 MYSQL_STMT * stmt_insert , * stmt_select ;
 char * query ;
 int rc ;
 uint i ;
 myheader ( "test_insert_select" ) ;
 rc = mysql_query ( mysql , "DROP TABLE IF EXISTS t1, t2" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "create table t1 (a int)" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "create table t2 (a int)" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "insert into t2 values (1)" ) ;
 myquery ( rc ) ;
 query = ( char * ) "insert into t1 select a from t2" ;
 stmt_insert = mysql_simple_prepare ( mysql , query ) ;
 check_stmt ( stmt_insert ) ;
 query = ( char * ) "select * from t1" ;
 stmt_select = mysql_simple_prepare ( mysql , query ) ;
 check_stmt ( stmt_select ) ;
 for ( i = 0 ;
 i < 3 ;
 i ++ ) {
 rc = mysql_stmt_execute ( stmt_insert ) ;
 check_execute ( stmt_insert , rc ) ;
 if ( ! opt_silent ) fprintf ( stdout , "insert %u\n" , i ) ;
 rc = mysql_stmt_execute ( stmt_select ) ;
 check_execute ( stmt_select , rc ) ;
 rc = my_process_stmt_result ( stmt_select ) ;
 DIE_UNLESS ( rc == ( int ) ( i + 1 ) ) ;
 }
 mysql_stmt_close ( stmt_insert ) ;
 mysql_stmt_close ( stmt_select ) ;
 rc = mysql_query ( mysql , "drop table t1, t2" ) ;
 myquery ( rc ) ;
 }