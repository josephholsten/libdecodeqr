/////////////////////////////////////////////////////////////////////////
//
// ecidecoder.h --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id$
//
#ifndef __QR_ECI_DECODER__
#define __QR_ECI_DECODER__

#include <stdio.h>

#ifdef WIN32
#include <winsock2.h>
#define snprintf _snprintf
#else
#include <netinet/in.h>
#endif

#include "bitstream.h"

namespace Qr{
    namespace ECI{
        class Decoder{
        public:
            int mode;
            int length;
            int byte_length;
            int eci_mode;

        protected:
            unsigned char *_raw_data;
            int _bit_par_block;
            int _char_par_block;
            int _byte_par_char;

            int _read_length;
            int _written_length;
            unsigned char *_current_pos;

        public:
            Decoder::Decoder();
            Decoder::~Decoder();
            
            unsigned char *Decoder::raw_data();
            virtual int Decoder::decode(int version,BitStream *bitstream);

        private:
            virtual int Decoder::_read_header(int version,BitStream *bitstream);
            virtual int Decoder::_get_charactor_count(int version)=0;
            virtual int Decoder::_read_data(BitStream *bitstream);
        };

        class NumericalDecoder :public Decoder{
        private:
            short _read_buf;
        public:
            NumericalDecoder::NumericalDecoder();
        private:
            virtual int NumericalDecoder::_get_charactor_count(int version);
            virtual int NumericalDecoder::_read_data(BitStream *bitstream);
        };

        class AlphabeticalDecoder :public Decoder{
        private:
            short _read_buf;
        public:
            AlphabeticalDecoder::AlphabeticalDecoder();
        private:
            virtual int AlphabeticalDecoder::_get_charactor_count(int version);
            virtual int AlphabeticalDecoder::_read_data(BitStream *bitstream);
        };

        class ByteDecoder :public Decoder{
        private:
            char _read_buf;
        public:
            ByteDecoder::ByteDecoder();
        private:
            virtual int ByteDecoder::_get_charactor_count(int version);
            virtual int ByteDecoder::_read_data(BitStream *bitstream);
        };
        
        class GenericDecoder :public Decoder{
        public:
            GenericDecoder::GenericDecoder();
        private:
            virtual int GenericDecoder::_get_charactor_count(int version);
            virtual int GenericDecoder::_read_data(BitStream *bitstream);
        };
        
        class KanjiDecoder :public Decoder{
        private:
            short _read_buf;
        public:
            KanjiDecoder::KanjiDecoder();
        private:
            virtual int KanjiDecoder::_get_charactor_count(int version);
            virtual int KanjiDecoder::_read_data(BitStream *bitstream);
        };
    };
};

#endif
