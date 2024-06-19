static OFCondition parsePresentationContext ( unsigned char type , PRV_PRESENTATIONCONTEXTITEM * context , unsigned char * buf , unsigned long * itemLength , unsigned long availData ) {
 unsigned long length ;
 unsigned long presentationLength ;
 OFCondition cond = EC_Normal ;
 DUL_SUBITEM * subItem ;
 if ( availData < 8 ) return makeLengthError ( "presentation context" , availData , 8 ) ;
 if ( ( context -> transferSyntaxList = LST_Create ( ) ) == NULL ) return EC_MemoryExhausted ;
 * itemLength = 0 ;
 context -> type = * buf ++ ;
 context -> rsv1 = * buf ++ ;
 EXTRACT_SHORT_BIG ( buf , context -> length ) ;
 buf += 2 ;
 context -> contextID = * buf ++ ;
 context -> rsv2 = * buf ++ ;
 context -> result = * buf ++ ;
 context -> rsv3 = * buf ++ ;
 length = context -> length ;
 * itemLength = 2 + 2 + length ;
 if ( availData - 4 < length || length < 4 ) return makeLengthError ( "presentation context" , availData , 4 , length ) ;
 DCMNET_TRACE ( "Parsing Presentation Context: (" << STD_NAMESPACE hex << STD_NAMESPACE setfill ( '0' ) << STD_NAMESPACE setw ( 2 ) << ( unsigned int ) context -> type << STD_NAMESPACE dec << "), Length: " << ( unsigned long ) context -> length << OFendl << "Presentation Context ID: " << STD_NAMESPACE hex << STD_NAMESPACE setfill ( '0' ) << STD_NAMESPACE setw ( 2 ) << ( unsigned int ) context -> contextID << STD_NAMESPACE dec ) ;
 presentationLength = length - 4 ;
 if ( ! ( ( type == DUL_TYPEPRESENTATIONCONTEXTAC ) && ( context -> result != DUL_PRESENTATION_ACCEPT ) ) ) {
 while ( presentationLength > 0 ) {
 DCMNET_TRACE ( "Parsing remaining " << presentationLength << " bytes of Presentation Context" << OFendl << "Next item type: " << STD_NAMESPACE hex << STD_NAMESPACE setfill ( '0' ) << STD_NAMESPACE setw ( 2 ) << ( unsigned int ) * buf ) ;
 switch ( * buf ) {
 case DUL_TYPEABSTRACTSYNTAX : cond = parseSubItem ( & context -> abstractSyntax , buf , & length , presentationLength ) ;
 if ( cond . bad ( ) ) return cond ;
 buf += length ;
 presentationLength -= length ;
 DCMNET_TRACE ( "Successfully parsed Abstract Syntax" ) ;
 break ;
 case DUL_TYPETRANSFERSYNTAX : subItem = ( DUL_SUBITEM * ) malloc ( sizeof ( DUL_SUBITEM ) ) ;
 if ( subItem == NULL ) return EC_MemoryExhausted ;
 cond = parseSubItem ( subItem , buf , & length , presentationLength ) ;
 if ( cond . bad ( ) ) return cond ;
 LST_Enqueue ( & context -> transferSyntaxList , ( LST_NODE * ) subItem ) ;
 buf += length ;
 presentationLength -= length ;
 DCMNET_TRACE ( "Successfully parsed Transfer Syntax" ) ;
 break ;
 default : cond = parseDummy ( buf , & length , presentationLength ) ;
 buf += length ;
 if ( ! OFStandard : : safeSubtract ( presentationLength , length , presentationLength ) ) return makeUnderflowError ( "unknown presentation context type" , presentationLength , length ) ;
 break ;
 }
 }
 }
 return EC_Normal ;
 }