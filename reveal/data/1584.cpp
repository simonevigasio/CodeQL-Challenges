void check_require ( DYNAMIC_STRING * ds , const char * fname ) {
 DBUG_ENTER ( "check_require" ) ;
 if ( dyn_string_cmp ( ds , fname ) ) {
 char reason [ FN_REFLEN ] ;
 fn_format ( reason , fname , "" , "" , MY_REPLACE_EXT | MY_REPLACE_DIR ) ;
 abort_not_supported_test ( "Test requires: '%s'" , reason ) ;
 }
 DBUG_VOID_RETURN ;
 }