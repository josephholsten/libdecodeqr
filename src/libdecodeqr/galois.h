/////////////////////////////////////////////////////////////////////////
//
// galois.h --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id:$
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
        static Nomial *Nomial::instance(void * gf,unsigned int x);
        Nomial *Nomial::dup();

        unsigned int Nomial::to_exp();
        unsigned int Nomial::to_vect();

        inline int m();
        inline int n();
        inline unsigned int Nomial::exp2vect(unsigned int x);
        inline unsigned int Nomial::vect2exp(unsigned int x);

        bool Nomial::is_zero();
        bool Nomial::operator==(Nomial x);
        bool Nomial::operator!=(Nomial x);
        Nomial Nomial::operator+(Nomial x);
        Nomial Nomial::operator-(Nomial x);
        Nomial Nomial::operator*(Nomial x);
        Nomial Nomial::operator/(Nomial x);

    protected:
        Nomial::Nomial(void * gf,unsigned int x);
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
        Field::Field(int m,const int *generator_polynomial=NULL);
        Field::~Field();

        int Field::pool_size();

        Nomial *Field::exp2nomial(unsigned int x);
        Nomial *Field::vect2nomial(unsigned int x);
        Nomial *Field::zero();

    private:
        void Field::_gen_gf(const int *generator_polynomial);
    };


    class Polynomial{
    public:
        int cols;
        int rows;
        Nomial **nomial;

        Polynomial::Polynomial();
        Polynomial::Polynomial(int rows);
        Polynomial::Polynomial(int cols,int rows);
        Polynomial::~Polynomial();

        Polynomial *Polynomial::dup();
        Polynomial *Polynomial::dup(int count);
        Polynomial *Polynomial::dup(int start_col,int start_row,int count);
        Polynomial *Polynomial::dup(int start_col,int start_row,
                                    int col_count,int row_count);

        Nomial *Polynomial::set(int row,Nomial *val);
        Nomial *Polynomial::set(int col,int row,Nomial *val);

        Nomial *Polynomial::get(int row);
        Nomial *Polynomial::get(int col,int row);

        Polynomial *Polynomial::lu();
        Polynomial *Polynomial::lu(int count);
        Polynomial *Polynomial::lu(int start_col,int start_row,int count);
        Polynomial *Polynomial::_lu(Polynomial *buf);

        Polynomial *Polynomial::solve();
        Polynomial *Polynomial::solve(Polynomial *lu);

        void Polynomial::swap_col(int i,int j);
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
        BCH::BCH(Field *gf,int size,int capability);
        BCH::~BCH();

        int BCH::decode(int syndorome_base=0);

    private:
        Galois::Nomial *BCH::_error_syndrome(int d);
    };
}

#endif
