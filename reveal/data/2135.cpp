static void Type_ProfileSequenceDesc_Free ( struct _cms_typehandler_struct * self , void * Ptr ) {
 cmsFreeProfileSequenceDescription ( ( cmsSEQ * ) Ptr ) ;
 return ;
 cmsUNUSED_PARAMETER ( self ) ;
 }