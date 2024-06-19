static void start_signal_handler ( void ) {
 int error ;
 pthread_attr_t thr_attr ;
 DBUG_ENTER ( "start_signal_handler" ) ;
 ( void ) pthread_attr_init ( & thr_attr ) ;
 pthread_attr_setscope ( & thr_attr , PTHREAD_SCOPE_SYSTEM ) ;
 ( void ) pthread_attr_setdetachstate ( & thr_attr , PTHREAD_CREATE_DETACHED ) ;
 ( void ) my_setstacksize ( & thr_attr , my_thread_stack_size ) ;
 mysql_mutex_lock ( & LOCK_thread_count ) ;
 if ( ( error = mysql_thread_create ( key_thread_signal_hand , & signal_thread , & thr_attr , signal_hand , 0 ) ) ) {
 sql_print_error ( "Can't create interrupt-thread (error %d, errno: %d)" , error , errno ) ;
 exit ( 1 ) ;
 }
 mysql_cond_wait ( & COND_thread_count , & LOCK_thread_count ) ;
 mysql_mutex_unlock ( & LOCK_thread_count ) ;
 ( void ) pthread_attr_destroy ( & thr_attr ) ;
 DBUG_VOID_RETURN ;
 }