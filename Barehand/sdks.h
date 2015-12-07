#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
#endif

#define MAX_QUEUE_HAND_DEPTH 30
#define HAND_MAX_THRES 1
#define HAND_MIN_THRES -1


//	common headers
//--------------------------------------------------------
#include "Form1.h"
#include <iostream>
#include <Math.h>
#include <time.h>
#include <process.h>
#include <vcclr.h>
#include <queue>

#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <ShlObj.h>


//
//	OpenCV headers
//
#include <opencv2/opencv.hpp>
// Version
#define CV_VERSION_STR CVAUX_STR(CV_MAJOR_VERSION) CVAUX_STR(CV_MINOR_VERSION) CVAUX_STR(CV_SUBMINOR_VERSION)
// Build Mode
#ifdef _DEBUG
#define CV_EXT_STR "d.lib"
#else
#define CV_EXT_STR ".lib"
#endif
// Linked libs
#pragma comment(lib, "opencv_core"			CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_highgui"		CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_imgproc"		CV_VERSION_STR CV_EXT_STR)
//#pragma comment(lib, "opencv_calib3d"		CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_gpu"			CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_video"			CV_VERSION_STR CV_EXT_STR)
#pragma comment(lib, "opencv_objdetect"		CV_VERSION_STR CV_EXT_STR)
//#pragma comment(lib, "opencv_features2d"	CV_VERSION_STR CV_EXT_STR)
//#pragma comment(lib, "opencv_flann"		CV_VERSION_STR CV_EXT_STR)
//#pragma comment(lib, "opencv_ffmpeg"		CV_VERSION_STR CV_EXT_STR)
//#pragma comment(lib, "opencv_ts"			CV_VERSION_STR CV_EXT_STR)
//#pragma comment(lib, "opencv_contrib"		CV_VERSION_STR CV_EXT_STR)
//#pragma comment(lib, "opencv_ml"			CV_VERSION_STR CV_EXT_STR)
//#pragma comment(lib, "opencv_legacy"		CV_VERSION_STR CV_EXT_STR)

//
//	Kinect SDK headers & libs
//
#include <Kinect.h>
#pragma comment(lib,"kinect20.lib")

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

//	Namespaces
//--------------------------------------------------------
using namespace Barehand;
using namespace System;
using namespace System::Runtime::InteropServices;
using namespace cv;

template<class Interface>
inline void SafeRelease(Interface *& pInterfaceToRelease)
{
	if (pInterfaceToRelease != NULL)
	{
		pInterfaceToRelease->Release();
		pInterfaceToRelease = NULL;
	}
}





//	Classes
//--------------------------------------------------------
class LaserTracker{
public:
	const static int	CAM_WIDTH = 640;
	const static int	CAM_HEIGHT = 480;
	const static int	CAM_THRESHOLD = 50;
	const static int	CAM_THRESHOLD_MAXVAL = 255;	
	const static int	CAM_INDEX = 0;

	cv::VideoCapture	capture;

	int					thresh;

	bool				flagLT, 
						flagLB,		
						flagRT, 
						flagRB;

	cv::Point			screenLT, 
						screenLB, 
						screenRT, 
						screenRB;

	Mat					outputMat, 
						outputMatBuffer, 
						camframe, 
						camframeBuffer, 
						background;

	cv::Point			outputPnt;
	cv::Point			currentLargestDotPnt;
	double				fps;
	short				framecount;

	clock_t				pretime, 
						curtime;

private:
	cv::Point			targetLT, 
						targetLB, 
						targetRT, 
						targetRB;
public:
	LaserTracker(){
		thresh = CAM_THRESHOLD;
		flagLT = false;
		flagLB = false;
		flagRT = false;
		flagRB = false;

		capture = cv::VideoCapture(CAM_INDEX);
		camframe = Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC3);
		background = Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC3);
		outputMat = Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC3);
		outputPnt = cv::Point(0, 0);
		currentLargestDotPnt = cv::Point(0, 0);

		screenLT = cv::Point( 0, 0 );
		screenLB = cv::Point( 0, CAM_HEIGHT - 1 );
		screenRT = cv::Point( CAM_WIDTH - 1, 0 );
		screenRB = cv::Point( CAM_WIDTH - 1, CAM_HEIGHT - 1 );
		targetLT = cv::Point( 0, 0 );
		targetLB = cv::Point( 0, CAM_HEIGHT - 1 );
		targetRT = cv::Point( CAM_WIDTH - 1, 0 );
		targetRB = cv::Point( CAM_WIDTH - 1, CAM_HEIGHT - 1 );

		capture >> camframe;
		camframe.copyTo(background);
		fps = 0;
		framecount = 0;
	}

	~LaserTracker(){
		capture.release(); 
	}

	void refreshBG(){ 
		camframe.copyTo(background);
	}

	void setCorners(int idx){
		switch(idx){
			case 0:
				//Commentted-out: using single laser-dot method
				//screenLT = outputPnt;
				screenLT = currentLargestDotPnt;
				flagLT = true;
				break;
			case 1:
				//screenLB = outputPnt;
				screenLB = currentLargestDotPnt;
				flagLB = true;
				break;
			case 2:
				//screenRT = outputPnt;
				screenRT = currentLargestDotPnt;
				flagRT = true;
				break;
			case 3:
				//screenRB = outputPnt;
				screenRB = currentLargestDotPnt;
				flagRB = true;
				break;
			default:
				break;
		}
	}

	void clearCorners(){
		flagLT = false;
		flagLB = false;
		flagRT = false;
		flagRB = false;
		screenLT = cv::Point( 0, 0 );
		screenLB = cv::Point( 0, CAM_HEIGHT - 1 );
		screenRT = cv::Point( CAM_WIDTH - 1, 0 );
		screenRB = cv::Point( CAM_WIDTH - 1, CAM_HEIGHT - 1 );
	}

	bool findLaserCoordinate(cv::Point &TargetCoordinate);

};


class HandTracker{
public:
	// ===KINECT10===
	//const static int KINECT_WIDTH = 640;
	//const static int KINECT_HEIGHT = 480;
	//const static int KINECT_PIXELDEPTH = 4;
	//const static int DEPTH_THRESHOLD_LOW = 215;
	//const static int DEPTH_THRESHOLD_HIGH = 254;
	//const static int DEPTH_THRESHOLD_MAXVAL = 255;
	//const static int HAND_DEPTH_RANGE = 70;
	// ===KINECT20===
	const static int	KINECT_WIDTH = 512;
	const static int	KINECT_HEIGHT = 424;
	const static int	DEPTH_THRESHOLD_LOW = 234;
	const static int	DEPTH_THRESHOLD_HIGH = 240;
	const static int	DEPTH_THRESHOLD_MAXVAL = 255;
	const static int	HAND_DEPTH_RANGE = 7;
	const static int	KINECT_TICK_LIFE_DEFAULT = 200;
	const double		ARM_STRETCH_THRESHOULD = 0.30f;
	const static int	BODY_JOINT_BUF_LENGTH = 50;
	// ==============
	const static int MAX_PALM_AREA = 30000;
	double MOUSE_PRESS_THRESH;

	HRESULT				hSensorInitResult;
	IKinectSensor*		m_pSensor;
	IDepthFrameReader*	m_pDepthFrameReader;
	ICoordinateMapper*	m_pCoordinateMapper;
	IBodyFrameReader*	m_pBodyFrameReader;
	USHORT*				m_pDepthImage;
	HRESULT				m_hSensorInitResult;
	Joint				m_aBufferedJoints[JointType_Count];
	LONG				m_lSkeletonRecongCnt;
	INT32				m_nKinectTickLife;
	
	double threshLMB, armStretchDist;
	int thresh_low, thresh_high;
	bool flagCursorFirstMove, mousedownflag;

	Mat		outputMat,
			outputMatBuffer,
			converted_depth,
			binMask1,
			binMask2,
			depthmap, 
			depthmask, 
			depthmaskBuffer;

	cv::Point outputPnt;
	std::deque<CameraSpacePoint> hQueueLeftShoulder;
	std::deque<CameraSpacePoint> hQueueRightShoulder;
	std::deque<CameraSpacePoint> hQueueSpinMid;

	CvFont font;
	char str[64];

	double fps;
	short framecount;
	clock_t pretime, curtime;

	int skeletonStatus;

	int nNose;

	double averageDepth;

private:
	double handContourLength, shapeRatio, areaRatio;
	//gcroot<Queue^> areaQueue;
	//gcroot<Queue^> handDepthQueue;
	bool bTouching, bHandFirstMove;
	double handLastDepth;

public:
	HandTracker(){
		MOUSE_PRESS_THRESH = 50.0;

		threshLMB = MOUSE_PRESS_THRESH;
		thresh_low = DEPTH_THRESHOLD_LOW;
		thresh_high = DEPTH_THRESHOLD_HIGH;
		flagCursorFirstMove = true;
		mousedownflag = false;

		outputMat = Mat(KINECT_HEIGHT, KINECT_WIDTH, CV_8UC3);
		converted_depth = Mat(KINECT_HEIGHT, KINECT_WIDTH, CV_8UC1);
		binMask1 = Mat(KINECT_HEIGHT, KINECT_WIDTH, CV_8UC1);
		binMask2 = Mat(KINECT_HEIGHT, KINECT_WIDTH, CV_8UC1);
		depthmask = Mat(KINECT_HEIGHT, KINECT_WIDTH, CV_16UC1);

		outputPnt = cv::Point(0,0);

		m_pSensor = NULL ;
		m_pDepthFrameReader = NULL;
		m_pCoordinateMapper = NULL;
		m_pBodyFrameReader = NULL;
		m_pDepthImage = new USHORT[KINECT_HEIGHT * KINECT_WIDTH];
		m_hSensorInitResult = InitSensor();
		m_lSkeletonRecongCnt = 0;
		m_nKinectTickLife = KINECT_TICK_LIFE_DEFAULT;


		cvInitFont(&font, 2, 1.0, 1.0, 0, 2);

		fps = 0;
		framecount = 0;

		//areaQueue = gcnew Queue();
		//handDepthQueue = gcnew Queue();

		bTouching = false;
		bHandFirstMove = true;

		handLastDepth = 0.0;
	}

	~HandTracker(){
		if (m_pSensor)
		{
			m_pSensor->Close();
		}
		SafeRelease(m_pSensor);
	}

	HRESULT InitSensor();
	BOOLEAN findHandCoordinate(cv::Point &TargetCoordinate, int &MouseLMB);
	BOOLEAN GetXthBody(Joint* outJoints, int bodyIndex);
};


class Vector3D{
public:
	double x, y, z;

	Vector3D(){	}
	Vector3D(double x, double y, double z)
	{
		this->x = x;
		this->y = y;
		this->z = z;
	}
	Vector3D(const Vector3D& v)
	{
		x = v.x;
		y = v.y;
		z = v.z;
	}
	Vector3D(const cv::Point3f& p)
	{
		x = p.x;
		y = p.y;
		z = p.z;
	}
	Vector3D(const CameraSpacePoint& p)
	{
		x = p.X;
		y = p.Y;
		z = p.Z;
	}

	Vector3D operator + (const Vector3D& v1) const
	{
		return Vector3D(x + v1.x, y + v1.y, z + v1.z);
	}

	//vector3d subtraction
	Vector3D operator - (const Vector3D& v) const
	{
		return Vector3D(x - v.x, y - v.y, z - v.z);
	}

	//get multiplied by a number
	Vector3D operator < (const double& vs) const
	{
		return Vector3D(x * vs, y * vs, z * vs);
	}

	//vector3d cross product
	Vector3D operator || (const Vector3D& vr) const
	{
		return Vector3D
		(
			(y*vr.z) - (z*vr.y),
			(z*vr.x) - (x*vr.z),
			(x*vr.y) - (y*vr.x)
		);
	}

	//Vector3d dot product
	double operator * (const Vector3D& vd) const
	{
		return x*vd.x + y*vd.y + z*vd.z;
	}


	//get euclidean length
	double Length()
	{
		return pow((x*x) + (y*y) + (z*z), 0.5);
	}

	//get squared length
	double SquaredLength()
	{
		return (x*x) + (y*y) + (z*z);
	}

	//normalization
	void Normalize()
	{
		double length = this->Length();
		x /= length;
		y /= length;
		z /= length;
	}
};
#define Vertex3D Vector3D



//	Global Values
//--------------------------------------------------------
extern bool flag;
extern SOCKET sendSock;
extern sockaddr_in addr;

extern LaserTracker lt;
extern HandTracker ht;

cv::Point findMaxAreaCoordinate(vector<vector<cv::Point> > contours, double &maxArea, UINT &maxidx);
cv::Point findTop2AreaCoordinate(vector<vector<cv::Point> > contours, double &maxArea, UINT &maxidx, cv::Point &maxCoord);
bool GetUnboundedROI(cv::Rect& ROI, cv::Point center, int rectRadius, int camWidth, int camHeight);
bool findTop(cv::Point& top, int& topVal, cv::Mat* src, cv::Rect rect);
//bool checkAreaInc(Queue^ queue);
//bool checkAreaDec(Queue^ queue);
//double shapeComplexity(vector<cv::Point> &contour);
double PointToLine3D(CameraSpacePoint a, CameraSpacePoint b, CameraSpacePoint p);
Vector3D CreatePolygonNormal(Vertex3D A, Vertex3D B, Vertex3D C);
double PointToPlane3D(CameraSpacePoint a, CameraSpacePoint b, CameraSpacePoint c, CameraSpacePoint p);
Vector3D TransformToUVNCam(Vector3D P, Vector3D N, Vector3D BaseP);
CameraSpacePoint CSPointAverage(std::deque<CameraSpacePoint> pntQueue);