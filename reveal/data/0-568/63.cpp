static void Type_NamedColor_Free ( struct _cms_typehandler_struct * self , void * Ptr ) {
 cmsFreeNamedColorList ( ( cmsNAMEDCOLORLIST * ) Ptr ) ;
 return ;
 cmsUNUSED_PARAMETER ( self ) ;
 }