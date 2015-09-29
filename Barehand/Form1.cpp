#include "stdafx.h"
#include "sdks.h"


void LaserProcess(void *dummy);
void HandProcess(void *dummy);
double PointToLine3D(cv::Point3d a, cv::Point3d b, cv::Point3d p);


//-----------------------------------------------------------------------------------
//	Initialization
//-----------------------------------------------------------------------------------
System::Void Form1::Form1_Load(System::Object^  sender, System::EventArgs^  e)
{
	if (FAILED(ht.hSensorInitResult)){
		MessageBox::Show("Kinect init failed.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		Form1::Close();
	}


	if(!lt.capture.isOpened()){
		MessageBox::Show("No conventional camera found.", "Error", MessageBoxButtons::OK, MessageBoxIcon::Exclamation);
		Form1::Close();
	}

	trackBar_thresh->Value = ht.thresh_low;
	trackBar_camthresh->Value = lt.thresh;
	label_thresh->Text = ht.thresh_low.ToString();
	label_camthresh->Text = lt.thresh.ToString();

	timer1->Interval = 1000; 
	timer1->Start();

	formTitle = Form1::Text;

	this->Focus();
}


//-----------------------------------------------------------------------------------
//	Clearing
//-----------------------------------------------------------------------------------
System::Void Form1::Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e)
{
	flag = false;
}


//-----------------------------------------------------------------------------------
//	Reset to default threshold
//-----------------------------------------------------------------------------------
System::Void Form1::button_reset_Click(System::Object^  sender, System::EventArgs^  e)
{
	trackBar_thresh->Value = ht.DEPTH_THRESHOLD_LOW;
	ht.thresh_low = trackBar_thresh->Value;
	label_thresh->Text = ht.thresh_low.ToString();

	trackBar_camthresh->Value = lt.CAM_THRESHOLD;
	lt.thresh = trackBar_camthresh->Value;
	label_camthresh->Text = lt.thresh.ToString();

	textBox1->Text = "8888";
}


//-----------------------------------------------------------------------------------
//	Refresh background
//-----------------------------------------------------------------------------------
System::Void Form1::button_bgrefresh_Click(System::Object^  sender, System::EventArgs^  e)
{
	lt.refreshBG();
}


System::Void Form1::timer1_Tick(System::Object^  sender, System::EventArgs^  e) 
{

}


//-----------------------------------------------------------------------------------
//	Perspective calibration
//-----------------------------------------------------------------------------------
System::Void Form1::button_lt_Click(System::Object^  sender, System::EventArgs^  e) {
	lt.setCorners(0);
	button_lt->Text = System::String::Format("({0},{1})", lt.screenLT.x, lt.screenLT.y);
}
System::Void Form1::button_lb_Click(System::Object^  sender, System::EventArgs^  e) {
	lt.setCorners(1);
	button_lb->Text = System::String::Format("({0},{1})", lt.screenLB.x, lt.screenLB.y);
}
System::Void Form1::button_rt_Click(System::Object^  sender, System::EventArgs^  e) {
	lt.setCorners(2);
	button_rt->Text = System::String::Format("({0},{1})", lt.screenRT.x, lt.screenRT.y);
}
System::Void Form1::button_rb_Click(System::Object^  sender, System::EventArgs^  e) {
	lt.setCorners(3);
	button_rb->Text = System::String::Format("({0},{1})", lt.screenRB.x, lt.screenRB.y);
}
System::Void Form1::button_clear_Click(System::Object^  sender, System::EventArgs^  e){
	lt.clearCorners();
	button_lt->Text = "LT";
	button_lb->Text = "LB";
	button_rt->Text = "RT";
	button_rb->Text = "RB";
}

System::Void Form1::trackBar_camthresh_Scroll(System::Object^  sender, System::EventArgs^  e){
	lt.thresh = trackBar_camthresh->Value;
	label_camthresh->Text = lt.thresh.ToString();
}

System::Void Form1::trackBar_thresh_Scroll(System::Object^  sender, System::EventArgs^  e){
	ht.thresh_low = trackBar_thresh->Value;
	label_thresh->Text = ht.thresh_low.ToString();
}

//-----------------------------------------------------------------------------------
//	Start hand tracking main loop
//-----------------------------------------------------------------------------------
System::Void Form1::button_start_Click(System::Object^  sender, System::EventArgs^  e){
	if(flag == true){
		flag = false;
		button_start->BackColor = System::Drawing::Color::Gray;
		button_start->Text= "Paused";
		return;
	}
	if(System::String::IsNullOrEmpty(comboBox1->Text) || System::String::IsNullOrEmpty(textBox1->Text)){
		return;
	}
	flag = true;
	button_start->BackColor = System::Drawing::Color::Cyan;
	button_start->Text= "Running";

	// - Socket Initialize --------------------
	//SOCKET sendSock;
	WSADATA wsaData;
	WORD sockVersion = MAKEWORD(2,2);
	WSAStartup(sockVersion, &wsaData);
	sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

	char stringToCharAster[256];
	ZeroMemory(stringToCharAster,sizeof(stringToCharAster));
	sprintf_s(stringToCharAster, sizeof(stringToCharAster), "%s", comboBox1->Text);

	//sockaddr_in addr;
	addr.sin_family = AF_INET;
	addr.sin_port = htons(Convert::ToInt32(textBox1->Text));
	addr.sin_addr.S_un.S_addr = inet_addr(stringToCharAster);

	HANDLE hThread[2];

	hThread[0] = (HANDLE)_beginthread(LaserProcess, 0, NULL);
	hThread[1] = (HANDLE)_beginthread(HandProcess, 0, NULL);
	
	clock_t pretime, curtime;
	short framecount = 0;
	double fps = 0;


	// - Main Loop ---------------------------------
	while(flag)
	{
		if(framecount == 0){
			pretime = clock();
		}

		labelSkeletonConsole->Text = "Arm Stretch: "+ht.armStretchDist.ToString("F4")+", Kinect Tick: "+ht.m_lSkeletonRecongCnt.ToString();


		//textBox2->Text = System::String::Format("{0},{1},{2}; {3},{4},{5}; {6},{7},{8}:    {9}", 
		//										(int)ht.monitor1.x, (int)ht.monitor1.y, (int)ht.monitor1.z, 
		//										(int)ht.monitor2.x, (int)ht.monitor2.y, (int)ht.monitor2.z, 
		//										(int)ht.monitor3.x, (int)ht.monitor3.y, (int)ht.monitor3.z, 
		//										ht.armStretchDist);
		//textBox2->Text = System::String::Format("{0}", PointToLine3D(cv::Point3d(0,99,0), cv::Point3d(0,0,0), cv::Point3d(1,1,1)));
		//textBox2->Text = System::String::Format("{0}", ht.averageDepth);


		{// - Laser controls -------------
			if(!lt.camframeBuffer.empty()){
				pictureBox1->Image = gcnew Bitmap(lt.camframeBuffer.cols, lt.camframeBuffer.rows, lt.camframeBuffer.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, IntPtr(lt.camframeBuffer.data));
			}

			if(!lt.outputMatBuffer.empty()){
				pictureBox2->Image = gcnew Bitmap(lt.outputMatBuffer.cols, lt.outputMatBuffer.rows, lt.outputMatBuffer.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, IntPtr(lt.outputMatBuffer.data));
			}

			toolStripStatusLabel2->Text = System::String::Format("{0},{1}", lt.outputPnt.x, lt.outputPnt.y);
		}

		{// - Hand controls ---------------
			if(!ht.outputMatBuffer.empty()){
				pictureBox3->Image = gcnew Bitmap(ht.outputMatBuffer.cols, ht.outputMatBuffer.rows, ht.outputMatBuffer.step, System::Drawing::Imaging::PixelFormat::Format24bppRgb, IntPtr(ht.outputMatBuffer.data));
			}
			if(!ht.depthmaskBuffer.empty()){
				pictureBox4->Image = gcnew Bitmap(ht.depthmaskBuffer.cols, ht.depthmaskBuffer.rows, ht.depthmaskBuffer.step, System::Drawing::Imaging::PixelFormat::Format8bppIndexed, IntPtr(ht.depthmaskBuffer.data));
			}
			toolStripStatusLabel3->Text = System::String::Format("{0},{1},{2}", ht.outputPnt.x, ht.outputPnt.y, ht.mousedownflag);
		}

		Sleep(10);

		framecount ++;
		if(framecount >= 5){	//count every 5 frames
			curtime = clock();
			fps = 5.0 / ((double)(curtime - pretime) / CLOCKS_PER_SEC);
			framecount = 0;
		}

		Text = formTitle + System::String::Format(" - [Viewport: \t{0} fps / Laser: \t{1} fps / Kinect: \t{2} fps]", (int)fps, (int)lt.fps, (int)ht.fps);

		Application::DoEvents();
	}//while

}