static void mime_list_stop ( NautilusDirectory * directory ) {
 NautilusFile * file ;
 if ( directory -> details -> mime_list_in_progress != NULL ) {
 file = directory -> details -> mime_list_in_progress -> mime_list_file ;
 if ( file != NULL ) {
 g_assert ( NAUTILUS_IS_FILE ( file ) ) ;
 g_assert ( file -> details -> directory == directory ) ;
 if ( is_needy ( file , should_get_mime_list , REQUEST_MIME_LIST ) ) {
 return ;
 }
 }
 mime_list_cancel ( directory ) ;
 }
 }