int buffer_write_bytes ( buffer_t buffer , const char * data , int size ) {
 if ( buffer_write ( buffer , data , size ) ) {
 PyErr_NoMemory ( ) ;
 return 0 ;
 }
 return 1 ;
 }