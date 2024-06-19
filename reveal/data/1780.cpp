static char * CreateHtmlSubtitle ( int * pi_align , char * psz_subtitle ) {
 char * psz_tag = malloc ( 1 ) ;
 if ( psz_tag == NULL ) return NULL ;
 char * psz_html = malloc ( 1 ) ;
 if ( psz_html == NULL ) {
 free ( psz_tag ) ;
 return NULL ;
 }
 psz_tag [ 0 ] = '\0' ;
 psz_html [ 0 ] = '\0' ;
 bool b_has_align = false ;
 HtmlPut ( & psz_html , "<text>" ) ;
 while ( * psz_subtitle ) {
 if ( * psz_subtitle == '\n' ) {
 HtmlPut ( & psz_html , "<br/>" ) ;
 psz_subtitle ++ ;
 }
 else if ( * psz_subtitle == '<' ) {
 if ( ! strncasecmp ( psz_subtitle , "<br/>" , 5 ) ) {
 HtmlCopy ( & psz_html , & psz_subtitle , "<br/>" ) ;
 }
 else if ( ! strncasecmp ( psz_subtitle , "<b>" , 3 ) ) {
 HtmlCopy ( & psz_html , & psz_subtitle , "<b>" ) ;
 HtmlPut ( & psz_tag , "b" ) ;
 }
 else if ( ! strncasecmp ( psz_subtitle , "<i>" , 3 ) ) {
 HtmlCopy ( & psz_html , & psz_subtitle , "<i>" ) ;
 HtmlPut ( & psz_tag , "i" ) ;
 }
 else if ( ! strncasecmp ( psz_subtitle , "<u>" , 3 ) ) {
 HtmlCopy ( & psz_html , & psz_subtitle , "<u>" ) ;
 HtmlPut ( & psz_tag , "u" ) ;
 }
 else if ( ! strncasecmp ( psz_subtitle , "<s>" , 3 ) ) {
 HtmlCopy ( & psz_html , & psz_subtitle , "<s>" ) ;
 HtmlPut ( & psz_tag , "s" ) ;
 }
 else if ( ! strncasecmp ( psz_subtitle , "<font " , 6 ) ) {
 const char * psz_attribs [ ] = {
 "face=" , "family=" , "size=" , "color=" , "outline-color=" , "shadow-color=" , "outline-level=" , "shadow-level=" , "back-color=" , "alpha=" , NULL }
 ;
 HtmlCopy ( & psz_html , & psz_subtitle , "<font " ) ;
 HtmlPut ( & psz_tag , "f" ) ;
 while ( * psz_subtitle != '>' && * psz_subtitle ) {
 int k ;
 while ( * psz_subtitle == ' ' ) psz_subtitle ++ ;
 for ( k = 0 ;
 psz_attribs [ k ] ;
 k ++ ) {
 int i_len = strlen ( psz_attribs [ k ] ) ;
 if ( ! strncasecmp ( psz_subtitle , psz_attribs [ k ] , i_len ) ) {
 HtmlPut ( & psz_html , psz_attribs [ k ] ) ;
 psz_subtitle += i_len ;
 while ( * psz_subtitle == ' ' ) psz_subtitle ++ ;
 if ( * psz_subtitle == '"' ) {
 psz_subtitle ++ ;
 i_len = strcspn ( psz_subtitle , "\"" ) ;
 }
 else if ( * psz_subtitle == '\'' ) {
 psz_subtitle ++ ;
 i_len = strcspn ( psz_subtitle , "'" ) ;
 }
 else {
 i_len = strcspn ( psz_subtitle , " \t>" ) ;
 }
 HtmlPut ( & psz_html , "\"" ) ;
 HtmlNPut ( & psz_html , psz_subtitle , i_len ) ;
 HtmlPut ( & psz_html , "\"" ) ;
 psz_subtitle += i_len ;
 if ( * psz_subtitle == '\"' || * psz_subtitle == '\'' ) psz_subtitle ++ ;
 break ;
 }
 }
 if ( psz_attribs [ k ] == NULL ) {
 int i_len = strcspn ( psz_subtitle , "\"" ) ;
 if ( psz_subtitle [ i_len ] == '\"' ) {
 i_len += 1 + strcspn ( & psz_subtitle [ i_len + 1 ] , "\"" ) ;
 if ( psz_subtitle [ i_len ] == '\"' ) i_len ++ ;
 }
 if ( i_len == 0 && * psz_subtitle != '\0' ) psz_subtitle ++ ;
 psz_subtitle += i_len ;
 }
 HtmlNPut ( & psz_html , psz_subtitle , strspn ( psz_subtitle , " " ) ) ;
 }
 HtmlPut ( & psz_html , ">" ) ;
 if ( * psz_subtitle == '\0' ) break ;
 psz_subtitle ++ ;
 }
 else if ( ! strncmp ( psz_subtitle , "</" , 2 ) ) {
 bool b_match = false ;
 bool b_ignore = false ;
 int i_len = ( psz_tag ? strlen ( psz_tag ) : 0 ) - 1 ;
 char * psz_lastTag = NULL ;
 if ( i_len >= 0 ) {
 psz_lastTag = psz_tag + i_len ;
 i_len = 0 ;
 switch ( * psz_lastTag ) {
 case 'b' : b_match = ! strncasecmp ( psz_subtitle , "</b>" , 4 ) ;
 i_len = 4 ;
 break ;
 case 'i' : b_match = ! strncasecmp ( psz_subtitle , "</i>" , 4 ) ;
 i_len = 4 ;
 break ;
 case 'u' : b_match = ! strncasecmp ( psz_subtitle , "</u>" , 4 ) ;
 i_len = 4 ;
 break ;
 case 's' : b_match = ! strncasecmp ( psz_subtitle , "</s>" , 4 ) ;
 i_len = 4 ;
 break ;
 case 'f' : b_match = ! strncasecmp ( psz_subtitle , "</font>" , 7 ) ;
 i_len = 7 ;
 break ;
 case 'I' : i_len = strcspn ( psz_subtitle , ">" ) ;
 b_match = psz_subtitle [ i_len ] == '>' ;
 b_ignore = true ;
 if ( b_match ) i_len ++ ;
 break ;
 }
 }
 if ( ! b_match ) {
 free ( psz_html ) ;
 psz_html = NULL ;
 break ;
 }
 * psz_lastTag = '\0' ;
 if ( ! b_ignore ) HtmlNPut ( & psz_html , psz_subtitle , i_len ) ;
 psz_subtitle += i_len ;
 }
 else if ( ( psz_subtitle [ 1 ] < 'a' || psz_subtitle [ 1 ] > 'z' ) && ( psz_subtitle [ 1 ] < 'A' || psz_subtitle [ 1 ] > 'Z' ) ) {
 HtmlPut ( & psz_html , "&lt;
" ) ;
 psz_subtitle ++ ;
 }
 else {
 char * psz_stop = psz_subtitle + 1 + strcspn ( & psz_subtitle [ 1 ] , "<>" ) ;
 char * psz_closing = strstr ( psz_subtitle , "/>" ) ;
 if ( psz_closing && psz_closing < psz_stop ) {
 psz_subtitle = & psz_closing [ 2 ] ;
 }
 else if ( * psz_stop == '>' ) {
 char psz_match [ 256 ] ;
 snprintf ( psz_match , sizeof ( psz_match ) , "</%s" , & psz_subtitle [ 1 ] ) ;
 psz_match [ strcspn ( psz_match , " \t>" ) ] = '\0' ;
 if ( strstr ( psz_subtitle , psz_match ) ) {
 psz_subtitle = & psz_stop [ 1 ] ;
 HtmlPut ( & psz_tag , "I" ) ;
 }
 else {
 int i_len = psz_stop + 1 - psz_subtitle ;
 for ( ;
 i_len > 0 ;
 i_len -- , psz_subtitle ++ ) {
 if ( * psz_subtitle == '<' ) HtmlPut ( & psz_html , "&lt;
" ) ;
 else if ( * psz_subtitle == '>' ) HtmlPut ( & psz_html , "&gt;
" ) ;
 else HtmlNPut ( & psz_html , psz_subtitle , 1 ) ;
 }
 }
 }
 else {
 HtmlPut ( & psz_html , "&lt;
" ) ;
 psz_subtitle ++ ;
 }
 }
 }
 else if ( * psz_subtitle == '&' ) {
 if ( ! strncasecmp ( psz_subtitle , "&lt;
" , 4 ) ) {
 HtmlCopy ( & psz_html , & psz_subtitle , "&lt;
" ) ;
 }
 else if ( ! strncasecmp ( psz_subtitle , "&gt;
" , 4 ) ) {
 HtmlCopy ( & psz_html , & psz_subtitle , "&gt;
" ) ;
 }
 else if ( ! strncasecmp ( psz_subtitle , "&amp;
" , 5 ) ) {
 HtmlCopy ( & psz_html , & psz_subtitle , "&amp;
" ) ;
 }
 else {
 HtmlPut ( & psz_html , "&amp;
" ) ;
 psz_subtitle ++ ;
 }
 }
 else if ( * psz_subtitle == '>' ) {
 HtmlPut ( & psz_html , "&gt;
" ) ;
 psz_subtitle ++ ;
 }
 else if ( psz_subtitle [ 0 ] == '{
' && psz_subtitle [ 1 ] == '\\' && strchr ( psz_subtitle , '}
' ) ) {
 if ( ! b_has_align && ! strncmp ( psz_subtitle , "{
\\an" , 4 ) && psz_subtitle [ 4 ] >= '1' && psz_subtitle [ 4 ] <= '9' && psz_subtitle [ 5 ] == '}
' ) {
 static const int pi_vertical [ 3 ] = {
 SUBPICTURE_ALIGN_BOTTOM , 0 , SUBPICTURE_ALIGN_TOP }
 ;
 static const int pi_horizontal [ 3 ] = {
 SUBPICTURE_ALIGN_LEFT , 0 , SUBPICTURE_ALIGN_RIGHT }
 ;
 const int i_id = psz_subtitle [ 4 ] - '1' ;
 b_has_align = true ;
 * pi_align = pi_vertical [ i_id / 3 ] | pi_horizontal [ i_id % 3 ] ;
 }
 psz_subtitle = strchr ( psz_subtitle , '}
' ) + 1 ;
 }
 else if ( psz_subtitle [ 0 ] == '{
' && ( psz_subtitle [ 1 ] == 'Y' || psz_subtitle [ 1 ] == 'y' ) && psz_subtitle [ 2 ] == ':' && strchr ( psz_subtitle , '}
' ) ) {
 if ( psz_subtitle [ 3 ] == 'i' ) {
 HtmlPut ( & psz_html , "<i>" ) ;
 HtmlPut ( & psz_tag , "i" ) ;
 }
 if ( psz_subtitle [ 3 ] == 'b' ) {
 HtmlPut ( & psz_html , "<b>" ) ;
 HtmlPut ( & psz_tag , "b" ) ;
 }
 if ( psz_subtitle [ 3 ] == 'u' ) {
 HtmlPut ( & psz_html , "<u>" ) ;
 HtmlPut ( & psz_tag , "u" ) ;
 }
 psz_subtitle = strchr ( psz_subtitle , '}
' ) + 1 ;
 }
 else if ( psz_subtitle [ 0 ] == '{
' && psz_subtitle [ 1 ] != '\0' && psz_subtitle [ 2 ] == ':' && strchr ( psz_subtitle , '}
' ) ) {
 psz_subtitle = strchr ( psz_subtitle , '}
' ) + 1 ;
 }
 else if ( psz_subtitle [ 0 ] == '\\' && psz_subtitle [ 1 ] ) {
 if ( psz_subtitle [ 1 ] == 'N' || psz_subtitle [ 1 ] == 'n' ) {
 HtmlPut ( & psz_html , "<br/>" ) ;
 psz_subtitle += 2 ;
 }
 else if ( psz_subtitle [ 1 ] == 'h' ) {
 HtmlPut ( & psz_html , NO_BREAKING_SPACE ) ;
 psz_subtitle += 2 ;
 }
 else {
 HtmlPut ( & psz_html , "\\" ) ;
 psz_subtitle ++ ;
 }
 }
 else {
 HtmlNPut ( & psz_html , psz_subtitle , 1 ) ;
 # if 0 if ( * psz_html ) {
 # error This test does not make sense . if ( ( * psz_html == ' ' || * psz_html == '\t' ) && ( * ( psz_html - 1 ) == ' ' || * ( psz_html - 1 ) == '\t' ) ) {
 HtmlPut ( & psz_html , NO_BREAKING_SPACE ) ;
 psz_html -- ;
 }
 }
 # endif psz_subtitle ++ ;
 }
 }
 while ( psz_tag && * psz_tag ) {
 char * psz_last = & psz_tag [ strlen ( psz_tag ) - 1 ] ;
 switch ( * psz_last ) {
 case 'b' : HtmlPut ( & psz_html , "</b>" ) ;
 break ;
 case 'i' : HtmlPut ( & psz_html , "</i>" ) ;
 break ;
 case 'u' : HtmlPut ( & psz_html , "</u>" ) ;
 break ;
 case 's' : HtmlPut ( & psz_html , "</s>" ) ;
 break ;
 case 'f' : HtmlPut ( & psz_html , "</font>" ) ;
 break ;
 case 'I' : break ;
 }
 * psz_last = '\0' ;
 }
 HtmlPut ( & psz_html , "</text>" ) ;
 free ( psz_tag ) ;
 return psz_html ;
 }