static cmsBool ReadOneElem ( cmsIOHANDLER * io , _cmsDICelem * e , cmsUInt32Number i , cmsUInt32Number BaseOffset ) {
 if ( ! _cmsReadUInt32Number ( io , & e -> Offsets [ i ] ) ) return FALSE ;
 if ( ! _cmsReadUInt32Number ( io , & e -> Sizes [ i ] ) ) return FALSE ;
 if ( e -> Offsets [ i ] > 0 ) e -> Offsets [ i ] += BaseOffset ;
 return TRUE ;
 }