static int SpoolssDeletePrinterData_r ( tvbuff_t * tvb , int offset , packet_info * pinfo , proto_tree * tree , dcerpc_info * di , guint8 * drep ) {
 proto_item * hidden_item ;
 hidden_item = proto_tree_add_uint ( tree , hf_printerdata , tvb , offset , 0 , 1 ) ;
 PROTO_ITEM_SET_HIDDEN ( hidden_item ) ;
 offset = dissect_doserror ( tvb , offset , pinfo , tree , di , drep , hf_rc , NULL ) ;
 return offset ;
 }