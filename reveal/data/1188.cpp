static void xhci_ep_kick_timer ( void * opaque ) {
 XHCIEPContext * epctx = opaque ;
 xhci_kick_epctx ( epctx , 0 ) ;
 }