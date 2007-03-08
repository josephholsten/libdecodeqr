/////////////////////////////////////////////////////////////////////////
//
// webcam.cpp --a part of libdecodeqr
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
#include <string.h>
#include <highgui.h>
#include "../../libdecodeqr/decodeqr.h"

int usage(char *program_name);

int main(int argc,char *argv[])
{
    int show_bin_image=0;

    if(argc>1){
        if(strcmp(argv[1],"-d"))
            return(usage(argv[0]));
        else
            show_bin_image=1;
    }


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

    if(show_bin_image)
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

    IplImage *camera=cvQueryFrame(capture);
    IplImage *src=NULL,*bin=NULL;
    if(camera){
        src=cvCloneImage(camera);
        qr_decoder_set_image_buffer(decoder,src);
    }
    else
        key=1;

    unsigned char *text=NULL;
    int text_size=0;

    while(key<=0){
        cvShowImage("src",camera);
        key=cvWaitKey(150);

        //
        // when [SPACE] key pressed, do decode.
        //
        if(key==0x20&&!qr_decoder_is_busy(decoder)){
            key=-1;

            //
            // if left-bottom origin (MS-Windows style) format,
            // it must be converted to left-top origin.
            //
            if(camera->origin)
                cvConvertImage(camera,src,CV_CVTIMG_FLIP);
            else
                cvCopy(camera,src);

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
            if(bin)
                cvReleaseImage(&bin);
            if(show_bin_image){
                bin=cvCloneImage(qr_decoder_get_binarized_image_buffer(decoder));
                cvShowImage("bin",bin);
            }
            printf("adaptive_th_size=%d, status=%04x\n",sz,stat);

            //
            // on suceed decoding, print decoded text.
            //
            QrCodeHeader header;
            if(qr_decoder_get_header(decoder,&header)){
                if(text_size<header.byte_size+1){
                    if(text)
                        delete text;
                    
                    text_size=header.byte_size+1;
                    text=new unsigned char[text_size];
                }
                qr_decoder_get_body(decoder,text,text_size);
                printf("%s\n\n",text);

                //
                // draw found code region with green line
                //
                CvPoint *vertexes=qr_decoder_get_coderegion_vertexes(decoder);
                CvPoint pt=vertexes[4];
                int i;
                for(i=0;i<4;i++){
                    cvLine(src,pt,vertexes[i],CV_RGB(0,255,0),3);
                    pt=vertexes[i];
                }

                //
                // draw found finder patterns with green ellipse
                //
                CvBox2D *boxes=qr_decoder_get_finderpattern_boxes(decoder);
                for(i=0;i<3;i++){
                    CvSize sz=cvSize((int)boxes[i].size.width/2,
                                     (int)boxes[i].size.height/2);
                    cvEllipse(src,
                              cvPointFrom32f(boxes[i].center),
                              sz,
                              boxes[i].angle,
                              0,360,
                              CV_RGB(0,255,0),2);
                }

                if(src->origin)
                    cvConvertImage(src,src,CV_CVTIMG_FLIP);

                cvShowImage("src",src);

                //
                // wait 1500msec.
                //
                key=cvWaitKey(1500);
            }
        }

        camera=cvQueryFrame(capture);
        if(!camera)
            break;
    }
    
    if(text)
        delete text;

    qr_decoder_close(decoder);
    if(bin)
        cvReleaseImage(&bin);
    if(src)
        cvReleaseImage(&src);
        
    cvReleaseCapture(&capture);

    return(0);
}


int usage(char *program_name)
{
    fprintf(stderr,"usage: %s [-d|-h]\n",program_name);
    fprintf(stderr,"-d\tturn on debug mode.\n");
    fprintf(stderr,"-h\tshow thismessage and quit.\n\n");
    
    return(-1);
}
