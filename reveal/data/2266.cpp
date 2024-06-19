static int lookup_gname_helper ( struct cpio * cpio , const char * * name , id_t id ) {
 struct group * grent ;
 ( void ) cpio ;
 errno = 0 ;
 grent = getgrgid ( ( gid_t ) id ) ;
 if ( grent == NULL ) {
 * name = NULL ;
 if ( errno != 0 ) lafe_warnc ( errno , "getgrgid(%s) failed" , cpio_i64toa ( ( int64_t ) id ) ) ;
 return ( errno ) ;
 }
 * name = grent -> gr_name ;
 return ( 0 ) ;
 }