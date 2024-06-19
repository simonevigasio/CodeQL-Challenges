static void test_bug11718 ( ) {
 MYSQL_RES * res ;
 int rc ;
 const char * query = "select str_to_date(concat(f3),'%Y%m%d') from t1,t2 " "where f1=f2 order by f1" ;
 myheader ( "test_bug11718" ) ;
 rc = mysql_query ( mysql , "drop table if exists t1, t2" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "create table t1 (f1 int)" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "create table t2 (f2 int, f3 numeric(8))" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "insert into t1 values (1), (2)" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , "insert into t2 values (1,20050101), (2,20050202)" ) ;
 myquery ( rc ) ;
 rc = mysql_query ( mysql , query ) ;
 myquery ( rc ) ;
 res = mysql_store_result ( mysql ) ;
 if ( ! opt_silent ) printf ( "return type: %s" , ( res -> fields [ 0 ] . type == MYSQL_TYPE_DATE ) ? "DATE" : "not DATE" ) ;
 DIE_UNLESS ( res -> fields [ 0 ] . type == MYSQL_TYPE_DATE ) ;
 mysql_free_result ( res ) ;
 rc = mysql_query ( mysql , "drop table t1, t2" ) ;
 myquery ( rc ) ;
 }