static void nlm_msg_res_unmatched_value_destroy ( gpointer value ) {
 nlm_msg_res_unmatched_data * umd = ( nlm_msg_res_unmatched_data * ) value ;
 g_free ( ( gpointer ) umd -> cookie ) ;
 g_free ( umd ) ;
 }