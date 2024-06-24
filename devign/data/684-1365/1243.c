static void qdm2_decode_super_block(QDM2Context *q)

{

    GetBitContext gb;

    QDM2SubPacket header, *packet;

    int i, packet_bytes, sub_packet_size, sub_packets_D;

    unsigned int next_index = 0;



    memset(q->tone_level_idx_hi1, 0, sizeof(q->tone_level_idx_hi1));

    memset(q->tone_level_idx_mid, 0, sizeof(q->tone_level_idx_mid));

    memset(q->tone_level_idx_hi2, 0, sizeof(q->tone_level_idx_hi2));



    q->sub_packets_B = 0;

    sub_packets_D    = 0;



    average_quantized_coeffs(q); // average elements in quantized_coeffs[max_ch][10][8]



    init_get_bits(&gb, q->compressed_data, q->compressed_size * 8);

    qdm2_decode_sub_packet_header(&gb, &header);



    if (header.type < 2 || header.type >= 8) {

        q->has_errors = 1;

        av_log(NULL, AV_LOG_ERROR, "bad superblock type\n");

        return;

    }



    q->superblocktype_2_3 = (header.type == 2 || header.type == 3);

    packet_bytes          = (q->compressed_size - get_bits_count(&gb) / 8);



    init_get_bits(&gb, header.data, header.size * 8);



    if (header.type == 2 || header.type == 4 || header.type == 5) {

        int csum = 257 * get_bits(&gb, 8);

        csum += 2 * get_bits(&gb, 8);



        csum = qdm2_packet_checksum(q->compressed_data, q->checksum_size, csum);



        if (csum != 0) {

            q->has_errors = 1;

            av_log(NULL, AV_LOG_ERROR, "bad packet checksum\n");

            return;

        }

    }



    q->sub_packet_list_B[0].packet = NULL;

    q->sub_packet_list_D[0].packet = NULL;



    for (i = 0; i < 6; i++)

        if (--q->fft_level_exp[i] < 0)

            q->fft_level_exp[i] = 0;



    for (i = 0; packet_bytes > 0; i++) {

        int j;



        if (i >= FF_ARRAY_ELEMS(q->sub_packet_list_A)) {

            SAMPLES_NEEDED_2("too many packet bytes");

            return;

        }



        q->sub_packet_list_A[i].next = NULL;



        if (i > 0) {

            q->sub_packet_list_A[i - 1].next = &q->sub_packet_list_A[i];



            /* seek to next block */

            init_get_bits(&gb, header.data, header.size * 8);

            skip_bits(&gb, next_index * 8);



            if (next_index >= header.size)

                break;

        }



        /* decode subpacket */

        packet = &q->sub_packets[i];

        qdm2_decode_sub_packet_header(&gb, packet);

        next_index      = packet->size + get_bits_count(&gb) / 8;

        sub_packet_size = ((packet->size > 0xff) ? 1 : 0) + packet->size + 2;



        if (packet->type == 0)

            break;



        if (sub_packet_size > packet_bytes) {

            if (packet->type != 10 && packet->type != 11 && packet->type != 12)

                break;

            packet->size += packet_bytes - sub_packet_size;

        }



        packet_bytes -= sub_packet_size;



        /* add subpacket to 'all subpackets' list */

        q->sub_packet_list_A[i].packet = packet;



        /* add subpacket to related list */

        if (packet->type == 8) {

            SAMPLES_NEEDED_2("packet type 8");

            return;

        } else if (packet->type >= 9 && packet->type <= 12) {

            /* packets for MPEG Audio like Synthesis Filter */

            QDM2_LIST_ADD(q->sub_packet_list_D, sub_packets_D, packet);

        } else if (packet->type == 13) {

            for (j = 0; j < 6; j++)

                q->fft_level_exp[j] = get_bits(&gb, 6);

        } else if (packet->type == 14) {

            for (j = 0; j < 6; j++)

                q->fft_level_exp[j] = qdm2_get_vlc(&gb, &fft_level_exp_vlc, 0, 2);

        } else if (packet->type == 15) {

            SAMPLES_NEEDED_2("packet type 15")

            return;

        } else if (packet->type >= 16 && packet->type < 48 &&

                   !fft_subpackets[packet->type - 16]) {

            /* packets for FFT */

            QDM2_LIST_ADD(q->sub_packet_list_B, q->sub_packets_B, packet);

        }

    } // Packet bytes loop



    if (q->sub_packet_list_D[0].packet != NULL) {

        process_synthesis_subpackets(q, q->sub_packet_list_D);

        q->do_synth_filter = 1;

    } else if (q->do_synth_filter) {

        process_subpacket_10(q, NULL);

        process_subpacket_11(q, NULL);

        process_subpacket_12(q, NULL);

    }

}