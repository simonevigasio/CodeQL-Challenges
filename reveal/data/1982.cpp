static char * * subsystems_from_mount_options ( const char * mount_options , char * * kernel_list ) {
 char * token , * str , * saveptr = NULL ;
 char * * result = NULL ;
 size_t result_capacity = 0 ;
 size_t result_count = 0 ;
 int saved_errno ;
 int r ;
 str = alloca ( strlen ( mount_options ) + 1 ) ;
 strcpy ( str , mount_options ) ;
 for ( ;
 ( token = strtok_r ( str , "," , & saveptr ) ) ;
 str = NULL ) {
 if ( ! strncmp ( token , "name=" , 5 ) || lxc_string_in_array ( token , ( const char * * ) kernel_list ) ) {
 r = lxc_grow_array ( ( void * * * ) & result , & result_capacity , result_count + 1 , 12 ) ;
 if ( r < 0 ) goto out_free ;
 result [ result_count + 1 ] = NULL ;
 result [ result_count ] = strdup ( token ) ;
 if ( ! result [ result_count ] ) goto out_free ;
 result_count ++ ;
 }
 }
 return result ;
 out_free : saved_errno = errno ;
 lxc_free_array ( ( void * * ) result , free ) ;
 errno = saved_errno ;
 return NULL ;
 }