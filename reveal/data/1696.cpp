apr_status_t xml_cleanup ( modsec_rec * msr ) {
 if ( msr -> xml -> doc != NULL ) {
 xmlFreeDoc ( msr -> xml -> doc ) ;
 msr -> xml -> doc = NULL ;
 }
 return 1 ;
 }