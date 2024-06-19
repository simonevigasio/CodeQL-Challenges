static void test_bug11766854 ( ) {
 struct st_mysql_client_plugin * plugin ;
 DBUG_ENTER ( "test_bug11766854" ) ;
 myheader ( "test_bug11766854" ) ;
 plugin = mysql_load_plugin ( mysql , "foo" , - 1 , 0 ) ;
 DIE_UNLESS ( plugin == 0 ) ;
 plugin = mysql_load_plugin ( mysql , "qa_auth_client" , - 1 , 0 ) ;
 DIE_UNLESS ( plugin != 0 ) ;
 DIE_IF ( mysql_errno ( mysql ) ) ;
 DBUG_VOID_RETURN ;
 }