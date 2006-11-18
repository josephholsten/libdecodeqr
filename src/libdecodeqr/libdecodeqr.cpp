/////////////////////////////////////////////////////////////////////////
//
// libdecodeqr.cpp --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id$
//
#include "imagereader.h"
#include "qrtypes.h"
#include "version.h"

char *qr_decoder_version()
{
    return(LIBDECODEQR_VERSION);
}
int qr_decoder_version_major()
{
    return(LIBDECODEQR_VERSION_MAJOR);
}
int qr_decoder_version_minor()
{
    return(LIBDECODEQR_VERSION_MINOR);
}
int qr_decoder_version_teeny()
{
    return(LIBDECODEQR_VERSION_TEENY);
}
char *qr_decoder_version_suffix()
{
    return(LIBDECODEQR_VERSION_SUFFIX);
}
char *qr_decoder_version_revision()
{
    return(LIBDECODEQR_VERSION_REVISION);
}


QrDecoderHandle qr_decoder_open()
{
    Qr::ImageReader *imagereader=new Qr::ImageReader();
    return((QrDecoderHandle)imagereader);
}

QrDecoderHandle qr_decoder_open_with_image_size(int width,int height,
                                                int depth,int channel)
{
    Qr::ImageReader *imagereader=new Qr::ImageReader(width,height,
                                                     depth,channel);
    return((QrDecoderHandle)imagereader);
}

void qr_decoder_close(QrDecoderHandle decoder)
{
    Qr::ImageReader *imagereader=(Qr::ImageReader *)decoder;
    delete imagereader;
}


QrDecoderHandle qr_decoder_set_image_size(QrDecoderHandle decoder,
                                          int width,int height,
                                          int depth,int channel)
{
    Qr::ImageReader *imagereader=(Qr::ImageReader *)decoder;
    IplImage *src=cvCreateImage(cvSize(width,height),depth,channel);
    imagereader->set_image(src);
    
    return((QrDecoderHandle)imagereader);
}


IplImage *qr_decoder_get_image_buffer(QrDecoderHandle decoder)
{
    Qr::ImageReader *imagereader=(Qr::ImageReader *)decoder;
    return(imagereader->src_buffer());
}


QrDecoderHandle qr_decoder_set_image_buffer(QrDecoderHandle decoder,
                                            IplImage *src)
{
    Qr::ImageReader *imagereader=(Qr::ImageReader *)decoder;
    imagereader->set_image(src);
    
    return((QrDecoderHandle)imagereader);
}

short qr_decoder_decode(QrDecoderHandle decoder,
                        int adaptive_th_size,
                        int adaptive_th_delta)
{
    Qr::ImageReader *imagereader=(Qr::ImageReader *)decoder;
    imagereader->decode(adaptive_th_size,adaptive_th_delta);
    return(imagereader->status);
}    

short qr_decoder_decode_image(QrDecoderHandle decoder,
                              IplImage *src,
                              int adaptive_th_size,
                              int adaptive_th_delta)
{
    Qr::ImageReader *imagereader=(Qr::ImageReader *)decoder;
    imagereader->decode(src,adaptive_th_size,adaptive_th_delta);
    return(imagereader->status);
}

int qr_decoder_get_header(QrDecoderHandle decoder,
                          QrCodeHeader *header)
{
    Qr::ImageReader *imagereader=(Qr::ImageReader *)decoder;
    if(!(imagereader->status&QR_IMAGEREADER_DECODED))
        return(0);

    header->model=imagereader->qr->model;
    header->version=imagereader->qr->version;
    header->level=imagereader->qr->formatinfo->level;
    //header->mode=0;
    //header->eci_mode=0;
    header->charactor_size=imagereader->qr->codedata->length;
    header->byte_size=imagereader->qr->codedata->byte_length;

    return(1);
}

int qr_decoder_get_body(QrDecoderHandle decoder,
                        unsigned char *buf,
                        int buf_size)
{
    Qr::ImageReader *imagereader=(Qr::ImageReader *)decoder;
    if(!(imagereader->status&QR_IMAGEREADER_DECODED))
        return(0);

    memset(buf,0,buf_size);
    int size=imagereader->qr->codedata->byte_length;
    if(size>buf_size)
        size=buf_size;

    memcpy(buf,imagereader->qr->codedata->raw_data(),size);

    return(size);
}
