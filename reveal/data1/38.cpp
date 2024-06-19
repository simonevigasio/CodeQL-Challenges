static int32_t u_scanf_octal_handler ( UFILE * input , u_scanf_spec_info * info , ufmt_args * args , const UChar * fmt , int32_t * fmtConsumed , int32_t * argConverted ) {
 int32_t len ;
 int32_t skipped ;
 void * num = ( void * ) ( args [ 0 ] . ptrValue ) ;
 int64_t result ;
 skipped = u_scanf_skip_leading_ws ( input , info -> fPadChar ) ;
 ufile_fill_uchar_buffer ( input ) ;
 len = ( int32_t ) ( input -> str . fLimit - input -> str . fPos ) ;
 if ( info -> fWidth != - 1 ) len = ufmt_min ( len , info -> fWidth ) ;
 result = ufmt_uto64 ( input -> str . fPos , & len , 8 ) ;
 input -> str . fPos += len ;
 if ( ! info -> fSkipArg ) {
 if ( info -> fIsShort ) * ( int16_t * ) num = ( int16_t ) ( UINT16_MAX & result ) ;
 else if ( info -> fIsLongLong ) * ( int64_t * ) num = result ;
 else * ( int32_t * ) num = ( int32_t ) ( UINT32_MAX & result ) ;
 }
 * argConverted = ! info -> fSkipArg ;
 return len + skipped ;
 }