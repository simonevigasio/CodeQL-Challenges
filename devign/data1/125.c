static int decode_frame_ilbm(AVCodecContext *avctx,

                            void *data, int *data_size,

                            AVPacket *avpkt)

{

    IffContext *s = avctx->priv_data;

    const uint8_t *buf = avpkt->size >= 2 ? avpkt->data + AV_RB16(avpkt->data) : NULL;

    const int buf_size = avpkt->size >= 2 ? avpkt->size - AV_RB16(avpkt->data) : 0;

    const uint8_t *buf_end = buf+buf_size;

    int y, plane, res;



    if ((res = extract_header(avctx, avpkt)) < 0)

        return res;



    if (s->init) {

        if ((res = avctx->reget_buffer(avctx, &s->frame)) < 0) {

            av_log(avctx, AV_LOG_ERROR, "reget_buffer() failed\n");

            return res;

        }

    } else if ((res = avctx->get_buffer(avctx, &s->frame)) < 0) {

        av_log(avctx, AV_LOG_ERROR, "get_buffer() failed\n");

        return res;

    } else if (avctx->bits_per_coded_sample <= 8 && avctx->pix_fmt != PIX_FMT_GRAY8) {

        if ((res = ff_cmap_read_palette(avctx, (uint32_t*)s->frame.data[1])) < 0)

            return res;

    }

    s->init = 1;



    if (avctx->codec_tag == MKTAG('A','C','B','M')) {

        if (avctx->pix_fmt == PIX_FMT_PAL8 || avctx->pix_fmt == PIX_FMT_GRAY8) {

            memset(s->frame.data[0], 0, avctx->height * s->frame.linesize[0]);

            for (plane = 0; plane < s->bpp; plane++) {

                for(y = 0; y < avctx->height && buf < buf_end; y++ ) {

                    uint8_t *row = &s->frame.data[0][ y*s->frame.linesize[0] ];

                    decodeplane8(row, buf, FFMIN(s->planesize, buf_end - buf), plane);

                    buf += s->planesize;

                }

            }

        } else if (s->ham) { // HAM to PIX_FMT_BGR32

            memset(s->frame.data[0], 0, avctx->height * s->frame.linesize[0]);

            for(y = 0; y < avctx->height; y++) {

                uint8_t *row = &s->frame.data[0][y * s->frame.linesize[0]];

                memset(s->ham_buf, 0, s->planesize * 8);

                for (plane = 0; plane < s->bpp; plane++) {

                    const uint8_t * start = buf + (plane * avctx->height + y) * s->planesize;

                    if (start >= buf_end)

                        break;

                    decodeplane8(s->ham_buf, start, FFMIN(s->planesize, buf_end - start), plane);

                }

                decode_ham_plane32((uint32_t *) row, s->ham_buf, s->ham_palbuf, s->planesize);

            }

        }

    } else if (avctx->codec_tag == MKTAG('D','E','E','P')) {

        int raw_width = avctx->width * (av_get_bits_per_pixel(&av_pix_fmt_descriptors[avctx->pix_fmt]) >> 3);

        int x;

        for(y = 0; y < avctx->height && buf < buf_end; y++ ) {

            uint8_t *row = &s->frame.data[0][y * s->frame.linesize[0]];

            memcpy(row, buf, FFMIN(raw_width, buf_end - buf));

            buf += raw_width;

            if (avctx->pix_fmt == PIX_FMT_BGR32) {

                for(x = 0; x < avctx->width; x++)

                    row[4 * x + 3] = row[4 * x + 3] & 0xF0 | (row[4 * x + 3] >> 4);

            }

        }

    } else if (avctx->codec_tag == MKTAG('I','L','B','M')) { // interleaved

        if (avctx->pix_fmt == PIX_FMT_PAL8 || avctx->pix_fmt == PIX_FMT_GRAY8) {

            for(y = 0; y < avctx->height; y++ ) {

                uint8_t *row = &s->frame.data[0][ y*s->frame.linesize[0] ];

                memset(row, 0, avctx->width);

                for (plane = 0; plane < s->bpp && buf < buf_end; plane++) {

                    decodeplane8(row, buf, FFMIN(s->planesize, buf_end - buf), plane);

                    buf += s->planesize;

                }

            }

        } else if (s->ham) { // HAM to PIX_FMT_BGR32

            for (y = 0; y < avctx->height; y++) {

                uint8_t *row = &s->frame.data[0][ y*s->frame.linesize[0] ];

                memset(s->ham_buf, 0, s->planesize * 8);

                for (plane = 0; plane < s->bpp && buf < buf_end; plane++) {

                    decodeplane8(s->ham_buf, buf, FFMIN(s->planesize, buf_end - buf), plane);

                    buf += s->planesize;

                }

                decode_ham_plane32((uint32_t *) row, s->ham_buf, s->ham_palbuf, s->planesize);

            }

        } else { // PIX_FMT_BGR32

            for(y = 0; y < avctx->height; y++ ) {

                uint8_t *row = &s->frame.data[0][y*s->frame.linesize[0]];

                memset(row, 0, avctx->width << 2);

                for (plane = 0; plane < s->bpp && buf < buf_end; plane++) {

                    decodeplane32((uint32_t *) row, buf, FFMIN(s->planesize, buf_end - buf), plane);

                    buf += s->planesize;

                }

            }

        }

    } else if (avctx->codec_tag == MKTAG('P','B','M',' ')) { // IFF-PBM

        if (avctx->pix_fmt == PIX_FMT_PAL8 || avctx->pix_fmt == PIX_FMT_GRAY8) {

            for(y = 0; y < avctx->height; y++ ) {

                uint8_t *row = &s->frame.data[0][y * s->frame.linesize[0]];

                memcpy(row, buf, FFMIN(avctx->width, buf_end - buf));

                buf += avctx->width + (avctx->width % 2); // padding if odd

            }

        } else if (s->ham) { // IFF-PBM: HAM to PIX_FMT_BGR32

            for (y = 0; y < avctx->height; y++) {

                uint8_t *row = &s->frame.data[0][ y*s->frame.linesize[0] ];

                memcpy(s->ham_buf, buf, FFMIN(avctx->width, buf_end - buf));

                buf += avctx->width + (avctx->width & 1); // padding if odd

                decode_ham_plane32((uint32_t *) row, s->ham_buf, s->ham_palbuf, s->planesize);

            }

        } else {

            av_log_ask_for_sample(avctx, "unsupported bpp\n");

            return AVERROR_INVALIDDATA;

        }

    }



    *data_size = sizeof(AVFrame);

    *(AVFrame*)data = s->frame;

    return buf_size;

}