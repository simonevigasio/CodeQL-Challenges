void revert_properties ( ) {
 if ( ! once_property ) return ;
 for ( int i = 0 ;
 i < ( int ) P_MAX ;
 i ++ ) {
 property & pr = prop_list [ i ] ;
 if ( pr . set ) {
 * pr . var = pr . old ;
 pr . set = 0 ;
 var_set_int ( pr . env_name , ( pr . old != pr . reverse ) ) ;
 }
 }
 once_property = FALSE ;
 }