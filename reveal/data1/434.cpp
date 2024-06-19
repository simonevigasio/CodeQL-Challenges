static int chacha_cipher ( EVP_CIPHER_CTX * ctx , unsigned char * out , const unsigned char * inp , size_t len ) {
 EVP_CHACHA_KEY * key = data ( ctx ) ;
 unsigned int n , rem , ctr32 ;
 if ( ( n = key -> partial_len ) ) {
 while ( len && n < CHACHA_BLK_SIZE ) {
 * out ++ = * inp ++ ^ key -> buf [ n ++ ] ;
 len -- ;
 }
 key -> partial_len = n ;
 if ( len == 0 ) return 1 ;
 if ( n == CHACHA_BLK_SIZE ) {
 key -> partial_len = 0 ;
 key -> counter [ 0 ] ++ ;
 if ( key -> counter [ 0 ] == 0 ) key -> counter [ 1 ] ++ ;
 }
 }
 rem = ( unsigned int ) ( len % CHACHA_BLK_SIZE ) ;
 len -= rem ;
 ctr32 = key -> counter [ 0 ] ;
 while ( len >= CHACHA_BLK_SIZE ) {
 size_t blocks = len / CHACHA_BLK_SIZE ;
 if ( sizeof ( size_t ) > sizeof ( unsigned int ) && blocks > ( 1U << 28 ) ) blocks = ( 1U << 28 ) ;
 ctr32 += ( unsigned int ) blocks ;
 if ( ctr32 < blocks ) {
 blocks -= ctr32 ;
 ctr32 = 0 ;
 }
 blocks *= CHACHA_BLK_SIZE ;
 ChaCha20_ctr32 ( out , inp , blocks , key -> key . d , key -> counter ) ;
 len -= blocks ;
 inp += blocks ;
 out += blocks ;
 key -> counter [ 0 ] = ctr32 ;
 if ( ctr32 == 0 ) key -> counter [ 1 ] ++ ;
 }
 if ( rem ) {
 memset ( key -> buf , 0 , sizeof ( key -> buf ) ) ;
 ChaCha20_ctr32 ( key -> buf , key -> buf , CHACHA_BLK_SIZE , key -> key . d , key -> counter ) ;
 for ( n = 0 ;
 n < rem ;
 n ++ ) out [ n ] = inp [ n ] ^ key -> buf [ n ] ;
 key -> partial_len = rem ;
 }
 return 1 ;
 }