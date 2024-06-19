int init_dumping_tables ( char * qdatabase ) {
 DBUG_ENTER ( "init_dumping_tables" ) ;
 if ( ! opt_create_db ) {
 char qbuf [ 256 ] ;
 MYSQL_ROW row ;
 MYSQL_RES * dbinfo ;
 my_snprintf ( qbuf , sizeof ( qbuf ) , "SHOW CREATE DATABASE IF NOT EXISTS %s" , qdatabase ) ;
 if ( mysql_query ( mysql , qbuf ) || ! ( dbinfo = mysql_store_result ( mysql ) ) ) {
 if ( opt_drop_database ) fprintf ( md_result_file , "\n/*!40000 DROP DATABASE IF EXISTS %s*/;
\n" , qdatabase ) ;
 fprintf ( md_result_file , "\nCREATE DATABASE /*!32312 IF NOT EXISTS*/ %s;
\n" , qdatabase ) ;
 }
 else {
 if ( opt_drop_database ) fprintf ( md_result_file , "\n/*!40000 DROP DATABASE IF EXISTS %s*/;
\n" , qdatabase ) ;
 row = mysql_fetch_row ( dbinfo ) ;
 if ( row [ 1 ] ) {
 fprintf ( md_result_file , "\n%s;
\n" , row [ 1 ] ) ;
 }
 mysql_free_result ( dbinfo ) ;
 }
 }
 DBUG_RETURN ( 0 ) ;
 }