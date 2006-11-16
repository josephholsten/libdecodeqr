/////////////////////////////////////////////////////////////////////////
//
// bitstream.h --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id:$
//
#ifndef __QR_BITSTREAM__
#define __QR_BITSTREAM__

#include <memory.h>

#ifdef _DEBUG
#define DEBUG
#endif

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
        BitStream::BitStream();
        BitStream::BitStream(void *src,int size);
        BitStream::~BitStream();

        bool BitStream::is_eod();
        int BitStream::position();
        int BitStream::seek(int pos);
        void BitStream::rewind();
        
        unsigned char *BitStream::read(int read_bits);
        int BitStream::read(void *dst,int buf_size,int bitsize);
    };
};

#endif
