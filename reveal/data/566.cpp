static void DupTagList ( struct _cmsContext_struct * ctx , const struct _cmsContext_struct * src ) {
 _cmsTagPluginChunkType newHead = {
 NULL }
 ;
 _cmsTagLinkedList * entry ;
 _cmsTagLinkedList * Anterior = NULL ;
 _cmsTagPluginChunkType * head = ( _cmsTagPluginChunkType * ) src -> chunks [ TagPlugin ] ;
 for ( entry = head -> Tag ;
 entry != NULL ;
 entry = entry -> Next ) {
 _cmsTagLinkedList * newEntry = ( _cmsTagLinkedList * ) _cmsSubAllocDup ( ctx -> MemPool , entry , sizeof ( _cmsTagLinkedList ) ) ;
 if ( newEntry == NULL ) return ;
 newEntry -> Next = NULL ;
 if ( Anterior ) Anterior -> Next = newEntry ;
 Anterior = newEntry ;
 if ( newHead . Tag == NULL ) newHead . Tag = newEntry ;
 }
 ctx -> chunks [ TagPlugin ] = _cmsSubAllocDup ( ctx -> MemPool , & newHead , sizeof ( _cmsTagPluginChunkType ) ) ;
 }