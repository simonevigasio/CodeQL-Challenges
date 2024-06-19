static void test_bug21206 ( ) {
 const size_t cursor_count = 1025 ;
 const char * create_table [ ] = {
 "DROP TABLE IF EXISTS t1" , "CREATE TABLE t1 (i INT)" , "INSERT INTO t1 VALUES (1), (2), (3)" }
 ;
 const char * query = "SELECT * FROM t1" ;
 Stmt_fetch * fetch_array = ( Stmt_fetch * ) calloc ( cursor_count , sizeof ( Stmt_fetch ) ) ;
 Stmt_fetch * fetch ;
 DBUG_ENTER ( "test_bug21206" ) ;
 myheader ( "test_bug21206" ) ;
 fill_tables ( create_table , sizeof ( create_table ) / sizeof ( * create_table ) ) ;
 for ( fetch = fetch_array ;
 fetch < fetch_array + cursor_count ;
 ++ fetch ) {
 stmt_fetch_init ( fetch , fetch - fetch_array , query ) ;
 }
 for ( fetch = fetch_array ;
 fetch < fetch_array + cursor_count ;
 ++ fetch ) stmt_fetch_close ( fetch ) ;
 free ( fetch_array ) ;
 DBUG_VOID_RETURN ;
 }