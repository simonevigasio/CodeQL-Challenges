static void unicode_escape_to_buffer ( FBuffer * buffer , char buf [ 6 ] , UTF16 character ) {
 unicode_escape ( buf , character ) ;
 fbuffer_append ( buffer , buf , 6 ) ;
 }