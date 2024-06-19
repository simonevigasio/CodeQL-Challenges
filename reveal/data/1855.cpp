void str_to_file2 ( const char * fname , char * str , int size , my_bool append ) {
 int fd ;
 char buff [ FN_REFLEN ] ;
 int flags = O_WRONLY | O_CREAT ;
 if ( ! test_if_hard_path ( fname ) ) {
 strxmov ( buff , opt_basedir , fname , NullS ) ;
 fname = buff ;
 }
 fn_format ( buff , fname , "" , "" , MY_UNPACK_FILENAME ) ;
 if ( ! append ) flags |= O_TRUNC ;
 if ( ( fd = my_open ( buff , flags , MYF ( MY_WME | MY_FFNF ) ) ) < 0 ) die ( "Could not open '%s' for writing, errno: %d" , buff , errno ) ;
 if ( append && my_seek ( fd , 0 , SEEK_END , MYF ( 0 ) ) == MY_FILEPOS_ERROR ) die ( "Could not find end of file '%s', errno: %d" , buff , errno ) ;
 if ( my_write ( fd , ( uchar * ) str , size , MYF ( MY_WME | MY_FNABP ) ) ) die ( "write failed, errno: %d" , errno ) ;
 my_close ( fd , MYF ( 0 ) ) ;
 }