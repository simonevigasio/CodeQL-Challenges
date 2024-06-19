static const char * cmd_request_body_access ( cmd_parms * cmd , void * _dcfg , const char * p1 ) {
 directory_config * dcfg = ( directory_config * ) _dcfg ;
 if ( dcfg == NULL ) return NULL ;
 if ( strcasecmp ( p1 , "on" ) == 0 ) dcfg -> reqbody_access = 1 ;
 else if ( strcasecmp ( p1 , "off" ) == 0 ) dcfg -> reqbody_access = 0 ;
 else return apr_psprintf ( cmd -> pool , "ModSecurity: Invalid value for SecRequestBodyAccess: %s" , p1 ) ;
 return NULL ;
 }