static void config_peers ( config_tree * ptree ) {
 sockaddr_u peeraddr ;
 struct addrinfo hints ;
 peer_node * curr_peer ;
 peer_resolved_ctx * ctx ;
 u_char hmode ;
 for ( ;
 cmdline_server_count > 0 ;
 cmdline_server_count -- , cmdline_servers ++ ) {
 ZERO_SOCK ( & peeraddr ) ;
 if ( is_ip_address ( * cmdline_servers , AF_UNSPEC , & peeraddr ) ) {
 SET_PORT ( & peeraddr , NTP_PORT ) ;
 if ( is_sane_resolved_address ( & peeraddr , T_Server ) ) peer_config ( & peeraddr , NULL , NULL , MODE_CLIENT , NTP_VERSION , 0 , 0 , FLAG_IBURST , 0 , 0 , NULL ) ;
 }
 else {
 # ifdef WORKER ctx = emalloc_zero ( sizeof ( * ctx ) ) ;
 ctx -> family = AF_UNSPEC ;
 ctx -> host_mode = T_Server ;
 ctx -> hmode = MODE_CLIENT ;
 ctx -> version = NTP_VERSION ;
 ctx -> flags = FLAG_IBURST ;
 memset ( & hints , 0 , sizeof ( hints ) ) ;
 hints . ai_family = ( u_short ) ctx -> family ;
 hints . ai_socktype = SOCK_DGRAM ;
 hints . ai_protocol = IPPROTO_UDP ;
 getaddrinfo_sometime ( * cmdline_servers , "ntp" , & hints , INITIAL_DNS_RETRY , & peer_name_resolved , ( void * ) ctx ) ;
 # else msyslog ( LOG_ERR , "hostname %s can not be used, please use IP address instead.\n" , curr_peer -> addr -> address ) ;
 # endif }
 }
 curr_peer = HEAD_PFIFO ( ptree -> peers ) ;
 for ( ;
 curr_peer != NULL ;
 curr_peer = curr_peer -> link ) {
 ZERO_SOCK ( & peeraddr ) ;
 hmode = get_correct_host_mode ( curr_peer -> host_mode ) ;
 NTP_INSIST ( hmode != 0 ) ;
 if ( T_Pool == curr_peer -> host_mode ) {
 AF ( & peeraddr ) = curr_peer -> addr -> type ;
 peer_config ( & peeraddr , curr_peer -> addr -> address , NULL , hmode , curr_peer -> peerversion , curr_peer -> minpoll , curr_peer -> maxpoll , peerflag_bits ( curr_peer ) , curr_peer -> ttl , curr_peer -> peerkey , curr_peer -> group ) ;
 }
 else if ( is_ip_address ( curr_peer -> addr -> address , curr_peer -> addr -> type , & peeraddr ) ) {
 SET_PORT ( & peeraddr , NTP_PORT ) ;
 if ( is_sane_resolved_address ( & peeraddr , curr_peer -> host_mode ) ) peer_config ( & peeraddr , NULL , NULL , hmode , curr_peer -> peerversion , curr_peer -> minpoll , curr_peer -> maxpoll , peerflag_bits ( curr_peer ) , curr_peer -> ttl , curr_peer -> peerkey , curr_peer -> group ) ;
 }
 else {
 # ifdef WORKER ctx = emalloc_zero ( sizeof ( * ctx ) ) ;
 ctx -> family = curr_peer -> addr -> type ;
 ctx -> host_mode = curr_peer -> host_mode ;
 ctx -> hmode = hmode ;
 ctx -> version = curr_peer -> peerversion ;
 ctx -> minpoll = curr_peer -> minpoll ;
 ctx -> maxpoll = curr_peer -> maxpoll ;
 ctx -> flags = peerflag_bits ( curr_peer ) ;
 ctx -> ttl = curr_peer -> ttl ;
 ctx -> keyid = curr_peer -> peerkey ;
 ctx -> group = curr_peer -> group ;
 memset ( & hints , 0 , sizeof ( hints ) ) ;
 hints . ai_family = ctx -> family ;
 hints . ai_socktype = SOCK_DGRAM ;
 hints . ai_protocol = IPPROTO_UDP ;
 getaddrinfo_sometime ( curr_peer -> addr -> address , "ntp" , & hints , INITIAL_DNS_RETRY , & peer_name_resolved , ctx ) ;
 # else msyslog ( LOG_ERR , "hostname %s can not be used, please use IP address instead.\n" , curr_peer -> addr -> address ) ;
 # endif }
 }
 }