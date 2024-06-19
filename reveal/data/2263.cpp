static void pdf_run_gs_CA ( fz_context * ctx , pdf_processor * proc , float alpha ) {
 pdf_run_processor * pr = ( pdf_run_processor * ) proc ;
 pdf_gstate * gstate = pdf_flush_text ( ctx , pr ) ;
 gstate -> stroke . alpha = fz_clamp ( alpha , 0 , 1 ) ;
 }