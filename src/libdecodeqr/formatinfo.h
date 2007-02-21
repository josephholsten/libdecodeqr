/////////////////////////////////////////////////////////////////////////
//
// formatinfo.h --a part of libdecodeqr
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
#ifndef __QR_FORMAT_INFO__
#define __QR_FORMAT_INFO__

#ifndef NULL
#define NULL 0
#endif

#include "qrerror.h"
#include "galois.h"

#define QR_FORMAT_INFO_GX 0x537 // G(x)=x^10+x^8+x^5+x^4+x^2+x+1
#define QR_FORMAT_INFO_XOR_MASK 0x5412 //0101010000010010B
#define QR_FORMAT_INFO_DATA_SIZE 15

namespace Qr{
    //
    // format info;
    //  matrix of (x,y); minus value means SymbolLength - Value;
    //  e.g; symbol size = 21 x 21, values {1,-1} points {1,20} module
    //
    const int format_info_addr[2][16][2]={
        {
            {0,8},{1,8},{2,8},{3,8},{4,8},{5,8},{7,8},
            {8,8},{8,7},{8,5},{8,4},{8,3},{8,2},{8,1},{8,0}
        },
        {
            {8,-8}, // always black
            {8,-1},{8,-2},{8,-3},{8,-4},{8,-5},{8,-6},{8,-7},
            {-8,8},{-7,8},{-6,8},{-5,8},{-4,8},{-3,8},{-2,8},{-1,8}
        }
    };

    class MaskPatterner{
    public:
        virtual unsigned char pixel(int i,int j)=0;
    };
    class MaskPatterner000:public MaskPatterner{
    public:
        virtual unsigned char pixel(int i,int j);
    };
    class MaskPatterner001:public MaskPatterner{
    public:
        virtual unsigned char pixel(int i,int j);
    };
    class MaskPatterner010:public MaskPatterner{
    public:
        virtual unsigned char pixel(int i,int j);
    };
    class MaskPatterner011:public MaskPatterner{
    public:
        virtual unsigned char pixel(int i,int j);
    };
    class MaskPatterner100:public MaskPatterner{
    public:
        virtual unsigned char pixel(int i,int j);
    };
    class MaskPatterner101:public MaskPatterner{
    public:
        virtual unsigned char pixel(int i,int j);
    };
    class MaskPatterner110:public MaskPatterner{
    public:
        virtual unsigned char pixel(int i,int j);
    };
    class MaskPatterner111:public MaskPatterner{
    public:
        virtual unsigned char pixel(int i,int j);
    };

    class FormatInfo{
    public:
        int level;
        int mask_pattern;
        short status;

    private:
        MaskPatterner *_patterner;

        int _pattern_c;

    public:
        FormatInfo();
        ~FormatInfo();

        int set_level(int l);
        int set_mask_pattern(int m);

        int decode_formatinfo(unsigned short data);

        unsigned char mask_pixel(int i,int j);

        FormatInfo *init_each_pattern_pixel();
        FormatInfo *each_pattern_pixel(int *x,int *y);
        FormatInfo *each_pattern_pixel(int pos,int *x,int *y);

    private:
        int _error_correct(unsigned short *src);
    };
}
#endif
