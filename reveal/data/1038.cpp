static void config_unpeers ( config_tree * ptree ) {
 sockaddr_u peeraddr ;
 struct addrinfo hints ;
 unpeer_node * curr_unpeer ;
 struct peer * p ;
 const char * name ;
 int rc ;
 curr_unpeer = HEAD_PFIFO ( ptree -> unpeers ) ;
 for ( ;
 curr_unpeer != NULL ;
 curr_unpeer = curr_unpeer -> link ) {
 if ( curr_unpeer -> assocID ) {
 p = findpeerbyassoc ( curr_unpeer -> assocID ) ;
 if ( p != NULL ) {
 msyslog ( LOG_NOTICE , "unpeered %s" , stoa ( & p -> srcadr ) ) ;
 peer_clear ( p , "GONE" ) ;
 unpeer ( p ) ;
 }
 continue ;
 }
 memset ( & peeraddr , 0 , sizeof ( peeraddr ) ) ;
 AF ( & peeraddr ) = curr_unpeer -> addr -> type ;
 name = curr_unpeer -> addr -> address ;
 rc = getnetnum ( name , & peeraddr , 0 , t_UNK ) ;
 if ( rc > 0 ) {
 DPRINTF ( 1 , ( "unpeer: searching for %s\n" , stoa ( & peeraddr ) ) ) ;
 p = findexistingpeer ( & peeraddr , NULL , NULL , - 1 ) ;
 if ( p != NULL ) {
 msyslog ( LOG_NOTICE , "unpeered %s" , stoa ( & peeraddr ) ) ;
 peer_clear ( p , "GONE" ) ;
 unpeer ( p ) ;
 }
 continue ;
 }
 for ( p = peer_list ;
 p != NULL ;
 p = p -> p_link ) if ( p -> hostname != NULL ) if ( ! strcasecmp ( p -> hostname , name ) ) break ;
 if ( p != NULL ) {
 msyslog ( LOG_NOTICE , "unpeered %s" , name ) ;
 peer_clear ( p , "GONE" ) ;
 unpeer ( p ) ;
 }
 # ifdef WORKER memset ( & hints , 0 , sizeof ( hints ) ) ;
 hints . ai_family = curr_unpeer -> addr -> type ;
 hints . ai_socktype = SOCK_DGRAM ;
 hints . ai_protocol = IPPROTO_UDP ;
 getaddrinfo_sometime ( name , "ntp" , & hints , INITIAL_DNS_RETRY , & unpeer_name_resolved , NULL ) ;
 # else msyslog ( LOG_ERR , "hostname %s can not be used, please use IP address instead.\n" , name ) ;
 # endif }
 }