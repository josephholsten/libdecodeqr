/////////////////////////////////////////////////////////////////////////
//
// container.cpp --a part of libdecodeqr
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
#include "container.h"

namespace Qr{

    Qr::Qr()
    {
        this->model=2;
        this->version=0;
        this->cells_par_side=0;

        this->formatinfo=new FormatInfo();
        this->codedata=NULL;

        this->status=0;
    }
    Qr::~Qr()
    {
        if(this->codedata)
            delete this->codedata;

        delete this->formatinfo;
    }

    int Qr::set_version(int v)
    {
        if(v<1||v>40){
            this->status|=QR_VERSIONINFO_INVALID;
            throw(0);
        }

        this->version=v;
        this->cells_par_side=this->version*4+17;
        return(this->version);
    }
    int Qr::decode_version(unsigned char *data)
    {
        // FIXME
        return(this->version);
    }

    int Qr::decode_formatinfo(unsigned short data)
    {
        this->status&=(~QR_FORMATINFO_ERROR);

        int ret=this->formatinfo->decode_formatinfo(data);
        this->status|=this->formatinfo->status;

        if(this->codedata)
            delete this->codedata;
        
        this->codedata=new CodeData(this->version,this->formatinfo->level);

        return(ret);
    }

    Qr *Qr::init_each_finder_pattern_pixel()
    {
        this->_finder_c=0;
        return(this);
    }
    Qr *Qr::init_each_timing_pattern_pixel()
    {
        this->_timing_c=0;
        return(this);
    }
    Qr *Qr::init_each_alignment_pattern_pixel()
    {
        this->_alignment_x=2;
        this->_alignment_y=1;
        this->_alignment_i=-2;
        this->_alignment_j=-2;
        return(this);
    }
    Qr *Qr::init_each_version_pattern_pixel()
    {
        this->_version_c=0;
        return(this);
    }
    Qr *Qr::init_each_formatinfo_pattern_pixel()
    {
        this->formatinfo->init_each_pattern_pixel();
        return(this);
    }
    Qr *Qr::init_each_function_pattern_pixel()
    {
        this->init_each_finder_pattern_pixel();
        this->init_each_timing_pattern_pixel();
        this->init_each_alignment_pattern_pixel();
        this->init_each_version_pattern_pixel();
        this->init_each_formatinfo_pattern_pixel();
        return(this);
    }
    
    Qr *Qr::each_finder_pattern_pixel(int *x,int *y)
    {
        int c=this->_finder_c;

        //
        // 8x8x3 pixels
        //
        if(this->_finder_c<64){
            *x=c%8;
            *y=c/8;
        }
        else if(this->_finder_c<128){
            c-=64;
            *x=this->cells_par_side-8+c%8;
            *y=c/8;
        }
        else if(this->_finder_c<192){
            c-=128;
            *x=c%8;
            *y=this->cells_par_side-8+c/8;
        }
        else
            return(NULL);
        
        this->_finder_c++;
        return(this);
    }

    Qr *Qr::each_timing_pattern_pixel(int *x,int *y)
    {
        if(this->_timing_c<this->cells_par_side){
            *x=6;
            *y=this->_timing_c;
        }
        else if(this->_timing_c<this->cells_par_side*2){
            *x=this->_timing_c-this->cells_par_side;
            *y=6;
        }
        else
            return(NULL);

        this->_timing_c++;
        return(this);
    }

    Qr *Qr::each_alignment_pattern_pixel(int *x,int *y)
    {
        int c=alignment_pattern_addr[this->version][0]+1;

        if((this->_alignment_y==1)&&(this->_alignment_x>=c-1)){
            this->_alignment_y++;
            if(this->_alignment_y==1||this->_alignment_y==c-1)
                this->_alignment_x=2;
            else
                this->_alignment_x=1;
        }
        if(this->_alignment_y>=c)
            return(NULL);

        *x=alignment_pattern_addr[version][this->_alignment_x]+
            this->_alignment_j;
        *y=alignment_pattern_addr[version][this->_alignment_y]+
            this->_alignment_i;
        
        this->_alignment_j++;
        if(this->_alignment_j>2){
            this->_alignment_j=-2;
            this->_alignment_i++;

            if(this->_alignment_i>2){
                this->_alignment_i=-2;
                this->_alignment_x++;

                if((this->_alignment_x>=c)||
                   ((this->_alignment_y==1)&&(this->_alignment_x>=c-1))){
                    this->_alignment_y++;
                    if(this->_alignment_y==1||this->_alignment_y==c-1)
                        this->_alignment_x=2;
                    else
                        this->_alignment_x=1;
                }
            }
        }

        return(this);
    }

    Qr *Qr::each_version_pattern_pixel(int *x,int *y)
    {
        if(this->version<7)
            return(NULL);

        if(this->_version_c<18){
            *x=version_info_addr[0][this->_version_c][0];
            *y=version_info_addr[0][this->_version_c][1];
        }
        else if(this->_version_c<36){
            *x=version_info_addr[1][this->_version_c-18][0];
            *y=version_info_addr[1][this->_version_c-18][1];
        }
        else
            return(NULL);

        if(*x<0)
            *x+=this->cells_par_side;
        if(*y<0)
            *y+=this->cells_par_side;
        this->_version_c++;
        return(this);
    }
    Qr *Qr::each_version_pattern_pixel(int pos,int *x,int *y)
    {
        if(this->version<7)
            return(NULL);

        if(this->_version_c<18){
            *x=version_info_addr[pos][this->_version_c][0];
            *y=version_info_addr[pos][this->_version_c][1];
        }
        else
            return(NULL);

        if(*x<0)
            *x+=this->cells_par_side;
        if(*y<0)
            *y+=this->cells_par_side;
        this->_version_c++;
        return(this);
    }

    Qr *Qr::each_formatinfo_pattern_pixel(int *x,int *y)
    {
        if(this->formatinfo->each_pattern_pixel(x,y)){
            if(*x<0)
                *x+=this->cells_par_side;
            if(*y<0)
                *y+=this->cells_par_side;
            return(this);
        }
        else
            return(NULL);
    }
    Qr *Qr::each_formatinfo_pattern_pixel(int pos,int *x,int *y)
    {
        if(this->formatinfo->each_pattern_pixel(pos,x,y)){
            if(*x<0)
                *x+=this->cells_par_side;
            if(*y<0)
                *y+=this->cells_par_side;
            return(this);
        }
        else
            return(NULL);
    }

    Qr *Qr::each_function_pattern_pixel(int *x,int *y)
    {
        if(this->each_finder_pattern_pixel(x,y))
            return(this);
        if(this->each_timing_pattern_pixel(x,y))
            return(this);
        if(this->each_alignment_pattern_pixel(x,y))
            return(this);
        if(this->each_version_pattern_pixel(x,y))
            return(this);
        return(this->each_formatinfo_pattern_pixel(x,y));
    }

    unsigned char *Qr::push_codedata(unsigned char data)
    {
        return(this->codedata->push(data));
    }


    int Qr::decode_codedata()
    {
        int ret=this->codedata->decode();
        this->status|=this->codedata->status;

        return(ret);
    }
}
