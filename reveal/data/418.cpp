static void init_error_log_mutex ( ) {
 mysql_mutex_init ( key_LOCK_error_log , & LOCK_error_log , MY_MUTEX_INIT_FAST ) ;
 }