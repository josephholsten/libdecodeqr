/////////////////////////////////////////////////////////////////////////
//
// simpletest.cpp --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id$
//
#include <stdio.h>
#include <highgui.h>
#include "../../libdecodeqr/decodeqr.h"

int main(int argc,char *argv[])
{
    cvNamedWindow("src",1);
    //
    // load image
    //
    IplImage *src=cvLoadImage(argv[1],1);
    cvShowImage("src",src);

    //
    // show version info
    //
    printf("libdecodeqr version %s\n",qr_decoder_version());

    //
    // initialize
    //
    QrDecoderHandle decoder=qr_decoder_open();

    //
    // do decode using default parameter
    //
    short stat=qr_decoder_decode_image(decoder,src);
    printf("STATUS=%04x\n",stat);

    //
    // get QR code header
    //
    QrCodeHeader header;
    qr_decoder_get_header(decoder,&header);

    //
    // get QR code text
    // To null terminate, a buffer size is larger than body size.
    //
    char *buf=new unsigned char[header.byte_size+1];
    qr_decoder_get_body(decoder,buf,header.byte_size+1);
    printf("%s\n",buf);

    //
    // finalize
    //
    qr_decoder_close(decoder);

    puts("");
    puts("Hit any key to end.");
    cvWaitKey(0);

    cvDestroyAllWindows();
    cvReleaseImage(&src);

    return(0);
}
