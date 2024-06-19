static const char * cmd_chroot_dir ( cmd_parms * cmd , void * _dcfg , const char * p1 ) {
 char cwd [ 1025 ] = "" ;
 if ( cmd -> server -> is_virtual ) {
 return "ModSecurity: SecChrootDir not allowed in VirtualHost" ;
 }
 chroot_dir = ( char * ) p1 ;
 if ( getcwd ( cwd , 1024 ) == NULL ) {
 return "ModSecurity: Failed to get the current working directory" ;
 }
 if ( chdir ( chroot_dir ) < 0 ) {
 return apr_psprintf ( cmd -> pool , "ModSecurity: Failed to chdir to %s, errno=%d (%s)" , chroot_dir , errno , strerror ( errno ) ) ;
 }
 if ( chdir ( cwd ) < 0 ) {
 return apr_psprintf ( cmd -> pool , "ModSecurity: Failed to chdir to %s, errno=%d (%s)" , cwd , errno , strerror ( errno ) ) ;
 }
 return NULL ;
 }