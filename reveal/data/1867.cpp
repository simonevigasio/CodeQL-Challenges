static void find_deltas ( struct object_entry * * list , unsigned * list_size , int window , int depth , unsigned * processed ) {
 uint32_t i , idx = 0 , count = 0 ;
 struct unpacked * array ;
 unsigned long mem_usage = 0 ;
 array = xcalloc ( window , sizeof ( struct unpacked ) ) ;
 for ( ;
 ;
 ) {
 struct object_entry * entry ;
 struct unpacked * n = array + idx ;
 int j , max_depth , best_base = - 1 ;
 progress_lock ( ) ;
 if ( ! * list_size ) {
 progress_unlock ( ) ;
 break ;
 }
 entry = * list ++ ;
 ( * list_size ) -- ;
 if ( ! entry -> preferred_base ) {
 ( * processed ) ++ ;
 display_progress ( progress_state , * processed ) ;
 }
 progress_unlock ( ) ;
 mem_usage -= free_unpacked ( n ) ;
 n -> entry = entry ;
 while ( window_memory_limit && mem_usage > window_memory_limit && count > 1 ) {
 uint32_t tail = ( idx + window - count ) % window ;
 mem_usage -= free_unpacked ( array + tail ) ;
 count -- ;
 }
 if ( entry -> preferred_base ) goto next ;
 max_depth = depth ;
 if ( entry -> delta_child ) {
 max_depth -= check_delta_limit ( entry , 0 ) ;
 if ( max_depth <= 0 ) goto next ;
 }
 j = window ;
 while ( -- j > 0 ) {
 int ret ;
 uint32_t other_idx = idx + j ;
 struct unpacked * m ;
 if ( other_idx >= window ) other_idx -= window ;
 m = array + other_idx ;
 if ( ! m -> entry ) break ;
 ret = try_delta ( n , m , max_depth , & mem_usage ) ;
 if ( ret < 0 ) break ;
 else if ( ret > 0 ) best_base = other_idx ;
 }
 if ( entry -> delta_data && ! pack_to_stdout ) {
 entry -> z_delta_size = do_compress ( & entry -> delta_data , entry -> delta_size ) ;
 cache_lock ( ) ;
 delta_cache_size -= entry -> delta_size ;
 delta_cache_size += entry -> z_delta_size ;
 cache_unlock ( ) ;
 }
 if ( entry -> delta && max_depth <= n -> depth ) continue ;
 if ( entry -> delta ) {
 struct unpacked swap = array [ best_base ] ;
 int dist = ( window + idx - best_base ) % window ;
 int dst = best_base ;
 while ( dist -- ) {
 int src = ( dst + 1 ) % window ;
 array [ dst ] = array [ src ] ;
 dst = src ;
 }
 array [ dst ] = swap ;
 }
 next : idx ++ ;
 if ( count + 1 < window ) count ++ ;
 if ( idx >= window ) idx = 0 ;
 }
 for ( i = 0 ;
 i < window ;
 ++ i ) {
 free_delta_index ( array [ i ] . index ) ;
 free ( array [ i ] . data ) ;
 }
 free ( array ) ;
 }