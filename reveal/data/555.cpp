static int dissect_h225_AdmissionRejectReason ( tvbuff_t * tvb _U_ , int offset _U_ , asn1_ctx_t * actx _U_ , proto_tree * tree _U_ , int hf_index _U_ ) {
 # line 669 "./asn1/h225/h225.cnf" gint32 value ;
 h225_packet_info * h225_pi ;
 h225_pi = ( h225_packet_info * ) p_get_proto_data ( wmem_packet_scope ( ) , actx -> pinfo , proto_h225 , 0 ) ;
 offset = dissect_per_choice ( tvb , offset , actx , tree , hf_index , ett_h225_AdmissionRejectReason , AdmissionRejectReason_choice , & value ) ;
 if ( h225_pi != NULL ) {
 h225_pi -> reason = value ;
 }
 return offset ;
 }