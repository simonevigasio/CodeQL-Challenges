static guint32 dissect_trunkcall_ts ( tvbuff_t * tvb , guint32 offset , proto_tree * iax2_tree , guint16 * scallno ) {
 proto_tree * call_tree ;
 guint16 datalen , rlen , ts ;
 datalen = tvb_get_ntohs ( tvb , offset ) ;
 * scallno = tvb_get_ntohs ( tvb , offset + 2 ) ;
 ts = tvb_get_ntohs ( tvb , offset + 4 ) ;
 rlen = MIN ( tvb_captured_length ( tvb ) - offset - 6 , datalen ) ;
 if ( iax2_tree ) {
 call_tree = proto_tree_add_subtree_format ( iax2_tree , tvb , offset , rlen + 6 , ett_iax2_trunk_call , NULL , "Trunk call from %u, ts: %u" , * scallno , ts ) ;
 proto_tree_add_item ( call_tree , hf_iax2_trunk_call_len , tvb , offset , 2 , ENC_BIG_ENDIAN ) ;
 proto_tree_add_item ( call_tree , hf_iax2_trunk_call_scallno , tvb , offset + 2 , 2 , ENC_BIG_ENDIAN ) ;
 proto_tree_add_item ( call_tree , hf_iax2_trunk_call_ts , tvb , offset + 4 , 2 , ENC_BIG_ENDIAN ) ;
 proto_tree_add_item ( call_tree , hf_iax2_trunk_call_data , tvb , offset + 6 , rlen , ENC_NA ) ;
 }
 offset += 6 + rlen ;
 return offset ;
 }