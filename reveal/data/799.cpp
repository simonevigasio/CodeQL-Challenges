static const char * array_value ( const char * * array , char key ) {
 for ( ;
 * array ;
 array += 2 ) if ( * * array == key ) return array [ 1 ] ;
 return 0 ;
 }