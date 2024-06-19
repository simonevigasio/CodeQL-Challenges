static const char * cmd_data_dir ( cmd_parms * cmd , void * _dcfg , const char * p1 ) {
 directory_config * dcfg = ( directory_config * ) _dcfg ;
 if ( cmd -> server -> is_virtual ) {
 return "ModSecurity: SecDataDir not allowed in VirtualHost." ;
 }
 dcfg -> data_dir = ap_server_root_relative ( cmd -> pool , p1 ) ;
 return NULL ;
 }