static void config_trap ( config_tree * ptree ) {
 addr_opts_node * curr_trap ;
 attr_val * curr_opt ;
 sockaddr_u addr_sock ;
 sockaddr_u peeraddr ;
 struct interface * localaddr ;
 struct addrinfo hints ;
 char port_text [ 8 ] ;
 settrap_parms * pstp ;
 u_short port ;
 int err_flag ;
 int rc ;
 AF ( & addr_sock ) = AF_UNSPEC ;
 curr_trap = HEAD_PFIFO ( ptree -> trap ) ;
 for ( ;
 curr_trap != NULL ;
 curr_trap = curr_trap -> link ) {
 err_flag = 0 ;
 port = 0 ;
 localaddr = NULL ;
 curr_opt = HEAD_PFIFO ( curr_trap -> options ) ;
 for ( ;
 curr_opt != NULL ;
 curr_opt = curr_opt -> link ) {
 if ( T_Port == curr_opt -> attr ) {
 if ( curr_opt -> value . i < 1 || curr_opt -> value . i > USHRT_MAX ) {
 msyslog ( LOG_ERR , "invalid port number " "%d, trap ignored" , curr_opt -> value . i ) ;
 err_flag = 1 ;
 }
 port = ( u_short ) curr_opt -> value . i ;
 }
 else if ( T_Interface == curr_opt -> attr ) {
 ZERO_SOCK ( & addr_sock ) ;
 if ( getnetnum ( curr_opt -> value . s , & addr_sock , 1 , t_UNK ) != 1 ) {
 err_flag = 1 ;
 break ;
 }
 localaddr = findinterface ( & addr_sock ) ;
 if ( NULL == localaddr ) {
 msyslog ( LOG_ERR , "can't find interface with address %s" , stoa ( & addr_sock ) ) ;
 err_flag = 1 ;
 }
 }
 }
 if ( ! err_flag ) {
 if ( ! port ) port = TRAPPORT ;
 ZERO_SOCK ( & peeraddr ) ;
 rc = getnetnum ( curr_trap -> addr -> address , & peeraddr , 1 , t_UNK ) ;
 if ( 1 != rc ) {
 # ifndef WORKER msyslog ( LOG_ERR , "trap: unable to use IP address %s." , curr_trap -> addr -> address ) ;
 # else memset ( & hints , 0 , sizeof ( hints ) ) ;
 hints . ai_protocol = IPPROTO_UDP ;
 hints . ai_socktype = SOCK_DGRAM ;
 snprintf ( port_text , sizeof ( port_text ) , "%u" , port ) ;
 hints . ai_flags = Z_AI_NUMERICSERV ;
 pstp = emalloc_zero ( sizeof ( * pstp ) ) ;
 if ( localaddr != NULL ) {
 hints . ai_family = localaddr -> family ;
 pstp -> ifaddr_nonnull = 1 ;
 memcpy ( & pstp -> ifaddr , & localaddr -> sin , sizeof ( pstp -> ifaddr ) ) ;
 }
 rc = getaddrinfo_sometime ( curr_trap -> addr -> address , port_text , & hints , INITIAL_DNS_RETRY , & trap_name_resolved , pstp ) ;
 if ( ! rc ) msyslog ( LOG_ERR , "config_trap: getaddrinfo_sometime(%s,%s): %m" , curr_trap -> addr -> address , port_text ) ;
 # endif continue ;
 }
 SET_PORT ( & peeraddr , port ) ;
 if ( NULL == localaddr ) localaddr = ANY_INTERFACE_CHOOSE ( & peeraddr ) ;
 else AF ( & peeraddr ) = AF ( & addr_sock ) ;
 if ( ! ctlsettrap ( & peeraddr , localaddr , 0 , NTP_VERSION ) ) msyslog ( LOG_ERR , "set trap %s -> %s failed." , latoa ( localaddr ) , stoa ( & peeraddr ) ) ;
 }
 }
 }