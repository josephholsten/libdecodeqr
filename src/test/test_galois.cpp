/////////////////////////////////////////////////////////////////////////
//
// test_galois.cpp --a part of libdecodeqr
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
#include <stdio.h>
#include <assert.h>
#include "../libdecodeqr/galois.h"

int main(int argc,char *argv[])
{
    puts("testing gaussian elimination on GF(2^8)");

    Galois::Field *gf=new Galois::Field(8);
    
    //
    // may return (1,2);
    //
    Galois::Polynomial *mat=new Galois::Polynomial(2,3);

    mat->set(0,0,gf->exp2nomial(1));
    mat->set(0,1,gf->exp2nomial(2));
    mat->set(0,2,&(*gf->exp2nomial(1)*
                   *gf->exp2nomial(1)+
                   *gf->exp2nomial(2)*
                   *gf->exp2nomial(2))); //a^52
    mat->set(1,0,gf->exp2nomial(2));
    mat->set(1,1,gf->exp2nomial(4));
    mat->set(1,2,&(*gf->exp2nomial(2)*
                   *gf->exp2nomial(1)+
                   *gf->exp2nomial(4)*
                   *gf->exp2nomial(2))); //a^226
    
    Galois::Polynomial *a=mat->solve();

    assert(a&&a->nomial[0]->val==1&&a->nomial[1]->val==2);

    delete a;
    delete mat;
    delete gf;

    puts("done");
    return(0);
}
