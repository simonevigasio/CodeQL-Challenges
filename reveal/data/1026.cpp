static int _fix_java ( const char * in , char * out ) {
 int i , j ;
 for ( i = 0 , j = 7 ;
 i < j ;
 i ++ , j -- ) {
 out [ i ] = in [ j ] ;
 out [ j ] = in [ i ] ;
 }
 for ( i = 8 , j = 15 ;
 i < j ;
 i ++ , j -- ) {
 out [ i ] = in [ j ] ;
 out [ j ] = in [ i ] ;
 }
 return 0 ;
 }