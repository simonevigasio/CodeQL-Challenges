SPL_METHOD ( SplDoublyLinkedList , add ) {
 zval * zindex , * value ;
 spl_dllist_object * intern ;
 spl_ptr_llist_element * element ;
 long index ;
 if ( zend_parse_parameters ( ZEND_NUM_ARGS ( ) TSRMLS_CC , "zz" , & zindex , & value ) == FAILURE ) {
 return ;
 }
 intern = ( spl_dllist_object * ) zend_object_store_get_object ( getThis ( ) TSRMLS_CC ) ;
 index = spl_offset_convert_to_long ( zindex TSRMLS_CC ) ;
 if ( index < 0 || index > intern -> llist -> count ) {
 zend_throw_exception ( spl_ce_OutOfRangeException , "Offset invalid or out of range" , 0 TSRMLS_CC ) ;
 return ;
 }
 Z_ADDREF_P ( value ) ;
 if ( index == intern -> llist -> count ) {
 spl_ptr_llist_push ( intern -> llist , value TSRMLS_CC ) ;
 }
 else {
 spl_ptr_llist_element * elem = emalloc ( sizeof ( spl_ptr_llist_element ) ) ;
 element = spl_ptr_llist_offset ( intern -> llist , index , intern -> flags & SPL_DLLIST_IT_LIFO ) ;
 elem -> data = value ;
 elem -> rc = 1 ;
 elem -> next = element ;
 elem -> prev = element -> prev ;
 if ( elem -> prev == NULL ) {
 intern -> llist -> head = elem ;
 }
 else {
 element -> prev -> next = elem ;
 }
 element -> prev = elem ;
 intern -> llist -> count ++ ;
 if ( intern -> llist -> ctor ) {
 intern -> llist -> ctor ( elem TSRMLS_CC ) ;
 }
 }
 }