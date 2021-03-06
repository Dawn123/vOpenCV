//vinjn's wrapper for OpenCV

#pragma once

#pragma warning( disable: 4244 )
#pragma warning( disable: 4996 )
#pragma warning( disable: 4305 )
#pragma warning( disable: 4018 )
#pragma warning( disable: 4099 )
#pragma warning( disable: 4819 )

#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include <vector>

#include "point2d.h"
#include "OpenGL.h"

#define OPENCV_VERSION CVAUX_STR(CV_MAJOR_VERSION)""CVAUX_STR(CV_MINOR_VERSION)""CVAUX_STR(CV_SUBMINOR_VERSION)
// 210/231/243

// enum T_VideoCodec
// {
// 	T_MPEG = CV_FOURCC('P','I','M','1'),		//	= MPEG-1 codec
// 	T_MJPG = CV_FOURCC('M','J','P','G') ,		//	= motion-jpeg codec (does not work well)
// 	T_MP42 = CV_FOURCC('M', 'P', '4', '2'),		// = MPEG-4.2 codec
// 	T_DIV3 = CV_FOURCC('D', 'I', 'V', '3'),		// = MPEG-4.3 codec
// 	T_DIVX = CV_FOURCC('D', 'I', 'V', 'X'),		// = MPEG-4 codec
// 	T_H263 = CV_FOURCC('U', '2', '6', '3'),		// = H263 codec
// 	T_H263I = CV_FOURCC('I', '2', '6', '3'),	// = H263I codec
// 	T_FLV = CV_FOURCC('F', 'L', 'V', '1'),		// = FLV1 codec
// };

template<class T> class Image
{
private:
	IplImage* imgp;
public:
	Image(IplImage* img=0) {imgp=img;}
	~Image(){imgp=0;}
	void operator=(IplImage* img) {imgp=img;}
	inline T* operator[](const int rowIndx) {
		return ((T *)(imgp->imageData + rowIndx*imgp->widthStep));}
};

typedef struct{
	unsigned char b,g,r;
} RgbPixel;

typedef struct{
	float b,g,r;
} RgbPixelFloat;

typedef Image<RgbPixel>       RgbImage;
typedef Image<RgbPixelFloat>  RgbImageFloat;
typedef Image<unsigned char>  BwImage;
typedef Image<float>          BwImageFloat;

/*
For a single-channel byte image:
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);
BwImage imgA(img);
imgA[i][j] = 111;
For a multi-channel byte image:
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);
RgbImage  imgA(img);
imgA[i][j].b = 111;
imgA[i][j].g = 111;
imgA[i][j].r = 111;
For a multi-channel float image:
IplImage* img=cvCreateImage(cvSize(640,480),IPL_DEPTH_32F,3);
RgbImageFloat imgA(img);
imgA[i][j].b = 111;
imgA[i][j].g = 111;
imgA[i][j].r = 111;
*/
void vFastCopyImageTo(const cv::Mat& src, cv::Mat& dst, const cv::Rect& roi);
void vCopyImageTo(const cv::Mat& src, cv::Mat& dst, const cv::Rect& roi);
void vFlip(cv::Mat& src, int flipHorizontal, int flipVertical);
void vDrawText(cv::Mat& img, int x,int y,char* str, CvScalar clr=CV_RGB(255,255,255));
void vPolyLine(cv::Mat& dst, std::vector<cv::Point>& pts, CvScalar clr=CV_RGB(255,255,255), int thick = 1);
CvScalar vDefaultColor(int idx);

#define show_image(img_name) do{\
	cvNamedWindow(#img_name);\
	cvShowImage(#img_name, img_name);}\
	while(0);

#define show_mat(img_name) do{\
	cv::namedWindow(#img_name);\
	cv::imshow(#img_name, img_name);}\
	while(0);

const CvScalar CV_RED = CV_RGB(255,0,0);
const CvScalar CV_GREEN = CV_RGB(0,255,0);
const CvScalar CV_BLUE = CV_RGB(0,0,255);
const CvScalar CV_BLACK = CV_RGB(0,0,0);
const CvScalar CV_WHITE = CV_RGB(255,255,255);
const CvScalar CV_GRAY = CV_RGB(122,122,122); 

inline CvScalar vRandomColor()
{
	static CvRNG   rng = cvRNG((unsigned)-1);
	int icolor = cvRandInt(&rng);
	return CV_RGB(icolor&255, (icolor>>8)&255, (icolor>>16)&255);
}

#define vDrawRect(image, rc, clr) cv::rectangle(image, cv::Point(rc.x,rc.y), cv::Point(rc.x+rc.width,rc.y+rc.height), clr)

#define WRITE_(key, var) fs<<key<<var
#define WRITE_FS(var) fs<<(#var)<<(var)

#define READ_(key, var) fs[key]>>var
#define READ_FS(var) fs[#var]>>(var)

#define vGrayScale(clr, gray) cv::cvtColor(clr, gray, CV_BGR2GRAY) 
#define vColorFul(gray, clr) cv::cvtColor(gray, clr , CV_GRAY2BGR) 
#define vThresh(gray, thresh) cv::threshold( gray, gray, thresh, 255, CV_THRESH_BINARY )//if > thresh -> white
#define vThreshInv(gray, thresh) cv::threshold( gray, gray, thresh, 255, CV_THRESH_BINARY_INV )//if < thresh -> white
#define vOpen(img, times) cv::morphologyEx( img, img, CV_MOP_OPEN, NULL, Point(-1,-1), times );//去除白色小区域
#define vClose(img, times) cv::morphologyEx( img, img, CV_MOP_CLOSE, NULL, Point(-1,-1), times );//去除黑色小区域
#define vDilate(img, times) cv::morphologyEx( img, img, CV_MOP_DILATE, NULL, Point(-1,-1), times );
#define vErode(img, times) cv::morphologyEx( img, img, CV_MOP_ERODE, NULL, Point(-1,-1), times );

#define vFullScreen(win_name) cvSetWindowProperty(win_name, CV_WND_PROP_FULLSCREEN, 1);

#define vCreateGray(clr) cvCreateImage(cvGetSize(clr), 8, 1);
#define vCreateColor(clr) cvCreateImage(cvGetSize(clr), 8, 3);

struct VideoInput
{
	int _fps;
	int device_id;

	enum e_InputType
	{
		From_Image = 0,
		From_Video,
		From_Camera,
		From_Count,
	}_InputType;

	int _argc;
	char** _argv;

	cv::VideoCapture _capture;

	void showSettingsDialog();

	cv::Mat _frame;
	int _cam_idx;
	cv::Size _size;
	cv::Size _half_size;
	int _channel;
	int _codec;

	int _frame_num;

	VideoInput();

	void resize(int w, int h);

	bool init(int cam_idx);
	bool init(const std::string& video_file);
	bool init(int argc, char** argv);

	void wait(int t);

	cv::Mat get_frame();
private:

	void _post_init();
	char buffer[256];
};


void vRotateImage(IplImage* image, float angle, float centreX, float centreY);

void vHighPass(const cv::Mat& src, cv::Mat& dst, int blurLevel = 10, int noiseLevel = 3);

void vPerspectiveTransform(const CvArr* src, CvArr* dst, cv::Point srcQuad[4], cv::Point dstQuad[4]);

void vGetPerspectiveMatrix(CvMat*& warp_matrix, cv::Point2f xsrcQuad[4], cv::Point2f xdstQuad[4]);

#define vAddWeighted(src, alpha, dst) cvAddWeighted(src, alpha, dst, 1-alpha, 0, dst);
 
void cvSkinSegment(IplImage* img, IplImage* mask);

void vFillPoly(IplImage* img, const std::vector<cv::Point>& pt_list, const cv::Scalar& clr = cv::Scalar(255,255,255));
void vLinePoly(IplImage* img, const std::vector<cv::Point>& pt_list, const cv::Scalar& clr = cv::Scalar(255,255,255), int thick = 1);
void vLinePoly(IplImage* img, const std::vector<cv::Point2f>& pt_list, const cv::Scalar& clr = cv::Scalar(255,255,255), int thick = 1);

inline bool isPointInsideRect(int x, int y, const cv::Rect& rect)
{
	return (x >= rect.x && x <= rect.x+rect.width &&
		y >= rect.y && y <= rect.height);
}

// Object-to-object bounding-box collision detector:
bool vTestRectHitRect(const cv::Rect& object1, const cv::Rect& object2);
