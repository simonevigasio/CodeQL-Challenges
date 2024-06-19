static struct pbase_tree_cache * pbase_tree_get ( const unsigned char * sha1 ) {
 struct pbase_tree_cache * ent , * nent ;
 void * data ;
 unsigned long size ;
 enum object_type type ;
 int neigh ;
 int my_ix = pbase_tree_cache_ix ( sha1 ) ;
 int available_ix = - 1 ;
 for ( neigh = 0 ;
 neigh < 8 ;
 neigh ++ ) {
 ent = pbase_tree_cache [ my_ix ] ;
 if ( ent && ! hashcmp ( ent -> sha1 , sha1 ) ) {
 ent -> ref ++ ;
 return ent ;
 }
 else if ( ( ( available_ix < 0 ) && ( ! ent || ! ent -> ref ) ) || ( ( 0 <= available_ix ) && ( ! ent && pbase_tree_cache [ available_ix ] ) ) ) available_ix = my_ix ;
 if ( ! ent ) break ;
 my_ix = pbase_tree_cache_ix_incr ( my_ix ) ;
 }
 data = read_sha1_file ( sha1 , & type , & size ) ;
 if ( ! data ) return NULL ;
 if ( type != OBJ_TREE ) {
 free ( data ) ;
 return NULL ;
 }
 if ( available_ix < 0 ) ent = NULL ;
 else {
 ent = pbase_tree_cache [ available_ix ] ;
 my_ix = available_ix ;
 }
 if ( ! ent ) {
 nent = xmalloc ( sizeof ( * nent ) ) ;
 nent -> temporary = ( available_ix < 0 ) ;
 }
 else {
 free ( ent -> tree_data ) ;
 nent = ent ;
 }
 hashcpy ( nent -> sha1 , sha1 ) ;
 nent -> tree_data = data ;
 nent -> tree_size = size ;
 nent -> ref = 1 ;
 if ( ! nent -> temporary ) pbase_tree_cache [ my_ix ] = nent ;
 return nent ;
 }