static int dissect_usb_video_endpoint_descriptor ( proto_tree * parent_tree , tvbuff_t * tvb , guint8 descriptor_len ) {
 proto_tree * tree = NULL ;
 int offset = 0 ;
 guint8 subtype ;
 subtype = tvb_get_guint8 ( tvb , offset + 2 ) ;
 if ( parent_tree ) {
 const gchar * subtype_str ;
 subtype_str = val_to_str ( subtype , vc_ep_descriptor_subtypes , "Unknown (0x%x)" ) ;
 tree = proto_tree_add_subtree_format ( parent_tree , tvb , offset , descriptor_len , ett_descriptor_video_endpoint , NULL , "VIDEO CONTROL ENDPOINT DESCRIPTOR [%s]" , subtype_str ) ;
 }
 dissect_usb_descriptor_header ( tree , tvb , offset , & vid_descriptor_type_vals_ext ) ;
 proto_tree_add_item ( tree , hf_usb_vid_epdesc_subtype , tvb , offset + 2 , 1 , ENC_LITTLE_ENDIAN ) ;
 offset += 3 ;
 if ( subtype == EP_INTERRUPT ) {
 proto_tree_add_item ( tree , hf_usb_vid_epdesc_max_transfer_sz , tvb , offset , 2 , ENC_LITTLE_ENDIAN ) ;
 offset += 2 ;
 }
 if ( offset < descriptor_len ) proto_tree_add_item ( tree , hf_usb_vid_descriptor_data , tvb , offset , descriptor_len - offset , ENC_NA ) ;
 return descriptor_len ;
 }