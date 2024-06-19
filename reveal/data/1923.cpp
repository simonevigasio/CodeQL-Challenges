static PyObject * string_rjust ( PyStringObject * self , PyObject * args ) {
 Py_ssize_t width ;
 char fillchar = ' ' ;
 if ( ! PyArg_ParseTuple ( args , "n|c:rjust" , & width , & fillchar ) ) return NULL ;
 if ( PyString_GET_SIZE ( self ) >= width && PyString_CheckExact ( self ) ) {
 Py_INCREF ( self ) ;
 return ( PyObject * ) self ;
 }
 return pad ( self , width - PyString_GET_SIZE ( self ) , 0 , fillchar ) ;
 }