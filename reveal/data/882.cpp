static VALUE cState_space_before ( VALUE self ) {
 GET_STATE ( self ) ;
 return state -> space_before ? rb_str_new ( state -> space_before , state -> space_before_len ) : rb_str_new2 ( "" ) ;
 }