#include "stdafx.h"
#include "sdks.h"

// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
// NOTE kinect v2: proper releasing of used frame recquired. unhandled frames may stop the body process.
// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!


BOOLEAN HandTracker::GetXthBody(Joint* outJoints, int bodyIndex)
{
	if (m_pBodyFrameReader == NULL)
	{
		return false;
	}

	IBodyFrame* pBodyFrame = NULL;
	BOOLEAN bDataAcqd = false;

	HRESULT hr = m_pBodyFrameReader->AcquireLatestFrame(&pBodyFrame);
	if (SUCCEEDED(hr))
	{
		IBody* ppBodies[BODY_COUNT] = { 0 };
		hr = pBodyFrame->GetAndRefreshBodyData(_countof(ppBodies), ppBodies);

		if (SUCCEEDED(hr))
		{
			// Get ppBodies[bodyIndex]
			// Currently return only the first body
			FLOAT fNearest = FLT_MAX;
			int nNearestIndex = 0;

			for (int i = 0; i < BODY_COUNT; ++i)
			{
				IBody* pBody = ppBodies[i];
				if (pBody)
				{
					BOOLEAN bTracked = false;
					hr = pBody->get_IsTracked(&bTracked);

					if (SUCCEEDED(hr) && bTracked)
					{
						Joint joints[JointType::JointType_Count];
						hr = pBody->GetJoints(_countof(joints), joints);
						if (SUCCEEDED(hr) &&
							joints[JointType_Head].Position.Z < fNearest)
						{
							bDataAcqd = true;
							fNearest = joints[JointType_Head].Position.Z;
							nNearestIndex = i;
						}
					}//if istracked
				}//if got a body
			}//for body count

			if (bDataAcqd)
			{
				//Output
				hr = ppBodies[nNearestIndex]->GetJoints(JointType_Count, outJoints);
			}
		}//if got bodies

		for (int i = 0; i < _countof(ppBodies); ++i)
		{
			SafeRelease(ppBodies[i]);
		}
	}//if acquired frame

	SafeRelease(pBodyFrame);

	if (FAILED(hr) || !bDataAcqd)
	{
		return false;
	}

	ht.m_lSkeletonRecongCnt++;
	return true;
}