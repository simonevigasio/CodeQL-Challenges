int find_found ( FOUND_SET * found_set , uint table_offset , int found_offset ) {
 int i ;
 for ( i = 0 ;
 ( uint ) i < found_sets ;
 i ++ ) if ( found_set [ i ] . table_offset == table_offset && found_set [ i ] . found_offset == found_offset ) return - i - 2 ;
 found_set [ i ] . table_offset = table_offset ;
 found_set [ i ] . found_offset = found_offset ;
 found_sets ++ ;
 return - i - 2 ;
 }