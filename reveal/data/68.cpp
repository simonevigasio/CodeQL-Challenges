static HA_KEYSEG * ha_find_null ( HA_KEYSEG * keyseg , uchar * a ) {
 for ( ;
 ( enum ha_base_keytype ) keyseg -> type != HA_KEYTYPE_END ;
 keyseg ++ ) {
 uchar * end ;
 if ( keyseg -> null_bit ) {
 if ( ! * a ++ ) return keyseg ;
 }
 end = a + keyseg -> length ;
 switch ( ( enum ha_base_keytype ) keyseg -> type ) {
 case HA_KEYTYPE_TEXT : case HA_KEYTYPE_BINARY : case HA_KEYTYPE_BIT : if ( keyseg -> flag & HA_SPACE_PACK ) {
 int a_length ;
 get_key_length ( a_length , a ) ;
 a += a_length ;
 break ;
 }
 else a = end ;
 break ;
 case HA_KEYTYPE_VARTEXT1 : case HA_KEYTYPE_VARTEXT2 : case HA_KEYTYPE_VARBINARY1 : case HA_KEYTYPE_VARBINARY2 : {
 int a_length ;
 get_key_length ( a_length , a ) ;
 a += a_length ;
 break ;
 }
 case HA_KEYTYPE_NUM : if ( keyseg -> flag & HA_SPACE_PACK ) {
 int alength = * a ++ ;
 end = a + alength ;
 }
 a = end ;
 break ;
 case HA_KEYTYPE_INT8 : case HA_KEYTYPE_SHORT_INT : case HA_KEYTYPE_USHORT_INT : case HA_KEYTYPE_LONG_INT : case HA_KEYTYPE_ULONG_INT : case HA_KEYTYPE_INT24 : case HA_KEYTYPE_UINT24 : # ifdef HAVE_LONG_LONG case HA_KEYTYPE_LONGLONG : case HA_KEYTYPE_ULONGLONG : # endif case HA_KEYTYPE_FLOAT : case HA_KEYTYPE_DOUBLE : a = end ;
 break ;
 case HA_KEYTYPE_END : DBUG_ASSERT ( 0 ) ;
 break ;
 }
 }
 return keyseg ;
 }