static void test_bug33831 ( void ) {
 MYSQL * l_mysql ;
 DBUG_ENTER ( "test_bug33831" ) ;
 if ( ! ( l_mysql = mysql_client_init ( NULL ) ) ) {
 myerror ( "mysql_client_init() failed" ) ;
 DIE_UNLESS ( 0 ) ;
 }
 if ( ! ( mysql_real_connect ( l_mysql , opt_host , opt_user , opt_password , current_db , opt_port , opt_unix_socket , 0 ) ) ) {
 myerror ( "connection failed" ) ;
 DIE_UNLESS ( 0 ) ;
 }
 if ( mysql_real_connect ( l_mysql , opt_host , opt_user , opt_password , current_db , opt_port , opt_unix_socket , 0 ) ) {
 myerror ( "connection should have failed" ) ;
 DIE_UNLESS ( 0 ) ;
 }
 mysql_close ( l_mysql ) ;
 DBUG_VOID_RETURN ;
 }