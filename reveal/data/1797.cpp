static VALUE mString_included_s ( VALUE self , VALUE modul ) {
 VALUE result = rb_funcall ( modul , i_extend , 1 , mString_Extend ) ;
 return result ;
 }