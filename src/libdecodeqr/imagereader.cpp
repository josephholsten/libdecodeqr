/////////////////////////////////////////////////////////////////////////
//
// imagereader.cpp --a part of libdecodeqr
//
// Copyright (c) 2006 NISHI Takao <zophos@koka-in.org>
//
// This is free software with ABSOLUTELY NO WARRANTY.
// You can redistribute and/or modify it under the terms of GPLv2.
//
// $Id$
//
#include "imagereader.h"

namespace Qr{

    //
    // avoid cvBoundingRect() fxxcin' bug. 
    // 
    typedef struct{
        CvRect feret;
        CvSeq *contour;
    } ImageReaderCandidate;


    /////////////////////////////////////////////////////////////////////
    //
    //
    //
    ImageReader::ImageReader()
    {
        this->_init();
    }
    ImageReader::ImageReader(int width,int height,int depth,int channel)
    {
        this->_init();
        this->_alloc_image(width,height,depth,channel);
    }
    ImageReader::~ImageReader()
    {
        if(this->qr)
            delete this->qr;
      
        if(this->_seq_code_area_contour)
            cvRelease((void **)&this->_seq_code_area_contour);

        cvRelease((void **)&this->_seq_finder_pattern);
        cvReleaseMemStorage(&this->_stor_tmp);
        cvReleaseMemStorage(&this->_stor);

        this->release_image();
    }
    
    void ImageReader::_init()
    {
        memset(this->_coderegion_vertexes,0,sizeof(CvPoint)*4);
        memset(this->_finderpattern_boxes,0,sizeof(CvBox2D)*3);

        this->_img_src_internal=NULL;
        this->_img_src=NULL;
        this->_img_transformed=NULL;
        this->_img_binarized=NULL;
        this->_img_tmp_1c=NULL;

        this->_stor=cvCreateMemStorage(0);
        this->_stor_tmp=cvCreateMemStorage(0);

        this->_seq_finder_pattern=cvCreateSeq(CV_SEQ_ELTYPE_GENERIC,
                                              sizeof(CvSeq),
                                              sizeof(CvBox2D),
                                              this->_stor);
        this->_seq_code_area_contour=NULL;

        this->status=0;
        this->qr=NULL;
    }
    void ImageReader::_alloc_image(int width,int height,int depth,int channel)
    {
        this->_img_src_internal=cvCreateImage(cvSize(width,height),
                                              depth,channel);
        cvZero(this->_img_src_internal);
        this->_img_src=this->_img_src_internal;

        this->_img_transformed=cvCloneImage(this->_img_src);
        this->_img_binarized=cvCreateImage(cvSize(this->_img_src->width,
                                                  this->_img_src->height),
                                           IPL_DEPTH_8U,1);
        cvZero(this->_img_binarized);
        this->_img_tmp_1c=cvCloneImage(this->_img_binarized);
    }

    IplImage *ImageReader::set_image(IplImage *src)
    {
        this->release_image();

        this->_img_src=src;
        this->_img_transformed=cvCloneImage(this->_img_src);
        this->_img_binarized=cvCreateImage(cvSize(this->_img_src->width,
                                                  this->_img_src->height),
                                           IPL_DEPTH_8U,1);
        cvZero(this->_img_binarized);
        this->_img_tmp_1c=cvCloneImage(this->_img_binarized);

        return(this->_img_src);
    }

    IplImage *ImageReader::set_image(int width,int height,
                                     int depth,int channel)
    {
        this->release_image();
        this->_alloc_image(width,height,depth,channel);

        return(this->_img_src);
    }

    void ImageReader::release_image()
    {
        if(this->_img_tmp_1c)
            cvReleaseImage(&this->_img_tmp_1c);
        if(this->_img_binarized)
            cvReleaseImage(&this->_img_binarized);
        if(this->_img_transformed)
            cvReleaseImage(&this->_img_transformed);
        if(this->_img_src_internal)
            cvReleaseImage(&this->_img_src_internal);

        this->_img_tmp_1c=NULL;
        this->_img_binarized=NULL;
        this->_img_transformed=NULL;
        this->_img_src_internal=NULL;
        this->_img_src=NULL;
    }


    IplImage *ImageReader::src_buffer()
    {
        return(this->_img_src);
    }
    IplImage *ImageReader::transformed_buffer()
    {
        return(this->_img_transformed);
    }
    IplImage *ImageReader::binarized_buffer()
    {
        return(this->_img_binarized);
    }
    IplImage *ImageReader::tmp_buffer()
    {
        return(this->_img_tmp_1c);
    }
    CvPoint *ImageReader::coderegion_vertexes()
    {
        return(this->_coderegion_vertexes);
    }
    CvBox2D *ImageReader::finderpattern_boxes()
    {
        return(this->_finderpattern_boxes);
    }


    Qr *ImageReader::decode(int adaptive_th_size,
                            int adaptive_th_delta)
    {
        if(this->status&QR_IMAGEREADER_WORKING)
            return(NULL);

        if(!this->_img_src){
            this->status=(QR_IMAGEREADER_ERROR|
                          QR_IMAGEREADER_NOT_INVALID_SRC_IMAGE);
            return(NULL);
        }

        this->status=QR_IMAGEREADER_WORKING;

        cvResetImageROI(this->_img_transformed);
        cvResetImageROI(this->_img_binarized);
        cvResetImageROI(this->_img_tmp_1c);

        Qr *ret=this->_decode(adaptive_th_size,adaptive_th_delta);

        this->status^=QR_IMAGEREADER_WORKING;
        return(ret);
    }

    Qr *ImageReader::decode(IplImage *src,
                            int adaptive_th_size,
                            int adaptive_th_delta)
    {
        if(this->status&QR_IMAGEREADER_WORKING)
            return(NULL);

        this->status=QR_IMAGEREADER_WORKING;

        this->set_image(src);
        Qr *ret=this->_decode(adaptive_th_size,adaptive_th_delta);

        this->_img_src=NULL;

        this->status^=QR_IMAGEREADER_WORKING;
        return(ret);
    }


    Qr *ImageReader::_decode(int adaptive_th_size,int adaptive_th_delta)
    {
        if(this->qr){
            delete this->qr;
            this->qr=NULL;
        }

        memset(this->_coderegion_vertexes,0,sizeof(CvPoint)*4);
        memset(this->_finderpattern_boxes,0,sizeof(CvBox2D)*3);

        //
        // binarize
        //
        if(this->_img_src->nChannels>1)
            cvCvtColor(this->_img_src,this->_img_tmp_1c,CV_BGR2GRAY);
        else
            cvCopy(this->_img_src,this->_img_tmp_1c);

        cvSmooth(this->_img_tmp_1c,this->_img_binarized,CV_MEDIAN,3);
        cvCopy(this->_img_binarized,this->_img_tmp_1c);
        if(adaptive_th_size>0)
            cvAdaptiveThreshold(this->_img_tmp_1c,
                                this->_img_binarized,
                                128,CV_ADAPTIVE_THRESH_MEAN_C,
                                CV_THRESH_BINARY_INV,
                                adaptive_th_size,
                                adaptive_th_delta);
        else
            cvThreshold(this->_img_tmp_1c,
                        this->_img_binarized,
                        adaptive_th_delta,
                        255,CV_THRESH_BINARY_INV);

        //
        // find finder patterns from binarized image
        //
        this->_find_finder_pattern();
        if(this->_seq_finder_pattern->total!=3){
            this->status|=(QR_IMAGEREADER_ERROR|
                           QR_IMAGEREADER_NOT_FOUND_FINDER_PATTERN);
            return(NULL);
        }
  
        //
        // find code area from binarized image using finder patterns
        //
        this->_find_code_area_contour(FIND_CODE_AREA_POLY_APPROX_TH);

        if(!this->_seq_code_area_contour||
            this->_seq_code_area_contour->total!=4){
            this->status|=(QR_IMAGEREADER_ERROR|
                           QR_IMAGEREADER_NOT_FOUND_CODE_AREA);
            return(NULL);
        }

        //
        // perspective transform from source image
        //
        this->_transform_image();
        IplImage *code_matrix=NULL;
        for(int th=POSTERIZED_TH_LOW;th<=POSTERIZED_TH_HI&&(!code_matrix);
            th+=POSTERIZED_TH_STEP){
            
            this->_create_posterized_image(adaptive_th_size,
                                           adaptive_th_delta*3,
                                           th,
                                           POSTERIZED_TH_HI);
            code_matrix=this->_get_code_matrix();
        }
        
        if(!code_matrix){
            this->status|=(QR_IMAGEREADER_ERROR|
                           QR_IMAGEREADER_NOT_DETERMINABLE_CODE_AREA);
            return(NULL);
        }
        
        this->qr=new Qr();
        this->qr->set_version((code_matrix->width-17)/4);
        if(this->qr->version>=7){
            //FIXME this->_verify_code_version();
        }
        
        if(this->_get_format_info(code_matrix,0)<0){
            if((this->_get_format_info(code_matrix,1)<0)&&
               (this->qr->formatinfo->status&QR_FORMATINFO_INVALID_LEVEL)){
                this->_get_format_info(code_matrix,0);
                if(this->qr->formatinfo->status&QR_FORMATINFO_INVALID_LEVEL){
                    cvReleaseImage(&code_matrix);
                    this->status|=(QR_IMAGEREADER_ERROR|
                                   this->qr->formatinfo->status);
                    return(NULL);
                }
            }
        }
        
        IplImage *function_patterns=this->_get_function_patterns();

        this->_unmask_code_matrix(code_matrix,function_patterns);
        this->_read_code_word(code_matrix,function_patterns);

        cvReleaseImage(&function_patterns);
        cvReleaseImage(&code_matrix);
        
        int errors=this->qr->decode_codedata();

#ifdef _DEBUG
        fprintf(stderr,"errors=%d\n",errors);
#endif
        this->status|=(QR_IMAGEREADER_DECODED|this->qr->status);
        
        return(this->qr);
    }

    CvSeq *ImageReader::_find_finder_pattern()
    {
        IplImage *src=this->_img_binarized;
        cvClearSeq(this->_seq_finder_pattern);

        // for contour list
        cvClearMemStorage(this->_stor_tmp);
        CvSeq *cont=cvCreateSeq(CV_SEQ_ELTYPE_POINT,
                                sizeof(CvSeq),sizeof(CvPoint),
                                this->_stor_tmp);

        //
        // Find all contours.
        //
        // cvFindContours() spoil source image.
        //
        CvRect roi=cvGetImageROI(src);
        cvSetImageROI(this->_img_tmp_1c,roi);
        cvCopy(src,this->_img_tmp_1c);

        cvFindContours(this->_img_tmp_1c,
                       this->_stor_tmp,&cont,sizeof(CvContour),
                       CV_RETR_LIST,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
        
        cvResetImageROI(this->_img_tmp_1c);

        // for marker candidates list
        CvSeq *candidates=cvCreateSeq(CV_SEQ_ELTYPE_GENERIC,
                                      sizeof(CvSeq),sizeof(ImageReaderCandidate),
                                      this->_stor_tmp);
        //
        // check each block
        //
        CvSeq *cont_head=cont;
        for(;cont;cont=cont->h_next){
            CvRect feret=cvBoundingRect(cont);
            double area=fabs(cvContourArea(cont));
            double area_ratio=area/(double)(feret.width*feret.height);
            double feret_ratio=((feret.width<feret.height)?
                                (double)feret.width/(double)feret.height:
                                (double)feret.height/(double)feret.width);
            
            //
            // search square
            //
            if(area>=MIN_AREA &&
               area_ratio>=MIN_AREA_RATIO &&
               feret_ratio>=MIN_FERET_RATIO){
                ImageReaderCandidate c;
                c.feret.x=feret.x;
                c.feret.y=feret.y;
                c.feret.width=feret.width;
                c.feret.height=feret.height;
                c.contour=cont;
                cvSeqPush(candidates,&c);
            }
            else{
                cvClearSeq(cont);
            }
        }
        cvRelease((void **)&cont_head);
        
        //
        // check each sqare has inner squire
        //
        int i;
        for(i=0;i<candidates->total;i++){
            ImageReaderCandidate *cand1=
                (ImageReaderCandidate *)cvGetSeqElem(candidates,i);
            
            int inner_contour=0;
            int j;
            for(j=0;j<candidates->total;j++){
                if(i==j)
                    continue;
                
                ImageReaderCandidate *cand2=
                    (ImageReaderCandidate *)cvGetSeqElem(candidates,j);
                CvRect max_rect=cvMaxRect(&(cand1->feret),&(cand2->feret));
                if(cand1->feret.x==max_rect.x&&
                   cand1->feret.y==max_rect.y&&
                   cand1->feret.width==max_rect.width&&
                   cand1->feret.height==max_rect.height)
                    inner_contour++;
            }
            
            //
            // There were 2 squires (white and black) inside a squire,
            // the most outer squire assumed as position marker.
            //
            if(inner_contour==2){
                CvBox2D box=cvMinAreaRect2(cand1->contour);
                cvSeqPush(this->_seq_finder_pattern,&box);
            }
        }
        
        //
        // clear buffers
        //
        for(i=0;i<candidates->total;i++){
            ImageReaderCandidate *cand1=
                (ImageReaderCandidate *)cvGetSeqElem(candidates,i);
            cvClearSeq(cand1->contour);
        }
        //cvClearSeq(candidates);
        cvRelease((void **)&candidates);

        cvClearMemStorage(this->_stor_tmp);
        
        return(this->_seq_finder_pattern);
    }
    
    /////////////////////////////////////////////////////////////////////////
    //
    //
    //
    CvSeq *ImageReader::_find_code_area_contour(double th)
    {
        IplImage *src=this->_img_binarized;
        IplImage *mask=this->_img_tmp_1c;
        cvZero(mask);
        
        cvClearMemStorage(this->_stor_tmp);
        
        //
        // create position maker mask
        //
        CvBox2D box;
        CvPoint2D32f pt_32f[4];
        CvSeq *markers_vertex=cvCreateSeq(CV_SEQ_ELTYPE_POINT,
                                          sizeof(CvSeq),
                                          sizeof(CvPoint),
                                          this->_stor_tmp);
        

        int c=this->_seq_finder_pattern->total,i;
        for(i=0;i<c;i++){
            box=*(CvBox2D *)cvGetSeqElem(this->_seq_finder_pattern,i);
            this->_finderpattern_boxes[i]=box;

            //
            // set 2-cells margin for fale-safe
            //
            box.size.width+=box.size.width/7.0F*4.0F;
            box.size.height+=box.size.height/7.0F*4.0F;
            
            //
            // get each position maker's vertex 
            //
            cvBoxPoints(box,pt_32f);
            for(int j=0;j<4;j++){
                CvPoint p=cvPointFrom32f(pt_32f[j]);
                cvSeqPush(markers_vertex,&p);
            }
        }

        //
        // create Minimal-area bounding rectangle which condist
        // every position makers
        //
        box=cvMinAreaRect2(markers_vertex);
        cvRelease((void **)&markers_vertex);
        
        //
        // create code area mask
        //
        cvBoxPoints(box,pt_32f);
        CvPoint *points=new CvPoint[4];
        for(i=0;i<4;i++){
            points[i]=cvPointFrom32f(pt_32f[i]);
        }
        i=4;
        cvFillPoly(mask,&points,&i,1,cvScalarAll(255));
        delete points;
        
        //
        //  apply mask to src image and reduce noise using opening
        //
        cvAnd(src,mask,mask);
        cvErode(mask,mask,NULL,1);
        cvDilate(mask,mask,NULL,1);
        
        //
        // get contours of masked image
        //
        CvSeq *cont=cvCreateSeq(CV_SEQ_ELTYPE_POINT,
                                sizeof(CvSeq),sizeof(CvPoint),
                                this->_stor_tmp);
        cvFindContours(mask,cont->storage,&cont,sizeof(CvContour),
                       CV_RETR_EXTERNAL,CV_CHAIN_APPROX_NONE,cvPoint(0,0));
        
        //
        // calcurate convex hull that assumed as code area
        //
        CvSeq *pts=cvCreateSeq(CV_SEQ_ELTYPE_POINT,
                               sizeof(CvSeq),sizeof(CvPoint),
                               this->_stor_tmp);
        CvSeq *cont_head=cont;
        for(;cont;cont=cont->h_next){
            int c=cont->total;
            for(i=0;i<c;i++){
                CvPoint pt=*((CvPoint *)cvGetSeqElem(cont,i));
                cvSeqPush(pts,&pt);
            }
            cvClearSeq(cont);
        }
        cvRelease((void **)&cont_head);

        CvSeq *hull=cvConvexHull2(pts);
        
        //
        // polygonal approximation to reduce noise
        //
        if(this->_seq_code_area_contour){
            cvClearSeq(this->_seq_code_area_contour);
            cvRelease((void **)&this->_seq_code_area_contour);
            this->_seq_code_area_contour=NULL;
        }
        if(hull){
            int hullcount=hull->total;
            CvMat* vector=cvCreateMat(1,hullcount,CV_32SC2);
            for(i=0;i<hullcount;i++ ){
                CV_MAT_ELEM(*vector,CvPoint,0,i)=
                    **CV_GET_SEQ_ELEM(CvPoint*,hull,i);
            }
            CvContour header;
            CvSeqBlock block;
            this->_seq_code_area_contour=cvApproxPoly(
                cvPointSeqFromMat(CV_SEQ_KIND_CURVE+CV_SEQ_FLAG_CLOSED,
                                  vector, &header, &block),
                sizeof(CvContour),
                this->_stor,
                CV_POLY_APPROX_DP,th,0);
            
            cvRelease((void **)&hull);
        }
        
        return(this->_seq_code_area_contour);
    }


    CvRect ImageReader::_transform_image()
    {
        IplImage *src=this->_img_src;
        IplImage *dst=this->_img_transformed;
        cvResetImageROI(dst);

        //
        // get code area's Centor of Gravity
        //
        int i;
        int c=this->_seq_code_area_contour->total;
        CvPoint2D32f cog;
        cog.x=0.0F;
        cog.y=0.0F;
        for(i=0;i<c;i++){
            CvPoint2D32f p=cvPointTo32f(
                *(CvPoint *)cvGetSeqElem(this->_seq_code_area_contour,i));
            cog.x+=p.x;
            cog.y+=p.y;
        }
        cog.x/=(float)c;
        cog.y/=(float)c;
        
        //
        // sort code_area_contour by clock-wise
        //
        cvSeqSort(this->_seq_code_area_contour,seq_cmp_by_clockwise,&cog);


        //
        // calculates matrix of perspective transform
        //
        // The src rectangle transform to a square which left-top vertex
        // is same as src rectangle and each side length is same as top side
        // length of src.
        //
        CvPoint2D32f spts[4];
        CvPoint2D32f dpts[4];
        float max_d=0.0F;
        int offset=0;

#ifdef _DEBUG
        fprintf(stderr,"target region: ");
#endif
        for(i=0;i<c;i++){
            this->_coderegion_vertexes[i]=*(CvPoint *)cvGetSeqElem(
                this->_seq_code_area_contour,i);
            spts[i]=cvPointTo32f(this->_coderegion_vertexes[i]);
            
            //
            // find nearest finder pattern
            //
            int j=0;
            float tmp_d=0.0F;
            float min_d=(this->_finderpattern_boxes[j].center.x-spts[i].x)*
                (this->_finderpattern_boxes[j].center.x-spts[i].x)+
                (this->_finderpattern_boxes[j].center.y-spts[i].y)*
                (this->_finderpattern_boxes[j].center.y-spts[i].y);
            
            for(j=1;j<3;j++){
                tmp_d=(this->_finderpattern_boxes[j].center.x-spts[i].x)*
                    (this->_finderpattern_boxes[j].center.x-spts[i].x)+
                    (this->_finderpattern_boxes[j].center.y-spts[i].y)*
                    (this->_finderpattern_boxes[j].center.y-spts[i].y);
                    
                    if(min_d>tmp_d)
                        min_d=tmp_d;
            }
            if(max_d<min_d){
                max_d=min_d;
                offset=i;
            }
            
#ifdef _DEBUG
            fprintf(stderr,"(%.0lf,%.0lf) ",spts[i].x,spts[i].y);
#endif
        }
        
        offset=(offset+2)%4;

#ifdef _DEBUG
        fprintf(stderr,", rotation offset=%d\n",offset);
#endif

        float side_len=cvSqrt((spts[0].x-spts[1].x)*(spts[0].x-spts[1].x)+
                              (spts[0].y-spts[1].y)*(spts[0].y-spts[1].y));
        for(i=0;i<4;i++)
            dpts[i]=cvPoint2D32f(0.0,0.0);

        dpts[(offset+1)%4].x+=side_len;
        dpts[(offset+2)%4].x+=side_len;
        dpts[(offset+2)%4].y+=side_len;
        dpts[(offset+3)%4].y+=side_len;

        CvMat *map=cvCreateMat(3,3,CV_64FC1);
        
        cvWarpPerspectiveQMatrix(spts,dpts,map);
        
        //
        // perspective transform
        // 
        cvWarpPerspective(src,dst,map);
        cvReleaseMat(&map);

        //
        // set ROI as code area
        //
        CvRect roi=cvRect((int)dpts[offset].x,(int)dpts[offset].y,
                          (int)side_len+1,(int)side_len+1);
        cvSetImageROI(dst,roi);
        
        return(roi);
    }
        

    void ImageReader::_create_posterized_image(int block_size,
                                               double delta,
                                               int low_th,
                                               int hi_th)
    {
        IplImage *src=this->_img_transformed;
        IplImage *dst=this->_img_binarized;
        IplImage *buf=this->_img_tmp_1c;

        CvRect roi=cvGetImageROI(src);
        cvSetImageROI(buf,roi);
        cvSetImageROI(dst,roi);

        if(this->_img_src->nChannels>1)
            cvCvtColor(src,buf,CV_BGR2GRAY);
        else
            cvCopy(src,buf);

        if(block_size>0)
            apaptive_white_leveling(buf,dst,128,
                                    CV_ADAPTIVE_THRESH_MEAN_C,
                                    CV_THRESH_BINARY_INV,
                                    block_size,delta);
        else
            cvThreshold(buf,dst,delta,
                        255,CV_THRESH_BINARY_INV);

        int a=0;
        int b=0;
        if(hi_th>low_th){
            a=128/(hi_th-low_th);
            b=-a*low_th;
        }
        else{
            hi_th=low_th;
        }
        
        uchar lut_data[256];
        int i;
        for(i=0;i<low_th;i++)
            lut_data[i]=0;
        for(i=low_th;i<hi_th;i++)
            lut_data[i]=a*i+b;
        for(i=hi_th;i<256;i++)
            lut_data[i]=255;
        CvMat lut=cvMat(1,256,CV_8UC1,lut_data);
        cvLUT(dst,buf,&lut);
        cvCopy(buf,dst);
        
        CvRect roi_mask=roi;
        cvDilate(buf,buf,NULL,1);
        cvErode(buf,buf,NULL,1);
        
        roi_mask.x+=1;
        roi_mask.y+=1;
        roi_mask.width-=2;
        roi_mask.height-=2;
        cvSetImageROI(buf,roi_mask);
        cvSet(buf,cvScalarAll(0));
        cvSetImageROI(buf,roi);
        cvOr(dst,buf,dst);

        cvResetImageROI(buf);
    }
    
    
    IplImage *ImageReader::_get_code_matrix()
    {
        IplImage *src=this->_img_binarized;

        double cell_size=this->_get_cell_size();
        if(cell_size<=0.0)
            return(NULL);
        
        CvRect roi=cvGetImageROI(src);
        int version=(int)(((double)roi.width/cell_size-17.0)/4.0);
        int w=4*version+17;
        
        IplImage *dst=cvCreateImage(cvSize(w,w),IPL_DEPTH_8U,1);
        cvResize(src,dst);
        
        
#ifdef _DEBUG
        fprintf(stderr,"version=%d (%dx%d); %lfpixel/module\n",
                version,w,w,cell_size*cell_size);
#endif
        return(dst);
    }

    int ImageReader::_get_format_info(IplImage *src,int pos)
    {
        unsigned int raw_data=0;
        this->qr->init_each_formatinfo_pattern_pixel();
        int x,y;
        while(this->qr->each_formatinfo_pattern_pixel(pos,&x,&y)){
            raw_data<<=1;
            if(CV_IMAGE_ELEM(src,uchar,y,x)==255)
                raw_data|=0x01;
        }
        
#ifdef _DEBUG
        fprintf(stderr,"format Info=%04x, ",raw_data);
#endif

        int ret=this->qr->decode_formatinfo(raw_data);
        
#ifdef _DEBUG
        fprintf(stderr,"corrected %d errors ",ret);
        
        char l[2]={0,0};
        switch(this->qr->formatinfo->level){
        case 0:
            l[0]='M';
            break;
        case 1:
            l[0]='L';
            break;
        case 2:
            l[0]='H';
            break;
        case 3:
            l[0]='Q';
        }
        int m=0;
        if(this->qr->formatinfo->mask_pattern&0x4)
            m|=0x100;
        if(this->qr->formatinfo->mask_pattern&0x2)
            m|=0x10;
        if(this->qr->formatinfo->mask_pattern&0x1)
            m|=0x1;
        
        fprintf(stderr," =>level %s, mask %03x\n",l,m);
#endif

        return(ret);
    }

    IplImage *ImageReader::_get_function_patterns()
    {
        IplImage *buf=cvCreateImage(cvSize(this->qr->cells_par_side,
                                           this->qr->cells_par_side),
                                    IPL_DEPTH_8U,1);
        cvZero(buf);

        int x,y;
        this->qr->init_each_function_pattern_pixel();
        while(this->qr->each_function_pattern_pixel(&x,&y)){
            CV_IMAGE_ELEM(buf,uchar,y,x)=255;
        }
        
        cvNot(buf,buf);

        return(buf);
    }

    void ImageReader::_unmask_code_matrix(IplImage *src,
                                          IplImage *function_patterns)
    {
        IplImage *mask=this->_get_mask_pattern();
        IplImage *unproc=cvCloneImage(mask);

        cvAnd(function_patterns,unproc,unproc);
        cvXor(src,mask,src,unproc);
        
        cvReleaseImage(&unproc);
        cvReleaseImage(&mask);
    }

    int ImageReader::_read_code_word(IplImage *src,IplImage *mask)
    {
        //
        // erace timing pattern
        //
        IplImage *codes=cvCreateImage(cvSize(src->width-1,src->height-1),
                                      IPL_DEPTH_8U,1);
        cvZero(codes);
        IplImage *funcs=cvCloneImage(codes);
        
        CvRect roi=cvGetImageROI(src);
        
        CvRect r=cvRect(7,0,roi.width-7,6);
        cvSetImageROI(src,r);
        cvSetImageROI(mask,r);
        r.x-=1;
        cvSetImageROI(codes,r);
        cvSetImageROI(funcs,r);
        cvCopy(src,codes);
        cvCopy(mask,funcs);
        
        r=cvRect(0,7,6,roi.height-6);
        cvSetImageROI(src,r);
        cvSetImageROI(mask,r);
        r.y-=1;
        cvSetImageROI(codes,r);
        cvSetImageROI(funcs,r);
        cvCopy(src,codes);
        cvCopy(mask,funcs);
        
        r=cvRect(7,7,roi.width-7,roi.height-7);
        cvSetImageROI(src,r);
        cvSetImageROI(mask,r);
        r.x-=1;
        r.y-=1;
        cvSetImageROI(codes,r);
        cvSetImageROI(funcs,r);
        cvCopy(src,codes);
        cvCopy(mask,funcs);
        
        cvSetImageROI(src,roi);
        cvSetImageROI(mask,roi);
        
        cvResetImageROI(codes);
        cvResetImageROI(funcs);

        //
        // read code from matrix
        //
        uchar data=0;
        int bit_count=0,word_count=0;
        int dir=-1;
        int x=codes->width-1;
        int y=codes->height-1;
        
        do{
            data<<=1;
            bit_count++;
            if(CV_IMAGE_ELEM(codes,uchar,y,x)>=128)
                data|=1;
            if(!(bit_count%8)){
                if(this->qr->push_codedata(data))
                    word_count++;
                data=0;
            }
            
            if(x&0x1){
                if(CV_IMAGE_ELEM(funcs,uchar,y,x-1))
                    x--;
                else{
                    do{
                        y+=dir;
                        if(y<0||y>=codes->height){
                            x-=2;
                            dir*=-1;
                            y+=dir;
                            if(x<0)
                                break;
                        }
                    }while(!CV_IMAGE_ELEM(funcs,uchar,y,x));
                }
            }
            else{
                x++;
                do{
                    y+=dir;
                    if(y>=0&&y<codes->height){
                        if(CV_IMAGE_ELEM(funcs,uchar,y,x)){
                            break;
                        }
                        else if(CV_IMAGE_ELEM(funcs,uchar,y,x-1)){
                            x--;
                            break;
                        }
                        else{
                            y+=dir;
                        }
                    }
                    else{
                        x-=2;
                        dir*=-1;
                        y+=dir;
                        if(x<0)
                            break;
                    }
                }while(!CV_IMAGE_ELEM(funcs,uchar,y,x));
            }
        }while(x>=0);
        
        if(data){
            if(this->qr->push_codedata(data))
                word_count++;
        }
        
        cvReleaseImage(&funcs);
        cvReleaseImage(&codes);
        
#ifdef _DEBUG
        fprintf(stderr,"%d words in %d blocks <= %d bits, %d words were read.\n",
                this->qr->codedata->total_words,
                this->qr->codedata->data_blocks,
                bit_count,word_count);
#endif
        
        return(bit_count);
    }


    /////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    double ImageReader::_get_cell_size()
    {
        IplImage *src=this->_img_binarized;
        this->_find_finder_pattern();
            
        int c=this->_seq_finder_pattern->total;
        if(c!=3)
            return(-1.0);

        double cell_size=0.0;
        for(int i=0;i<c;i++){
            CvBox2D box=*(CvBox2D *)cvGetSeqElem(this->_seq_finder_pattern,
                                                 i);
            cell_size+=box.size.width+box.size.height;
        }
        
        cell_size/=42.0;
        
        return(cell_size);
    }

    /////////////////////////////////////////////////////////////////////////
    //
    // 
    //
    IplImage *ImageReader::_get_mask_pattern()
    {
        IplImage *mask=cvCreateImage(cvSize(this->qr->cells_par_side,
                                            this->qr->cells_par_side),
                                     IPL_DEPTH_8U,1);
        cvZero(mask);

        CvRect roi=cvGetImageROI(mask);
        roi.width+=roi.x;
        roi.height+=roi.y;
        
        for(int y=roi.y;y<roi.height;y++){
            for(int x=roi.x;x<roi.width;x++){
                CV_IMAGE_ELEM(mask,uchar,y,x)=this->qr->
                    formatinfo->mask_pixel(y,x);
            }
        }
        
        return(mask);
    }


    /////////////////////////////////////////////////////////////////////////
    //
    // sort by clock-wise
    //
    static int seq_cmp_by_clockwise(const void *_a,const void *_b,void *_cog)
    {
        CvPoint* a = (CvPoint*)_a;
        CvPoint* b = (CvPoint*)_b;
        CvPoint2D32f *cog=(CvPoint2D32f *)_cog;
        
        float aa=cvFastArctan((float)(a->y)-cog->y,cog->x-(float)(a->x));
        float ba=cvFastArctan((float)(b->y)-cog->y,cog->x-(float)(b->x));
        
        return(aa < ba ? 1 : aa > ba ? -1 : 0);
    }

    /////////////////////////////////////////////////////////////////////////
    //
    // following code originates in OpenCV/cv/src/cvadapthresh.cpp
    //
    void apaptive_white_leveling(const CvArr* src,
                                 CvArr* dst,
                                 double middle_value,
                                 int adaptive_method,
                                 int threshold_type,
                                 int block_size,
                                 double param1)
    {
        CV_FUNCNAME( "apaptive_white_leveling" );
        CvMat src_stub, dst_stub;
        CvMat *srcMat,*dstMat,*mean,*mask;
        
        __BEGIN__
        if( adaptive_method != CV_ADAPTIVE_THRESH_MEAN_C &&
            adaptive_method != CV_ADAPTIVE_THRESH_GAUSSIAN_C )
            CV_ERROR( CV_StsBadArg,
                      "Only CV_ADAPTIVE_THRESH_MEAN_C and CV_ADAPTIVE_THRESH_GAUSSIAN_C "
                      "adaptive method are acceptable" );
        
        if( threshold_type != CV_THRESH_BINARY &&
            threshold_type != CV_THRESH_BINARY_INV )
            CV_ERROR( CV_StsBadArg, "Only CV_TRESH_BINARY and CV_THRESH_BINARY_INV "
                      "threshold types are acceptable" );
        
        srcMat=cvGetMat(src, &src_stub );
        dstMat=cvGetMat(dst, &dst_stub );
        
        if( !CV_ARE_CNS_EQ( srcMat, dstMat ))
            CV_ERROR( CV_StsUnmatchedFormats, "" );
        
        if( CV_MAT_TYPE(dstMat->type) != CV_8UC1 )
            CV_ERROR( CV_StsUnsupportedFormat, "" );
        
        if( !CV_ARE_SIZES_EQ( srcMat, dstMat ) )
            CV_ERROR( CV_StsUnmatchedSizes, "" );
        
        mean=cvCreateMat(srcMat->rows,srcMat->cols,CV_8UC1);
        mask=cvCreateMat(srcMat->rows,srcMat->cols,CV_8UC1);
        
        cvSmooth( srcMat, mean, adaptive_method == CV_ADAPTIVE_THRESH_MEAN_C ?
                  CV_BLUR : CV_GAUSSIAN, block_size, block_size );
        cvSubS(mean,cvRealScalar(param1),mean);
        cvAbsDiff(srcMat,mean,dstMat);
        cvCmp(srcMat,mean,mask,CV_CMP_GT);
        cvAddS(dstMat,cvRealScalar(middle_value),dstMat,mask);
        cvNot(mask,mask);
        cvSubRS(dstMat,cvRealScalar(middle_value),dstMat,mask);
        
        if(threshold_type!=CV_THRESH_BINARY)
            cvNot(dstMat,dstMat);
        
        cvReleaseMat( &mask );
        cvReleaseMat( &mean );
        
        __END__
            
    }
    
}
