/////////////////////////////////////////////////////////////////////////
//
// codedata.h --a part of libdecodeqr
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
#ifndef __QR_CODE_DATA__
#define __QR_CODE_DATA__

#include <memory.h>
#include "galois.h"
#include "bitstream.h"
#include "ecidecoder.h"
#include "qrerror.h"

#ifndef NULL
#define NULL 0
#endif

#define QR_CODE_DATA_GX 0x11d //G(x)=x^8+x^4+x^3+x^2+1

namespace Qr{
    class CodeBlock{
    public:
        int total_words;
        int data_words;
        int capability;
        unsigned char *data;
        short status;

    private:
        int _size;
        Galois::Field *_gf;
        
    public:
        CodeBlock(int total_words,int data_words,int capability,
                             Galois::Field *gf);
        ~CodeBlock();
        
        void clear();
        unsigned char *push(unsigned char data);
        bool has_vacant_data();
        
        int error_correct();
    };
    
    class CodeData{
    public:
        int version;
        int level;
        
        int total_words; // total words (data+ecc)
        int data_words;  // number of data words
        int data_blocks; // number of rs_block
        CodeBlock **data;
        
        int length;   // number of decoded charactors
        int byte_length;

        short status;
    private:
        Galois::Field *_gf;
        
        int _size;
        int _index;
        
        unsigned char *_raw_data;
        
    public:
        CodeData(int version,int level);
        ~CodeData();
        
        void clear();
        unsigned char *push(unsigned char data);
        
        unsigned char *dump();
        unsigned char *dump_block(int index);
        unsigned char *dump_data();
        unsigned char *raw_data();
        
        int decode();

    private:
        int _error_correct();
    };
};

#endif
