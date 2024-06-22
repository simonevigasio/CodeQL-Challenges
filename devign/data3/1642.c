static int iff_read_header(AVFormatContext *s)

{

    IffDemuxContext *iff = s->priv_data;

    AVIOContext *pb = s->pb;

    AVStream *st;

    uint8_t *buf;

    uint32_t chunk_id, data_size;

    uint32_t screenmode = 0, num, den;

    unsigned transparency = 0;

    unsigned masking = 0; // no mask

    uint8_t fmt[16];

    int fmt_size;



    st = avformat_new_stream(s, NULL);

    if (!st)

        return AVERROR(ENOMEM);



    st->codec->channels = 1;

    st->codec->channel_layout = AV_CH_LAYOUT_MONO;

    avio_skip(pb, 8);

    // codec_tag used by ByteRun1 decoder to distinguish progressive (PBM) and interlaced (ILBM) content

    st->codec->codec_tag = avio_rl32(pb);

    iff->bitmap_compression = -1;

    iff->svx8_compression = -1;

    iff->maud_bits = -1;

    iff->maud_compression = -1;



    while(!url_feof(pb)) {

        uint64_t orig_pos;

        int res;

        const char *metadata_tag = NULL;

        chunk_id = avio_rl32(pb);

        data_size = avio_rb32(pb);

        orig_pos = avio_tell(pb);



        switch(chunk_id) {

        case ID_VHDR:

            st->codec->codec_type = AVMEDIA_TYPE_AUDIO;



            if (data_size < 14)

                return AVERROR_INVALIDDATA;

            avio_skip(pb, 12);

            st->codec->sample_rate = avio_rb16(pb);

            if (data_size >= 16) {

                avio_skip(pb, 1);

                iff->svx8_compression = avio_r8(pb);

            }

            break;



        case ID_MHDR:

            st->codec->codec_type = AVMEDIA_TYPE_AUDIO;



            if (data_size < 32)

                return AVERROR_INVALIDDATA;

            avio_skip(pb, 4);

            iff->maud_bits = avio_rb16(pb);

            avio_skip(pb, 2);

            num = avio_rb32(pb);

            den = avio_rb16(pb);

            if (!den)

                return AVERROR_INVALIDDATA;

            avio_skip(pb, 2);

            st->codec->sample_rate = num / den;

            st->codec->channels = avio_rb16(pb);

            iff->maud_compression = avio_rb16(pb);

            if (st->codec->channels == 1)

                st->codec->channel_layout = AV_CH_LAYOUT_MONO;

            else if (st->codec->channels == 2)

                st->codec->channel_layout = AV_CH_LAYOUT_STEREO;

            break;



        case ID_ABIT:

        case ID_BODY:

        case ID_DBOD:

        case ID_MDAT:

            iff->body_pos = avio_tell(pb);

            iff->body_end = iff->body_pos + data_size;

            iff->body_size = data_size;

            break;



        case ID_CHAN:

            if (data_size < 4)

                return AVERROR_INVALIDDATA;

            if (avio_rb32(pb) < 6) {

                st->codec->channels       = 1;

                st->codec->channel_layout = AV_CH_LAYOUT_MONO;

            } else {

                st->codec->channels       = 2;

                st->codec->channel_layout = AV_CH_LAYOUT_STEREO;

            }

            break;



        case ID_CAMG:

            if (data_size < 4)

                return AVERROR_INVALIDDATA;

            screenmode                = avio_rb32(pb);

            break;



        case ID_CMAP:

            if (data_size > INT_MAX - IFF_EXTRA_VIDEO_SIZE - FF_INPUT_BUFFER_PADDING_SIZE)

                return AVERROR_INVALIDDATA;

            st->codec->extradata_size = data_size + IFF_EXTRA_VIDEO_SIZE;

            st->codec->extradata      = av_malloc(data_size + IFF_EXTRA_VIDEO_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);

            if (!st->codec->extradata)

                return AVERROR(ENOMEM);

            if (avio_read(pb, st->codec->extradata + IFF_EXTRA_VIDEO_SIZE, data_size) < 0)

                return AVERROR(EIO);

            break;



        case ID_BMHD:

            st->codec->codec_type            = AVMEDIA_TYPE_VIDEO;

            if (data_size <= 8)

                return AVERROR_INVALIDDATA;

            st->codec->width                 = avio_rb16(pb);

            st->codec->height                = avio_rb16(pb);

            avio_skip(pb, 4); // x, y offset

            st->codec->bits_per_coded_sample = avio_r8(pb);

            if (data_size >= 10)

                masking                      = avio_r8(pb);

            if (data_size >= 11)

                iff->bitmap_compression      = avio_r8(pb);

            if (data_size >= 14) {

                avio_skip(pb, 1); // padding

                transparency                 = avio_rb16(pb);

            }

            if (data_size >= 16) {

                st->sample_aspect_ratio.num  = avio_r8(pb);

                st->sample_aspect_ratio.den  = avio_r8(pb);

            }

            break;



        case ID_DPEL:

            if (data_size < 4 || (data_size & 3))

                return AVERROR_INVALIDDATA;

            if ((fmt_size = avio_read(pb, fmt, sizeof(fmt))) < 0)

                return fmt_size;

            if (fmt_size == sizeof(deep_rgb24) && !memcmp(fmt, deep_rgb24, sizeof(deep_rgb24)))

                st->codec->pix_fmt = AV_PIX_FMT_RGB24;

            else if (fmt_size == sizeof(deep_rgba) && !memcmp(fmt, deep_rgba, sizeof(deep_rgba)))

                st->codec->pix_fmt = AV_PIX_FMT_RGBA;

            else if (fmt_size == sizeof(deep_bgra) && !memcmp(fmt, deep_bgra, sizeof(deep_bgra)))

                st->codec->pix_fmt = AV_PIX_FMT_BGRA;

            else if (fmt_size == sizeof(deep_argb) && !memcmp(fmt, deep_argb, sizeof(deep_argb)))

                st->codec->pix_fmt = AV_PIX_FMT_ARGB;

            else if (fmt_size == sizeof(deep_abgr) && !memcmp(fmt, deep_abgr, sizeof(deep_abgr)))

                st->codec->pix_fmt = AV_PIX_FMT_ABGR;

            else {

                av_log_ask_for_sample(s, "unsupported color format\n");

                return AVERROR_PATCHWELCOME;

            }

            break;



        case ID_DGBL:

            st->codec->codec_type            = AVMEDIA_TYPE_VIDEO;

            if (data_size < 8)

                return AVERROR_INVALIDDATA;

            st->codec->width                 = avio_rb16(pb);

            st->codec->height                = avio_rb16(pb);

            iff->bitmap_compression          = avio_rb16(pb);

            st->sample_aspect_ratio.num      = avio_r8(pb);

            st->sample_aspect_ratio.den      = avio_r8(pb);

            st->codec->bits_per_coded_sample = 24;

            break;



        case ID_DLOC:

            if (data_size < 4)

                return AVERROR_INVALIDDATA;

            st->codec->width  = avio_rb16(pb);

            st->codec->height = avio_rb16(pb);

            break;



        case ID_TVDC:

            if (data_size < sizeof(iff->tvdc))

                return AVERROR_INVALIDDATA;

            res = avio_read(pb, iff->tvdc, sizeof(iff->tvdc));

            if (res < 0)

                return res;

            break;



        case ID_ANNO:

        case ID_TEXT:      metadata_tag = "comment";   break;

        case ID_AUTH:      metadata_tag = "artist";    break;

        case ID_COPYRIGHT: metadata_tag = "copyright"; break;

        case ID_NAME:      metadata_tag = "title";     break;

        }



        if (metadata_tag) {

            if ((res = get_metadata(s, metadata_tag, data_size)) < 0) {

                av_log(s, AV_LOG_ERROR, "cannot allocate metadata tag %s!\n", metadata_tag);

                return res;

            }

        }

        avio_skip(pb, data_size - (avio_tell(pb) - orig_pos) + (data_size & 1));

    }



    avio_seek(pb, iff->body_pos, SEEK_SET);



    switch(st->codec->codec_type) {

    case AVMEDIA_TYPE_AUDIO:

        avpriv_set_pts_info(st, 32, 1, st->codec->sample_rate);



        if (st->codec->codec_tag == ID_16SV)

            st->codec->codec_id = AV_CODEC_ID_PCM_S16BE_PLANAR;

        else if (st->codec->codec_tag == ID_MAUD) {

            if (iff->maud_bits == 8 && !iff->maud_compression) {

                st->codec->codec_id = AV_CODEC_ID_PCM_U8;

            } else if (iff->maud_bits == 16 && !iff->maud_compression) {

                st->codec->codec_id = AV_CODEC_ID_PCM_S16BE;

            } else if (iff->maud_bits ==  8 && iff->maud_compression == 2) {

                st->codec->codec_id = AV_CODEC_ID_PCM_ALAW;

            } else if (iff->maud_bits ==  8 && iff->maud_compression == 3) {

                st->codec->codec_id = AV_CODEC_ID_PCM_MULAW;

            } else {

                av_log_ask_for_sample(s, "unsupported compression %d and bit depth %d\n", iff->maud_compression, iff->maud_bits);

                return AVERROR_PATCHWELCOME;

            }



            st->codec->bits_per_coded_sample =

                av_get_bits_per_sample(st->codec->codec_id);



            st->codec->block_align =

                st->codec->bits_per_coded_sample * st->codec->channels / 8;

        } else {

        switch (iff->svx8_compression) {

        case COMP_NONE:

            st->codec->codec_id = AV_CODEC_ID_PCM_S8_PLANAR;

            break;

        case COMP_FIB:

            st->codec->codec_id = AV_CODEC_ID_8SVX_FIB;

            break;

        case COMP_EXP:

            st->codec->codec_id = AV_CODEC_ID_8SVX_EXP;

            break;

        default:

            av_log(s, AV_LOG_ERROR,

                   "Unknown SVX8 compression method '%d'\n", iff->svx8_compression);

            return -1;

        }

        }



        st->codec->bits_per_coded_sample = av_get_bits_per_sample(st->codec->codec_id);

        st->codec->bit_rate = st->codec->channels * st->codec->sample_rate * st->codec->bits_per_coded_sample;

        st->codec->block_align = st->codec->channels * st->codec->bits_per_coded_sample;

        break;



    case AVMEDIA_TYPE_VIDEO:

        iff->bpp          = st->codec->bits_per_coded_sample;

        if ((screenmode & 0x800 /* Hold And Modify */) && iff->bpp <= 8) {

            iff->ham      = iff->bpp > 6 ? 6 : 4;

            st->codec->bits_per_coded_sample = 24;

        }

        iff->flags        = (screenmode & 0x80 /* Extra HalfBrite */) && iff->bpp <= 8;

        iff->masking      = masking;

        iff->transparency = transparency;



        if (!st->codec->extradata) {

            st->codec->extradata_size = IFF_EXTRA_VIDEO_SIZE;

            st->codec->extradata      = av_malloc(IFF_EXTRA_VIDEO_SIZE + FF_INPUT_BUFFER_PADDING_SIZE);

            if (!st->codec->extradata)

                return AVERROR(ENOMEM);

        }


        buf = st->codec->extradata;

        bytestream_put_be16(&buf, IFF_EXTRA_VIDEO_SIZE);

        bytestream_put_byte(&buf, iff->bitmap_compression);

        bytestream_put_byte(&buf, iff->bpp);

        bytestream_put_byte(&buf, iff->ham);

        bytestream_put_byte(&buf, iff->flags);

        bytestream_put_be16(&buf, iff->transparency);

        bytestream_put_byte(&buf, iff->masking);

        bytestream_put_buffer(&buf, iff->tvdc, sizeof(iff->tvdc));

        st->codec->codec_id = AV_CODEC_ID_IFF_ILBM;

        break;

    default:

        return -1;

    }



    return 0;

}