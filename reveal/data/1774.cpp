void append_metadata ( DYNAMIC_STRING * ds , MYSQL_FIELD * field , uint num_fields ) {
 MYSQL_FIELD * field_end ;
 dynstr_append ( ds , "Catalog\tDatabase\tTable\tTable_alias\tColumn\t" "Column_alias\tType\tLength\tMax length\tIs_null\t" "Flags\tDecimals\tCharsetnr\n" ) ;
 for ( field_end = field + num_fields ;
 field < field_end ;
 field ++ ) {
 dynstr_append_mem ( ds , field -> catalog , field -> catalog_length ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 dynstr_append_mem ( ds , field -> db , field -> db_length ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 dynstr_append_mem ( ds , field -> org_table , field -> org_table_length ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 dynstr_append_mem ( ds , field -> table , field -> table_length ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 dynstr_append_mem ( ds , field -> org_name , field -> org_name_length ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 dynstr_append_mem ( ds , field -> name , field -> name_length ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 replace_dynstr_append_uint ( ds , field -> type ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 replace_dynstr_append_uint ( ds , field -> length ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 replace_dynstr_append_uint ( ds , field -> max_length ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 dynstr_append_mem ( ds , ( char * ) ( IS_NOT_NULL ( field -> flags ) ? "N" : "Y" ) , 1 ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 replace_dynstr_append_uint ( ds , field -> flags ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 replace_dynstr_append_uint ( ds , field -> decimals ) ;
 dynstr_append_mem ( ds , "\t" , 1 ) ;
 replace_dynstr_append_uint ( ds , field -> charsetnr ) ;
 dynstr_append_mem ( ds , "\n" , 1 ) ;
 }
 }