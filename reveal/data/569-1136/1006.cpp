static size_t choose_block ( char * d , size_t dlen , int col , const char * fromcode , const char * tocode , encoder_t * encoder , size_t * wlen ) {
 const int utf8 = fromcode && ( mutt_str_strcasecmp ( fromcode , "utf-8" ) == 0 ) ;
 size_t n = dlen ;
 while ( true ) {
 assert ( n > 0 ) ;
 const size_t nn = try_block ( d , n , fromcode , tocode , encoder , wlen ) ;
 if ( ( nn == 0 ) && ( ( col + * wlen ) <= ( ENCWORD_LEN_MAX + 1 ) || ( n <= 1 ) ) ) break ;
 n = ( nn ? nn : n ) - 1 ;
 assert ( n > 0 ) ;
 if ( utf8 ) while ( ( n > 1 ) && CONTINUATION_BYTE ( d [ n ] ) ) n -- ;
 }
 return n ;
 }