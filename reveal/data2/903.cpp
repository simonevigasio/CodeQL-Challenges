static void pk_proxy_appeared_cb ( GObject * source , GAsyncResult * res , gpointer user_data ) {
 ActivateParametersInstall * parameters_install = user_data ;
 char * mime_type , * name_owner ;
 char * error_message ;
 GtkWidget * dialog ;
 GDBusProxy * proxy ;
 GError * error = NULL ;
 proxy = g_dbus_proxy_new_for_bus_finish ( res , & error ) ;
 name_owner = g_dbus_proxy_get_name_owner ( proxy ) ;
 if ( error != NULL || name_owner == NULL ) {
 g_warning ( "Couldn't call Modify on the PackageKit interface: %s" , error != NULL ? error -> message : "no owner for PackageKit" ) ;
 g_clear_error ( & error ) ;
 show_unhandled_type_error ( parameters_install ) ;
 return ;
 }
 g_free ( name_owner ) ;
 mime_type = nautilus_file_get_mime_type ( parameters_install -> file ) ;
 error_message = get_application_no_mime_type_handler_message ( parameters_install -> file , parameters_install -> uri ) ;
 dialog = gtk_message_dialog_new ( parameters_install -> parent_window , 0 , GTK_MESSAGE_ERROR , GTK_BUTTONS_YES_NO , "%s" , error_message ) ;
 gtk_message_dialog_format_secondary_text ( GTK_MESSAGE_DIALOG ( dialog ) , _ ( "There is no application installed for “%s” files.\n" "Do you want to search for an application to open this file?" ) , g_content_type_get_description ( mime_type ) ) ;
 gtk_window_set_resizable ( GTK_WINDOW ( dialog ) , FALSE ) ;
 parameters_install -> dialog = dialog ;
 parameters_install -> proxy = proxy ;
 g_signal_connect ( dialog , "response" , G_CALLBACK ( application_unhandled_file_install ) , parameters_install ) ;
 g_signal_connect ( dialog , "delete-event" , G_CALLBACK ( delete_cb ) , NULL ) ;
 gtk_widget_show_all ( dialog ) ;
 g_free ( mime_type ) ;
 }