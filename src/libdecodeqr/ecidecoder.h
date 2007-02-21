/////////////////////////////////////////////////////////////////////////
//
// ecidecoder.h --a part of libdecodeqr
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
            Decoder();
            ~Decoder();
            
            unsigned char *raw_data();
            virtual int decode(int version,BitStream *bitstream);

        private:
            virtual int _read_header(int version,BitStream *bitstream);
            virtual int _get_charactor_count(int version)=0;
            virtual int _read_data(BitStream *bitstream);
        };

        class NumericalDecoder :public Decoder{
        private:
            short _read_buf;
        public:
            NumericalDecoder();
        private:
            virtual int _get_charactor_count(int version);
            virtual int _read_data(BitStream *bitstream);
        };

        class AlphabeticalDecoder :public Decoder{
        private:
            short _read_buf;
        public:
            AlphabeticalDecoder();
        private:
            virtual int _get_charactor_count(int version);
            virtual int _read_data(BitStream *bitstream);
        };

        class ByteDecoder :public Decoder{
        private:
            char _read_buf;
        public:
            ByteDecoder();
        private:
            virtual int _get_charactor_count(int version);
            virtual int _read_data(BitStream *bitstream);
        };
        
        class GenericDecoder :public Decoder{
        public:
            GenericDecoder();
        private:
            virtual int _get_charactor_count(int version);
            virtual int _read_data(BitStream *bitstream);
        };
        
        class KanjiDecoder :public Decoder{
        private:
            short _read_buf;
        public:
            KanjiDecoder();
        private:
            virtual int _get_charactor_count(int version);
            virtual int _read_data(BitStream *bitstream);
        };
    };
}

#endif
