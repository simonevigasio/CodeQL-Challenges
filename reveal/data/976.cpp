nic_rule_node * create_nic_rule_node ( int match_class , char * if_name , int action ) {
 nic_rule_node * my_node ;
 NTP_REQUIRE ( match_class != 0 || if_name != NULL ) ;
 my_node = emalloc_zero ( sizeof ( * my_node ) ) ;
 my_node -> match_class = match_class ;
 my_node -> if_name = if_name ;
 my_node -> action = action ;
 return my_node ;
 }