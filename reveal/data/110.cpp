static ossl_inline STACK_OF ( t1 ) * sk_ ## t1 ## _new ( sk_ ## t1 ## _compfunc compare ) {
 return ( STACK_OF ( t1 ) * ) OPENSSL_sk_new ( ( OPENSSL_sk_compfunc ) compare ) ;
 }
 static ossl_inline STACK_OF ( t1 ) * sk_ ## t1 ## _new_null ( void ) {
 return ( STACK_OF ( t1 ) * ) OPENSSL_sk_new_null ( ) ;
 }
 static ossl_inline STACK_OF ( t1 ) * sk_ ## t1 ## _new_reserve ( sk_ ## t1 ## _compfunc compare , int n ) {
 return ( STACK_OF ( t1 ) * ) OPENSSL_sk_new_reserve ( ( OPENSSL_sk_compfunc ) compare , n ) ;
 }
 static ossl_inline int sk_ ## t1 ## _reserve ( STACK_OF ( t1 ) * sk , int n ) {
 return OPENSSL_sk_reserve ( ( OPENSSL_STACK * ) sk , n ) ;
 }
 static ossl_inline void sk_ ## t1 ## _free ( STACK_OF ( t1 ) * sk ) {
 OPENSSL_sk_free ( ( OPENSSL_STACK * ) sk ) ;
 }
 static ossl_inline void sk_ ## t1 ## _zero ( STACK_OF ( t1 ) * sk ) {
 OPENSSL_sk_zero ( ( OPENSSL_STACK * ) sk ) ;
 }
 static ossl_inline t2 * sk_ ## t1 ## _delete ( STACK_OF ( t1 ) * sk , int i ) {
 return ( t2 * ) OPENSSL_sk_delete ( ( OPENSSL_STACK * ) sk , i ) ;
 }
 static ossl_inline t2 * sk_ ## t1 ## _delete_ptr ( STACK_OF ( t1 ) * sk , t2 * ptr ) {
 return ( t2 * ) OPENSSL_sk_delete_ptr ( ( OPENSSL_STACK * ) sk , ( const void * ) ptr ) ;
 }
 static ossl_inline int sk_ ## t1 ## _push ( STACK_OF ( t1 ) * sk , t2 * ptr ) {
 return OPENSSL_sk_push ( ( OPENSSL_STACK * ) sk , ( const void * ) ptr ) ;
 }
 static ossl_inline int sk_ ## t1 ## _unshift ( STACK_OF ( t1 ) * sk , t2 * ptr ) {
 return OPENSSL_sk_unshift ( ( OPENSSL_STACK * ) sk , ( const void * ) ptr ) ;
 }
 static ossl_inline t2 * sk_ ## t1 ## _pop ( STACK_OF ( t1 ) * sk ) {
 return ( t2 * ) OPENSSL_sk_pop ( ( OPENSSL_STACK * ) sk ) ;
 }
 static ossl_inline t2 * sk_ ## t1 ## _shift ( STACK_OF ( t1 ) * sk ) {
 return ( t2 * ) OPENSSL_sk_shift ( ( OPENSSL_STACK * ) sk ) ;
 }
 static ossl_inline void sk_ ## t1 ## _pop_free ( STACK_OF ( t1 ) * sk , sk_ ## t1 ## _freefunc freefunc ) {
 OPENSSL_sk_pop_free ( ( OPENSSL_STACK * ) sk , ( OPENSSL_sk_freefunc ) freefunc ) ;
 }
 static ossl_inline int sk_ ## t1 ## _insert ( STACK_OF ( t1 ) * sk , t2 * ptr , int idx ) {
 return OPENSSL_sk_insert ( ( OPENSSL_STACK * ) sk , ( const void * ) ptr , idx ) ;
 }
 static ossl_inline t2 * sk_ ## t1 ## _set ( STACK_OF ( t1 ) * sk , int idx , t2 * ptr ) {
 return ( t2 * ) OPENSSL_sk_set ( ( OPENSSL_STACK * ) sk , idx , ( const void * ) ptr ) ;
 }
 static ossl_inline int sk_ ## t1 ## _find ( STACK_OF ( t1 ) * sk , t2 * ptr ) {
 return OPENSSL_sk_find ( ( OPENSSL_STACK * ) sk , ( const void * ) ptr ) ;
 }
 static ossl_inline int sk_ ## t1 ## _find_ex ( STACK_OF ( t1 ) * sk , t2 * ptr ) {
 return OPENSSL_sk_find_ex ( ( OPENSSL_STACK * ) sk , ( const void * ) ptr ) ;
 }
 static ossl_inline void sk_ ## t1 ## _sort ( STACK_OF ( t1 ) * sk ) {
 OPENSSL_sk_sort ( ( OPENSSL_STACK * ) sk ) ;
 }
 static ossl_inline int sk_ ## t1 ## _is_sorted ( const STACK_OF ( t1 ) * sk ) {
 return OPENSSL_sk_is_sorted ( ( const OPENSSL_STACK * ) sk ) ;
 }
 static ossl_inline STACK_OF ( t1 ) * sk_ ## t1 ## _dup ( const STACK_OF ( t1 ) * sk ) {
 return ( STACK_OF ( t1 ) * ) OPENSSL_sk_dup ( ( const OPENSSL_STACK * ) sk ) ;
 }
 static ossl_inline STACK_OF ( t1 ) * sk_ ## t1 ## _deep_copy ( const STACK_OF ( t1 ) * sk , sk_ ## t1 ## _copyfunc copyfunc , sk_ ## t1 ## _freefunc freefunc ) {
 return ( STACK_OF ( t1 ) * ) OPENSSL_sk_deep_copy ( ( const OPENSSL_STACK * ) sk , ( OPENSSL_sk_copyfunc ) copyfunc , ( OPENSSL_sk_freefunc ) freefunc ) ;
 }
 static ossl_inline sk_ ## t1 ## _compfunc sk_ ## t1 ## _set_cmp_func ( STACK_OF ( t1 ) * sk , sk_ ## t1 ## _compfunc compare ) {
 return ( sk_ ## t1 ## _compfunc ) OPENSSL_sk_set_cmp_func ( ( OPENSSL_STACK * ) sk , ( OPENSSL_sk_compfunc ) compare ) ;
 }
 # define DEFINE_SPECIAL_STACK_OF ( t1 , t2 ) SKM_DEFINE_STACK_OF ( t1 , t2 , t2 ) # define DEFINE_STACK_OF ( t ) SKM_DEFINE_STACK_OF ( t , t , t ) # define DEFINE_SPECIAL_STACK_OF_CONST ( t1 , t2 ) SKM_DEFINE_STACK_OF ( t1 , const t2 , t2 ) # define DEFINE_STACK_OF_CONST ( t ) SKM_DEFINE_STACK_OF ( t , const t , t ) typedef char * OPENSSL_STRING ;
 typedef const char * OPENSSL_CSTRING ;
 DEFINE_SPECIAL_STACK_OF ( OPENSSL_STRING , char )