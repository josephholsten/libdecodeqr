/////////////////////////////////////////////////////////////////////////
//
// imagereader.h --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id$
//
#ifndef __QR_IMAGE_READER__
#define __QR_IMAGE_READER__

#ifdef DEBUG
#define _DEBUG
#endif

#ifdef _DEBUG
#include <stdio.h>
#endif

#include <cv.h>
#include "errorcode.h"
#include "container.h"


/////////////////////////////////////////////////////////////////////////
//
// image processing parameters
//
#define DEFAULT_ADAPTIVE_TH_SIZE 25
#define DEFAULT_ADAPTIVE_TH_DELTA 10
#define MIN_AREA 49
#define MIN_AREA_RATIO .65
#define MIN_FERET_RATIO .7
#define FIND_CODE_AREA_POLY_APPROX_TH 50
#define POSTERIZED_TH_LOW 64
#define POSTERIZED_TH_HI 96
#define POSTERIZED_TH_STEP 8

namespace Qr{
    class ImageReader{
    public:
        Qr *qr;
        short status;

    private:
        IplImage *_img_src_internal;
        IplImage *_img_src;
        IplImage *_img_transformed;
        IplImage *_img_binarized;
        IplImage *_img_tmp_1c;
        CvMemStorage *_stor;
        CvMemStorage *_stor_tmp;
        CvSeq *_seq_finder_pattern;
        CvSeq *_seq_code_area_contour;

    public:
        ImageReader();
        ImageReader(int width,int height,
                                 int depth=IPL_DEPTH_8U,
                                 int channel=3);
        ~ImageReader();
        
        IplImage *set_image(IplImage *src);
        uchar *set_image(uchar *buffer,int size);
        IplImage *set_image(int width,int height,
                                         int depth,int channel);
        void release_image();

        IplImage *src_buffer();
        IplImage *transformed_buffer();
        IplImage *binarized_buffer();
        IplImage *tmp_buffer();

        Qr *decode(int adaptive_th_size=
                                DEFAULT_ADAPTIVE_TH_SIZE,
                                int adaptive_th_delta=
                                DEFAULT_ADAPTIVE_TH_DELTA);
        Qr *decode(IplImage *src,
                                int adaptive_th_size=
                                DEFAULT_ADAPTIVE_TH_SIZE,
                                int adaptive_th_delta=
                                DEFAULT_ADAPTIVE_TH_DELTA);

    private:
        void _init();
        void _alloc_image(int width,int height,
                                       int depth,int channel);

        Qr *_decode(int adaptive_th_size,int adaptive_th_delta);
        
        CvSeq *_find_finder_pattern();
        CvSeq *_find_code_area_contour(double th);
        CvRect _transform_image();
        void _create_posterized_image(int block_size,
                                                   double delta,
                                                   int low_th,
                                                   int hi_th);
        IplImage *_get_code_matrix();
        int _get_format_info(IplImage *src,int pos=0);
        IplImage *_get_function_patterns();
        void _unmask_code_matrix(IplImage *src,
                                              IplImage *function_patterns);
        int _read_code_word(IplImage *src,IplImage *mask);

        double _get_cell_size();
        IplImage *_get_mask_pattern();

    };

    static int seq_cmp_by_clockwise(const void *_a,
                                    const void *_b,
                                    void *_cog);

    void apaptive_white_leveling(const CvArr* src,CvArr* dst,
                                 double middle_value,int adaptive_method,
                                 int threshold_type,int block_size,
                                 double param1);

}

#endif
