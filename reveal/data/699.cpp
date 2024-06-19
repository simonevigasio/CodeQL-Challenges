static void pdf_run_gs_UseBlackPtComp ( fz_context * ctx , pdf_processor * proc , pdf_obj * obj ) {
 pdf_run_processor * pr = ( pdf_run_processor * ) proc ;
 pdf_gstate * gstate = pdf_flush_text ( ctx , pr ) ;
 int on = pdf_name_eq ( ctx , obj , PDF_NAME_ON ) ;
 gstate -> stroke . color_params . bp = on ;
 gstate -> fill . color_params . bp = on ;
 }