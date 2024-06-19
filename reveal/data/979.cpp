SPL_METHOD ( SplFileInfo , getBasename ) {
 spl_filesystem_object * intern = ( spl_filesystem_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 char * fname , * suffix = 0 ;
 size_t flen ;
 int slen = 0 , path_len ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "|s" , & suffix , & slen ) == FAILURE ) {
 return ;
 }
 spl_filesystem_object_get_path ( intern , & path_len TSRMLS_CC ) ;
 if ( path_len && path_len < intern -> file_name_len ) {
 fname = intern -> file_name + path_len + 1 ;
 flen = intern -> file_name_len - ( path_len + 1 ) ;
 }
 else {
 fname = intern -> file_name ;
 flen = intern -> file_name_len ;
 }
 php_basename ( fname , flen , suffix , slen , & fname , & flen TSRMLS_CC ) ;
 RETURN_STRINGL ( fname , flen , 0 ) ;
 }