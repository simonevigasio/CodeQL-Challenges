char * curl_escape ( const char * string , int inlength ) {
 return curl_easy_escape ( NULL , string , inlength ) ;
 }