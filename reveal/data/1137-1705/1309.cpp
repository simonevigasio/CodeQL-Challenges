static int request_parse ( u8 * packet , int length , struct evdns_server_port * port , struct sockaddr * addr , socklen_t addrlen ) {
 int j = 0 ;
 u16 _t ;
 char tmp_name [ 256 ] ;
 int i ;
 u16 trans_id , flags , questions , answers , authority , additional ;
 struct server_request * server_req = NULL ;
 GET16 ( trans_id ) ;
 GET16 ( flags ) ;
 GET16 ( questions ) ;
 GET16 ( answers ) ;
 GET16 ( authority ) ;
 GET16 ( additional ) ;
 if ( flags & 0x8000 ) return - 1 ;
 flags &= 0x0110 ;
 server_req = malloc ( sizeof ( struct server_request ) ) ;
 if ( server_req == NULL ) return - 1 ;
 memset ( server_req , 0 , sizeof ( struct server_request ) ) ;
 server_req -> trans_id = trans_id ;
 memcpy ( & server_req -> addr , addr , addrlen ) ;
 server_req -> addrlen = addrlen ;
 server_req -> base . flags = flags ;
 server_req -> base . nquestions = 0 ;
 server_req -> base . questions = malloc ( sizeof ( struct evdns_server_question * ) * questions ) ;
 if ( server_req -> base . questions == NULL ) goto err ;
 for ( i = 0 ;
 i < questions ;
 ++ i ) {
 u16 type , class ;
 struct evdns_server_question * q ;
 int namelen ;
 if ( name_parse ( packet , length , & j , tmp_name , sizeof ( tmp_name ) ) < 0 ) goto err ;
 GET16 ( type ) ;
 GET16 ( class ) ;
 namelen = strlen ( tmp_name ) ;
 q = malloc ( sizeof ( struct evdns_server_question ) + namelen ) ;
 if ( ! q ) goto err ;
 q -> type = type ;
 q -> dns_question_class = class ;
 memcpy ( q -> name , tmp_name , namelen + 1 ) ;
 server_req -> base . questions [ server_req -> base . nquestions ++ ] = q ;
 }
 server_req -> port = port ;
 port -> refcnt ++ ;
 if ( flags & 0x7800 ) {
 evdns_server_request_respond ( & ( server_req -> base ) , DNS_ERR_NOTIMPL ) ;
 return - 1 ;
 }
 port -> user_callback ( & ( server_req -> base ) , port -> user_data ) ;
 return 0 ;
 err : if ( server_req ) {
 if ( server_req -> base . questions ) {
 for ( i = 0 ;
 i < server_req -> base . nquestions ;
 ++ i ) free ( server_req -> base . questions [ i ] ) ;
 free ( server_req -> base . questions ) ;
 }
 free ( server_req ) ;
 }
 return - 1 ;
 # undef SKIP_NAME # undef GET32 # undef GET16 # undef GET8 }