static int aes_init_key ( EVP_CIPHER_CTX * ctx , const unsigned char * key , const unsigned char * iv , int enc ) {
 int ret , mode ;
 EVP_AES_KEY * dat = EVP_C_DATA ( EVP_AES_KEY , ctx ) ;
 mode = EVP_CIPHER_CTX_mode ( ctx ) ;
 if ( ( mode == EVP_CIPH_ECB_MODE || mode == EVP_CIPH_CBC_MODE ) && ! enc ) {
 # ifdef HWAES_CAPABLE if ( HWAES_CAPABLE ) {
 ret = HWAES_set_decrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) HWAES_decrypt ;
 dat -> stream . cbc = NULL ;
 # ifdef HWAES_cbc_encrypt if ( mode == EVP_CIPH_CBC_MODE ) dat -> stream . cbc = ( cbc128_f ) HWAES_cbc_encrypt ;
 # endif }
 else # endif # ifdef BSAES_CAPABLE if ( BSAES_CAPABLE && mode == EVP_CIPH_CBC_MODE ) {
 ret = AES_set_decrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) AES_decrypt ;
 dat -> stream . cbc = ( cbc128_f ) bsaes_cbc_encrypt ;
 }
 else # endif # ifdef VPAES_CAPABLE if ( VPAES_CAPABLE ) {
 ret = vpaes_set_decrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) vpaes_decrypt ;
 dat -> stream . cbc = mode == EVP_CIPH_CBC_MODE ? ( cbc128_f ) vpaes_cbc_encrypt : NULL ;
 }
 else # endif {
 ret = AES_set_decrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) AES_decrypt ;
 dat -> stream . cbc = mode == EVP_CIPH_CBC_MODE ? ( cbc128_f ) AES_cbc_encrypt : NULL ;
 }
 }
 else # ifdef HWAES_CAPABLE if ( HWAES_CAPABLE ) {
 ret = HWAES_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) HWAES_encrypt ;
 dat -> stream . cbc = NULL ;
 # ifdef HWAES_cbc_encrypt if ( mode == EVP_CIPH_CBC_MODE ) dat -> stream . cbc = ( cbc128_f ) HWAES_cbc_encrypt ;
 else # endif # ifdef HWAES_ctr32_encrypt_blocks if ( mode == EVP_CIPH_CTR_MODE ) dat -> stream . ctr = ( ctr128_f ) HWAES_ctr32_encrypt_blocks ;
 else # endif ( void ) 0 ;
 }
 else # endif # ifdef BSAES_CAPABLE if ( BSAES_CAPABLE && mode == EVP_CIPH_CTR_MODE ) {
 ret = AES_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) AES_encrypt ;
 dat -> stream . ctr = ( ctr128_f ) bsaes_ctr32_encrypt_blocks ;
 }
 else # endif # ifdef VPAES_CAPABLE if ( VPAES_CAPABLE ) {
 ret = vpaes_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) vpaes_encrypt ;
 dat -> stream . cbc = mode == EVP_CIPH_CBC_MODE ? ( cbc128_f ) vpaes_cbc_encrypt : NULL ;
 }
 else # endif {
 ret = AES_set_encrypt_key ( key , EVP_CIPHER_CTX_key_length ( ctx ) * 8 , & dat -> ks . ks ) ;
 dat -> block = ( block128_f ) AES_encrypt ;
 dat -> stream . cbc = mode == EVP_CIPH_CBC_MODE ? ( cbc128_f ) AES_cbc_encrypt : NULL ;
 # ifdef AES_CTR_ASM if ( mode == EVP_CIPH_CTR_MODE ) dat -> stream . ctr = ( ctr128_f ) AES_ctr32_encrypt ;
 # endif }
 if ( ret < 0 ) {
 EVPerr ( EVP_F_AES_INIT_KEY , EVP_R_AES_KEY_SETUP_FAILED ) ;
 return 0 ;
 }
 return 1 ;
 }