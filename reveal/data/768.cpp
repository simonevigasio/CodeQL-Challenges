static void test_bug9992 ( ) {
 MYSQL * mysql1 ;
 MYSQL_RES * res ;
 int rc ;
 myheader ( "test_bug9992" ) ;
 if ( ! opt_silent ) printf ( "Establishing a connection with option CLIENT_MULTI_STATEMENTS..\n" ) ;
 mysql1 = mysql_client_init ( NULL ) ;
 if ( ! mysql_real_connect ( mysql1 , opt_host , opt_user , opt_password , opt_db ? opt_db : "test" , opt_port , opt_unix_socket , CLIENT_MULTI_STATEMENTS ) ) {
 fprintf ( stderr , "Failed to connect to the database\n" ) ;
 DIE_UNLESS ( 0 ) ;
 }
 rc = mysql_query ( mysql1 , "SHOW TABLES;
 SHOW DATABASE;
 SELECT 1;
" ) ;
 if ( rc ) {
 fprintf ( stderr , "[%d] %s\n" , mysql_errno ( mysql1 ) , mysql_error ( mysql1 ) ) ;
 DIE_UNLESS ( 0 ) ;
 }
 if ( ! opt_silent ) printf ( "Testing mysql_store_result/mysql_next_result..\n" ) ;
 res = mysql_store_result ( mysql1 ) ;
 DIE_UNLESS ( res ) ;
 mysql_free_result ( res ) ;
 rc = mysql_next_result ( mysql1 ) ;
 DIE_UNLESS ( rc == 1 ) ;
 if ( ! opt_silent ) fprintf ( stdout , "Got error, as expected:\n [%d] %s\n" , mysql_errno ( mysql1 ) , mysql_error ( mysql1 ) ) ;
 mysql_close ( mysql1 ) ;
 }