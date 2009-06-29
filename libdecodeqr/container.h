/////////////////////////////////////////////////////////////////////////
//
// container.h --a part of libdecodeqr
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
#ifndef __QR_CONTAINER__
#define __QR_CONTAINER__

#include "qrerror.h"
#include "formatinfo.h"
#include "codedata.h"

#ifndef NULL
#define NULL 0
#endif

#define QR_VERSION_INFO_GX 0x1f25 // G(x)=x^12+x^11+x^10+x^9+x^8+x^5+x^2+1
#define QR_VERSION_INFO_DATA_SIZE 18

namespace Qr{
    //
    // alignment pattern center coordinates
    //  ( from JISX0510(2004) Appendix E Table.1 p.71)
    //
    //  {{number of data without me,coord_1,...coord_n},...}
    //
    //  number of alignment pattern = (number of data)^2 - 3
    //
    const int alignment_pattern_addr[41][8]={
        // version 0 (not exist, just a dummy)
        {0,0,0,0,0,0,0,0},
        
        // version 1
        {0,0,0,0,0,0,0,0},
        {2,6,18,0,0,0,0,0},
        {2,6,22,0,0,0,0,0},
        {2,6,26,0,0,0,0,0},
        
        {2,6,30,0,0,0,0,0},
        {2,6,34,0,0,0,0,0},
        {3,6,22,38,0,0,0,0},
        {3,6,24,42,0,0,0,0},
        {3,6,26,46,0,0,0,0},
        
        // version 10
        {3,6,28,50,0,0,0,0},
        {3,6,30,54,0,0,0,0},
        {3,6,32,58,0,0,0,0},
        {3,6,34,62,0,0,0,0},
        {4,6,26,46,66,0,0,0},
        
        {4,6,26,48,70,0,0,0},
        {4,6,26,50,74,0,0,0},
        {4,6,30,54,78,0,0,0},
        {4,6,30,56,82,0,0,0},
        {4,6,30,58,86,0,0,0},
        
        // version 20
        {4,6,34,62,90,0,0,0},
        {5,6,28,50,72,94,0,0},
        {5,6,26,50,74,98,0,0},
        {5,6,30,54,78,102,0,0},
        {5,6,28,54,80,106,0,0},
        
        {5,6,32,58,84,110,0,0},
        {5,6,30,58,86,114,0,0},
        {5,6,34,62,90,118,0,0},
        {6,6,26,50,74,98,122,0},
        {6,6,30,54,78,102,126,0},
        
        // version 30
        {6,6,26,52,78,104,130,0},
        {6,6,30,56,82,108,134,0},
        {6,6,34,60,86,112,138,0},
        {6,6,30,58,86,114,142,0},
        {6,6,34,62,90,118,146,0},
        
        {7,6,30,54,78,102,126,150},
        {7,6,24,50,76,102,128,154},
        {7,6,28,54,80,106,132,158},
        {7,6,32,58,84,110,136,162},
        {7,6,26,54,82,110,138,166},
        
        // version 40
        {7,6,30,58,86,114,142,170}
    };

    //
    // version info;
    //  array of {x,y}; minus value means SymbolLength - Value;
    //  e.g; symbol size = 21 x 21, values {1,-1} points {1,20} module
    //
    const int version_info_addr[2][18][2]={
        {
            {-11,0},{-10,0},{-9,0},
            {-11,1},{-10,1},{-9,1},
            {-11,2},{-10,2},{-9,2},
            {-11,3},{-10,3},{-9,3},
            {-11,4},{-10,4},{-9,4},
            {-11,5},{-10,5},{-9,5}
        },
        {
            {0,-11},{0,-10},{0,-9},
            {1,-11},{1,-10},{1,-9},
            {2,-11},{2,-10},{2,-9},
            {3,-11},{3,-10},{3,-9},
            {4,-11},{4,-10},{4,-9},
            {5,-11},{5,-10},{5,-9}
        }
    };
    
    class Qr{
    public:
        int model;
        int version;
        int cells_par_side;

        //int finder_pattern;
        //int timing_pattern;
        //int alignment_pattern;

        short status;

        FormatInfo *formatinfo;
        CodeData *codedata;

    private:
        int _finder_c;
        int _timing_c;
        int _alignment_x;
        int _alignment_y;
        int _alignment_i;
        int _alignment_j;
        int _version_c;
        
    public:
        Qr();
        ~Qr();
        
        int set_version(int v);
        int decode_version(unsigned char *data); //FIXME

        int decode_formatinfo(unsigned short data);

        Qr *init_each_finder_pattern_pixel();
        Qr *init_each_timing_pattern_pixel();
        Qr *init_each_alignment_pattern_pixel();
        Qr *init_each_version_pattern_pixel();
        Qr *init_each_formatinfo_pattern_pixel();
        Qr *init_each_function_pattern_pixel();

        Qr *each_finder_pattern_pixel(int *x,int *y);
        Qr *each_timing_pattern_pixel(int *x,int *y);
        Qr *each_alignment_pattern_pixel(int *x,int *y);
        Qr *each_version_pattern_pixel(int *x,int *y);
        Qr *each_version_pattern_pixel(int pos,int *x,int *y);
        Qr *each_formatinfo_pattern_pixel(int *x,int *y);
        Qr *each_formatinfo_pattern_pixel(int pos,int *x,int *y);
        Qr *each_function_pattern_pixel(int *x,int *y);

        unsigned char *push_codedata(unsigned char data);
        int decode_codedata();
    };

}

#endif
