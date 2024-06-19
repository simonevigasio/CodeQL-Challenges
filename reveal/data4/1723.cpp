static int replace_table_table ( THD * thd , GRANT_TABLE * grant_table , TABLE * table , const LEX_USER & combo , const char * db , const char * table_name , ulong rights , ulong col_rights , bool revoke_grant ) {
 char grantor [ USER_HOST_BUFF_SIZE ] ;
 int old_row_exists = 1 ;
 int error = 0 ;
 ulong store_table_rights , store_col_rights ;
 uchar user_key [ MAX_KEY_LENGTH ] ;
 DBUG_ENTER ( "replace_table_table" ) ;
 get_grantor ( thd , grantor ) ;
 if ( ! find_acl_user ( combo . host . str , combo . user . str , FALSE ) ) {
 my_message ( ER_PASSWORD_NO_MATCH , ER ( ER_PASSWORD_NO_MATCH ) , MYF ( 0 ) ) ;
 DBUG_RETURN ( - 1 ) ;
 }
 table -> use_all_columns ( ) ;
 restore_record ( table , s -> default_values ) ;
 table -> field [ 0 ] -> store ( combo . host . str , combo . host . length , system_charset_info ) ;
 table -> field [ 1 ] -> store ( db , ( uint ) strlen ( db ) , system_charset_info ) ;
 table -> field [ 2 ] -> store ( combo . user . str , combo . user . length , system_charset_info ) ;
 table -> field [ 3 ] -> store ( table_name , ( uint ) strlen ( table_name ) , system_charset_info ) ;
 store_record ( table , record [ 1 ] ) ;
 key_copy ( user_key , table -> record [ 0 ] , table -> key_info , table -> key_info -> key_length ) ;
 if ( table -> file -> index_read_idx_map ( table -> record [ 0 ] , 0 , user_key , HA_WHOLE_KEY , HA_READ_KEY_EXACT ) ) {
 if ( revoke_grant ) {
 my_error ( ER_NONEXISTING_TABLE_GRANT , MYF ( 0 ) , combo . user . str , combo . host . str , table_name ) ;
 DBUG_RETURN ( - 1 ) ;
 }
 old_row_exists = 0 ;
 restore_record ( table , record [ 1 ] ) ;
 }
 store_table_rights = get_rights_for_table ( rights ) ;
 store_col_rights = get_rights_for_column ( col_rights ) ;
 if ( old_row_exists ) {
 ulong j , k ;
 store_record ( table , record [ 1 ] ) ;
 j = ( ulong ) table -> field [ 6 ] -> val_int ( ) ;
 k = ( ulong ) table -> field [ 7 ] -> val_int ( ) ;
 if ( revoke_grant ) {
 store_table_rights = j & ~ store_table_rights ;
 }
 else {
 store_table_rights |= j ;
 store_col_rights |= k ;
 }
 }
 table -> field [ 4 ] -> store ( grantor , ( uint ) strlen ( grantor ) , system_charset_info ) ;
 table -> field [ 6 ] -> store ( ( longlong ) store_table_rights , TRUE ) ;
 table -> field [ 7 ] -> store ( ( longlong ) store_col_rights , TRUE ) ;
 rights = fix_rights_for_table ( store_table_rights ) ;
 col_rights = fix_rights_for_column ( store_col_rights ) ;
 if ( old_row_exists ) {
 if ( store_table_rights || store_col_rights ) {
 if ( ( error = table -> file -> ha_update_row ( table -> record [ 1 ] , table -> record [ 0 ] ) ) && error != HA_ERR_RECORD_IS_THE_SAME ) goto table_error ;
 }
 else if ( ( error = table -> file -> ha_delete_row ( table -> record [ 1 ] ) ) ) goto table_error ;
 }
 else {
 error = table -> file -> ha_write_row ( table -> record [ 0 ] ) ;
 if ( table -> file -> is_fatal_error ( error , HA_CHECK_DUP_KEY ) ) goto table_error ;
 }
 if ( rights | col_rights ) {
 grant_table -> privs = rights ;
 grant_table -> cols = col_rights ;
 }
 else {
 hash_delete ( & column_priv_hash , ( uchar * ) grant_table ) ;
 }
 DBUG_RETURN ( 0 ) ;
 table_error : table -> file -> print_error ( error , MYF ( 0 ) ) ;
 DBUG_RETURN ( - 1 ) ;
 }