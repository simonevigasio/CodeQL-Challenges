static int option_cmp ( my_option * a , my_option * b ) {
 const char * sa = a -> name ;
 const char * sb = b -> name ;
 for ( ;
 * sa || * sb ;
 sa ++ , sb ++ ) {
 if ( * sa < * sb ) {
 if ( * sa == '-' && * sb == '_' ) continue ;
 else return - 1 ;
 }
 if ( * sa > * sb ) {
 if ( * sa == '_' && * sb == '-' ) continue ;
 else return 1 ;
 }
 }
 DBUG_ASSERT ( a -> name == b -> name ) ;
 return 0 ;
 }