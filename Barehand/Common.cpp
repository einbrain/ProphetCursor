#include "stdafx.h"
#include "sdks.h"


bool GetUnboundedROI(cv::Rect& ROI, cv::Point center, int rectRadius, int camWidth, int camHeight){
	int rectLeft = center.x - rectRadius;
	int rectTop = center.y - rectRadius;
	int rectWidth = rectRadius * 2 + 1;
	int rectHeight = rectRadius * 2 + 1;

	if(rectLeft < 1){
		if(rectLeft + rectRadius < 1){
			rectLeft = 1;
			rectWidth = 0;
		}
		else{
			rectWidth = rectRadius * 2 + 1 - (1 - rectLeft);
			rectLeft = 1;
		}
	}
	if(rectTop < 1){
		if( rectHeight + rectRadius < 1){
			rectTop = 1;
			rectHeight = 0;
		}
		else{
			rectHeight = rectRadius * 2 + 1 - (1 - rectTop);
			rectTop = 1;
		}
	}
	if(rectLeft + rectWidth > camWidth + 1){
		if( rectLeft > camWidth ) rectWidth = 0;
		else rectWidth = rectRadius * 2 - (rectLeft + rectWidth - camWidth - 1);
	}
	if(rectTop + rectHeight > camHeight + 1){
		if(rectTop > camHeight ) rectHeight = 0;
		else rectHeight = rectRadius * 2 - (rectTop + rectHeight - camHeight - 1);
	}

	if(rectHeight > 0 && rectWidth > 0){
		ROI = cv::Rect(rectLeft, rectTop, rectWidth, rectHeight);
		return true;
	}
	else{
		return false;
	}
}


bool findTop(cv::Point& top, int& topVal, cv::Mat* src, cv::Rect rect){
	cv::Point res(0, 0);
	int x = 0, y = 0;
	bool bFound = false;

	topVal = 65535;

	if(!src->empty())
	for(int i = rect.tl().y; i < rect.br().y; ++i){
		for(int j = rect.tl().x; j < rect.br().x; ++j){
			{
				Int16 curVarVec = Convert::ToInt16((src->at<Int16>(cv::Point(j, i))) * 255.0f / 8191.0f);
				if(curVarVec < topVal && curVarVec > 0)
				{
					topVal = curVarVec;
					x = j;
					y = i;
					bFound = true;
				}
			}
		}
	}
	//ht.nNose = src->at<Int16>(0,0);
	if(bFound) top = cv::Point(x,y);

	return bFound;
}

bool checkAreaInc(Queue^ queue){
	array<double>^ copy = gcnew array<double>(queue->Count);
	queue->CopyTo(copy, 0);
	bool flag = false;
	if (copy->Length > 0){
		flag = true;
		for (int i = 1; i < queue->Count; i++){
			if (copy[i] <= copy[i - 1]) flag = false;
		}
		if (copy[copy->Length - 1] - copy[0] < 0.05) flag = false;
	}

	return flag;
}

bool checkAreaDec(Queue^ queue){
	array<double>^ copy = gcnew array<double>(queue->Count);
	queue->CopyTo(copy, 0);
	bool flag = false;
	if (copy->Length > 0){
		flag = true;
		for (int i = 1; i < queue->Count; i++){
			if (copy[i] >= copy[i - 1]) flag = false;
		}
		if (copy[copy->Length - 1] - copy[0] > -0.05) flag = false;
	}

	return flag;
}

double shapeComplexity(vector<cv::Point> &contour){
	long varX = 0, difX = 0;
	long varY = 0, difY = 0;
	int size = contour.size();
	for (UINT i = 0; i < contour.size(); i++){
		varX += contour.at(i).x;
		varY += contour.at(i).y;
	}
	for (UINT i = 0; i < contour.size(); i++){
		difX += Convert::ToInt32(pow((double)(contour.at(i).x - varX), 2.0));
		difY += Convert::ToInt32(pow((double)(contour.at(i).y - varY), 2.0));
	}

	return ((double)difX + (double)difY) / (double)contour.size();

}




// 3D coordinator process


double PointToLine3D(CameraSpacePoint a, CameraSpacePoint b, CameraSpacePoint p){
 	double res;
	double f1, f2;
	Vector3D 
		A = Vector3D(a), 
		B = Vector3D(b), 
		P = Vector3D(p);

	f1 = ((P - A)||(P - B)).Length();
	f2 = (B-A).Length();
	if (f2)
	{
		res = f1 / f2;
	}
	else res = 0;

	return res;
}

Vector3D CreatePolygonNormal(Vertex3D A, Vertex3D B, Vertex3D C)
{
	Vector3D AB(B - A);
	Vector3D BC(C - B);

	Vector3D normal = AB||BC;
	normal.Normalize();

	return normal;
}

double PointToPlane3D(CameraSpacePoint a, CameraSpacePoint b, CameraSpacePoint c, CameraSpacePoint p)
//a,b,c: 3 points that form a plane
//p: the point to be measured
{
	double res;
	Vector3D f1, f2, f3;
	Vector3D
		A = Vector3D(a),
		B = Vector3D(b),
		C = Vector3D(c),
		P = Vector3D(p);

	f1 = (B - A) || (C - A);
	if (f1.Length())
	{
		f2 = f1 < (1 / f1.Length());
	}
	else f2 = Vector3D(0, 0, 0);
	f3 = P - A;
	res = f2*f3;

	return res;
}

Vector3D TransformToUVNCam(Vector3D P, Vector3D N, Vector3D BaseP)
//Transform world point P to a UVN-Cam plane
//P: Point to be transformed
//N: Plane normal (Target axis)
//BaseP: Base point in the plane
{
	//Use solved equations
	Vector3D U = Vector3D(-N.z, 0, N.x);	//Right-Hand axis , in the x-z plane
	U.Normalize();

	Vector3D V = Vector3D(
		-N.x*N.y*(N.z + 1),
		N.x*N.x + N.z*N.z - N.x*N.y*N.z,
		-N.y*N.z);							//Over-Head axis , solved by rotating P around U by 90 degree
	V.Normalize();

	// Transform: first translate then rotate
	P.x -= BaseP.x;
	P.y -= BaseP.y;
	P.z -= BaseP.z;
	return Vector3D(
		U.x*P.x + U.y*P.y + U.z*P.z,
		V.x*P.x + V.y*P.y + V.z*P.z,
		N.x*P.x + N.y*P.y + N.z*P.z);
}


CameraSpacePoint CSPointAverage(std::deque<CameraSpacePoint> pntQueue)
{
	float x = 0, y = 0, z = 0;

	for (int i = 0; i < pntQueue.size(); ++i)
	{
		x += pntQueue.at(i).X;
		y += pntQueue.at(i).Y;
		z += pntQueue.at(i).Z;
	}
	x /= pntQueue.size();
	y /= pntQueue.size();
	z /= pntQueue.size();

	CameraSpacePoint res;
	res.X = x;
	res.Y = y;
	res.Z = z;

	return res;
}