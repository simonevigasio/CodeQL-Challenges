static VALUE ossl_asn1_traverse ( VALUE self , VALUE obj ) {
 unsigned char * p ;
 VALUE tmp ;
 long len , read = 0 , offset = 0 ;
 obj = ossl_to_der_if_possible ( obj ) ;
 tmp = rb_str_new4 ( StringValue ( obj ) ) ;
 p = ( unsigned char * ) RSTRING_PTR ( tmp ) ;
 len = RSTRING_LEN ( tmp ) ;
 ossl_asn1_decode0 ( & p , len , & offset , 0 , 1 , & read ) ;
 RB_GC_GUARD ( tmp ) ;
 int_ossl_decode_sanity_check ( len , read , offset ) ;
 return Qnil ;
 }