static int dissect_NOTIFY_OPTIONS_ARRAY ( tvbuff_t * tvb , int offset , packet_info * pinfo , proto_tree * tree , dcerpc_info * di , guint8 * drep ) {
 offset = dissect_ndr_ucarray ( tvb , offset , pinfo , tree , di , drep , dissect_NOTIFY_OPTION ) ;
 return offset ;
 }