static void test_bug29306 ( ) {
 MYSQL_FIELD * field ;
 int rc ;
 MYSQL_RES * res ;
 DBUG_ENTER ( "test_bug29306" ) ;
 myheader ( "test_bug29306" ) ;
 rc = mysql_query ( mysql , "DROP TABLE IF EXISTS tab17557" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "DROP VIEW IF EXISTS view17557" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "CREATE TABLE tab17557 (dd decimal (3,1))" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "CREATE VIEW view17557 as SELECT dd FROM tab17557" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "INSERT INTO tab17557 VALUES (7.6)" ) ;
 myquery ( rc ) ;
 res = mysql_list_fields ( mysql , "view17557" , NULL ) ;
 while ( ( field = mysql_fetch_field ( res ) ) ) {
 if ( ! opt_silent ) {
 printf ( "field name %s\n" , field -> name ) ;
 printf ( "field table %s\n" , field -> table ) ;
 printf ( "field decimals %d\n" , field -> decimals ) ;
 if ( field -> decimals < 1 ) printf ( "Error! No decimals! \n" ) ;
 printf ( "\n\n" ) ;
 }
 DIE_UNLESS ( field -> decimals == 1 ) ;
 }
 mysql_free_result ( res ) ;
 rc = mysql_query ( mysql , "DROP TABLE tab17557" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "DROP VIEW view17557" ) ;
 myquery ( rc ) ;
 DBUG_VOID_RETURN ;
 }