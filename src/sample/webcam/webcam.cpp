/////////////////////////////////////////////////////////////////////////
//
// webcam.cpp --a part of libdecodeqr
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
    //
    // start camera
    //
    CvCapture *capture=cvCaptureFromCAM(0);
    if(!capture)
        return(-1);

    //
    // initialize qr decoder
    //
    QrDecoderHandle decoder=qr_decoder_open();
    printf("libdecodeqr version %s\n",qr_decoder_version());

    
    cvNamedWindow("src",1);
    cvNamedWindow("bin",1);

    puts("Hit [SPACE] key to grab, or any key to end.");
    puts("");

    //
    // 1 shot grabing
    //
    //
    // allocate grabed buffer to decoder
    //
    int key=-1;

    IplImage *src=cvQueryFrame(capture);
    if(src)
        qr_decoder_set_image_buffer(decoder,src);
    else
        key=1;

    unsigned char *text=NULL;
    int text_size=0;

    while(key<=0){
        cvShowImage("src",src);
        key=cvWaitKey(150);

        //
        // when [SPACE] key pressed, do decode.
        //
        if(key==0x20){
            key=-1;

            //
            // if left-bottom origin (MS-Windows style) format,
            // it must be converted to left-top origin.
            //
            if(src->origin)
                cvConvertImage(src,src,CV_CVTIMG_FLIP);

            //
            // While decoding is a failure, decrease the
            // adaptive_th_size parameter.
            // Note that the adaptive_th_size must be odd.
            //
            short sz,stat;
            for(sz=25,stat=0;
                (sz>=3)&&((stat&QR_IMAGEREADER_DECODED)==0);
                sz-=2)
                stat=qr_decoder_decode(decoder,sz);

            //
            // for debug, show binarized image.
            //
            cvShowImage("bin",
                        qr_decoder_get_binarized_image_buffer(decoder));
            printf("adaptive_th_size=%d, status=%04x\n",sz,stat);

            //
            // on suceed decoding, print decoded text.
            //
            if(stat&=QR_IMAGEREADER_DECODED){
                QrCodeHeader header;
                qr_decoder_get_header(decoder,&header);
                if(text_size<header.byte_size+1){
                    if(text)
                        delete text;
                    
                    text_size=header.byte_size+1;
                    text=new unsigned char[text_size];
                }
                qr_decoder_get_body(decoder,text,text_size);
                printf("%s\n\n",text);

                key=cvWaitKey(1000);
            }
        }

        src=cvQueryFrame(capture);
        if(!src)
            break;
    }
    
    if(text)
        delete text;

    qr_decoder_close(decoder);
    cvReleaseCapture(&capture);

    return(0);
}
