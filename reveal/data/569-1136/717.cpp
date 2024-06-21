static gboolean is_trusted_desktop_file ( GFile * file , GCancellable * cancellable ) {
 char * basename ;
 gboolean res ;
 GFileInfo * info ;
 if ( ! g_file_is_native ( file ) ) {
 return FALSE ;
 }
 basename = g_file_get_basename ( file ) ;
 if ( ! g_str_has_suffix ( basename , ".desktop" ) ) {
 g_free ( basename ) ;
 return FALSE ;
 }
 g_free ( basename ) ;
 info = g_file_query_info ( file , G_FILE_ATTRIBUTE_STANDARD_TYPE "," G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE , G_FILE_QUERY_INFO_NOFOLLOW_SYMLINKS , cancellable , NULL ) ;
 if ( info == NULL ) {
 return FALSE ;
 }
 res = FALSE ;
 if ( g_file_info_get_file_type ( info ) == G_FILE_TYPE_REGULAR && ! g_file_info_get_attribute_boolean ( info , G_FILE_ATTRIBUTE_ACCESS_CAN_EXECUTE ) && nautilus_is_in_system_dir ( file ) ) {
 res = TRUE ;
 }
 g_object_unref ( info ) ;
 return res ;
 }