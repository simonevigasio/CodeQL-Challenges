static OFCondition makeUnderflowError ( const char * pdu , unsigned long minuend , unsigned long subtrahend ) {
 OFStringStream stream ;
 stream << "DUL Illegal " << pdu << ". Got " << minuend << " bytes of data and told to subtrat " << subtrahend << " bytes of data" ;
 stream << "." << OFStringStream_ends ;
 OFCondition ret ;
 OFSTRINGSTREAM_GETSTR ( stream , tmpString ) ret = makeDcmnetCondition ( DULC_INCORRECTBUFFERLENGTH , OF_error , tmpString ) ;
 OFSTRINGSTREAM_FREESTR ( tmpString ) return ret ;
 }