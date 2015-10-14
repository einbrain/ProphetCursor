#include "stdafx.h"
#include "sdks.h"


LaserTracker lt;
HandTracker ht;

bool flag = false;
SOCKET sendSock;
sockaddr_in addr;



//----------------------------------------------------------------
//	Member methods
//----------------------------------------------------------------

// - Laser Processing ----------------------------
bool LaserTracker::findLaserCoordinate(cv::Point &TargetCoordinate){
	if(framecount == 0){
		pretime = clock();
	}

	capture >> camframe;

	Mat lasermask = Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC1);
	Mat resV = Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC1);
	Mat backV = Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC1);
	Mat foreV = Mat(CAM_HEIGHT, CAM_WIDTH, CV_8UC1);

	assert(camframe.cols == CAM_WIDTH && camframe.rows == CAM_HEIGHT);

	int fromto[] = {2,0};
	cv::mixChannels(&background, 1, &backV, 1, fromto, 1);
	cv::mixChannels(&camframe, 1, &foreV, 1, fromto, 1);

	cv::absdiff(backV, foreV, resV);
	cv::threshold(resV, lasermask, thresh, CAM_THRESHOLD_MAXVAL, cv::THRESH_BINARY);
	cv::dilate(lasermask, lasermask, Mat());

	// - Perspective Transforming ------------------
	cv::Point2f srcpnt[] = { screenLT, screenLB, screenRT, screenRB };
	cv::Point2f dstpnt[] = { targetLT, targetLB, targetRT, targetRB };
	if( flagLT && flagLB && flagRT && flagRB ){
		Mat perspswap;
		lasermask.copyTo(perspswap);
		cv::warpPerspective(perspswap, lasermask, cv::getPerspectiveTransform( srcpnt, dstpnt ), cv::Size(CAM_WIDTH, CAM_HEIGHT),cv::INTER_NEAREST, cv::BORDER_DEFAULT); 
	}

	cv::cvtColor(lasermask, outputMat, CV_GRAY2BGR);

	vector< vector<cv::Point> > spotContours;
	cv::findContours(lasermask, spotContours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);

	double maxLaserArea;
	UINT maxLaserIdx;
	cv::Point coordinate = findMaxAreaCoordinate(spotContours,maxLaserArea,maxLaserIdx);

	if(maxLaserArea>0){
		// draw a green circle
		cv::circle(outputMat, coordinate, 3, cv::Scalar(0, 255, 0), 3, CV_AA, 0);

		outputMat.copyTo(outputMatBuffer);
		camframe.copyTo(camframeBuffer);

		outputPnt = coordinate;
		TargetCoordinate = coordinate;

		framecount ++;
		if(framecount>=5){	//count every 5 frames
			curtime = clock();
			fps = 5.0 / ((double)(curtime - pretime) / CLOCKS_PER_SEC);
			framecount=0;
		}

		return true;
	}

	outputMat.copyTo(outputMatBuffer);
	camframe.copyTo(camframeBuffer);


	framecount ++;
	if(framecount>=5){	//count every 5 frames
		curtime = clock();
		fps = 5.0 / ((double)(curtime - pretime) / CLOCKS_PER_SEC);
		framecount=0;
	}

	return false;
}


// - Hand Processing ----------------------------
BOOLEAN HandTracker::findHandCoordinate(cv::Point &TargetCoordinate, int &MouseLMB)
// RESULT: found hand position or not
{
	bool functionRes = false;

	if(framecount == 0){
		pretime = clock();
	}

	depthmask.setTo(0);

	// - Masking -------------------------------
	if (!m_pDepthFrameReader || !m_pBodyFrameReader)
	{
		return false;
	}

	IDepthFrame* pDepthFrame;
	HRESULT hr = m_pDepthFrameReader->AcquireLatestFrame(&pDepthFrame);
	if (SUCCEEDED(hr))
	{
		IFrameDescription* pFrameDescription = NULL;
		int nWidth = 0;
		int nHeight = 0;
		USHORT nDepthMinReliableDistance = 0;
		USHORT nDepthMaxdistance = 0;
		UINT nBufferSize = 0;
		UINT16 *pBuffer = NULL;

		HRESULT hr1 = pDepthFrame->get_FrameDescription(&pFrameDescription);
		if (SUCCEEDED(hr1)) //get depth height
		{
			hr1 = pFrameDescription->get_Height(&nHeight);
		}
		if (SUCCEEDED(hr1)) //get depth width
		{
			hr1 = pFrameDescription->get_Width(&nWidth);
		}
		if (SUCCEEDED(hr1)) //get min distance
		{
			hr1 = pDepthFrame->get_DepthMinReliableDistance(&nDepthMinReliableDistance);
		}
		if (SUCCEEDED(hr1)) //get max distance
		{
			//use the max reliable distance,
			hr1 = pDepthFrame->get_DepthMaxReliableDistance(&nDepthMaxdistance);
			//or use the datatype maximum.
			//nDepthMaxdistance = USHRT_MAX;
		}
		if (SUCCEEDED(hr1)) //set buffer pointer
		{
			hr1 = pDepthFrame->AccessUnderlyingBuffer(&nBufferSize, &pBuffer);
		}
		if (SUCCEEDED(hr1)) //get depth image & converted rgb image
		{
			if (pBuffer && (nWidth == KINECT_WIDTH) && (nHeight == KINECT_HEIGHT))
			{
				USHORT* pDepthX = m_pDepthImage;
				const UINT16* pBufferEnd = pBuffer + (nWidth*nHeight);

				while (pBuffer < pBufferEnd)
				{
					USHORT depth = *pBuffer;

					*pDepthX = (depth >= nDepthMinReliableDistance) && (depth <= nDepthMaxdistance) ? depth : USHRT_MAX;
					++pDepthX;
					++pBuffer;
				}

				depthmap = cv::Mat(KINECT_HEIGHT, KINECT_WIDTH, CV_16UC1, m_pDepthImage);
			}
			else return false;
		}

		SafeRelease(pFrameDescription);

		if (FAILED(hr1)) return false;
	}
	SafeRelease(pDepthFrame);

	// ==============


	assert(depthmap.cols == KINECT_WIDTH && depthmap.rows == KINECT_HEIGHT);

	try
	{
		depthmap.convertTo(converted_depth, CV_8U, -255.0f/8191.0f, 255.0f);
		cv::cvtColor(converted_depth, outputMat, CV_GRAY2BGR);
		cv::threshold(converted_depth, binMask1, thresh_high, DEPTH_THRESHOLD_MAXVAL, cv::THRESH_BINARY_INV);
		cv::threshold(converted_depth, binMask2, thresh_low, DEPTH_THRESHOLD_MAXVAL, cv::THRESH_BINARY);
		this->binMask1.copyTo(depthmask, binMask2);
	}
	catch (...)
	{
		MessageBox::Show("<?> OpenCV image conversion error in HandTracker");
	}
	// - ---------------------------------------


	

	// - Body joints ---------------------------
	// Some weird naming principles from Microsoft...
	//		CameraSpacePoint:	3D coordinates
	//		DepthSpacePoint:	2D coordinates relative to the (Depth) Image
	cv::Point2f cvHead, cvLShoulder, cvRShoulder, cvRHand, cvSpineMid;
	DepthSpacePoint dspHead, dspLShoulder, dspRShoulder, dspHand, dspSpineMid;

	// Acquire skeleton/body data and copy to buffer if successful.
	Joint joints[JointType_Count];
	BOOLEAN bSkelRes = this->GetXthBody(joints, 1);
	if (bSkelRes)
	{
		std::copy(std::begin(joints), std::end(joints), std::begin(m_aBufferedJoints));
		this->m_nKinectTickLife = KINECT_TICK_LIFE_DEFAULT;
	}

	// Control the life time of the body data buffer
	--m_nKinectTickLife;
	boolean bSkelSufficient = true; // body data sufficient
	if (m_nKinectTickLife <= 0)
	{
		bSkelSufficient = false;	// body data insufficient, i.e. absence of user for a while
	}

	if (bSkelSufficient) // Do body drawing and data sending only when body data is sufficient.
	{
		m_pCoordinateMapper->MapCameraPointToDepthSpace(m_aBufferedJoints[JointType_ShoulderLeft].Position, &dspLShoulder);
		m_pCoordinateMapper->MapCameraPointToDepthSpace(m_aBufferedJoints[JointType_ShoulderRight].Position, &dspRShoulder);
		m_pCoordinateMapper->MapCameraPointToDepthSpace(m_aBufferedJoints[JointType_Head].Position, &dspHead);
		m_pCoordinateMapper->MapCameraPointToDepthSpace(m_aBufferedJoints[JointType_SpineMid].Position, &dspSpineMid);
		m_pCoordinateMapper->MapCameraPointToDepthSpace(m_aBufferedJoints[JointType_HandRight].Position, &dspHand);
		cvHead.x = dspHead.X;
		cvHead.y = dspHead.Y;
		cvLShoulder.x = dspLShoulder.X;
		cvLShoulder.y = dspLShoulder.Y;
		cvRShoulder.x = dspRShoulder.X;
		cvRShoulder.y = dspRShoulder.Y;
		cvRHand.x = dspHand.X;
		cvRHand.y = dspHand.Y;
		cvSpineMid.x = dspSpineMid.X;
		cvSpineMid.y = dspSpineMid.Y;

		cv::Point polyPts[] = { cvLShoulder, cvRShoulder, cvSpineMid };
		cv::fillConvexPoly(outputMat, polyPts, 3, cv::Scalar(0, 255, 255, 50.0));
		cv::circle(outputMat, cvRHand, 3, cv::Scalar(0, 0, 255), 3, CV_AA, 0);
		cv::circle(outputMat, cvLShoulder, 3, cv::Scalar(0, 255, 0), 3, CV_AA, 0);
		cv::circle(outputMat, cvRShoulder, 3, cv::Scalar(0, 255, 0), 3, CV_AA, 0);
		cv::circle(outputMat, cvHead, 3, cv::Scalar(255, 0, 0), 3, CV_AA, 0);
		cv::circle(outputMat, cvSpineMid, 3, cv::Scalar(255, 0, 0), 3, CV_AA, 0);

		// Body plane buffer operations
		hQueueLeftShoulder.push_back(m_aBufferedJoints[JointType_ShoulderLeft].Position);
		while (hQueueLeftShoulder.size() > BODY_JOINT_BUF_LENGTH) hQueueLeftShoulder.pop_front();
		CameraSpacePoint AvgLeftPos = CSPointAverage(hQueueLeftShoulder);

		hQueueRightShoulder.push_back(m_aBufferedJoints[JointType_ShoulderRight].Position);
		while (hQueueRightShoulder.size() > BODY_JOINT_BUF_LENGTH) hQueueRightShoulder.pop_front();
		CameraSpacePoint AvgRightPos = CSPointAverage(hQueueRightShoulder);

		hQueueSpinMid.push_back(m_aBufferedJoints[JointType_SpineMid].Position);
		while (hQueueSpinMid.size() > BODY_JOINT_BUF_LENGTH) hQueueSpinMid.pop_front();
		CameraSpacePoint AvgMidPos = CSPointAverage(hQueueSpinMid);

		armStretchDist = PointToPlane3D(
			AvgRightPos,
			AvgMidPos,
			AvgLeftPos,
			m_aBufferedJoints[JointType_HandRight].Position
			);

		Vector3D bodyPlaneNormal = CreatePolygonNormal(
			Vector3D(AvgRightPos),
			Vector3D(AvgMidPos),
			Vector3D(AvgLeftPos)
			);

		Vector3D relativeRHandPosition = TransformToUVNCam(
			Vector3D(m_aBufferedJoints[JointType_HandRight].Position),
			bodyPlaneNormal,
			Vector3D(m_aBufferedJoints[JointType_SpineMid].Position)
			);

		// convert kinect axes to screen ones
		outputPnt.x = int(relativeRHandPosition.x * 3000);
		outputPnt.y = int(relativeRHandPosition.y * -3000);

		// - ---------------------------------------
		if (armStretchDist > ARM_STRETCH_THRESHOULD)	//rule for touch event toggling
		{
			TargetCoordinate = outputPnt;

			cv::Rect ROI;
			if (GetUnboundedROI(ROI, cvRHand, (int)(51.200 / m_aBufferedJoints[JointType_HandRight].Position.Z), KINECT_WIDTH, KINECT_HEIGHT))	//handROI
			{
				try
				{
					cv::Point handTop;
					int nHandTop;
					if (findTop(handTop, nHandTop, &depthmap, ROI))
					{
						thresh_low = DEPTH_THRESHOLD_MAXVAL - ((nHandTop + HAND_DEPTH_RANGE));
						thresh_high = DEPTH_THRESHOLD_MAXVAL - ((nHandTop - HAND_DEPTH_RANGE));
					}
					cv::rectangle(outputMat, ROI, cv::Scalar(0, 255, 0), 2);
				}
				catch (...)
				{
					System::Media::SystemSound ^sound = System::Media::SystemSounds::Exclamation;
					sound->Play();
				}
			}

			functionRes = true;
		} // if (armStretchDist > ARM_STRETCH_THRESHOULD)

	} // bSkelSufficient == true


	outputMat.copyTo(outputMatBuffer);
	depthmask.copyTo(depthmaskBuffer);

	framecount ++;
	if(framecount>=5){	//count every 5 frames
		curtime = clock();
		fps = 5.0 / ((double)(curtime - pretime) / CLOCKS_PER_SEC);
		framecount=0;
	}

	return functionRes;
}


HRESULT HandTracker::InitSensor()
{
	HRESULT hr = GetDefaultKinectSensor(&m_pSensor);
	if (FAILED(hr))
	{
		return hr;
	}

	if (m_pSensor)
	{
		IDepthFrameSource* pDepthFrameSource = NULL;
		IBodyFrameSource* pBodyFrameSource = NULL;
		hr = m_pSensor->Open();

		if (SUCCEEDED(hr))
		{
			hr = m_pSensor->get_DepthFrameSource(&pDepthFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pDepthFrameSource->OpenReader(&m_pDepthFrameReader);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pSensor->get_CoordinateMapper(&m_pCoordinateMapper);
		}

		if (SUCCEEDED(hr))
		{
			hr = m_pSensor->get_BodyFrameSource(&pBodyFrameSource);
		}

		if (SUCCEEDED(hr))
		{
			hr = pBodyFrameSource->OpenReader(&m_pBodyFrameReader);
		}

		SafeRelease(pDepthFrameSource);
		SafeRelease(pBodyFrameSource);
	}

	if (!m_pSensor || FAILED(hr))
	{
		return E_FAIL;
	}

	return hr;
}






//----------------------------------------------------------------
//	functions
//----------------------------------------------------------------

// find the center of the largest area 
cv::Point findMaxAreaCoordinate( vector<vector<cv::Point> > contours, double &maxArea, UINT &maxidx ) {
	maxidx = contours.size();
	maxArea = 0;
	long max_x = 0;
	long max_y = 0;
	double currentArea = 0;
	UINT idx;

	for(idx = 0; idx < contours.size(); idx++){
		if((currentArea = cv::contourArea((cv::Mat)contours.at(idx)) ) > maxArea){
			maxArea = currentArea;
			maxidx = idx;
		}
	}
	if( maxidx < contours.size() ){
		for(idx = 0; idx < contours.at(maxidx).size(); idx++){
			max_x += contours.at(maxidx).at(idx).x;
			max_y += contours.at(maxidx).at(idx).y;
		}
		max_x = max_x / contours.at(maxidx).size();
		max_y = max_y / contours.at(maxidx).size();
	}

	return cv::Point(max_x, max_y);
}


// - Laser Thread Func. --------------
void LaserProcess(void *dummy){
	while(flag){
		cv::Point laserCoordinate;

		if(lt.findLaserCoordinate(laserCoordinate)){
			//Send through UDP
			//Format: Byte(type) + Int32 + Int32 + double(8 bytes)
			char sendBuffer[10];
			{
				Byte *bytePtr;
				Int32 i32_x = (Int32)laserCoordinate.x;
				Int32 i32_y = (Int32)laserCoordinate.y;
				ZeroMemory(sendBuffer, sizeof(sendBuffer));

				sendBuffer[0] = 0x0;
				sendBuffer[1] = true;
				bytePtr = (Byte *)&i32_x;
				sendBuffer[2] = bytePtr[0];
				sendBuffer[3] = bytePtr[1];
				sendBuffer[4] = bytePtr[2];
				sendBuffer[5] = bytePtr[3];
				bytePtr = (Byte *)&i32_y;
				sendBuffer[6] = bytePtr[0];
				sendBuffer[7] = bytePtr[1];
				sendBuffer[8] = bytePtr[2];
				sendBuffer[9] = bytePtr[3];
			}
			sendto(sendSock, sendBuffer, sizeof(sendBuffer), 0, (sockaddr*)&addr, sizeof(addr));
		}//if
		else{
			char sendBuffer[2];
			{
				ZeroMemory(sendBuffer, sizeof(sendBuffer));

				sendBuffer[0] = 0x0;
				sendBuffer[1] = false;
			}
			sendto(sendSock, sendBuffer, sizeof(sendBuffer), 0, (sockaddr*)&addr, sizeof(addr));
		}
	}//while
}


// - Hand Thread Func. --------------
void HandProcess(void *dummy){
	Sleep(1000);
	while(flag){
		cv::Point palmCoordinate = cv::Point(0, 0);
		int mouseLMB = 0;

		if( ht.findHandCoordinate(palmCoordinate, mouseLMB) ){
			//Sending movement through UDP
			//Format: Byte(type) + Int32 + Int32
			char sendBuffer[10];
			{
				Byte *bytePtr;
				Int32 i32_x = (Int32)palmCoordinate.x;
				Int32 i32_y = (Int32)palmCoordinate.y;
				ZeroMemory(sendBuffer, sizeof(sendBuffer));

				sendBuffer[0] = 0x1;
				sendBuffer[1] = true;
				bytePtr = (Byte *)&i32_x;
				sendBuffer[2] = bytePtr[0];
				sendBuffer[3] = bytePtr[1];
				sendBuffer[4] = bytePtr[2];
				sendBuffer[5] = bytePtr[3];
				bytePtr = (Byte *)&i32_y;
				sendBuffer[6] = bytePtr[0];
				sendBuffer[7] = bytePtr[1];
				sendBuffer[8] = bytePtr[2];
				sendBuffer[9] = bytePtr[3];
			}
			sendto(sendSock, sendBuffer, sizeof(sendBuffer), 0, (sockaddr*)&addr, sizeof(addr));

			// - Left mouse button function --------------
			// Send through UDP
			// Format: Byte(type) + bool
			char mouseEventBuffer[2];
			switch(mouseLMB){
					case 0:
						break;
					case 1:
						mouseEventBuffer[0] = 0x2;
						mouseEventBuffer[1] = true;
						sendto(sendSock, mouseEventBuffer, sizeof(mouseEventBuffer), 0, (sockaddr*)&addr, sizeof(addr));
						break;
					case 2:
						mouseEventBuffer[0] = 0x2;
						mouseEventBuffer[1] = false;
						sendto(sendSock, mouseEventBuffer, sizeof(mouseEventBuffer), 0, (sockaddr*)&addr, sizeof(addr));
						break;
					default:
						break;

			}
			// - -----------------------------------------
		}
		else{
			char sendBuffer[2];
			{
				ZeroMemory(sendBuffer, sizeof(sendBuffer));

				sendBuffer[0] = 0x1;
				sendBuffer[1] = false;
			}
			sendto(sendSock, sendBuffer, sizeof(sendBuffer), 0, (sockaddr*)&addr, sizeof(addr));


			if(ht.mousedownflag != false)
			{
				ht.mousedownflag = false;

				// Send through UDP
				// Format: Byte(type) + bool
				char mouseEventBuffer[2] = { 0x2, false };
				sendto(sendSock, mouseEventBuffer, sizeof(mouseEventBuffer), 0, (sockaddr*)&addr, sizeof(addr));
			}
		}
	}

	//ht.SkeletonPurge();
}