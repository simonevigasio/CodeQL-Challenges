static int SpoolssStartDocPrinter_q ( tvbuff_t * tvb , int offset , packet_info * pinfo , proto_tree * tree , dcerpc_info * di , guint8 * drep ) {
 e_ctx_hnd policy_hnd ;
 char * pol_name ;
 offset = dissect_nt_policy_hnd ( tvb , offset , pinfo , tree , di , drep , hf_hnd , & policy_hnd , NULL , FALSE , FALSE ) ;
 dcerpc_fetch_polhnd_data ( & policy_hnd , & pol_name , NULL , NULL , NULL , pinfo -> num ) ;
 if ( pol_name ) col_append_fstr ( pinfo -> cinfo , COL_INFO , ", %s" , pol_name ) ;
 offset = dissect_spoolss_doc_info_ctr ( tvb , offset , pinfo , tree , di , drep ) ;
 return offset ;
 }