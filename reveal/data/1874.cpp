proto_item * proto_tree_add_text_valist_internal ( proto_tree * tree , tvbuff_t * tvb , gint start , gint length , const char * format , va_list ap ) {
 proto_item * pi ;
 header_field_info * hfinfo ;
 if ( length == - 1 ) {
 length = 0 ;
 }
 tvb_ensure_bytes_exist ( tvb , start , length ) ;
 CHECK_FOR_NULL_TREE ( tree ) ;
 TRY_TO_FAKE_THIS_ITEM ( tree , hf_text_only , hfinfo ) ;
 pi = proto_tree_add_text_node ( tree , tvb , start , length ) ;
 TRY_TO_FAKE_THIS_REPR ( pi ) ;
 proto_tree_set_representation ( pi , format , ap ) ;
 return pi ;
 }