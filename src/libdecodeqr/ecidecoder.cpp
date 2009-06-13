/////////////////////////////////////////////////////////////////////////
//
// ecidecoder.cpp --a part of libdecodeqr
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
#include "ecidecoder.h"

namespace Qr{
    namespace ECI{

        //
        // length of charactor count indicator
        // [version][mode]
        //
        const static int CHARACTOR_COUNTS[41][4]={
            //
            // version 0 (not exist, just a dummy)
            //
            {0,0,0,0},
            //
            // version 1-9
            //
            {10,9,8,8},{10,9,8,8},{10,9,8,8},{10,9,8,8},
            {10,9,8,8},{10,9,8,8},{10,9,8,8},{10,9,8,8},{10,9,8,8},
            //
            // version 10-26
            //
            {12,11,16,10},{12,11,16,10},{12,11,16,10},{12,11,16,10},{12,11,16,10},
            {12,11,16,10},{12,11,16,10},{12,11,16,10},{12,11,16,10},{12,11,16,10},
            {12,11,16,10},{12,11,16,10},{12,11,16,10},{12,11,16,10},{12,11,16,10},
            {12,11,16,10},{12,11,16,10},
            //
            // version 27-40
            //
            {14,13,16,12},{14,13,16,12},{14,13,16,12},
            {14,13,16,12},{14,13,16,12},{14,13,16,12},{14,13,16,12},{14,13,16,12},
            {14,13,16,12},{14,13,16,12},{14,13,16,12},{14,13,16,12},{14,13,16,12},
            {14,13,16,12}
        };
        
        //
        // number to alphabet conversion table
        //
        const static char NUM2ALPABET[45]={
            0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39, // 0-9
            0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4a, // A-J
            0x4b,0x4c,0x4d,0x4e,0x4f,0x50,0x51,0x52,0x53,0x54, // K-T
            0x55,0x56,0x57,0x58,0x59,0x5a,                     // U-Z
            0x20,0x23,0x25,0x2a,0x2b,0x2d,0x2e,0x2f,0x3a
        };

        Decoder::Decoder()
        {
            this->mode=0;
            this->length=0;
            this->byte_length=0;
            this->eci_mode=20;

            this->_raw_data=NULL;
            this->_bit_par_block=0;
            this->_char_par_block=0;
            this->_byte_par_char=0;

            this->_read_length=0;
            this->_written_length=0;
        }

        Decoder::~Decoder()
        {
            if(this->_raw_data)
                delete [] this->_raw_data;
        }
        
        unsigned char *Decoder::raw_data()
        {
            return(this->_raw_data);
        }

        int Decoder::decode(int version,BitStream *bitstream)
        {
            this->_read_header(version,bitstream);
            int blocks=this->length/this->_char_par_block+
                (this->length%this->_char_par_block?1:0);

            for(int i=0;i<blocks;i++){
                if(!this->_read_data(bitstream))
                    break;
            }

            return(this->length);
        }
        int Decoder::_read_header(int version,BitStream *bitstream)
        {
            int charactor_count=this->_get_charactor_count(version);
            bitstream->read(&(this->length),sizeof(this->length),
                            charactor_count);
            this->length=ntohl(this->length);
            this->byte_length=this->length*this->_byte_par_char;

            int storage_sz=this->byte_length;
            if(this->mode!=4)
                storage_sz++;

            this->_raw_data=new unsigned char[storage_sz];
            memset(this->_raw_data,0,storage_sz);
            this->_current_pos=this->_raw_data;

            return(bitstream->position());
        }
        int Decoder::_get_charactor_count(int version)
        {
            return(0);
        }
        int Decoder::_read_data(BitStream *bitstream)
        {
            return(0);
        }

        NumericalDecoder::NumericalDecoder()
        {
            this->mode=1;
            this->_bit_par_block=10;
            this->_char_par_block=3;
            this->_byte_par_char=1;
        }
        int NumericalDecoder::_get_charactor_count(int version)
        {
            return(CHARACTOR_COUNTS[version][0]);
        }
        int NumericalDecoder::_read_data(BitStream *bitstream)
        {
            if(bitstream->is_eod())
                return(0);

            int read_bits=this->_bit_par_block;
            int write_bytes=this->_char_par_block*this->_byte_par_char;
            switch(this->length-this->_read_length){
            case 0:
                return(0);
            case 1:
                read_bits=4;
                write_bytes=1;
                break;
            case 2:
                read_bits=7;
                write_bytes=2;
                break;
            }
            this->_read_buf=0;
            bitstream->read(&(this->_read_buf),sizeof(this->_read_buf),
                            read_bits);
            this->_read_buf=ntohs(this->_read_buf);
            this->_read_length+=write_bytes;
            
            int remain_bytes=this->byte_length-this->_written_length;
            if(write_bytes>remain_bytes)
                write_bytes=remain_bytes;

            char format_str[5];
            snprintf(format_str,5,"%%0%dd",write_bytes);
            int ret=snprintf((char *)this->_current_pos,
                             write_bytes+1,
                             format_str,
                             this->_read_buf);

            if(ret<0)
                ret=0-ret;
            this->_current_pos+=ret;
            this->_written_length+=ret;

            return(ret);
        }


        AlphabeticalDecoder::AlphabeticalDecoder()
        {
            this->mode=2;
            this->_bit_par_block=11;
            this->_char_par_block=2;
            this->_byte_par_char=1;
        }
        int AlphabeticalDecoder::_get_charactor_count(int version)
        {
            return(CHARACTOR_COUNTS[version][1]);
        }
        int AlphabeticalDecoder::_read_data(BitStream *bitstream)
        {
            if(bitstream->is_eod())
                return(0);

            int read_bits=this->_bit_par_block;
            int write_bytes=this->_char_par_block*this->_byte_par_char;
            switch(this->length-this->_read_length){
            case 0:
                return(0);
            case 1:
                read_bits=6;
                write_bytes=1;
                break;
            }
            this->_read_buf=0;
            bitstream->read(&(this->_read_buf),sizeof(this->_read_buf),
                            read_bits);
            this->_read_buf=ntohs(this->_read_buf);
            this->_read_length+=write_bytes;

            int remain_bytes=this->byte_length-this->_written_length;
            int ret=0;
            if(read_bits==this->_bit_par_block){
                int x=this->_read_buf/45;
                char c=NUM2ALPABET[x];
                if(remain_bytes>0){
                    if(snprintf((char *)this->_current_pos,2,"%c",c)>0){
                        ret++;
                        this->_current_pos++;
                        remain_bytes--;
                        this->_written_length++;
                    }
                }
            }
            if(remain_bytes>0){
                int x=this->_read_buf%45;
                char c=NUM2ALPABET[x];
                if(snprintf((char *)this->_current_pos,2,"%c",c)>0){
                    ret++;
                    this->_current_pos++;
                    remain_bytes--;
                    this->_written_length++;
                }
            }
            
            return(ret);
        }

        ByteDecoder::ByteDecoder()
        {
            this->mode=4;
            this->_bit_par_block=8;
            this->_char_par_block=1;
            this->_byte_par_char=1;
        }
        int ByteDecoder::_get_charactor_count(int version)
        {
            return(CHARACTOR_COUNTS[version][2]);
        }
        int ByteDecoder::_read_data(BitStream *bitstream)
        {
            if(bitstream->is_eod())
                return(0);

            int read_bits=this->_bit_par_block;
            int write_bytes=this->_char_par_block*this->_byte_par_char;
            this->_read_buf=0;
            bitstream->read(&(this->_read_buf),sizeof(this->_read_buf),
                            read_bits);
            this->_read_length+=write_bytes;

            if(this->byte_length-this->_written_length){
                *this->_current_pos=(unsigned char)this->_read_buf;
                this->_current_pos++;
                this->_written_length++;
                return(1);
            }
            else{
                return(0);
            }
        }

        GenericDecoder::GenericDecoder()
        {
            this->mode=7;
        }
        int GenericDecoder::_get_charactor_count(int version)
        {
            return(0);
        }
        int GenericDecoder::_read_data(BitStream *bitstream)
        {
            return(0);
        }

        KanjiDecoder::KanjiDecoder()
        {
            this->mode=8;
            this->_bit_par_block=13;
            this->_char_par_block=1;
            this->_byte_par_char=2;
        }
        int KanjiDecoder::_get_charactor_count(int version)
        {
            return(CHARACTOR_COUNTS[version][3]);
        }
        int KanjiDecoder::_read_data(BitStream *bitstream)
        {
            if(bitstream->is_eod())
                return(0);

            int read_bits=this->_bit_par_block;
            int write_bytes=this->_char_par_block*this->_byte_par_char;
            if(this->length-this->_read_length<=0)
                return(0);

            this->_read_buf=0;
            bitstream->read(&(this->_read_buf),sizeof(this->_read_buf),
                            read_bits);
            this->_read_buf=ntohs(this->_read_buf);
            this->_read_length++;

            if(this->byte_length-this->_written_length>=write_bytes){
                int c=this->_read_buf/0xc0;
                c*=0x100;
                c+=this->_read_buf%0xc0;
                if(c>=0x1f00)
                    c+=0xc140;
                else
                    c+=0x8140;

                unsigned char c1=(unsigned char)(c/0x100);
                unsigned char c2=(unsigned char)(c%0x100);
                
                *this->_current_pos=c1;
                *(this->_current_pos+1)=c2;
                this->_current_pos+=2;
                this->_written_length+=2;
                return(2);
            }
            else
                return(0);
        }
    };
};


