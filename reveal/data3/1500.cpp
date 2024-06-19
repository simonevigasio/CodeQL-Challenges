static int test_if_case_insensitive ( const char * dir_name ) {
 int result = 0 ;
 File file ;
 char buff [ FN_REFLEN ] , buff2 [ FN_REFLEN ] ;
 MY_STAT stat_info ;
 DBUG_ENTER ( "test_if_case_insensitive" ) ;
 fn_format ( buff , glob_hostname , dir_name , ".lower-test" , MY_UNPACK_FILENAME | MY_REPLACE_EXT | MY_REPLACE_DIR ) ;
 fn_format ( buff2 , glob_hostname , dir_name , ".LOWER-TEST" , MY_UNPACK_FILENAME | MY_REPLACE_EXT | MY_REPLACE_DIR ) ;
 mysql_file_delete ( key_file_casetest , buff2 , MYF ( 0 ) ) ;
 if ( ( file = mysql_file_create ( key_file_casetest , buff , 0666 , O_RDWR , MYF ( 0 ) ) ) < 0 ) {
 if ( ! opt_abort ) sql_print_warning ( "Can't create test file %s" , buff ) ;
 DBUG_RETURN ( - 1 ) ;
 }
 mysql_file_close ( file , MYF ( 0 ) ) ;
 if ( mysql_file_stat ( key_file_casetest , buff2 , & stat_info , MYF ( 0 ) ) ) result = 1 ;
 mysql_file_delete ( key_file_casetest , buff , MYF ( MY_WME ) ) ;
 DBUG_PRINT ( "exit" , ( "result: %d" , result ) ) ;
 DBUG_RETURN ( result ) ;
 }