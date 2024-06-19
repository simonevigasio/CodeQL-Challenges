static PyObject * _cbson_dict_to_bson ( PyObject * self , PyObject * args ) {
 PyObject * dict ;
 PyObject * result ;
 unsigned char check_keys ;
 unsigned char uuid_subtype ;
 unsigned char top_level = 1 ;
 buffer_t buffer ;
 if ( ! PyArg_ParseTuple ( args , "Obb|b" , & dict , & check_keys , & uuid_subtype , & top_level ) ) {
 return NULL ;
 }
 buffer = buffer_new ( ) ;
 if ( ! buffer ) {
 PyErr_NoMemory ( ) ;
 return NULL ;
 }
 if ( ! write_dict ( self , buffer , dict , check_keys , uuid_subtype , top_level ) ) {
 buffer_free ( buffer ) ;
 return NULL ;
 }
 # if PY_MAJOR_VERSION >= 3 result = Py_BuildValue ( "y#" , buffer_get_buffer ( buffer ) , buffer_get_position ( buffer ) ) ;
 # else result = Py_BuildValue ( "s#" , buffer_get_buffer ( buffer ) , buffer_get_position ( buffer ) ) ;
 # endif buffer_free ( buffer ) ;
 return result ;
 }