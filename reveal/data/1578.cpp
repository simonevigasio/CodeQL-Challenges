static void test_bug12925 ( ) {
 myheader ( "test_bug12925" ) ;
 if ( opt_getopt_ll_test ) DIE_UNLESS ( opt_getopt_ll_test == LL ( 25600 * 1024 * 1024 ) ) ;
 }