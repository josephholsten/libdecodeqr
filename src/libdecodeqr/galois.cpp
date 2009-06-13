/////////////////////////////////////////////////////////////////////////
//
// galois.cpp --a part of libdecodeqr
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
#include "galois.h"

namespace Galois{

    /////////////////////////////////////////////////////////////////////
    //
    // galois field GF(2^4); G(x)=x^10+x^8+x^5+x^4+x^2+x+1
    //

    //
    // generator polynomial
    //
    static const int bch_15_5_generator_polynomial[11]={
        1,1,1,0,1,1,0,0,1,0,1
    };

    //
    // exponent -> vector  notation convert table
    //
    static const unsigned int gf2_4_exp2vect[16]={
        1,2,4,8,3,6,12,11,5,10,7,14,15,13,9,0
    };

    //
    // vector -> exponent notation convert table
    //
    static const unsigned int gf2_4_vect2exp[16]={
        15,0,1,4,2,8,5,10,3,14,9,7,6,13,11,12
    };
    
    /////////////////////////////////////////////////////////////////////
    //
    // galois field GF(2^8); G(x)=X^8+x^4+x^3+x^2+1
    //

    //
    // generator polynomial
    //
    static const int bch_16_8_generator_polynomial[9]={
        1,0,1,1,1,0,0,0,1
    };
    
    //
    // exponent -> vector notation convert table
    //
    static const unsigned int gf2_8_exp2vect[256]={
        1,2,4,8,16,32,64,128,29,58,116,232,205,135,19,38,
        76,152,45,90,180,117,234,201,143,3,6,12,24,48,96,192,
        157,39,78,156,37,74,148,53,106,212,181,119,238,193,159,35,
        70,140,5,10,20,40,80,160,93,186,105,210,185,111,222,161,
        95,190,97,194,153,47,94,188,101,202,137,15,30,60,120,240,
        253,231,211,187,107,214,177,127,254,225,223,163,91,182,113,226,
        217,175,67,134,17,34,68,136,13,26,52,104,208,189,103,206,
        129,31,62,124,248,237,199,147,59,118,236,197,151,51,102,204,
        133,23,46,92,184,109,218,169,79,158,33,66,132,21,42,84,
        168,77,154,41,82,164,85,170,73,146,57,114,228,213,183,115,
        230,209,191,99,198,145,63,126,252,229,215,179,123,246,241,255,
        227,219,171,75,150,49,98,196,149,55,110,220,165,87,174,65,
        130,25,50,100,200,141,7,14,28,56,112,224,221,167,83,166,
        81,162,89,178,121,242,249,239,195,155,43,86,172,69,138,9,
        18,36,72,144,61,122,244,245,247,243,251,235,203,139,11,22,
        44,88,176,125,250,233,207,131,27,54,108,216,173,71,142,0
    };
    
    //
    // vector -> exponent notation convert table
    //
    static const unsigned int gf2_8_vect2exp[256]={
        255,0,1,25,2,50,26,198,3,223,51,238,27,104,199,75,
        4,100,224,14,52,141,239,129,28,193,105,248,200,8,76,113,
        5,138,101,47,225,36,15,33,53,147,142,218,240,18,130,69,
        29,181,194,125,106,39,249,185,201,154,9,120,77,228,114,166,
        6,191,139,98,102,221,48,253,226,152,37,179,16,145,34,136,
        54,208,148,206,143,150,219,189,241,210,19,92,131,56,70,64,
        30,66,182,163,195,72,126,110,107,58,40,84,250,133,186,61,
        202,94,155,159,10,21,121,43,78,212,229,172,115,243,167,87,
        7,112,192,247,140,128,99,13,103,74,222,237,49,197,254,24,
        227,165,153,119,38,184,180,124,17,68,146,217,35,32,137,46,
        55,63,209,91,149,188,207,205,144,135,151,178,220,252,190,97,
        242,86,211,171,20,42,93,158,132,60,57,83,71,109,65,162,
        31,45,67,216,183,123,164,118,196,23,73,236,127,12,111,246,
        108,161,59,82,41,157,85,170,251,96,134,177,187,204,62,90,
        203,89,95,176,156,169,160,81,11,245,22,235,122,117,44,215,
        79,174,213,233,230,231,173,232,116,214,244,234,168,80,88,175
    };

    //
    // G(x)=x^12+x^11+x^10+x^9+x^8+x^5+x^2+1
    //
    static const int bch_18_6_generator_polynomial[13]={
        1,0,1,0,0,1,0,0,1,1,1,1,1
    };
    

    /////////////////////////////////////////////////////////////////////
    //
    //
    //
    Nomial::Nomial(void *gf,unsigned int x)
    {
        this->val=x;
        this->_gf=gf;
        ((Field *)(this->_gf))->pool[x]=this;
    }

    Nomial *Nomial::instance(void *gf,unsigned int x)
    {
        x=x%(((Field *)gf)->pool_size());
        if(!((Field *)gf)->pool[x])
            new Nomial(gf,x);

        return(((Field *)gf)->pool[x]);
    }

    Nomial *Nomial::dup()
    {
        return(Nomial::instance(this->_gf,this->val));
    }

    
    unsigned int Nomial::to_exp()
    {
        return(this->val);
    }
    unsigned int Nomial::to_vect()
    {
        return(((Field *)this->_gf)->exp2vect[this->val]);
    }

    inline int Nomial::m()
    {
        return(((Field *)(this->_gf))->m);
    }
    inline int Nomial::n()
    {
        return(((Field *)(this->_gf))->n);
    }
    inline unsigned int Nomial::exp2vect(unsigned int x)
    {
        return(((Field *)(this->_gf))->exp2vect[x]);
    }
    inline unsigned int Nomial::vect2exp(unsigned int x)
    {
        return(((Field *)(this->_gf))->vect2exp[x]);
    }

    bool Nomial::is_zero()
    {
        return(this->to_vect()==0);
    }
    bool Nomial::operator==(Nomial x)
    {
        return(this->val==x.val);
    }
    bool Nomial::operator!=(Nomial x)
    {
        return(this->val!=x.val);
    }
    Nomial Nomial::operator+(Nomial x)
    {
        return(*Nomial::instance(this->_gf,
                                 this->vect2exp(this->to_vect()^
                                                x.to_vect())));
    }
    Nomial Nomial::operator-(Nomial x)
    {
        return(*this+x);
    }
    Nomial Nomial::operator*(Nomial x)
    {
        if(this->is_zero()||x.is_zero())
            return(*(((Field *)(this->_gf))->zero()));
        else
            return(*Nomial::instance(this->_gf,(this->val+x.val)%this->n()));

    }
    Nomial Nomial::operator/(Nomial x)
    {
        if(x.is_zero())
            throw(x);
        else if(this->is_zero())
            return(*(((Field *)(this->_gf))->zero()));
        else
            return(*Nomial::instance(this->_gf,
                                 (this->val+this->n()-x.val)%this->n()));
    }

    /////////////////////////////////////////////////////////////////////
    //
    //
    //
    Field::Field(int m)
    {
        this->_need_delete=false;

        this->m=m;
        int i;
        for(i=0,this->n=1;i<m;i++)
            this->n*=2;
        this->n-=1;

        switch(this->m){
        case 4:
            this->exp2vect=(unsigned int *)gf2_4_exp2vect;
            this->vect2exp=(unsigned int *)gf2_4_vect2exp;
            break;
        case 8:
            this->exp2vect=(unsigned int *)gf2_8_exp2vect;
            this->vect2exp=(unsigned int *)gf2_8_vect2exp;
            break;
        default:
            throw;
        }

       this->_pool_size=this->n+1;
       this->pool=new Nomial *[this->_pool_size];
       for(i=0;i<this->_pool_size;i++){
           this->pool[i]=NULL;
           Nomial::instance(this,i);
       }
    }
    Field::~Field()
    {
        for(int i=0;i<this->_pool_size;i++){
            if(this->pool[i])
                delete this->pool[i];
	}
        delete [] this->pool;

        if(this->_need_delete){
            delete this->exp2vect;
            delete this->vect2exp;
        }
    }

    int Field::pool_size()
    {
        return(this->_pool_size);
    }

    Nomial *Field::exp2nomial(unsigned int x)
    {
        return(this->pool[x%(this->n)]);
    }
    Nomial *Field::vect2nomial(unsigned int x)
    {
        return(this->pool[this->vect2exp[x%(this->n)]]);
    }
    Nomial *Field::zero()
    {
        return(this->pool[this->n]);
    }

    ////////////////////////////////////////////////////////////////////////
    //
    //
    //
    Polynomial::Polynomial()
    {
        this->cols=0;
        this->rows=0;

        this->nomial=NULL;
    }

    Polynomial::Polynomial(int rows)
    {
        this->cols=1;
        this->rows=rows;

        this->nomial=new Galois::Nomial *[this->rows];
    }
    Polynomial::Polynomial(int cols,int rows)
    {
        this->cols=cols;
        this->rows=rows;

        this->nomial=new Nomial *[this->cols*this->rows];
    }
    Polynomial::~Polynomial()
    {
        delete this->nomial;
    }
    
    Polynomial *Polynomial::dup()
    {
        Polynomial *ret=new Polynomial(this->cols,this->rows);
        memcpy(ret->nomial,this->nomial,
               sizeof(Nomial *)*this->cols*this->rows);
        
        return(ret);
    }
    Polynomial *Polynomial::dup(int count)
    {
        return(this->dup(0,0,count,count));
    }
    Polynomial *Polynomial::dup(int start_col,int start_row,int count)
    {
        return(this->dup(start_col,start_row,count,count));
    }
    Polynomial *Polynomial::dup(int start_col,int start_row,
                              int col_count,int row_count)
    {
        Polynomial *ret=new Polynomial(col_count,row_count);
        
        for(int i=0,c=start_col;i<col_count;i++,c++){
            for(int j=0,r=start_row;j<row_count;j++,r++){
                ret->set(i,j,this->get(c,r));
            }
        }

        return(ret);
    }


    Nomial *Polynomial::set(int row,Nomial *val)
    {
        return(this->set(0,row,val));
    }
    Nomial *Polynomial::set(int col,int row,Nomial *val)
    {
        this->nomial[col*rows+row]=val->dup();
        return(this->nomial[col*rows+row]);
    }

    Nomial *Polynomial::get(int row)
    {
        return(this->get(0,row));
    }
    Nomial *Polynomial::get(int col,int row)
    {
        return(this->nomial[col*rows+row]->dup());
    }


    /////////////////////////////////////////////////////////////////////
    //
    // LU analyze with pivot selection
    //
    Polynomial *Polynomial::lu()
    {
        Polynomial *buf=this->dup();
        Polynomial *ret=buf->_lu(buf);
        if(!ret)
            delete buf;
        return(ret);
    }
    Polynomial *Polynomial::lu(int count)
    {
        Polynomial *buf=this->dup(0,0,count);
        Polynomial *ret=buf->_lu(buf);
        if(!ret)
            delete buf;
        return(ret);
    }
    Polynomial *Polynomial::lu(int start_col,int start_row,int count)
    {
        Polynomial *buf=this->dup(start_col,start_row,count);
        Polynomial *ret=buf->_lu(buf);
        if(!ret)
            delete buf;
        return(ret);
    }

    Polynomial *Polynomial::_lu(Polynomial *buf)
    {
        int count=buf->cols,i,j;
        if(buf->rows<buf->cols)
            count=buf->rows;

        for(j=0;j<count;j++){
            Nomial *l;
            //
            // pivot
            //
            for(i=j;i<count;i++){
                l=buf->get(i,j);
                if(!l->is_zero())
                    break;
            }
            if(i>=count)
                return(NULL);
            else if(i>j)
                buf->swap_col(j,i);

            for(i=j+1;i<count;i++){
                Nomial *n=&(*buf->get(i,j)/(*l));
                buf->set(i,j,n);
                for(int k=j+1;k<count;k++){
                    Nomial *m=&(*buf->get(i,k)-*buf->get(j,k)**n);
                    *buf->set(i,k,m);
                }
            }
        }
        return(buf);
    }

    /////////////////////////////////////////////////////////////////////
    //
    // Gaussian elimination
    //
    Polynomial *Polynomial::solve()
    {
        Polynomial *lu=this->lu();
        if(lu){
            Polynomial *ret=this->solve(lu);
            delete lu;
            return(ret);
        }
        else{
            return(NULL);
        }
    }

    Polynomial *Polynomial::solve(Polynomial *lu)
    {
        if(!lu)
            return(NULL);

        if(lu->rows!=this->cols+1)
            throw((void *)NULL);

        //
        // check rank
        //
        int rank=0,i,j,k;
        for(j=0;j<lu->cols;j++){
            bool is_zero_cols=true;
            for(k=j;k<lu->cols;k++){
                is_zero_cols&=lu->get(j,k)->is_zero();
            }
            if(!is_zero_cols)
                rank++;
        }
        if(rank<lu->cols)
            return(NULL);

        Polynomial *ret=new Polynomial(lu->cols);
        for(i=0;i<lu->cols;i++){
            ret->set(i,lu->get(i,lu->cols));
        }

        for(j=0;j<lu->cols;j++){
            for(i=j+1;i<lu->cols;i++){
                ret->set(i,&(*ret->get(i)-*ret->get(j)**lu->get(i,j)));
            }
        }
        for(j=lu->cols-1;j>=0;j--){
            for(int i=j+1;i<lu->cols;i++){
                ret->set(j,&(*ret->get(j)-
                             *lu->get(j,i)**ret->get(i)));
            }
            ret->set(j,&(*ret->get(j)/(*lu->get(j,j))));
        }
        return(ret);
    }

    void Polynomial::swap_col(int i,int j)
    {
        size_t sz=sizeof(Nomial *)*this->rows;

        Nomial **tmp=new Nomial *[this->rows];

        memcpy(tmp,this->nomial+i*this->rows,sz);
        memcpy(this->nomial+i*this->rows,
               this->nomial+j*this->rows,sz);
        memcpy(this->nomial+j*this->rows,tmp,sz);
        
        delete [] tmp;
    }

    
    /////////////////////////////////////////////////////////////////////
    //
    //
    //
    BCH::BCH(Field *gf,int size,int capability)
    {
        this->_gf=gf;
        this->rows=size;
        this->cols=1;
        this->_capability=capability;
        this->error_size=0;
        this->error_pos=NULL;
        this->nomial=new Galois::Nomial *[this->rows];
        this->syndrome_size=0;
        this->syndromes=NULL;
    }
    BCH::~BCH()
    {
        if(this->error_pos)
            delete this->error_pos;

        if(this->syndromes)
            delete this->syndromes;
    }

    int BCH::decode(int syndrome_base)
    {
        //
        // error syndromes
        //
        this->syndrome_size=this->_capability*2+syndrome_base;
        this->syndromes=new Galois::Nomial *[this->syndrome_size];

        int errors=0,i;
        for(i=0;i<this->syndrome_size;i++){
            this->syndromes[i]=this->_error_syndrome(i);
            if(!this->syndromes[i]->is_zero())
                errors++;
        }
        if(!errors)
            return(0);
                
        //
        // calculate error position variables
        //
        Galois::Polynomial *err=NULL;
        for(errors=this->_capability;errors>0;errors--){
            Galois::Polynomial *mat=new Galois::Polynomial(errors,
                                                           errors+1);
            
            for(int j=0;j<errors;j++){
                for(i=0;i<=errors;i++){
                    mat->set(j,i,this->syndromes[i+j+syndrome_base]);
                }
            }
            
            err=mat->solve();
            delete mat;
            
            if(err)
                break;
        }
        if(!err){
            this->error_size=-1;
            return(-1);
        }

        if(err){
            //
            // get error position
            //
            this->error_pos=new int[errors];
            memset(this->error_pos,-1,errors);
            
            int c,i,j;
            for(j=0,c=0;j<this->rows;j++){
                Galois::Nomial *sigma=err->get(0);
                for(i=1;i<errors;i++){
                    sigma=&(*sigma+*err->get(i)*
                            *this->_gf->exp2nomial(j*i));
                }
                sigma=&(*sigma+*this->_gf->exp2nomial(j*i));
                
                if(sigma->is_zero()){
                    if(c<errors){
                        this->error_pos[c]=j;
                    }
                    c++;
                }
            }
            delete err;
            if(c==errors)
                this->error_size=errors;
            else
                this->error_size=-1;
        }

        return(this->error_size);
    }

    Galois::Nomial *BCH::_error_syndrome(int d)
    {
        Galois::Nomial *x=this->_gf->zero();
        
        for(int i=0;i<this->rows;i++){
            x=&(*x+*this->get(i)*
                *this->_gf->exp2nomial(i*d));
        }
        
        return(x->dup());
    }

}
