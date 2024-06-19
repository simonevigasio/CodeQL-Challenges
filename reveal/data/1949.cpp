static struct sock * udp_v6_mcast_next ( struct net * net , struct sock * sk , __be16 loc_port , struct in6_addr * loc_addr , __be16 rmt_port , struct in6_addr * rmt_addr , int dif ) {
 struct hlist_nulls_node * node ;
 struct sock * s = sk ;
 unsigned short num = ntohs ( loc_port ) ;
 sk_nulls_for_each_from ( s , node ) {
 struct inet_sock * inet = inet_sk ( s ) ;
 if ( ! net_eq ( sock_net ( s ) , net ) ) continue ;
 if ( udp_sk ( s ) -> udp_port_hash == num && s -> sk_family == PF_INET6 ) {
 struct ipv6_pinfo * np = inet6_sk ( s ) ;
 if ( inet -> inet_dport ) {
 if ( inet -> inet_dport != rmt_port ) continue ;
 }
 if ( ! ipv6_addr_any ( & np -> daddr ) && ! ipv6_addr_equal ( & np -> daddr , rmt_addr ) ) continue ;
 if ( s -> sk_bound_dev_if && s -> sk_bound_dev_if != dif ) continue ;
 if ( ! ipv6_addr_any ( & np -> rcv_saddr ) ) {
 if ( ! ipv6_addr_equal ( & np -> rcv_saddr , loc_addr ) ) continue ;
 }
 if ( ! inet6_mc_check ( s , loc_addr , rmt_addr ) ) continue ;
 return s ;
 }
 }
 return NULL ;
 }