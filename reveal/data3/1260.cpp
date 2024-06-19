static int register_interfaces ( extcap_parameters * extcap_conf , const char * adb_server_ip , unsigned short * adb_server_tcp_port ) {
 static char packet [ PACKET_LENGTH ] ;
 static char helpful_packet [ PACKET_LENGTH ] ;
 char * response ;
 char * device_list ;
 gssize data_length ;
 gssize device_length ;
 socket_handle_t sock ;
 const char * adb_transport_serial_templace = "%04x" "host:transport:%s" ;
 const char * adb_check_port_templace = "%04x" "shell:cat /proc/%set/tcp" ;
 const char * adb_devices = "000E" "host:devices-l" ;
 const char * adb_api_level = "0022" "shell:getprop ro.build.version.sdk" ;
 const char * adb_hcidump_version = "0017" "shell:hcidump --version" ;
 const char * adb_ps_droid_bluetooth = "0018" "shell:ps droid.bluetooth" ;
 const char * adb_ps_bluetooth_app = "001E" "shell:ps com.android.bluetooth" ;
 const char * adb_tcpdump_help = "0010" "shell:tcpdump -h" ;
 char serial_number [ SERIAL_NUMBER_LENGTH_MAX ] ;
 size_t serial_number_length ;
 char model_name [ MODEL_NAME_LENGTH_MAX ] ;
 int result ;
 char * pos ;
 char * i_pos ;
 char * model_pos ;
 char * device_pos ;
 char * prev_pos ;
 int api_level ;
 int disable_interface ;
 sock = adb_connect ( adb_server_ip , adb_server_tcp_port ) ;
 if ( sock == INVALID_SOCKET ) return EXIT_CODE_INVALID_SOCKET_INTERFACES_LIST ;
 device_list = adb_send_and_receive ( sock , adb_devices , packet , sizeof ( packet ) , & device_length ) ;
 closesocket ( sock ) ;
 if ( ! device_list ) {
 errmsg_print ( "ERROR: Cannot get list of interfaces from devices" ) ;
 return EXIT_CODE_CANNOT_GET_INTERFACES_LIST ;
 }
 device_list [ device_length ] = '\0' ;
 pos = ( char * ) device_list ;
 while ( pos < ( char * ) ( device_list + device_length ) ) {
 prev_pos = pos ;
 pos = strchr ( pos , ' ' ) ;
 i_pos = pos ;
 result = ( int ) ( pos - prev_pos ) ;
 pos = strchr ( pos , '\n' ) + 1 ;
 if ( result >= ( int ) sizeof ( serial_number ) ) {
 verbose_print ( "WARNING: Serial number too long, ignore device\n" ) ;
 continue ;
 }
 memcpy ( serial_number , prev_pos , result ) ;
 serial_number [ result ] = '\0' ;
 serial_number_length = strlen ( serial_number ) ;
 model_name [ 0 ] = '\0' ;
 model_pos = g_strstr_len ( i_pos , pos - i_pos , "model:" ) ;
 if ( model_pos ) {
 device_pos = g_strstr_len ( i_pos , pos - i_pos , "device:" ) ;
 if ( device_pos && device_pos - model_pos - 6 - 1 < MODEL_NAME_LENGTH_MAX ) {
 memcpy ( model_name , model_pos + 6 , device_pos - model_pos - 6 - 1 ) ;
 model_name [ device_pos - model_pos - 6 - 1 ] = '\0' ;
 }
 }
 if ( model_name [ 0 ] == '\0' ) strcpy ( model_name , "unknown" ) ;
 verbose_print ( "VERBOSE: Processing device: \"%s\" <%s>\n" , serial_number , model_name ) ;
 sock = adb_connect ( adb_server_ip , adb_server_tcp_port ) ;
 if ( sock == INVALID_SOCKET ) continue ;
 result = g_snprintf ( ( char * ) helpful_packet , PACKET_LENGTH , adb_transport_serial_templace , 15 + serial_number_length , serial_number ) ;
 if ( result <= 0 || result > PACKET_LENGTH ) {
 errmsg_print ( "ERROR: Error while completing adb packet for transport" ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_BAD_SIZE_OF_ASSEMBLED_ADB_PACKET_1 ;
 }
 result = adb_send ( sock , helpful_packet ) ;
 if ( result ) {
 verbose_print ( "WARNING: Error while setting adb transport for <%s>\n" , helpful_packet ) ;
 closesocket ( sock ) ;
 }
 else {
 response = adb_send_and_read ( sock , adb_tcpdump_help , helpful_packet , sizeof ( helpful_packet ) , & data_length ) ;
 closesocket ( sock ) ;
 if ( response ) {
 response [ data_length ] = '\0' ;
 if ( strstr ( response , "tcpdump version" ) ) {
 new_interface ( extcap_conf , INTERFACE_ANDROID_WIFI_TCPDUMP , model_name , serial_number , "Android WiFi" ) ;
 }
 }
 else {
 verbose_print ( "WARNING: Error on socket: <%s>\n" , helpful_packet ) ;
 }
 }
 sock = adb_connect ( adb_server_ip , adb_server_tcp_port ) ;
 if ( sock == INVALID_SOCKET ) continue ;
 result = g_snprintf ( ( char * ) helpful_packet , PACKET_LENGTH , adb_transport_serial_templace , 15 + serial_number_length , serial_number ) ;
 if ( result <= 0 || result > PACKET_LENGTH ) {
 errmsg_print ( "ERROR: Error while completing adb packet" ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_BAD_SIZE_OF_ASSEMBLED_ADB_PACKET_2 ;
 }
 result = adb_send ( sock , helpful_packet ) ;
 if ( result ) {
 verbose_print ( "WARNING: Error while setting adb transport for <%s>\n" , helpful_packet ) ;
 closesocket ( sock ) ;
 continue ;
 }
 response = adb_send_and_read ( sock , adb_api_level , helpful_packet , sizeof ( helpful_packet ) , & data_length ) ;
 closesocket ( sock ) ;
 if ( ! response ) {
 verbose_print ( "WARNING: Error on socket: <%s>\n" , helpful_packet ) ;
 continue ;
 }
 response [ data_length ] = '\0' ;
 api_level = ( int ) g_ascii_strtoll ( response , NULL , 10 ) ;
 verbose_print ( "VERBOSE: Android API Level for %s is %i\n" , serial_number , api_level ) ;
 if ( api_level < 21 ) {
 new_interface ( extcap_conf , INTERFACE_ANDROID_LOGCAT_MAIN , model_name , serial_number , "Android Logcat Main" ) ;
 new_interface ( extcap_conf , INTERFACE_ANDROID_LOGCAT_SYSTEM , model_name , serial_number , "Android Logcat System" ) ;
 new_interface ( extcap_conf , INTERFACE_ANDROID_LOGCAT_RADIO , model_name , serial_number , "Android Logcat Radio" ) ;
 new_interface ( extcap_conf , INTERFACE_ANDROID_LOGCAT_EVENTS , model_name , serial_number , "Android Logcat Events" ) ;
 }
 else {
 new_interface ( extcap_conf , INTERFACE_ANDROID_LOGCAT_TEXT_MAIN , model_name , serial_number , "Android Logcat Main" ) ;
 new_interface ( extcap_conf , INTERFACE_ANDROID_LOGCAT_TEXT_SYSTEM , model_name , serial_number , "Android Logcat System" ) ;
 new_interface ( extcap_conf , INTERFACE_ANDROID_LOGCAT_TEXT_RADIO , model_name , serial_number , "Android Logcat Radio" ) ;
 new_interface ( extcap_conf , INTERFACE_ANDROID_LOGCAT_TEXT_EVENTS , model_name , serial_number , "Android Logcat Events" ) ;
 new_interface ( extcap_conf , INTERFACE_ANDROID_LOGCAT_TEXT_CRASH , model_name , serial_number , "Android Logcat Crash" ) ;
 }
 if ( api_level >= 5 && api_level < 17 ) {
 disable_interface = 0 ;
 sock = adb_connect ( adb_server_ip , adb_server_tcp_port ) ;
 if ( sock == INVALID_SOCKET ) continue ;
 result = g_snprintf ( ( char * ) helpful_packet , PACKET_LENGTH , adb_transport_serial_templace , 15 + serial_number_length , serial_number ) ;
 if ( result <= 0 || result > PACKET_LENGTH ) {
 errmsg_print ( "ERROR: Error while completing adb packet" ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_BAD_SIZE_OF_ASSEMBLED_ADB_PACKET_3 ;
 }
 result = adb_send ( sock , helpful_packet ) ;
 if ( result ) {
 errmsg_print ( "ERROR: Error while setting adb transport for <%s>" , helpful_packet ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_GENERIC ;
 }
 response = adb_send_and_read ( sock , adb_hcidump_version , helpful_packet , sizeof ( helpful_packet ) , & data_length ) ;
 closesocket ( sock ) ;
 if ( ! response || data_length < 1 ) {
 verbose_print ( "WARNING: Error while getting hcidump version by <%s> (%p len=%" G_GSSIZE_FORMAT ")\n" , adb_hcidump_version , ( void * ) response , data_length ) ;
 verbose_print ( "VERBOSE: Android hcidump version for %s is unknown\n" , serial_number ) ;
 disable_interface = 1 ;
 }
 else {
 response [ data_length ] = '\0' ;
 if ( g_ascii_strtoull ( response , NULL , 10 ) == 0 ) {
 verbose_print ( "VERBOSE: Android hcidump version for %s is unknown\n" , serial_number ) ;
 disable_interface = 1 ;
 }
 else {
 verbose_print ( "VERBOSE: Android hcidump version for %s is %s\n" , serial_number , response ) ;
 }
 }
 if ( ! disable_interface ) {
 new_interface ( extcap_conf , INTERFACE_ANDROID_BLUETOOTH_HCIDUMP , model_name , serial_number , "Android Bluetooth Hcidump" ) ;
 }
 }
 if ( api_level >= 17 && api_level < 21 ) {
 disable_interface = 0 ;
 sock = adb_connect ( adb_server_ip , adb_server_tcp_port ) ;
 if ( sock == INVALID_SOCKET ) continue ;
 result = g_snprintf ( ( char * ) helpful_packet , PACKET_LENGTH , adb_transport_serial_templace , 15 + serial_number_length , serial_number ) ;
 if ( result <= 0 || result > PACKET_LENGTH ) {
 errmsg_print ( "ERROR: Error while completing adb packet" ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_BAD_SIZE_OF_ASSEMBLED_ADB_PACKET_4 ;
 }
 result = adb_send ( sock , helpful_packet ) ;
 if ( result ) {
 errmsg_print ( "ERROR: Error while setting adb transport for <%s>" , helpful_packet ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_GENERIC ;
 }
 response = adb_send_and_read ( sock , adb_ps_droid_bluetooth , helpful_packet , sizeof ( helpful_packet ) , & data_length ) ;
 closesocket ( sock ) ;
 if ( ! response || data_length < 1 ) {
 verbose_print ( "WARNING: Error while getting Bluetooth application process id by <%s> " "(%p len=%" G_GSSIZE_FORMAT ")\n" , adb_hcidump_version , ( void * ) response , data_length ) ;
 verbose_print ( "VERBOSE: Android Bluetooth application PID for %s is unknown\n" , serial_number ) ;
 disable_interface = 1 ;
 }
 else {
 char * data_str ;
 char pid [ 16 ] ;
 memset ( pid , 0 , sizeof ( pid ) ) ;
 response [ data_length ] = '\0' ;
 data_str = strchr ( response , '\n' ) ;
 if ( data_str && sscanf ( data_str , "%*s %s" , pid ) == 1 ) {
 verbose_print ( "VERBOSE: Android Bluetooth application PID for %s is %s\n" , serial_number , pid ) ;
 sock = adb_connect ( adb_server_ip , adb_server_tcp_port ) ;
 if ( sock == INVALID_SOCKET ) return EXIT_CODE_INVALID_SOCKET_1 ;
 result = g_snprintf ( ( char * ) helpful_packet , PACKET_LENGTH , adb_transport_serial_templace , 15 + serial_number_length , serial_number ) ;
 if ( result <= 0 || result > PACKET_LENGTH ) {
 errmsg_print ( "ERROR: Error while completing adb packet" ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_BAD_SIZE_OF_ASSEMBLED_ADB_PACKET_5 ;
 }
 result = adb_send ( sock , helpful_packet ) ;
 if ( result ) {
 errmsg_print ( "ERROR: Error while setting adb transport for <%s>" , helpful_packet ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_GENERIC ;
 }
 result = g_snprintf ( ( char * ) helpful_packet , PACKET_LENGTH , adb_check_port_templace , strlen ( adb_check_port_templace ) - 6 + strlen ( pid ) , pid ) ;
 if ( result <= 0 || result > PACKET_LENGTH ) {
 errmsg_print ( "ERROR: Error while completing adb packet" ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_BAD_SIZE_OF_ASSEMBLED_ADB_PACKET_6 ;
 }
 response = adb_send_and_read ( sock , helpful_packet , helpful_packet , sizeof ( helpful_packet ) , & data_length ) ;
 closesocket ( sock ) ;
 if ( ! response ) {
 disable_interface = 1 ;
 }
 else {
 response [ data_length ] = '\0' ;
 data_str = strchr ( response , '\n' ) ;
 if ( data_str && sscanf ( data_str , "%*s %s" , pid ) == 1 && strcmp ( pid + 9 , "10EA" ) == 0 ) {
 verbose_print ( "VERBOSE: Bluedroid External Parser Port for %s is %s\n" , serial_number , pid + 9 ) ;
 }
 else {
 disable_interface = 1 ;
 verbose_print ( "VERBOSE: Bluedroid External Parser Port for %s is unknown\n" , serial_number ) ;
 }
 }
 }
 else {
 disable_interface = 1 ;
 verbose_print ( "VERBOSE: Android Bluetooth application PID for %s is unknown\n" , serial_number ) ;
 }
 }
 if ( ! disable_interface ) {
 new_interface ( extcap_conf , INTERFACE_ANDROID_BLUETOOTH_EXTERNAL_PARSER , model_name , serial_number , "Android Bluetooth External Parser" ) ;
 }
 }
 if ( api_level >= 21 ) {
 disable_interface = 0 ;
 sock = adb_connect ( adb_server_ip , adb_server_tcp_port ) ;
 if ( sock == INVALID_SOCKET ) continue ;
 result = g_snprintf ( ( char * ) helpful_packet , PACKET_LENGTH , adb_transport_serial_templace , 15 + serial_number_length , serial_number ) ;
 if ( result <= 0 || result > PACKET_LENGTH ) {
 errmsg_print ( "ERROR: Error while completing adb packet" ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_BAD_SIZE_OF_ASSEMBLED_ADB_PACKET_7 ;
 }
 result = adb_send ( sock , helpful_packet ) ;
 if ( result ) {
 errmsg_print ( "ERROR: Error while setting adb transport for <%s>" , helpful_packet ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_GENERIC ;
 }
 if ( api_level >= 23 ) {
 response = adb_send_and_read ( sock , adb_ps_bluetooth_app , helpful_packet , sizeof ( helpful_packet ) , & data_length ) ;
 }
 else response = adb_send_and_read ( sock , adb_ps_droid_bluetooth , helpful_packet , sizeof ( helpful_packet ) , & data_length ) ;
 closesocket ( sock ) ;
 if ( ! response || data_length < 1 ) {
 verbose_print ( "WARNING: Error while getting Bluetooth application process id by <%s> " "(%p len=%" G_GSSIZE_FORMAT ")\n" , adb_hcidump_version , ( void * ) response , data_length ) ;
 verbose_print ( "VERBOSE: Android Bluetooth application PID for %s is unknown\n" , serial_number ) ;
 disable_interface = 1 ;
 }
 else {
 char * data_str ;
 char pid [ 16 ] ;
 memset ( pid , 0 , sizeof ( pid ) ) ;
 response [ data_length ] = '\0' ;
 data_str = strchr ( response , '\n' ) ;
 if ( data_str && sscanf ( data_str , "%*s %s" , pid ) == 1 ) {
 verbose_print ( "VERBOSE: Android Bluetooth application PID for %s is %s\n" , serial_number , pid ) ;
 sock = adb_connect ( adb_server_ip , adb_server_tcp_port ) ;
 if ( sock == INVALID_SOCKET ) return EXIT_CODE_INVALID_SOCKET_2 ;
 result = g_snprintf ( ( char * ) helpful_packet , PACKET_LENGTH , adb_transport_serial_templace , 15 + serial_number_length , serial_number ) ;
 if ( result <= 0 || result > PACKET_LENGTH ) {
 errmsg_print ( "ERROR: Error while completing adb packet" ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_BAD_SIZE_OF_ASSEMBLED_ADB_PACKET_8 ;
 }
 result = adb_send ( sock , helpful_packet ) ;
 if ( result ) {
 errmsg_print ( "ERROR: Error while setting adb transport for <%s>" , helpful_packet ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_GENERIC ;
 }
 result = g_snprintf ( ( char * ) helpful_packet , PACKET_LENGTH , adb_check_port_templace , strlen ( adb_check_port_templace ) - 6 + strlen ( pid ) , pid ) ;
 if ( result <= 0 || result > PACKET_LENGTH ) {
 errmsg_print ( "ERROR: Error while completing adb packet" ) ;
 closesocket ( sock ) ;
 return EXIT_CODE_BAD_SIZE_OF_ASSEMBLED_ADB_PACKET_9 ;
 }
 response = adb_send_and_read ( sock , helpful_packet , helpful_packet , sizeof ( helpful_packet ) , & data_length ) ;
 closesocket ( sock ) ;
 if ( ! response ) {
 disable_interface = 1 ;
 }
 else {
 response [ data_length ] = '\0' ;
 data_str = strchr ( response , '\n' ) ;
 if ( data_str && sscanf ( data_str , "%*s %s" , pid ) == 1 && strcmp ( pid + 9 , "22A8" ) == 0 ) {
 verbose_print ( "VERBOSE: Btsnoop Net Port for %s is %s\n" , serial_number , pid + 9 ) ;
 }
 else {
 disable_interface = 1 ;
 verbose_print ( "VERBOSE: Btsnoop Net Port for %s is unknown\n" , serial_number ) ;
 }
 }
 }
 else {
 disable_interface = 1 ;
 verbose_print ( "VERBOSE: Android Bluetooth application PID for %s is unknown\n" , serial_number ) ;
 }
 }
 if ( ! disable_interface ) {
 new_interface ( extcap_conf , INTERFACE_ANDROID_BLUETOOTH_BTSNOOP_NET , model_name , serial_number , "Android Bluetooth Btsnoop Net" ) ;
 }
 }
 }
 return EXIT_CODE_SUCCESS ;
 }