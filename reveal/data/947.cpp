static VALUE cState_array_nl ( VALUE self ) {
 GET_STATE ( self ) ;
 return state -> array_nl ? rb_str_new ( state -> array_nl , state -> array_nl_len ) : rb_str_new2 ( "" ) ;
 }