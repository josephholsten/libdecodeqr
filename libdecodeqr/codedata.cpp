/////////////////////////////////////////////////////////////////////////
//
// codedata.cpp --a part of libdecodeqr
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
#include "codedata.h"

namespace Qr{

    /////////////////////////////////////////////////////////////////////////
    //
    //  RS block structure table
    //   ( from JISX0510(2004) 8.5.1 Table.12-18 pp.30-36)
    // 
    //  [version][level]={
    //                     Tw:  total words,
    //                     Sb:  number of smaller RS blocks,
    //                     Tcs: total codes in each smaller RS block,
    //                     Dcs: data codes in each smaller RS block,
    //                     Ec:  error correctable words in each block
    //                   }
    //
    //  level M: 0
    //        L: 1
    //        H: 2
    //        Q: 3
    //
    //  Tcl: total codes in each largerer RS block = Tcs + 1
    //  Dcs: data codes in each larger RS block = Dcs + 1
    //  Lb:  Number of larger RS blocks = (Tw - Sb * Tcs) / Tcl
    //  Tb:  Total RS Blocks = Sb + Lb 
    //
    const static int RS_BLOCK_ALIGN[41][4][5]={
        // version 0 (not exist, just a dummy)
        {{0,0,0,0,0},{0,0,0,0,0},
         {0,0,0,0,0},{0,0,0,0,0}},
        
        // version 1
        {{26,1,26,16,4},{26,1,26,19,2},
         {26,1,26,9,8},{26,1,26,13,6}},
        {{44,1,44,28,8},{44,1,44,34,4},
         {44,1,44,16,14},{44,1,44,22,11}},
        {{70,1,70,44,13},{70,1,70,55,7},
         {70,2,35,13,11},{70,2,35,17,9}},
        {{100,2,50,32,9},{100,1,100,80,10},
         {100,4,25,9,8},{100,2,50,24,13}},
        
        {{134,2,67,43,12},{134,1,134,108,13},
         {134,2,33,11,11},{134,2,33,15,9}},
        {{172,4,43,27,8},{172,2,86,68,9},
         {172,4,43,15,14},{172,4,43,19,12}},
        {{196,4,49,31,9},{196,2,98,78,10},
         {196,4,39,13,13},{196,2,32,14,9}},
        {{242,2,60,38,11},{242,2,121,97,12},
         {242,4,40,14,13},{242,4,40,18,11}},
        {{292,3,58,36,11},{292,2,146,116,15},
         {292,4,36,12,12},{292,4,36,16,10}},
        
        // version 10
        {{346,4,69,43,13},{346,2,86,68,9},
         {346,6,43,15,14},{346,6,43,19,12}},
        {{404,1,80,50,15},{404,4,101,81,10},
         {404,3,36,12,12},{404,4,50,22,14}},
        {{466,6,58,36,11},{466,2,116,92,12},
         {466,7,42,14,14},{466,4,46,20,13}},
        {{532,8,59,37,11},{532,4,133,107,13},
         {532,12,33,11,11},{532,8,44,20,12}},
        {{581,4,64,40,12},{581,3,145,115,15},
         {581,11,36,12,12},{581,11,36,16,10}},
        
        {{655,5,65,41,12},{655,5,109,87,11},
         {655,11,36,12,12},{655,5,54,24,15}},
        {{733,7,73,45,14},{733,5,122,98,12},
         {733,3,45,15,15},{733,15,43,19,12}},
        {{815,10,74,46,14},{815,1,135,107,14},
         {815,2,42,14,14},{815,1,50,22,14}},
        {{901,9,69,43,13},{901,5,150,120,15},
         {901,2,42,14,14},{901,17,50,22,14}},
        {{991,3,70,44,13},{991,3,141,113,14},
         {991,9,39,13,13},{991,17,47,21,13}},
        
        // version 20
        {{1085,3,67,41,13},{1085,3,135,107,14},
         {1085,15,43,15,14},{1085,15,54,24,15}},
        {{1156,17,68,42,13},{1156,4,144,116,14},
         {1156,19,46,16,15},{1156,17,50,22,14}},
        {{1258,17,74,46,14},{1258,2,139,111,14},
         {1258,34,37,13,12},{1258,7,54,24,15}},
        {{1364,4,75,47,14},{1364,4,151,121,15},
         {1364,16,45,15,15},{1364,11,54,24,15}},
        {{1474,6,73,45,14},{1474,6,147,117,15},
         {1474,30,46,16,15},{1474,11,54,24,15}},
        
        {{1588,8,75,47,14},{1588,8,132,106,13},
         {1588,22,45,15,15},{1588,7,54,24,15}},
        {{1706,19,74,46,14},{1706,10,142,114,14},
         {1706,33,46,16,15},{1706,28,50,22,14}},
        {{1828,22,73,45,14},{1828,8,152,122,15},
         {1828,12,45,15,15},{1828,8,53,23,15}},
        {{1921,3,73,45,14},{1921,3,147,117,15},
         {1921,11,45,15,15},{1921,4,54,24,15}},
        {{2051,21,73,45,14},{2051,7,146,116,15},
         {2051,19,45,15,15},{2051,21,53,23,15}},
        
        // version 30
        {{2185,19,75,47,14},{2185,5,145,115,15},
         {2185,23,45,15,15},{2185,15,54,24,15}},
        {{2323,2,74,46,14},{2323,13,145,115,15},
         {2323,23,45,15,15},{2323,42,54,24,15}},
        {{2465,10,74,46,14},{2465,17,145,115,15},
         {2465,19,45,15,15},{2465,10,54,24,15}},
        {{2611,14,74,46,14},{2611,17,145,115,15},
         {2611,11,45,15,15},{2611,29,54,24,15}},
        {{2761,14,74,46,14},{2761,13,145,115,15},
         {2761,59,46,16,15},{2761,44,54,24,15}},
        
        {{2876,12,75,47,14},{2876,12,151,121,15},
         {2876,22,45,15,15},{2876,39,54,24,15}},
        {{3034,6,75,47,14},{3034,6,151,121,15},
         {3034,2,45,15,15},{3034,46,54,24,15}},
        {{3196,29,74,46,14},{3196,17,152,122,15},
         {3196,24,45,15,15},{3196,49,54,24,15}},
        {{3362,13,74,46,14},{3362,4,152,122,15},
         {3362,42,45,15,15},{3362,48,54,24,15}},
        {{3532,40,75,47,14},{3532,20,147,117,15},
         {3532,10,45,15,15},{3532,43,54,24,15}},
        
        // version 40
        {{3706,18,75,47,14},{3706,19,148,118,15},
         {3706,20,45,15,15},{3706,34,54,24,15}}
    };

    /////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    CodeBlock::CodeBlock(int total_words,int data_words,int capability,
                 Galois::Field *gf)
    {
        this->total_words=total_words;
        this->data_words=data_words;
        this->capability=capability;
        this->_gf=gf;
        
        this->data=new unsigned char[this->total_words];
        this->clear();
    }
    CodeBlock::~CodeBlock()
    {
        delete [] this->data;
    }
    
    void CodeBlock::clear()
    {
        memset(this->data,0,this->total_words);
        this->_size=0;
    }
    
    unsigned char *CodeBlock::push(unsigned char data)
    {
        if(this->_size>=this->total_words)
            return(NULL);
        
        this->data[this->_size]=data;
        this->_size++;
        return(&(this->data[this->_size-1]));
    }
    bool CodeBlock::has_vacant_data()
    {
        return(this->_size<this->data_words);
    }
    
    int CodeBlock::error_correct()
    {
        Galois::BCH *bch=new Galois::BCH(this->_gf,
                                         this->total_words,
                                         this->capability);

        int i,j;
        for(i=this->total_words-1,j=0;i>=0;i--,j++){
            bch->set(j,this->_gf->vect2nomial(this->data[i]));
        }
        int errors=bch->decode();
        if(errors<=0){
            delete bch;
            return(errors);
        }
        

        //
        // get each error size
        //
        //
        // JISX0510:2004 Appendix B (p.64) step c) makes us mistake.
        //
        // s(n): n.th digit of sent data on GF(2^8)
        // r(n): n.th digit of received data on GF(2^8)
        // e(n): n.th error data on GF(2^8)
        //
        // If there was 3 errors on 1st ,2nd ,and n.th digit, received
        // data R can be express shown as below;
        //
        //  R = ( r(0), r(1), r(2), ... r(n) )
        //    = ( s(0), s(1)+e(0), s(2)+e(1), ... s(n)+e(2) )
        //
        // Error syndromes ES(n) are;
        //
        //  ES(0) = sum{r(i) * a(0*i)}
        //        = sum{s(i)} +e(0) +e(1) +e(2)
        //        = e(0) +e(1) +e(2)
        //  ES(1) = sum{r(i)*a(1*i)}
        //        = sum{s(i)*a(i)} +a(1)*e(0) +a(2)*e(1) +a(n)*e(2)
        //        = a(1)*e(0) +a(2)*e(1) +a(n)*e(2)
        //    :
        //
        //  ES(x) = sum{r(i)*a(x*i)}
        //        = sum{s(i)*a(x*i)} +a(x)*e(0) +a(2*x)*e(1) +a(n*x)*e(2)
        //        = a(x)*e(0) +a(2*x)*e(1) +a(n*x)*e(2)
        //
        //        where a(x) is primitive of GF(2^8)
        //
        // So, each error size e(n) can be taken following equatation;
        //
        //  e(0)      +e(1)      +e(2)        =ES(0)
        //  a(1)*e(0) +a(2)*e(1) +a(n)*e(2)   =ES(1)
        //  a(2)*e(0) +a(4)*e(1) +a(2*n)*e(2) =ES(2)
        //
        Galois::Polynomial *mat=new Galois::Polynomial(errors,
                                                       errors+1);
        for(j=0;j<errors;j++){
            for(i=0;i<errors;i++){
                mat->set(j,i,this->_gf->exp2nomial(j*bch->error_pos[i]));
            }
            mat->set(j,i,bch->syndromes[j]);
        }
        Galois::Polynomial *err=mat->solve();
                
        if(err){
            //
            // correct received data
            //
            //
            // s(n) = r(n) + e(n)
            //
            // s(n): n.th digit of send data on GF(2^8)
            // r(n): n.th digit of recieved data on GF(2^8)
            // e(n): error size of n.th digit on GF(2^8)
            //
            for(i=0;i<errors;i++){
                int p=this->total_words-bch->error_pos[i]-1;
                this->data[p]=(*this->_gf->vect2nomial(this->data[p])+
                               *err->get(i)).to_vect();
            }
            delete err;
        }
        else{
            errors=-1;
        }
        
        delete mat;
        delete bch;
        
        return(errors);
    }
    
    
    /////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    CodeData::CodeData(int version,int level)
    {
        this->_gf=new Galois::Field(8);
        
        this->version=version;
        this->level=level;
        
        this->total_words=RS_BLOCK_ALIGN[version][level][0];
        int larger_block_words=RS_BLOCK_ALIGN[version][level][2]+1;
        int larger_block_datas=RS_BLOCK_ALIGN[version][level][3]+1;
        int larger_blocks=(this->total_words-
                           RS_BLOCK_ALIGN[version][level][1]*
                           RS_BLOCK_ALIGN[version][level][2]
            )/larger_block_words;
        
        this->data_blocks=RS_BLOCK_ALIGN[version][level][1]+larger_blocks;
        
        this->data_words=RS_BLOCK_ALIGN[version][level][1]*
            RS_BLOCK_ALIGN[version][level][3]+
            larger_blocks*larger_block_datas;
        
        this->data=new CodeBlock *[this->data_blocks];
        
        int i;
        for(i=0;i<RS_BLOCK_ALIGN[version][level][1];i++){
            this->data[i]=new CodeBlock(
                RS_BLOCK_ALIGN[version][level][2],
                RS_BLOCK_ALIGN[version][level][3],
                RS_BLOCK_ALIGN[version][level][4],
                this->_gf);
        }
        for(i=RS_BLOCK_ALIGN[version][level][1];i<this->data_blocks;i++){
            this->data[i]=new CodeBlock(
                larger_block_words,
                larger_block_datas,
                RS_BLOCK_ALIGN[version][level][4],
                this->_gf);
        }
        
        this->length=0;
        this->byte_length=0;
        this->_raw_data=NULL;
        
        
        this->_size=0;
        this->_index=0;
        
        this->status=0;
    }
    CodeData::~CodeData()
    {
        if(this->_raw_data)
            delete [] this->_raw_data;
        
        for(int i=0;i<this->data_blocks;i++){
            delete this->data[i];
        }
        delete [] this->data;
        delete this->_gf;
    }
    
    void CodeData::clear()
    {
        for(int i=0;i<this->data_blocks;i++){
            this->data[i]->clear();
        }
        
        this->length=0;
        this->byte_length=0;
        if(this->_raw_data){
            delete [] this->_raw_data;
            this->_raw_data=NULL;
        }
        this->_size=0;
        this->_index=0;

        this->status=0;
    }

    unsigned char *CodeData::push(unsigned char data)
    {
        if(this->_size<this->data_words){
            for(int i=0;i<this->data_blocks;i++){
                this->_index=(this->_index+i)%this->data_blocks;
                if(this->data[this->_index]->has_vacant_data()){
                    unsigned char *ret=this->data[this->_index]->push(data);
                    this->_size++;
                    this->_index++;
                    return(ret);
                }
            }
            throw;
        }
        else{
            this->_index=this->_index%this->data_blocks;
            unsigned char *ret=this->data[this->_index]->push(data);
            this->_size++;
            this->_index++;
            return(ret);
        }
    }
    
    unsigned char *CodeData::dump()
    {
        unsigned char *buf=new unsigned char[this->total_words];
        for(int i=0,j=0;i<data_blocks;i++){
            memcpy(buf+j,this->data[i]->data,
                   this->data[i]->total_words);
            j+=this->data[i]->total_words;
        }
        
        return(buf);
    }
    unsigned char *CodeData::dump_block(int index)
    {
        unsigned char *buf=new unsigned char[this->data[index]->total_words];
        memcpy(buf,this->data[index]->data,
               this->data[index]->total_words);
        
        return(buf);
    }
    unsigned char *CodeData::dump_data()
    {
        unsigned char *buf=new unsigned char[this->data_words];
        for(int i=0,j=0;i<data_blocks;i++){
            memcpy(buf+j,this->data[i]->data,
                   this->data[i]->data_words);
            j+=this->data[i]->data_words;
        }
        
        return(buf);
    }
    
    unsigned char *CodeData::raw_data()
    {
        return(this->_raw_data);
    }
    
    int CodeData::decode()
    {
        if(this->_raw_data)
            return(0);

        int ret=this->_error_correct();
        if(ret<0)
            this->status|=QR_CODEDATA_UNRECOVERABLE;

        unsigned char *tmp=this->dump_data();
        BitStream *bitstream=new BitStream(tmp,this->data_words);
        delete tmp;
        
        unsigned char mode=0;
        Qr::ECI::Decoder *decoder=NULL;
        do{
            bitstream->read(&mode,sizeof(mode),4);
            switch(mode){
            case 0: // end of data
                decoder=NULL;
                break;
            case 1:
                decoder=new Qr::ECI::NumericalDecoder();
                break;
            case 2: // arabic and numeric
                decoder=new Qr::ECI::AlphabeticalDecoder();
                break;
            case 4: // 8-bit byte
                decoder=new Qr::ECI::ByteDecoder();
                break;
            case 8: // kanji
                decoder=new Qr::ECI::KanjiDecoder();
                break;
            case 7: // ECI
            case 3: // joint
            case 5: // FNC1 1st
            case 9: // FNC1 2nd
            default:
                this->status|=QR_CODEDATA_NOT_SUPPORT_ECI;
                decoder=NULL;
            }
            
            if(!decoder)
                break;
            
            int ret=decoder->decode(this->version,bitstream);
            if(ret){
                if(this->_raw_data){
                    unsigned char *buf=this->_raw_data;
                    int sz=this->byte_length+decoder->byte_length;
                    this->_raw_data=new unsigned char[sz];
                    memcpy(this->_raw_data,buf,this->byte_length);
                    delete [] buf;
                    memcpy(this->_raw_data+this->byte_length,
                           decoder->raw_data(),
                           decoder->byte_length);
                }
                else{
                    this->_raw_data=new unsigned char[decoder->byte_length];
                    memcpy(this->_raw_data,
                           decoder->raw_data(),
                           decoder->byte_length);
                }
                this->length+=decoder->length;
                this->byte_length+=decoder->byte_length;
                
                delete decoder;
            }
        }while(!bitstream->is_eod());
        
        delete bitstream;
        
        return(ret);
    }

    int CodeData::_error_correct()
    {
        int ret=0;
        for(int i=0;i<this->data_blocks;i++){
            int c=this->data[i]->error_correct();
            if(ret<0||c<0){
                ret=-1;
            }
            else
                ret+=c;
        }
        
        return(ret);
    }
}
