/////////////////////////////////////////////////////////////////////////
//
// bitstream.h --a part of libdecodeqr
//
// Copyright(C) 2007 NISHI Takao <zophos@koka-in.org>
//                   JMA  (Japan Medical Association)
//                   NaCl (Network Applied Communication Laboratory Ltd.)
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of LGPL.
//
// $Id$
//
#ifndef __QR_BITSTREAM__
#define __QR_BITSTREAM__

#include <memory.h>

#ifndef NULL
#define NULL 0
#endif

namespace Qr{
    class BitStream{
    public:
        unsigned char *data;
        int byte_size;
        int bit_size;

    private:
        int _pos;

    public:
        BitStream();
        BitStream(void *src,int size);
        ~BitStream();

        bool is_eod();
        int position();
        int seek(int pos);
        void rewind();
        
        unsigned char *read(int read_bits);
        int read(void *dst,int buf_size,int bitsize);
    };
};

#endif
