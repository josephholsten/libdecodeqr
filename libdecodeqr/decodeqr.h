/////////////////////////////////////////////////////////////////////////
//
// libdecodeqr.h --a part of libdecodeqr
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
#ifndef __QR_DECODER__
#define __QR_DECODER__

#include <cv.h>
#include "qrerror.h"
#include "qrtypes.h"

#define DEFAULT_ADAPTIVE_TH_SIZE 25
#define DEFAULT_ADAPTIVE_TH_DELTA 10


#ifdef __cplusplus
extern "C" {
#endif

/////////////////////////////////////////////////////////////////////////
//
// initializer
//
// ARGS: none
// RETURN:
//   QrDecoderHandle handle
//
extern QrDecoderHandle qr_decoder_open();

/////////////////////////////////////////////////////////////////////////
//
// initializer with source image size
//
// ARGS:
//   int width:  pixel width of source image
//   int height: pixel height of source image
//   int depth:  image depth (bit par pixel; use OpenCV IPL_DEPTH_*)
//   int channel: number of image channel
//
// RETURN:
//   QrDecoderHandle handle
//
// NOTE:
//   24-bit full color image has IPL_DEPTH_8U depth and 3 channels.
//
extern QrDecoderHandle qr_decoder_open_with_image_size(
    int width,int height,int depth,int channel);


/////////////////////////////////////////////////////////////////////////
//
// finalizer
//
// ARGS:
//   QrDecoderHandle decoder: handler
//
// RETURN: none
//
extern void qr_decoder_close(QrDecoderHandle decoder);


/////////////////////////////////////////////////////////////////////////
//
// get status
//
// ARGS:
//   QrDecoderHandle decoder: handler
//
// RETURN: status code
//
extern short qr_decoder_get_status(QrDecoderHandle decoder);

/////////////////////////////////////////////////////////////////////////
//
// get working status
//
// ARGS:
//   QrDecoderHandle decoder: handler
//
// RETURN: status code
//
extern int qr_decoder_is_busy(QrDecoderHandle decoder);


/////////////////////////////////////////////////////////////////////////
//
// set source image size
//
// ARGS:
//   QrDecoderHandle decoder: handler
//   int width:  pixel width of source image
//   int height: pixel height of source image
//   int depth:  image depth (bit par pixel; use OpenCV IPL_DEPTH_*)
//   int channel: number of image channel
//
// RETURN:
//   QrDecoderHandle handle
//
// NOTE:
//   This method provide same function as qr_decoder_open_with_image_size().
//
extern QrDecoderHandle qr_decoder_set_image_size(
    QrDecoderHandle decoder,int width,int height,int depth,int channel);


/////////////////////////////////////////////////////////////////////////
//
// preset gaven image as source image
//
// ARGS:
//   QrDecoderHandle decoder: handler
//   IplImage *src: source image
//
// RETURN:
//   QrDecoderHandle handle
//
extern QrDecoderHandle qr_decoder_set_image_buffer(
    QrDecoderHandle decoder,IplImage *src);

/////////////////////////////////////////////////////////////////////////
//
// get source image buffer
//
// ARGS:
//   QrDecoderHandle decoder: handler
//
// RETURN:
//   IplImage *: pointer to buffer source image|NULL
//
// NOTE:
//   See OpenCV reference manual to access to IplImage *
//
extern IplImage *qr_decoder_get_image_buffer(QrDecoderHandle decoder);

extern IplImage *qr_decoder_get_transformed_image_buffer(
    QrDecoderHandle decoder);
extern IplImage *qr_decoder_get_binarized_image_buffer(
    QrDecoderHandle decoder);
extern IplImage *qr_decoder_get_tmp_image_buffer(
    QrDecoderHandle decoder);

/////////////////////////////////////////////////////////////////////////
//
// decode preset source image
//
// ARGS:
//   QrDecoderHandle decoder: handler
//   int adaptive_th_size: value of AdaptiveThreshold size
//   int adaptive_th_delta: value of AdaptiveThreshold delta
//
// RETURN:
//   short: status code of decoder
//
// NOTE:
//   On succeeded, status code has 0x2000.
//   See qrtypes.h for details of status code.
//
//   In case of adaptive_th_size=0, binarizing methods will be
//   used cvThreshlod() instead of cvAdaptiveThreshold()
//
#ifdef __cplusplus
extern short qr_decoder_decode(QrDecoderHandle decoder,
                               int adaptive_th_size=
                               DEFAULT_ADAPTIVE_TH_SIZE,
                               int adaptive_th_delta=
                               DEFAULT_ADAPTIVE_TH_DELTA);
#else
extern short qr_decoder_decode(QrDecoderHandle decoder,
                               int adaptive_th_size,
                               int adaptive_th_delta);
#endif

/////////////////////////////////////////////////////////////////////////
//
// decode gaven image
//
// ARGS:
//   QrDecoderHandle decoder: handler
//   IplImage *src: image to decode
//   int adaptive_th_size: value of AdaptiveThreshold size
//   int adaptive_th_delta: value of AdaptiveThreshold delta
//
// RETURN:
//   short: status code of decoder
//
#ifdef __cplusplus
extern short qr_decoder_decode_image(QrDecoderHandle decoder,
                                     IplImage *src,
                                     int adaptive_th_size=
                                     DEFAULT_ADAPTIVE_TH_SIZE,
                                     int adaptive_th_delta=
                                     DEFAULT_ADAPTIVE_TH_DELTA);
#else
extern short qr_decoder_decode_image(QrDecoderHandle decoder,
                                     IplImage *src,
                                     int adaptive_th_size,
                                     int adaptive_th_delta);
#endif

/////////////////////////////////////////////////////////////////////////
//
// get abstruction of decoded data 
//
// ARGS:
//   QrDecoderHandle decoder: handler
//   QrCodeHeader *header: pointer to buffer of header
//
// RETURN:
//   1 (on success)||0 (on error)
//
extern int qr_decoder_get_header(QrDecoderHandle decoder,
                                 QrCodeHeader *header);

/////////////////////////////////////////////////////////////////////////
//
// get decoded text data 
//
// ARGS:
//   QrDecoderHandle decoder: handler
//   unsigned char *buf: pointer to buffer of header
//   int buf_size: buffer size
//
// RETURN:
//   copied data size||0 (on error)
//
// NOTE:
//   The data DOES NOT TERMINATE with null.
//   To get actual buffer size, use QrCodeHeader's .byte_size element.
//
extern int qr_decoder_get_body(QrDecoderHandle decoder,
                               unsigned char *buf,int buf_size);


/////////////////////////////////////////////////////////////////////////
//
// get vertexes of decoded code region
//
// ARGS:
//   QrDecoderHandle decoder: handler
//
// RETURN:
//   Pointer to CvPoint[4] which consist vertexes of code region
//
extern CvPoint *qr_decoder_get_coderegion_vertexes(QrDecoderHandle decoder);


/////////////////////////////////////////////////////////////////////////
//
// get Box array of decoded finder patterns
//
// ARGS:
//   QrDecoderHandle decoder: handler
//
// RETURN:
//   Pointer to CvBox2D[3] which consist boxes of finder pattern
//
extern CvBox2D *qr_decoder_get_finderpattern_boxes(QrDecoderHandle decoder);


/////////////////////////////////////////////////////////////////////////
//
// version information
//
extern char *qr_decoder_version();
extern char *qr_decoder_version_description();
extern char *qr_decoder_version_product();
extern int qr_decoder_version_major();
extern int qr_decoder_version_minor();
extern int qr_decoder_version_teeny();
extern char *qr_decoder_version_suffix();
extern char *qr_decoder_version_revision();

#ifdef __cplusplus
}
#endif

#endif
