static int get_object_list_from_bitmap ( struct rev_info * revs ) {
 if ( prepare_bitmap_walk ( revs ) < 0 ) return - 1 ;
 if ( pack_options_allow_reuse ( ) && ! reuse_partial_packfile_from_bitmap ( & reuse_packfile , & reuse_packfile_objects , & reuse_packfile_offset ) ) {
 assert ( reuse_packfile_objects ) ;
 nr_result += reuse_packfile_objects ;
 display_progress ( progress_state , nr_result ) ;
 }
 traverse_bitmap_commit_list ( & add_object_entry_from_bitmap ) ;
 return 0 ;
 }