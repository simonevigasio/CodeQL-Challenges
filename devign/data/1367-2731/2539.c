static void bootp_reply(struct bootp_t *bp)

{

    BOOTPClient *bc;

    struct mbuf *m;

    struct bootp_t *rbp;

    struct sockaddr_in saddr, daddr;

    struct in_addr dns_addr;

    int dhcp_msg_type, val;

    uint8_t *q;



    /* extract exact DHCP msg type */

    dhcp_decode(bp->bp_vend, DHCP_OPT_LEN, &dhcp_msg_type);

    dprintf("bootp packet op=%d msgtype=%d\n", bp->bp_op, dhcp_msg_type);



    if (dhcp_msg_type == 0)

        dhcp_msg_type = DHCPREQUEST; /* Force reply for old BOOTP clients */



    if (dhcp_msg_type != DHCPDISCOVER &&

        dhcp_msg_type != DHCPREQUEST)

        return;

    /* XXX: this is a hack to get the client mac address */

    memcpy(client_ethaddr, bp->bp_hwaddr, 6);



    if ((m = m_get()) == NULL)

        return;

    m->m_data += IF_MAXLINKHDR;

    rbp = (struct bootp_t *)m->m_data;

    m->m_data += sizeof(struct udpiphdr);

    memset(rbp, 0, sizeof(struct bootp_t));



    if (dhcp_msg_type == DHCPDISCOVER) {

    new_addr:

        bc = get_new_addr(&daddr.sin_addr);

        if (!bc) {

            dprintf("no address left\n");

            return;

        }

        memcpy(bc->macaddr, client_ethaddr, 6);

    } else {

        bc = find_addr(&daddr.sin_addr, bp->bp_hwaddr);

        if (!bc) {

            /* if never assigned, behaves as if it was already

               assigned (windows fix because it remembers its address) */

            goto new_addr;

        }

    }



    if (bootp_filename)

        snprintf((char *)rbp->bp_file, sizeof(rbp->bp_file), "%s",

                 bootp_filename);



    dprintf("offered addr=%08x\n", ntohl(daddr.sin_addr.s_addr));



    saddr.sin_addr.s_addr = htonl(ntohl(special_addr.s_addr) | CTL_ALIAS);

    saddr.sin_port = htons(BOOTP_SERVER);



    daddr.sin_port = htons(BOOTP_CLIENT);



    rbp->bp_op = BOOTP_REPLY;

    rbp->bp_xid = bp->bp_xid;

    rbp->bp_htype = 1;

    rbp->bp_hlen = 6;

    memcpy(rbp->bp_hwaddr, bp->bp_hwaddr, 6);



    rbp->bp_yiaddr = daddr.sin_addr; /* Client IP address */

    rbp->bp_siaddr = saddr.sin_addr; /* Server IP address */



    daddr.sin_addr.s_addr = 0xffffffffu;



    q = rbp->bp_vend;

    memcpy(q, rfc1533_cookie, 4);

    q += 4;



    if (dhcp_msg_type == DHCPDISCOVER) {

        *q++ = RFC2132_MSG_TYPE;

        *q++ = 1;

        *q++ = DHCPOFFER;

    } else if (dhcp_msg_type == DHCPREQUEST) {

        *q++ = RFC2132_MSG_TYPE;

        *q++ = 1;

        *q++ = DHCPACK;

    }



    if (dhcp_msg_type == DHCPDISCOVER ||

        dhcp_msg_type == DHCPREQUEST) {

        *q++ = RFC2132_SRV_ID;

        *q++ = 4;

        memcpy(q, &saddr.sin_addr, 4);

        q += 4;



        *q++ = RFC1533_NETMASK;

        *q++ = 4;

        *q++ = 0xff;

        *q++ = 0xff;

        *q++ = 0xff;

        *q++ = 0x00;



        if (!slirp_restrict) {

            *q++ = RFC1533_GATEWAY;

            *q++ = 4;

            memcpy(q, &saddr.sin_addr, 4);

            q += 4;



            *q++ = RFC1533_DNS;

            *q++ = 4;

            dns_addr.s_addr = htonl(ntohl(special_addr.s_addr) | CTL_DNS);

            memcpy(q, &dns_addr, 4);

            q += 4;

        }



        *q++ = RFC2132_LEASE_TIME;

        *q++ = 4;

        val = htonl(LEASE_TIME);

        memcpy(q, &val, 4);

        q += 4;



        if (*slirp_hostname) {

            val = strlen(slirp_hostname);

            *q++ = RFC1533_HOSTNAME;

            *q++ = val;

            memcpy(q, slirp_hostname, val);

            q += val;

        }

    }

    *q++ = RFC1533_END;



    m->m_len = sizeof(struct bootp_t) -

        sizeof(struct ip) - sizeof(struct udphdr);

    udp_output2(NULL, m, &saddr, &daddr, IPTOS_LOWDELAY);

}