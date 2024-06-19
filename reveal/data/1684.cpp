static void main_get_appheader ( xd3_stream * stream , main_file * ifile , main_file * output , main_file * sfile ) {
 uint8_t * apphead ;
 usize_t appheadsz ;
 int ret ;
 if ( ! option_use_appheader ) {
 return ;
 }
 ret = xd3_get_appheader ( stream , & apphead , & appheadsz ) ;
 if ( ret != 0 ) {
 return ;
 }
 if ( appheadsz > 0 ) {
 char * start = ( char * ) apphead ;
 char * slash ;
 int place = 0 ;
 char * parsed [ 4 ] ;
 memset ( parsed , 0 , sizeof ( parsed ) ) ;
 while ( ( slash = strchr ( start , '/' ) ) != NULL ) {
 * slash = 0 ;
 parsed [ place ++ ] = start ;
 start = slash + 1 ;
 }
 parsed [ place ++ ] = start ;
 if ( place == 2 || place == 4 ) {
 main_get_appheader_params ( output , parsed , 1 , "output" , ifile ) ;
 }
 if ( place == 4 ) {
 main_get_appheader_params ( sfile , parsed + 2 , 0 , "source" , ifile ) ;
 }
 }
 option_use_appheader = 0 ;
 return ;
 }