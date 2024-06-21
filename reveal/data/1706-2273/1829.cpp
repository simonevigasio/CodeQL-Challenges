static void debug_wait_for_kill ( const char * info ) {
 DBUG_ENTER ( "debug_wait_for_kill" ) ;
 const char * prev_info ;
 THD * thd ;
 thd = current_thd ;
 prev_info = thd_proc_info ( thd , info ) ;
 while ( ! thd -> killed ) my_sleep ( 1000 ) ;
 DBUG_PRINT ( "info" , ( "Exit debug_wait_for_kill" ) ) ;
 thd_proc_info ( thd , prev_info ) ;
 DBUG_VOID_RETURN ;
 }