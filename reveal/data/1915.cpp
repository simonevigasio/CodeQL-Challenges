int reuse_partial_packfile_from_bitmap ( struct packed_git * * packfile , uint32_t * entries , off_t * up_to ) {
 static const double REUSE_PERCENT = 0.9 ;
 struct bitmap * result = bitmap_git . result ;
 uint32_t reuse_threshold ;
 uint32_t i , reuse_objects = 0 ;
 assert ( result ) ;
 for ( i = 0 ;
 i < result -> word_alloc ;
 ++ i ) {
 if ( result -> words [ i ] != ( eword_t ) ~ 0 ) {
 reuse_objects += ewah_bit_ctz64 ( ~ result -> words [ i ] ) ;
 break ;
 }
 reuse_objects += BITS_IN_EWORD ;
 }
 # ifdef GIT_BITMAP_DEBUG {
 const unsigned char * sha1 ;
 struct revindex_entry * entry ;
 entry = & bitmap_git . reverse_index -> revindex [ reuse_objects ] ;
 sha1 = nth_packed_object_sha1 ( bitmap_git . pack , entry -> nr ) ;
 fprintf ( stderr , "Failed to reuse at %d (%016llx)\n" , reuse_objects , result -> words [ i ] ) ;
 fprintf ( stderr , " %s\n" , sha1_to_hex ( sha1 ) ) ;
 }
 # endif if ( ! reuse_objects ) return - 1 ;
 if ( reuse_objects >= bitmap_git . pack -> num_objects ) {
 bitmap_git . reuse_objects = * entries = bitmap_git . pack -> num_objects ;
 * up_to = - 1 ;
 * packfile = bitmap_git . pack ;
 return 0 ;
 }
 reuse_threshold = bitmap_popcount ( bitmap_git . result ) * REUSE_PERCENT ;
 if ( reuse_objects < reuse_threshold ) return - 1 ;
 bitmap_git . reuse_objects = * entries = reuse_objects ;
 * up_to = bitmap_git . pack -> revindex [ reuse_objects ] . offset ;
 * packfile = bitmap_git . pack ;
 return 0 ;
 }