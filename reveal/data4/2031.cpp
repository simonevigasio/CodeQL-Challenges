static void generate_json_fixnum ( FBuffer * buffer , VALUE Vstate , JSON_Generator_State * state , VALUE obj ) {
 fbuffer_append_long ( buffer , FIX2LONG ( obj ) ) ;
 }