static const char * cmd_hash_key ( cmd_parms * cmd , void * _dcfg , const char * _p1 , const char * _p2 ) {
 directory_config * dcfg = ( directory_config * ) _dcfg ;
 char * p1 = NULL ;
 if ( dcfg == NULL ) return NULL ;
 if ( _p1 == NULL ) return NULL ;
 if ( strcasecmp ( _p1 , "Rand" ) == 0 ) {
 p1 = apr_pstrdup ( cmd -> pool , getkey ( cmd -> pool ) ) ;
 dcfg -> crypto_key = p1 ;
 dcfg -> crypto_key_len = strlen ( dcfg -> crypto_key ) ;
 }
 else {
 p1 = apr_pstrdup ( cmd -> pool , _p1 ) ;
 dcfg -> crypto_key = p1 ;
 dcfg -> crypto_key_len = strlen ( p1 ) ;
 }
 if ( _p2 == NULL ) {
 return NULL ;
 }
 else {
 if ( strcasecmp ( _p2 , "KeyOnly" ) == 0 ) dcfg -> crypto_key_add = HASH_KEYONLY ;
 else if ( strcasecmp ( _p2 , "SessionID" ) == 0 ) dcfg -> crypto_key_add = HASH_SESSIONID ;
 else if ( strcasecmp ( _p2 , "RemoteIP" ) == 0 ) dcfg -> crypto_key_add = HASH_REMOTEIP ;
 }
 return NULL ;
 }