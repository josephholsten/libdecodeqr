/////////////////////////////////////////////////////////////////////////
//
// codedata.h --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id:$
//
#ifndef __QR_CODE_DATA__
#define __QR_CODE_DATA__

#include <memory.h>
#include "galois.h"
#include "bitstream.h"
#include "ecidecoder.h"
#include "errorcode.h"

#ifdef _DEBUG
#define DEBUG
#endif

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
        CodeBlock::CodeBlock(int total_words,int data_words,int capability,
                             Galois::Field *gf);
        CodeBlock::~CodeBlock();
        
        void CodeBlock::clear();
        unsigned char *CodeBlock::push(unsigned char data);
        bool CodeBlock::has_vacant_data();
        
        int CodeBlock::error_correct();
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
        CodeData::CodeData(int version,int level);
        CodeData::~CodeData();
        
        void CodeData::clear();
        unsigned char *CodeData::push(unsigned char data);
        
        unsigned char *CodeData::dump();
        unsigned char *CodeData::dump_block(int index);
        unsigned char *CodeData::dump_data();
        unsigned char *CodeData::raw_data();
        
        int CodeData::decode();

    private:
        int CodeData::_error_correct();
    };
};

#endif
