static const char * cmd_content_injection ( cmd_parms * cmd , void * _dcfg , int flag ) {
 directory_config * dcfg = ( directory_config * ) _dcfg ;
 if ( dcfg == NULL ) return NULL ;
 dcfg -> content_injection_enabled = flag ;
 return NULL ;
 }