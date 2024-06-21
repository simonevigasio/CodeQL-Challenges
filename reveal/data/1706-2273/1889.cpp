static const char * cmd_resquest_body_limit_action ( cmd_parms * cmd , void * _dcfg , const char * p1 ) {
 directory_config * dcfg = ( directory_config * ) _dcfg ;
 if ( dcfg == NULL ) return NULL ;
 if ( dcfg -> is_enabled == MODSEC_DETECTION_ONLY ) {
 dcfg -> if_limit_action = REQUEST_BODY_LIMIT_ACTION_PARTIAL ;
 return NULL ;
 }
 if ( strcasecmp ( p1 , "ProcessPartial" ) == 0 ) dcfg -> if_limit_action = REQUEST_BODY_LIMIT_ACTION_PARTIAL ;
 else if ( strcasecmp ( p1 , "Reject" ) == 0 ) dcfg -> if_limit_action = REQUEST_BODY_LIMIT_ACTION_REJECT ;
 else return apr_psprintf ( cmd -> pool , "ModSecurity: Invalid value for SecRequestBodyLimitAction: %s" , p1 ) ;
 return NULL ;
 }