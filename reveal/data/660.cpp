static void print_tab_data ( MYSQL_RES * result ) {
 MYSQL_ROW cur ;
 MYSQL_FIELD * field ;
 ulong * lengths ;
 if ( opt_silent < 2 && column_names ) {
 int first = 0 ;
 while ( ( field = mysql_fetch_field ( result ) ) ) {
 if ( first ++ ) ( void ) tee_fputs ( "\t" , PAGER ) ;
 ( void ) tee_fputs ( field -> name , PAGER ) ;
 }
 ( void ) tee_fputs ( "\n" , PAGER ) ;
 }
 while ( ( cur = mysql_fetch_row ( result ) ) ) {
 lengths = mysql_fetch_lengths ( result ) ;
 safe_put_field ( cur [ 0 ] , lengths [ 0 ] ) ;
 for ( uint off = 1 ;
 off < mysql_num_fields ( result ) ;
 off ++ ) {
 ( void ) tee_fputs ( "\t" , PAGER ) ;
 safe_put_field ( cur [ off ] , lengths [ off ] ) ;
 }
 ( void ) tee_fputs ( "\n" , PAGER ) ;
 }
 }