static int SpoolssRFNPCNEX_q ( tvbuff_t * tvb , int offset , packet_info * pinfo , proto_tree * tree , dcerpc_info * di , guint8 * drep ) {
 guint32 changeid ;
 offset = dissect_nt_policy_hnd ( tvb , offset , pinfo , tree , di , drep , hf_hnd , NULL , NULL , FALSE , FALSE ) ;
 offset = dissect_ndr_uint32 ( tvb , offset , pinfo , tree , di , drep , hf_rrpcn_changelow , & changeid ) ;
 col_append_fstr ( pinfo -> cinfo , COL_INFO , ", changeid %d" , changeid ) ;
 offset = dissect_ndr_pointer ( tvb , offset , pinfo , tree , di , drep , dissect_NOTIFY_OPTIONS_ARRAY_CTR , NDR_POINTER_UNIQUE , "Notify Options Array Container" , - 1 ) ;
 return offset ;
 }