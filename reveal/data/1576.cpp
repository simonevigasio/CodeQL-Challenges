static int dissect_h225_TBCD_STRING_SIZE_3_16 ( tvbuff_t * tvb _U_ , int offset _U_ , asn1_ctx_t * actx _U_ , proto_tree * tree _U_ , int hf_index _U_ ) {
 offset = dissect_per_size_constrained_type ( tvb , offset , actx , tree , hf_index , dissect_h225_TBCD_STRING , "TBCD_STRING" , 3 , 16 , FALSE ) ;
 return offset ;
 }