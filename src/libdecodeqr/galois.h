/////////////////////////////////////////////////////////////////////////
//
// galois.h --a part of libdecodeqr
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
#ifndef __GALOIS__
#define __GALOIS__

#include <memory.h>

#ifndef NULL
#define NULL 0
#endif

namespace Galois{
    class Nomial{
    public:
        unsigned int val;
        
    private:
        void *_gf;
        
    public:
        static Nomial *instance(void * gf,unsigned int x);
        Nomial *dup();

        unsigned int to_exp();
        unsigned int to_vect();

        inline int m();
        inline int n();
        inline unsigned int exp2vect(unsigned int x);
        inline unsigned int vect2exp(unsigned int x);

        bool is_zero();
        bool operator==(Nomial x);
        bool operator!=(Nomial x);
        Nomial operator+(Nomial x);
        Nomial operator-(Nomial x);
        Nomial operator*(Nomial x);
        Nomial operator/(Nomial x);

    protected:
        Nomial(void * gf,unsigned int x);
    };

    class Field{
    public:
        int m;
        int n;
        unsigned int *exp2vect;
        unsigned int *vect2exp;
        Nomial **pool;
    private:
        int _pool_size;
        bool _need_delete;
        
    public:
        Field(int m);
        ~Field();

        int pool_size();

        Nomial *exp2nomial(unsigned int x);
        Nomial *vect2nomial(unsigned int x);
        Nomial *zero();
    };


    class Polynomial{
    public:
        int cols;
        int rows;
        Nomial **nomial;

        Polynomial();
        Polynomial(int rows);
        Polynomial(int cols,int rows);
        ~Polynomial();

        Polynomial *dup();
        Polynomial *dup(int count);
        Polynomial *dup(int start_col,int start_row,int count);
        Polynomial *dup(int start_col,int start_row,
                                    int col_count,int row_count);

        Nomial *set(int row,Nomial *val);
        Nomial *set(int col,int row,Nomial *val);

        Nomial *get(int row);
        Nomial *get(int col,int row);

        Polynomial *lu();
        Polynomial *lu(int count);
        Polynomial *lu(int start_col,int start_row,int count);
        Polynomial *_lu(Polynomial *buf);

        Polynomial *solve();
        Polynomial *solve(Polynomial *lu);

        void swap_col(int i,int j);
    };

    class BCH :public Polynomial{
    public:
        int error_size;
        int *error_pos;
        int syndrome_size;
        Galois::Nomial **syndromes;

    private:
        Field *_gf;
        int _capability;

    public:
        BCH(Field *gf,int size,int capability);
        ~BCH();

        int decode(int syndorome_base=0);

    private:
        Galois::Nomial *_error_syndrome(int d);
    };
}

#endif
