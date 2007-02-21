/////////////////////////////////////////////////////////////////////////
//
// bitstream.cpp --a part of libdecodeqr
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
#include "bitstream.h"

namespace Qr{
    BitStream::BitStream()
    {
        this->data=NULL;
        this->byte_size=0;
        this->bit_size=0;
        this->_pos=0;
    }
    BitStream::BitStream(void *src,int size)
    {
        this->byte_size=size;
        this->bit_size=size<<3;
        this->data=new unsigned char[size];
        memcpy(this->data,src,size);
        this->_pos=0;
    }
    BitStream::~BitStream()
    {
        if(this->data)
            delete this->data;
    }

    bool BitStream::is_eod()
    {
        return(this->_pos>=this->bit_size);
    }
    int BitStream::position()
    {
        return(this->_pos);
    }
    int BitStream::seek(int pos)
    {
        this->_pos+=pos;
        if(this->_pos<0)
            this->_pos=0;
        if(this->_pos>this->bit_size)
            this->_pos=this->bit_size;
        
        return(this->_pos);
    }
    void BitStream::rewind()
    {
        this->_pos=0;
    }

    unsigned char *BitStream::read(int read_bits)
    {
        int byte_size=(read_bits>>3)+(read_bits&0x07?1:0);
        unsigned char *buf=new unsigned char[byte_size];
        memset(buf,0,byte_size);

        this->read(buf,byte_size,read_bits);

        return(buf);
    }
    int BitStream::read(void *dst,int buf_size,int read_bits)
    {
        memset(dst,0,buf_size);

        if(read_bits>(buf_size<<3))
            read_bits=buf_size<<3;

        int end_bit=this->_pos+read_bits-1;
        if(end_bit>=this->bit_size){
            end_bit=this->bit_size-1;
            read_bits=this->bit_size-this->_pos;
        }
        int remain_bits=(end_bit+1)&0x07;
        int offset=this->_pos>>3;
        int read_bytes=(read_bits>>3)+(read_bits&0x07?1:0);

        if(read_bytes<buf_size){
            int diff=buf_size-read_bytes;
            unsigned char *tmp=(unsigned char *)dst;
            tmp+=diff;
            dst=tmp;
        }

        if(remain_bits){
            unsigned char *src=data+offset;
            unsigned char *tmp=(unsigned char *)dst;

            int i=0;
            if(read_bytes==(end_bit>>3)-offset+1){
                tmp++;
                i++;
            }
            for(;i<read_bytes;i++,src++,tmp++){
                *tmp=*src<<remain_bits;
            }
            remain_bits=8-remain_bits;
            src=data+(end_bit>>3);
            tmp--;
            for(i=0;i<read_bytes;i++,src--,tmp--){
                *tmp|=*src>>remain_bits;
            }
        }
        else{
            memcpy(dst,this->data+offset,read_bytes);
        }

        unsigned char mask=0xff;
        switch(8-(read_bits&0x07)){
        case 1:
            mask=0x7f;
            break;
        case 2:
            mask=0x3f;
            break;
        case 3:
            mask=0x1f;
            break;
        case 4:
            mask=0x0f;
            break;
        case 5:
            mask=0x07;
            break;
        case 6:
            mask=0x03;
            break;
        case 7:
            mask=0x01;
            break;
        }            
        *((unsigned char *)dst)&=mask;
        this->_pos+=read_bits;

        return(read_bits);
    }

}
