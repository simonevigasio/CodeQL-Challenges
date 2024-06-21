static void _slurm_rpc_job_will_run ( slurm_msg_t * msg ) {
 DEF_TIMERS ;
 int error_code = SLURM_SUCCESS ;
 struct job_record * job_ptr = NULL ;
 job_desc_msg_t * job_desc_msg = ( job_desc_msg_t * ) msg -> data ;
 slurmctld_lock_t job_read_lock = {
 READ_LOCK , READ_LOCK , READ_LOCK , READ_LOCK , READ_LOCK }
 ;
 slurmctld_lock_t job_write_lock = {
 READ_LOCK , WRITE_LOCK , WRITE_LOCK , READ_LOCK , READ_LOCK }
 ;
 uid_t uid = g_slurm_auth_get_uid ( msg -> auth_cred , slurmctld_config . auth_info ) ;
 gid_t gid = g_slurm_auth_get_gid ( msg -> auth_cred , slurmctld_config . auth_info ) ;
 uint16_t port ;
 slurm_addr_t resp_addr ;
 will_run_response_msg_t * resp = NULL ;
 char * err_msg = NULL , * job_submit_user_msg = NULL ;
 if ( slurmctld_config . submissions_disabled ) {
 info ( "Submissions disabled on system" ) ;
 error_code = ESLURM_SUBMISSIONS_DISABLED ;
 goto send_reply ;
 }
 START_TIMER ;
 debug2 ( "Processing RPC: REQUEST_JOB_WILL_RUN from uid=%d" , uid ) ;
 if ( ( error_code = _valid_id ( "REQUEST_JOB_WILL_RUN" , job_desc_msg , uid , gid ) ) ) goto send_reply ;
 if ( ( job_desc_msg -> alloc_node == NULL ) || ( job_desc_msg -> alloc_node [ 0 ] == '\0' ) ) {
 error_code = ESLURM_INVALID_NODE_NAME ;
 error ( "REQUEST_JOB_WILL_RUN lacks alloc_node from uid=%d" , uid ) ;
 }
 if ( error_code == SLURM_SUCCESS ) {
 lock_slurmctld ( job_read_lock ) ;
 job_desc_msg -> pack_job_offset = NO_VAL ;
 error_code = validate_job_create_req ( job_desc_msg , uid , & err_msg ) ;
 unlock_slurmctld ( job_read_lock ) ;
 }
 if ( err_msg ) job_submit_user_msg = xstrdup ( err_msg ) ;
 if ( ! slurm_get_peer_addr ( msg -> conn_fd , & resp_addr ) ) {
 job_desc_msg -> resp_host = xmalloc ( 16 ) ;
 slurm_get_ip_str ( & resp_addr , & port , job_desc_msg -> resp_host , 16 ) ;
 dump_job_desc ( job_desc_msg ) ;
 if ( error_code == SLURM_SUCCESS ) {
 lock_slurmctld ( job_write_lock ) ;
 if ( job_desc_msg -> job_id == NO_VAL ) {
 job_desc_msg -> pack_job_offset = NO_VAL ;
 error_code = job_allocate ( job_desc_msg , false , true , & resp , true , uid , & job_ptr , & err_msg , msg -> protocol_version ) ;
 }
 else {
 error_code = job_start_data ( job_desc_msg , & resp ) ;
 }
 unlock_slurmctld ( job_write_lock ) ;
 END_TIMER2 ( "_slurm_rpc_job_will_run" ) ;
 }
 }
 else if ( errno ) error_code = errno ;
 else error_code = SLURM_ERROR ;
 send_reply : if ( error_code ) {
 debug2 ( "_slurm_rpc_job_will_run: %s" , slurm_strerror ( error_code ) ) ;
 if ( err_msg ) slurm_send_rc_err_msg ( msg , error_code , err_msg ) ;
 else slurm_send_rc_msg ( msg , error_code ) ;
 }
 else if ( resp ) {
 slurm_msg_t response_msg ;
 slurm_msg_t_init ( & response_msg ) ;
 response_msg . flags = msg -> flags ;
 response_msg . protocol_version = msg -> protocol_version ;
 response_msg . address = msg -> address ;
 response_msg . conn = msg -> conn ;
 response_msg . msg_type = RESPONSE_JOB_WILL_RUN ;
 response_msg . data = resp ;
 resp -> job_submit_user_msg = job_submit_user_msg ;
 job_submit_user_msg = NULL ;
 slurm_send_node_msg ( msg -> conn_fd , & response_msg ) ;
 slurm_free_will_run_response_msg ( resp ) ;
 debug2 ( "_slurm_rpc_job_will_run success %s" , TIME_STR ) ;
 }
 else {
 debug2 ( "_slurm_rpc_job_will_run success %s" , TIME_STR ) ;
 if ( job_desc_msg -> job_id == NO_VAL ) slurm_send_rc_msg ( msg , SLURM_SUCCESS ) ;
 }
 xfree ( err_msg ) ;
 xfree ( job_submit_user_msg ) ;
 }