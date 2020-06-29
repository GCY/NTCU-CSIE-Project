/*
 * A Demo to OpenCV Implementation of SURF
 * Further Information Refer to "SURF: Speed-Up Robust Feature"
 * Author: Liu Liu
 * liuliu.1987+opencv@gmail.com
 */

    #include <opencv2/objdetect/objdetect.hpp>
    #include <opencv2/features2d/features2d.hpp>
    #include <opencv2/core/core.hpp>
    #include <opencv2/highgui/highgui.hpp>
    #include <opencv2/legacy/legacy.hpp>
    #include <opencv2/legacy/compat.hpp>
    #include <opencv2/flann/flann.hpp>
    #include <opencv2/calib3d/calib3d.hpp>
    #include <opencv2/nonfree/features2d.hpp>
    #include <opencv2/nonfree/nonfree.hpp>
 
#include <cv.h>
#include <highgui.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

#include <OpenNI.h>
#include <NiTE.h>
 
#include <iostream>
#include <vector>
 
using namespace std;
 
IplImage *image = 0;

const int SCREEN_WIDTH = 640;
const int SCREEN_HIGHT = 480;

	cv::CascadeClassifier face_cascade;
	cv::CascadeClassifier eyes_cascade;

void FindFace(cv::Mat &RGBMat)
{
	std::vector<cv::Rect> faces;
	cv::Mat frame_gray;

	cvtColor( RGBMat, frame_gray, CV_BGR2GRAY );
	equalizeHist( frame_gray, frame_gray );
	//-- Detect faces
	face_cascade.detectMultiScale( frame_gray, faces, 1.1f, 2, 0|CV_HAAR_SCALE_IMAGE, cv::Size(40,40) );

	std::vector<int> areas;
	for(size_t i = 0; i < faces.size(); i++ ){
		areas.push_back(faces[i].area());
	}

	int temp = 0;
	int max_area = 0;
	for(size_t i = 0; i < areas.size(); i++ ){
		if(areas[i] > temp){
			temp = areas[i];
			max_area = i;
		}
	}

	if(faces.size() > 0){
		cv::Point center( faces[max_area].x + faces[max_area].width*0.5, faces[max_area].y + faces[max_area].height*0.5 );
		cv::ellipse( RGBMat, center, cv::Size( faces[max_area].width*0.5, faces[max_area].height*0.5), 0, 0, 360, cv::Scalar( 0, 255, 0 ), 2, 8, 0 );
 
		cv::Mat faceROI = frame_gray( faces[max_area] );
		std::vector<cv::Rect> eyes;
 
		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, cv::Size(0.1f,0.1f) );
 
		for(size_t j = 0; j < eyes.size(); j++ ){
			cv::Point center( faces[max_area].x + eyes[j].x + eyes[j].width*0.5, faces[max_area].y + eyes[j].y + eyes[j].height*0.5 );
			int radius = cvRound( (eyes[j].width + eyes[max_area].height)*0.25 );
			cv::circle( RGBMat, center, radius, cv::Scalar( 255, 0, 0 ), 3, 8, 0 );
		}
	}
 /*
	for(size_t i = 0; i < faces.size(); i++ )
	{
		std::cout << faces.size() << std::endl;
		std::cout << faces[i].area() << std::endl;
		cv::Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
		cv::ellipse( RGBMat, center, cv::Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, cv::Scalar( 0, 255, 0 ), 2, 8, 0 );
 
		cv::Mat faceROI = frame_gray( faces[i] );
		std::vector<cv::Rect> eyes;
 
		//-- In each face, detect eyes
		eyes_cascade.detectMultiScale( faceROI, eyes, 1.1, 2, 0 |CV_HAAR_SCALE_IMAGE, cv::Size(0.1f,0.1f) );
 
		for(size_t j = 0; j < eyes.size(); j++ ){
			cv::Point center( faces[i].x + eyes[j].x + eyes[j].width*0.5, faces[i].y + eyes[j].y + eyes[j].height*0.5 );
			int radius = cvRound( (eyes[j].width + eyes[i].height)*0.25 );
			cv::circle( RGBMat, center, radius, cv::Scalar( 255, 0, 0 ), 3, 8, 0 );
		}
	}
	*/
}
 
double
compareSURFDescriptors( const float* d1, const float* d2, double best, int length )
{
    double total_cost = 0;
    assert( length % 4 == 0 );
    for( int i = 0; i < length; i += 4 )
    {
        double t0 = d1[i] - d2[i];
        double t1 = d1[i+1] - d2[i+1];
        double t2 = d1[i+2] - d2[i+2];
        double t3 = d1[i+3] - d2[i+3];
        total_cost += t0*t0 + t1*t1 + t2*t2 + t3*t3;
        if( total_cost > best )
            break;
    }
    return total_cost;
}
 
int
naiveNearestNeighbor( const float* vec, int laplacian,
                      const CvSeq* model_keypoints,
                      const CvSeq* model_descriptors )
{
    int length = (int)(model_descriptors->elem_size/sizeof(float));
    int i, neighbor = -1;
    double d, dist1 = 1e6, dist2 = 1e6;
    CvSeqReader reader, kreader;
    cvStartReadSeq( model_keypoints, &kreader, 0 );
    cvStartReadSeq( model_descriptors, &reader, 0 );
 
    for( i = 0; i < model_descriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* mvec = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        if( laplacian != kp->laplacian )
            continue;
        d = compareSURFDescriptors( vec, mvec, dist2, length );
        if( d < dist1 )
        {
            dist2 = dist1;
            dist1 = d;
            neighbor = i;
        }
        else if ( d < dist2 )
            dist2 = d;
    }
    if ( dist1 < 0.6*dist2 )
        return neighbor;
    return -1;
}
 
void
findPairs( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
           const CvSeq* imageKeypoints, const CvSeq* imageDescriptors, vector<int>& ptpairs )
{
    int i;
    CvSeqReader reader, kreader;
    cvStartReadSeq( objectKeypoints, &kreader );
    cvStartReadSeq( objectDescriptors, &reader );
    ptpairs.clear();
 
    for( i = 0; i < objectDescriptors->total; i++ )
    {
        const CvSURFPoint* kp = (const CvSURFPoint*)kreader.ptr;
        const float* descriptor = (const float*)reader.ptr;
        CV_NEXT_SEQ_ELEM( kreader.seq->elem_size, kreader );
        CV_NEXT_SEQ_ELEM( reader.seq->elem_size, reader );
        int nearest_neighbor = naiveNearestNeighbor( descriptor, kp->laplacian, imageKeypoints, imageDescriptors );
        if( nearest_neighbor >= 0 )
        {
            ptpairs.push_back(i);
            ptpairs.push_back(nearest_neighbor);
        }
    }
}
 
/* a rough implementation for object location */
int
locatePlanarObject( const CvSeq* objectKeypoints, const CvSeq* objectDescriptors,
                    const CvSeq* imageKeypoints, const CvSeq* imageDescriptors,
                    const CvPoint src_corners[4], CvPoint dst_corners[4] )
{
    double h[9];
    CvMat _h = cvMat(3, 3, CV_64F, h);
    vector<int> ptpairs;
    vector<CvPoint2D32f> pt1, pt2;
    CvMat _pt1, _pt2;
    int i, n;
 
    findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
    n = ptpairs.size()/2;
    if( n < 4 )
        return 0;
 
    pt1.resize(n);
    pt2.resize(n);
    for( i = 0; i < n; i++ )
    {
        pt1[i] = ((CvSURFPoint*)cvGetSeqElem(objectKeypoints,ptpairs[i*2]))->pt;
        pt2[i] = ((CvSURFPoint*)cvGetSeqElem(imageKeypoints,ptpairs[i*2+1]))->pt;
    }
 
    _pt1 = cvMat(1, n, CV_32FC2, &pt1[0] );
    _pt2 = cvMat(1, n, CV_32FC2, &pt2[0] );
    if( !cvFindHomography( &_pt1, &_pt2, &_h, CV_RANSAC, 5 ))
        return 0;
 
    for( i = 0; i < 4; i++ )
    {
        double x = src_corners[i].x, y = src_corners[i].y;
        double Z = 1./(h[6]*x + h[7]*y + h[8]);
        double X = (h[0]*x + h[1]*y + h[2])*Z;
        double Y = (h[3]*x + h[4]*y + h[5])*Z;
        dst_corners[i] = cvPoint(cvRound(X), cvRound(Y));
    }
 
    return 1;
}
 
int main(void)
{
	 cv::initModule_nonfree();

	openni::Device device;
	nite::UserTracker user_tracker;
	openni::VideoMode color_mode;
	openni::VideoStream color_stream;
	openni::VideoFrameRef color_frame;
	openni::OpenNI::initialize();
	nite::NiTE::initialize();
	device.open(openni::ANY_DEVICE);
	device.setImageRegistrationMode(openni::IMAGE_REGISTRATION_DEPTH_TO_COLOR);
	color_mode.setFps(30);
	color_mode.setResolution(SCREEN_WIDTH,SCREEN_HIGHT);
	color_mode.setPixelFormat(openni::PIXEL_FORMAT_RGB888);
	color_stream.setVideoMode(color_mode);
	color_stream.create(device,openni::SENSOR_COLOR);
	color_stream.start();

	CvFont font;
	cvInitFont(&font,CV_FONT_HERSHEY_COMPLEX,0.5,0.5,1,2,8);
    //template image to be recognized
        const char* object_filename = "face5.png";

	if(!face_cascade.load(std::string("./haarcascades/haarcascade_frontalface_alt.xml"))){

	}
	if(!eyes_cascade.load(std::string("./haarcascades/haarcascade_eye_tree_eyeglasses.xml"))){

	}

 
    CvMemStorage* storage = cvCreateMemStorage(0);
 
    cvNamedWindow("Template", 1);
    cvNamedWindow("Matching Display", 1);
 
    static CvScalar colors[] =
    {
        {{0,0,255}}, //0=red
        {{0,128,255}},
        {{0,255,255}},
        {{0,255,0}}, //3=green
        {{255,128,0}},
        {{255,255,0}},
        {{255,0,0}},
        {{255,0,255}},
        {{255,255,255}}
    };
 
    IplImage* object = cvLoadImage( object_filename, CV_LOAD_IMAGE_GRAYSCALE );
    IplImage* image = NULL, *image_layer=NULL/*, *image_layer2*/;
    IplImage* image_color = NULL;
 
    while(1){
		char s = cv::waitKey(1);
		if(s == 'l' || s == 'L'){
			object = cvLoadImage("face5.png", CV_LOAD_IMAGE_GRAYSCALE );
		}
		else if(s == 'g' || s == 'G'){
			object = cvLoadImage("face4.png", CV_LOAD_IMAGE_GRAYSCALE );
		}
	color_stream.readFrame(&color_frame);
	cv::Mat RGBMat(color_frame.getHeight(),color_frame.getWidth(),CV_8UC3,(void*)color_frame.getData());
	//FindFace(RGBMat);
	IplImage color_image(RGBMat);
	cvCvtColor(&color_image,&color_image,CV_RGB2BGR);
	image = cvCreateImage(cvGetSize(&color_image),IPL_DEPTH_8U,3);
	cvCopyImage(&color_image,image);
        image_layer = cvCreateImage(cvSize(image->width,image->height),image->depth,1);
        //Convert frame to gray and store in image
        cvCvtColor(image, image_layer, CV_BGR2GRAY);
 
        IplImage* object_color = cvCreateImage(cvGetSize(object), 8, 3);
        cvCvtColor( object, object_color, CV_GRAY2BGR );
 
        CvSeq *objectKeypoints = 0, *objectDescriptors = 0;
        CvSeq *imageKeypoints = 0, *imageDescriptors = 0;
        int i;
        CvSURFParams params = cvSURFParams(100, 1);
 
        double tt = (double)cvGetTickCount();
        //Calculate and extract feature point from template image
        cvExtractSURF( object, 0, &objectKeypoints, &objectDescriptors, storage, params );
        printf("Object Descriptors: %d\n", objectDescriptors->total);
 
        //Calculate and extract feature point from camera frame
        cvExtractSURF( image_layer, 0, &imageKeypoints, &imageDescriptors, storage, params );
        printf("Image Descriptors: %d\n", imageDescriptors->total);
        tt = (double)cvGetTickCount() - tt;
        printf( "Extraction time = %gms\n", tt/(cvGetTickFrequency()*1000.));
 
        CvPoint src_corners[4] = {{0,0}, {object->width,0}, {object->width, object->height}, {0, object->height}};
        CvPoint dst_corners[4];
 
        CvPoint img_corners[4] = {{0,0}, {image_layer->width,0}, {image_layer->width, image_layer->height}, {0, image_layer->height}};
        CvPoint dstimg_corners[4];
 
        //Mark the matching area with green rectangle
        if( locatePlanarObject( objectKeypoints, objectDescriptors, imageKeypoints,
            imageDescriptors, src_corners, dst_corners ))
        {
			cv::Rect detec_r(dst_corners[0],dst_corners[3]);
			
			char str[30];
			sprintf(str, "%d",detec_r.area());
			cvPutText(image, str, cvPoint(20,20), &font, CV_RGB(110,210,100));
			
			printf("%d\n",detec_r.area());
            for( i = 0; i < 4; i++ )
            {
                CvPoint r1 = dst_corners[i%4];
                CvPoint r2 = dst_corners[(i+1)%4];
                //cvPutText(image, "Matched!!", cvPoint(r1.x+5, r1.y+5), );
                //Green rectangle
                cvLine( image, cvPoint(r1.x, r1.y ),
                    cvPoint(r2.x, r2.y ), colors[i], 2 );
            }
 
            for( i = 0; i < 2; i++)
            {
                CvPoint r1 = dst_corners[i];
                CvPoint r2 = dst_corners[(i+2)];
                cvLine(image, cvPoint(r1.x, r1.y), cvPoint(r2.x, r2.y), colors[1]);
 
            }
        }
 
        vector<int> ptpairs;
        findPairs( objectKeypoints, objectDescriptors, imageKeypoints, imageDescriptors, ptpairs );
        printf("Matching points = %d\n\n", (int)ptpairs.size());
 
        //Display the feature point
        for( i = 0; i < objectKeypoints->total; i++ )
        {
            CvSURFPoint* r = (CvSURFPoint*)cvGetSeqElem( objectKeypoints, i );
            CvPoint center;
            int radius;
            center.x = cvRound(r->pt.x);
            center.y = cvRound(r->pt.y);
            radius = cvRound(r->size*1.2/9.*2);
            cvCircle( object_color, center, radius, colors[4], 1, 8, 0 );
        }
 
        cvShowImage( "Template", object_color );
        cvShowImage("Matching Display", image);

		cvReleaseImage(&image);
		cvReleaseImage(&image_layer);
		cvReleaseImage(&object_color);
 
        char c = cvWaitKey(1);
		if(c == 'q' || c == 'Q'){
			break;
		}
    }
 
    cvDestroyWindow("Template");
    cvDestroyWindow("Matching Display");
 
	color_stream.destroy();
	device.close();
	openni::OpenNI::shutdown();
	nite::NiTE::shutdown();
 
    return 0;
}