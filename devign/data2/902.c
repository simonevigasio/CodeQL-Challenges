static int rle_unpack(const unsigned char *src, unsigned char *dest,

                      int src_count, int src_size, int dest_len)

{

    unsigned char *pd;

    int i, l;

    unsigned char *dest_end = dest + dest_len;

    GetByteContext gb;



    bytestream2_init(&gb, src, src_size);

    pd = dest;

    if (src_count & 1) {

        if (bytestream2_get_bytes_left(&gb) < 1)

            return 0;

        *pd++ = bytestream2_get_byteu(&gb);

    }



    src_count >>= 1;

    i = 0;

    do {

        if (bytestream2_get_bytes_left(&gb) < 1)

            break;

        l = bytestream2_get_byteu(&gb);

        if (l & 0x80) {

            l = (l & 0x7F) * 2;

            if (dest_end - pd < l || bytestream2_get_bytes_left(&gb) < l)

                return bytestream2_tell(&gb);

            bytestream2_get_bufferu(&gb, pd, l);

            pd += l;

        } else {

            if (dest_end - pd < i || bytestream2_get_bytes_left(&gb) < 2)

                return bytestream2_tell(&gb);

            for (i = 0; i < l; i++) {

                *pd++ = bytestream2_get_byteu(&gb);

                *pd++ = bytestream2_get_byteu(&gb);

            }

            bytestream2_skip(&gb, 2);

        }

        i += l;

    } while (i < src_count);



    return bytestream2_tell(&gb);

}