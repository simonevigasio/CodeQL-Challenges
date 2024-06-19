static int pack_offset_sort ( const void * _a , const void * _b ) {
 const struct object_entry * a = * ( struct object_entry * * ) _a ;
 const struct object_entry * b = * ( struct object_entry * * ) _b ;
 if ( ! a -> in_pack && ! b -> in_pack ) return hashcmp ( a -> idx . sha1 , b -> idx . sha1 ) ;
 if ( a -> in_pack < b -> in_pack ) return - 1 ;
 if ( a -> in_pack > b -> in_pack ) return 1 ;
 return a -> in_pack_offset < b -> in_pack_offset ? - 1 : ( a -> in_pack_offset > b -> in_pack_offset ) ;
 }