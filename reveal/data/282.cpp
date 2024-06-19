static int selinux_socket_sock_rcv_skb ( struct sock * sk , struct sk_buff * skb ) {
 int err ;
 struct sk_security_struct * sksec = sk -> sk_security ;
 u16 family = sk -> sk_family ;
 u32 sk_sid = sksec -> sid ;
 struct common_audit_data ad ;
 struct lsm_network_audit net = {
 0 , }
 ;
 char * addrp ;
 u8 secmark_active ;
 u8 peerlbl_active ;
 if ( family != PF_INET && family != PF_INET6 ) return 0 ;
 if ( family == PF_INET6 && skb -> protocol == htons ( ETH_P_IP ) ) family = PF_INET ;
 if ( ! selinux_policycap_netpeer ) return selinux_sock_rcv_skb_compat ( sk , skb , family ) ;
 secmark_active = selinux_secmark_enabled ( ) ;
 peerlbl_active = selinux_peerlbl_enabled ( ) ;
 if ( ! secmark_active && ! peerlbl_active ) return 0 ;
 ad . type = LSM_AUDIT_DATA_NET ;
 ad . u . net = & net ;
 ad . u . net -> netif = skb -> skb_iif ;
 ad . u . net -> family = family ;
 err = selinux_parse_skb ( skb , & ad , & addrp , 1 , NULL ) ;
 if ( err ) return err ;
 if ( peerlbl_active ) {
 u32 peer_sid ;
 err = selinux_skb_peerlbl_sid ( skb , family , & peer_sid ) ;
 if ( err ) return err ;
 err = selinux_inet_sys_rcv_skb ( sock_net ( sk ) , skb -> skb_iif , addrp , family , peer_sid , & ad ) ;
 if ( err ) {
 selinux_netlbl_err ( skb , family , err , 0 ) ;
 return err ;
 }
 err = avc_has_perm ( sk_sid , peer_sid , SECCLASS_PEER , PEER__RECV , & ad ) ;
 if ( err ) {
 selinux_netlbl_err ( skb , family , err , 0 ) ;
 return err ;
 }
 }
 if ( secmark_active ) {
 err = avc_has_perm ( sk_sid , skb -> secmark , SECCLASS_PACKET , PACKET__RECV , & ad ) ;
 if ( err ) return err ;
 }
 return err ;
 }