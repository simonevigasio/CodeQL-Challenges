static int lxc_cgroup_create_legacy ( struct cgroup_process_info * base_info , const char * name , pid_t pid ) {
 struct cgroup_process_info * info_ptr ;
 int r ;
 for ( info_ptr = base_info ;
 info_ptr ;
 info_ptr = info_ptr -> next ) {
 if ( ! lxc_string_in_array ( "ns" , ( const char * * ) info_ptr -> hierarchy -> subsystems ) ) continue ;
 char * tmp = cgroup_rename_nsgroup ( ( const char * ) info_ptr -> designated_mount_point -> mount_point , info_ptr -> cgroup_path , pid , name ) ;
 if ( ! tmp ) return - 1 ;
 free ( info_ptr -> cgroup_path ) ;
 info_ptr -> cgroup_path = tmp ;
 r = lxc_grow_array ( ( void * * * ) & info_ptr -> created_paths , & info_ptr -> created_paths_capacity , info_ptr -> created_paths_count + 1 , 8 ) ;
 if ( r < 0 ) return - 1 ;
 tmp = strdup ( tmp ) ;
 if ( ! tmp ) return - 1 ;
 info_ptr -> created_paths [ info_ptr -> created_paths_count ++ ] = tmp ;
 }
 return 0 ;
 }