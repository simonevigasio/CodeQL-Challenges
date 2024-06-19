static void dtap_cc_facility ( tvbuff_t * tvb , proto_tree * tree , packet_info * pinfo _U_ , guint32 offset , guint len ) {
 guint32 curr_offset ;
 guint32 consumed ;
 guint curr_len ;
 curr_offset = offset ;
 curr_len = len ;
 is_uplink = IS_UPLINK_TRUE ;
 ELEM_MAND_LV ( GSM_A_PDU_TYPE_DTAP , DE_FACILITY , NULL ) ;
 ELEM_OPT_TLV ( 0x7f , GSM_A_PDU_TYPE_DTAP , DE_SS_VER_IND , NULL ) ;
 EXTRANEOUS_DATA_CHECK ( curr_len , 0 , pinfo , & ei_gsm_a_dtap_extraneous_data ) ;
 }