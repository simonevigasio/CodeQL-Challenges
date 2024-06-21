static void report_trash_progress ( CommonJob * job , SourceInfo * source_info , TransferInfo * transfer_info ) {
 int files_left ;
 double elapsed , transfer_rate ;
 int remaining_time ;
 gint64 now ;
 char * details ;
 char * status ;
 DeleteJob * delete_job ;
 delete_job = ( DeleteJob * ) job ;
 now = g_get_monotonic_time ( ) ;
 files_left = source_info -> num_files - transfer_info -> num_files ;
 if ( files_left < 0 ) {
 files_left = 0 ;
 }
 if ( transfer_info -> last_report_time != 0 && ABS ( ( gint64 ) ( transfer_info -> last_report_time - now ) ) < 100 * NSEC_PER_MICROSEC && files_left > 0 ) {
 return ;
 }
 transfer_info -> last_report_time = now ;
 if ( source_info -> num_files == 1 ) {
 if ( files_left > 0 ) {
 status = _ ( "Trashing “%B”" ) ;
 }
 else {
 status = _ ( "Trashed “%B”" ) ;
 }
 nautilus_progress_info_take_status ( job -> progress , f ( status , ( GFile * ) delete_job -> files -> data ) ) ;
 }
 else {
 if ( files_left > 0 ) {
 status = ngettext ( "Trashing %'d file" , "Trashing %'d files" , source_info -> num_files ) ;
 }
 else {
 status = ngettext ( "Trashed %'d file" , "Trashed %'d files" , source_info -> num_files ) ;
 }
 nautilus_progress_info_take_status ( job -> progress , f ( status , source_info -> num_files ) ) ;
 }
 elapsed = g_timer_elapsed ( job -> time , NULL ) ;
 transfer_rate = 0 ;
 remaining_time = INT_MAX ;
 if ( elapsed > 0 ) {
 transfer_rate = transfer_info -> num_files / elapsed ;
 if ( transfer_rate > 0 ) {
 remaining_time = ( source_info -> num_files - transfer_info -> num_files ) / transfer_rate ;
 }
 }
 if ( elapsed < SECONDS_NEEDED_FOR_RELIABLE_TRANSFER_RATE ) {
 if ( files_left > 0 ) {
 details = f ( _ ( "%'d / %'d" ) , transfer_info -> num_files + 1 , source_info -> num_files ) ;
 }
 else {
 details = f ( _ ( "%'d / %'d" ) , transfer_info -> num_files , source_info -> num_files ) ;
 }
 }
 else {
 if ( files_left > 0 ) {
 gchar * time_left_message ;
 gchar * files_per_second_message ;
 gchar * concat_detail ;
 time_left_message = ngettext ( "%'d / %'d \xE2\x80\x94 %T left" , "%'d / %'d \xE2\x80\x94 %T left" , seconds_count_format_time_units ( remaining_time ) ) ;
 files_per_second_message = ngettext ( "(%d file/sec)" , "(%d files/sec)" , ( int ) ( transfer_rate + 0.5 ) ) ;
 concat_detail = g_strconcat ( time_left_message , " " , files_per_second_message , NULL ) ;
 details = f ( concat_detail , transfer_info -> num_files + 1 , source_info -> num_files , remaining_time , ( int ) transfer_rate + 0.5 ) ;
 g_free ( concat_detail ) ;
 }
 else {
 details = f ( _ ( "%'d / %'d" ) , transfer_info -> num_files , source_info -> num_files ) ;
 }
 }
 nautilus_progress_info_set_details ( job -> progress , details ) ;
 if ( elapsed > SECONDS_NEEDED_FOR_APROXIMATE_TRANSFER_RATE ) {
 nautilus_progress_info_set_remaining_time ( job -> progress , remaining_time ) ;
 nautilus_progress_info_set_elapsed_time ( job -> progress , elapsed ) ;
 }
 if ( source_info -> num_files != 0 ) {
 nautilus_progress_info_set_progress ( job -> progress , transfer_info -> num_files , source_info -> num_files ) ;
 }
 }