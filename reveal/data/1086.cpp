static void test_prepare_grant ( ) {
 int rc ;
 char query [ MAX_TEST_QUERY_LENGTH ] ;
 myheader ( "test_prepare_grant" ) ;
 mysql_autocommit ( mysql , TRUE ) ;
 rc = mysql_query ( mysql , "DROP TABLE IF EXISTS test_grant" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "CREATE TABLE test_grant(a tinyint primary key auto_increment)" ) ;
 myquery ( rc ) ;
 strxmov ( query , "GRANT INSERT, UPDATE, SELECT ON " , current_db , ".test_grant TO 'test_grant'@" , opt_host ? opt_host : "'localhost'" , NullS ) ;
 if ( mysql_query ( mysql , query ) ) {
 myerror ( "GRANT failed" ) ;
 if ( mysql_errno ( mysql ) != 1047 ) exit ( 1 ) ;
 }
 else {
 MYSQL * org_mysql = mysql , * lmysql ;
 MYSQL_STMT * stmt ;
 if ( ! opt_silent ) fprintf ( stdout , "\n Establishing a test connection ..." ) ;
 if ( ! ( lmysql = mysql_client_init ( NULL ) ) ) {
 myerror ( "mysql_client_init() failed" ) ;
 exit ( 1 ) ;
 }
 if ( ! ( mysql_real_connect ( lmysql , opt_host , "test_grant" , "" , current_db , opt_port , opt_unix_socket , 0 ) ) ) {
 myerror ( "connection failed" ) ;
 mysql_close ( lmysql ) ;
 exit ( 1 ) ;
 }
 lmysql -> reconnect = 1 ;
 if ( ! opt_silent ) fprintf ( stdout , "OK" ) ;
 mysql = lmysql ;
 rc = mysql_query ( mysql , "INSERT INTO test_grant VALUES(NULL)" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "INSERT INTO test_grant(a) VALUES(NULL)" ) ;
 myquery ( rc ) ;
 execute_prepare_query ( "INSERT INTO test_grant(a) VALUES(NULL)" , 1 ) ;
 execute_prepare_query ( "INSERT INTO test_grant VALUES(NULL)" , 1 ) ;
 execute_prepare_query ( "UPDATE test_grant SET a=9 WHERE a=1" , 1 ) ;
 rc = my_stmt_result ( "SELECT a FROM test_grant" ) ;
 DIE_UNLESS ( rc == 4 ) ;
 rc = mysql_query ( mysql , "DELETE FROM test_grant" ) ;
 myquery_r ( rc ) ;
 stmt = mysql_simple_prepare ( mysql , "DELETE FROM test_grant" ) ;
 check_stmt_r ( stmt ) ;
 rc = my_stmt_result ( "SELECT * FROM test_grant" ) ;
 DIE_UNLESS ( rc == 4 ) ;
 mysql_close ( lmysql ) ;
 mysql = org_mysql ;
 rc = mysql_query ( mysql , "delete from mysql.user where User='test_grant'" ) ;
 myquery ( rc ) ;
 DIE_UNLESS ( 1 == mysql_affected_rows ( mysql ) ) ;
 rc = mysql_query ( mysql , "delete from mysql.tables_priv where User='test_grant'" ) ;
 myquery ( rc ) ;
 DIE_UNLESS ( 1 == mysql_affected_rows ( mysql ) ) ;
 }
 }