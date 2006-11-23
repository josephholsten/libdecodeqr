/////////////////////////////////////////////////////////////////////////
//
// test_codedata.cpp --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id$
//
#include <stdio.h>
#include <assert.h>
#include "../libdecodeqr/codedata.h"

int main(int argc,char *argv[])
{
    int i;
    /*
    Qr::CodeData *words=new Qr::CodeData(1,1);
    unsigned char test_data2[26]={
        128,68,133,167,73,167,139,108,
        0,236,17,236,17,236,17,236,
        17,236,17,249,187,11,161,75,
        69,244
    };
    for(i=0;i<26;i++){
        words->push(test_data2[i]);
    }
    */

    Qr::CodeData *words=new Qr::CodeData(1,2);

    puts("testing no-error data");
    unsigned char sent[26]={
        32,65,205,69,41,220,46,128,
        236,42,159,74,221,244,169,239,
        150,138,70,237,85,224,96,74,
        219,61
    };
    for(i=0;i<26;i++){
        words->push(sent[i]);
    }
    assert(words->decode()==0);
    
    puts("testing 2 errors data");
    words->clear();
    unsigned char recv[26]={
        33,64,205,69,41,220,46,128,
        236,42,159,74,221,244,169,239,
        150,138,70,237,85,224,96,74,
        219,61
    };
    for(i=0;i<26;i++){
        words->push(recv[i]);
    }
    assert(words->decode()==2);
    
    puts("verifing corrected data");
    unsigned char *corrected=words->dump();
    for(i=0;i<26;i++){
        assert(corrected[i]==sent[i]);
    }

    delete corrected;
    delete words;

    puts("done");
    return(0);
}
