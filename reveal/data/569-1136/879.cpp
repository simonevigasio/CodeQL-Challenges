static void test_view_where ( ) {
 MYSQL_STMT * stmt ;
 int rc , i ;
 const char * query = "select v1.c,v2.c from v1, v2" ;
 myheader ( "test_view_where" ) ;
 rc = mysql_query ( mysql , "DROP TABLE IF EXISTS t1,v1,v2" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "DROP VIEW IF EXISTS v1,v2,t1" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "CREATE TABLE t1 (a int, b int)" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "insert into t1 values (1,2), (1,3), (2,4), (2,5), (3,10)" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "create view v1 (c) as select b from t1 where a<3" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "create view v2 (c) as select b from t1 where a>=3" ) ;
 myquery ( rc ) ;
 stmt = mysql_stmt_init ( mysql ) ;
 rc = mysql_stmt_prepare ( stmt , query , strlen ( query ) ) ;
 check_execute ( stmt , rc ) ;
 for ( i = 0 ;
 i < 3 ;
 i ++ ) {
 rc = mysql_stmt_execute ( stmt ) ;
 check_execute ( stmt , rc ) ;
 rc = my_process_stmt_result ( stmt ) ;
 DIE_UNLESS ( 4 == rc ) ;
 }
 mysql_stmt_close ( stmt ) ;
 rc = mysql_query ( mysql , "DROP TABLE t1" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "DROP VIEW v1, v2" ) ;
 myquery ( rc ) ;
 }