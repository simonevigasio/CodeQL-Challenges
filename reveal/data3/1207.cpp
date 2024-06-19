static int dissect_spoolss_string_parm_data ( tvbuff_t * tvb , int offset , packet_info * pinfo , proto_tree * tree , dcerpc_info * di , guint8 * drep ) {
 guint32 buffer_len , len ;
 gchar * s ;
 proto_item * item = NULL ;
 if ( di -> conformant_run ) return offset ;
 offset = dissect_ndr_uint32 ( tvb , offset , pinfo , tree , di , drep , hf_string_parm_size , & buffer_len ) ;
 s = tvb_get_stringz_enc ( wmem_packet_scope ( ) , tvb , offset , & len , ENC_UTF_16 | ENC_LITTLE_ENDIAN ) ;
 if ( tree && buffer_len ) {
 tvb_ensure_bytes_exist ( tvb , offset , buffer_len ) ;
 item = proto_tree_add_string ( tree , hf_string_parm_data , tvb , offset , len , s ) ;
 }
 offset += buffer_len ;
 col_append_fstr ( pinfo -> cinfo , COL_INFO , ", %s" , s ) ;
 if ( tree && item ) {
 item = item -> parent != NULL ? item -> parent : item ;
 proto_item_append_text ( item , ": %s" , s ) ;
 }
 return offset ;
 }