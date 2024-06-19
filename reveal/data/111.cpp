static void query_filesystem_info_callback ( GObject * source_object , GAsyncResult * res , gpointer user_data ) {
 GFileInfo * info ;
 FilesystemInfoState * state ;
 state = user_data ;
 if ( state -> directory == NULL ) {
 filesystem_info_state_free ( state ) ;
 return ;
 }
 info = g_file_query_filesystem_info_finish ( G_FILE ( source_object ) , res , NULL ) ;
 got_filesystem_info ( state , info ) ;
 if ( info != NULL ) {
 g_object_unref ( info ) ;
 }
 }