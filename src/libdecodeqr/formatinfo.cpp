/////////////////////////////////////////////////////////////////////////
//
// formatinfo.cpp --a part of libdecodeqr
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
#include "formatinfo.h"

namespace Qr{
    unsigned char MaskPatterner000::pixel(int i,int j)
    {
        return((i+j)%2?0:255);
    }
    unsigned char MaskPatterner001::pixel(int i,int j)
    {
        return(i%2?0:255);
    }
    unsigned char MaskPatterner010::pixel(int i,int j)
    {
        return(j%3?0:255);
    }
    unsigned char MaskPatterner011::pixel(int i,int j)
    {
        return((i+j)%3?0:255);
    }
    unsigned char MaskPatterner100::pixel(int i,int j)
    {
        return((i/2+j/3)%2?0:255);
    }
    unsigned char MaskPatterner101::pixel(int i,int j)
    {
        return(((i*j)%2)+((i*j)%3)?0:255);
    }
    unsigned char MaskPatterner110::pixel(int i,int j)
    {
        return((((i*j)%2)+((i*j)%3))%2?0:255);
    }
    unsigned char MaskPatterner111::pixel(int i,int j)
    {
        return((((i*j)%3)+((i*j)%2))%2?0:255);
    }

    FormatInfo::FormatInfo()
    {
        this->level=0;
        this->mask_pattern=0;
        this->status=0;

        this->_patterner=NULL;
    }
    FormatInfo::~FormatInfo()
    {
        if(this->_patterner)
            delete this->_patterner;
    }

    int FormatInfo::set_level(int l)
    {
        this->level=l;
        if(l<0||l>3){
            this->status|=QR_FORMATINFO_INVALID_LEVEL;
            this->level=0;
        }
        return(this->level);
    }
    int FormatInfo::set_mask_pattern(int m)
    {
        if(this->_patterner)
            delete this->_patterner;

        this->mask_pattern=m;
        switch(this->mask_pattern){
        case 0:
            this->_patterner=new MaskPatterner000();
            break;
        case 1:
            this->_patterner=new MaskPatterner001();
            break;
        case 2:
            this->_patterner=new MaskPatterner010();
            break;
        case 3:
            this->_patterner=new MaskPatterner011();
            break;
        case 4:
            this->_patterner=new MaskPatterner100();
            break;
        case 5:
            this->_patterner=new MaskPatterner101();
            break;
        case 6:
            this->_patterner=new MaskPatterner110();
            break;
        case 7:
            this->_patterner=new MaskPatterner111();
            break;
        }

        return(this->mask_pattern);
    }

    int FormatInfo::decode_formatinfo(unsigned short data)
    {
        data^=QR_FORMAT_INFO_XOR_MASK;

        this->status=0;
        int ret=this->_error_correct(&data);

        this->set_level(data>>13);
        this->set_mask_pattern((data>>10)&0x7);

        if(ret<0)
            this->status|=QR_FORMATINFO_UNRECOVERABLE;

        return(ret);
    }

    int FormatInfo::_error_correct(unsigned short *src)
    {
        Galois::Field *gf=new Galois::Field(4);
        Galois::BCH *bch=new Galois::BCH(gf,15,3);
        unsigned short mask=0x01;
        int i;
        for(i=0;i<15;i++,mask<<=1){
            if(*src&mask)
                bch->set(i,gf->exp2nomial(0));
            else
                bch->set(i,gf->zero());
        }
        int errors=bch->decode();
        if(errors>0){
            mask=0x01;
            for(i=0;i<errors;i++)
                (*src)^=(mask<<bch->error_pos[i]);
        }
        
        delete bch;
        delete gf;
        return(errors);
    }

    unsigned char FormatInfo::mask_pixel(int i,int j){
        return(this->_patterner->pixel(i,j));
    }
    
    FormatInfo *FormatInfo::init_each_pattern_pixel()
    {
        this->_pattern_c=0;
        return(this);
    }
    FormatInfo *FormatInfo::each_pattern_pixel(int *x,int *y)
    {
        if(this->_pattern_c<16){
            *x=format_info_addr[0][this->_pattern_c][0];
            *y=format_info_addr[0][this->_pattern_c][1];
        }
        else if(this->_pattern_c<32){
            *x=format_info_addr[1][this->_pattern_c-16][0];
            *y=format_info_addr[1][this->_pattern_c-16][1];
        }
        else
            return(NULL);

        this->_pattern_c++;
        return(this);
    }
    FormatInfo *FormatInfo::each_pattern_pixel(int pos,int *x,int *y)
    {
        switch(pos){
        case 0:
            if(this->_pattern_c>14)
                return(NULL);
            break;
        case 1:
            if(this->_pattern_c>15)
                return(NULL);
            break;
        default:
            return(NULL);
        }
        
        *x=format_info_addr[pos][this->_pattern_c][0];
        *y=format_info_addr[pos][this->_pattern_c][1];
        
        this->_pattern_c++;
        return(this);
    }
}
