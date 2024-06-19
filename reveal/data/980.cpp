static int SpoolssSetJob_q ( tvbuff_t * tvb , int offset , packet_info * pinfo , proto_tree * tree , dcerpc_info * di , guint8 * drep ) {
 guint32 jobid , cmd ;
 offset = dissect_nt_policy_hnd ( tvb , offset , pinfo , tree , di , drep , hf_hnd , NULL , NULL , FALSE , FALSE ) ;
 offset = dissect_ndr_uint32 ( tvb , offset , pinfo , tree , di , drep , hf_job_id , & jobid ) ;
 offset = dissect_ndr_uint32 ( tvb , offset , pinfo , tree , di , drep , hf_level , NULL ) ;
 offset = dissect_ndr_uint32 ( tvb , offset , pinfo , tree , di , drep , hf_setjob_cmd , & cmd ) ;
 col_append_fstr ( pinfo -> cinfo , COL_INFO , ", %s jobid %d" , val_to_str ( cmd , setjob_commands , "Unknown (%d)" ) , jobid ) ;
 return offset ;
 }