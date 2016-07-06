#pragma once


namespace Barehand {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for Form1
	///
	/// WARNING: If you change the name of this class, you will need to change the
	///          'Resource File Name' property for the managed resource compiler tool
	///          associated with all .resx files this class depends on.  Otherwise,
	///          the designers will not be able to interact properly with localized
	///          resources associated with this form.
	/// </summary>
	public ref class Form1 : public System::Windows::Forms::Form
	{
	public:
		Form1(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~Form1()
		{
			if (components)
			{
				delete components;
			}
		}


	protected: 

	protected: 


	private: System::Windows::Forms::Button^  button_start;


	private: System::Windows::Forms::TrackBar^  trackBar_thresh;



	private: System::Windows::Forms::Label^  label4;



	private: System::Windows::Forms::Label^  label_thresh;
	private: System::Windows::Forms::Button^  button_reset;
	private: System::Windows::Forms::GroupBox^  groupBox1;
	private: System::Windows::Forms::TrackBar^  trackBar_camthresh;
	private: System::Windows::Forms::Label^  label5;
	private: System::Windows::Forms::Label^  label_camthresh;


	private: System::Windows::Forms::Button^  button_bgrefresh;






	private: System::Windows::Forms::Button^  button_rb;
	private: System::Windows::Forms::Button^  button_lb;
	private: System::Windows::Forms::Button^  button_rt;
	private: System::Windows::Forms::Button^  button_lt;




	private: System::Windows::Forms::Button^  button_clear;
	private: System::Windows::Forms::PictureBox^  pictureBox1;
	private: System::Windows::Forms::PictureBox^  pictureBox2;






	private: System::Windows::Forms::StatusStrip^  statusStrip1;

	private: System::Windows::Forms::ComboBox^  comboBox1;
	private: System::Windows::Forms::TextBox^  textBox1;



	private: System::Windows::Forms::PictureBox^  pictureBox3;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel2;
	private: System::Windows::Forms::ToolStripStatusLabel^  toolStripStatusLabel3;
	private: System::Windows::Forms::PictureBox^  pictureBox4;
	private: System::Windows::Forms::Timer^  timer1;
	private: System::Windows::Forms::Label^  labelSkeletonConsole;
	private: System::Windows::Forms::TextBox^  textBox2;




	private: System::ComponentModel::IContainer^  components;








	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>


#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->components = (gcnew System::ComponentModel::Container());
			this->button_start = (gcnew System::Windows::Forms::Button());
			this->trackBar_thresh = (gcnew System::Windows::Forms::TrackBar());
			this->label4 = (gcnew System::Windows::Forms::Label());
			this->label_thresh = (gcnew System::Windows::Forms::Label());
			this->button_reset = (gcnew System::Windows::Forms::Button());
			this->groupBox1 = (gcnew System::Windows::Forms::GroupBox());
			this->button_clear = (gcnew System::Windows::Forms::Button());
			this->button_rb = (gcnew System::Windows::Forms::Button());
			this->button_lb = (gcnew System::Windows::Forms::Button());
			this->button_rt = (gcnew System::Windows::Forms::Button());
			this->button_lt = (gcnew System::Windows::Forms::Button());
			this->button_bgrefresh = (gcnew System::Windows::Forms::Button());
			this->trackBar_camthresh = (gcnew System::Windows::Forms::TrackBar());
			this->label5 = (gcnew System::Windows::Forms::Label());
			this->label_camthresh = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox2 = (gcnew System::Windows::Forms::PictureBox());
			this->statusStrip1 = (gcnew System::Windows::Forms::StatusStrip());
			this->toolStripStatusLabel2 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->toolStripStatusLabel3 = (gcnew System::Windows::Forms::ToolStripStatusLabel());
			this->comboBox1 = (gcnew System::Windows::Forms::ComboBox());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->pictureBox3 = (gcnew System::Windows::Forms::PictureBox());
			this->pictureBox4 = (gcnew System::Windows::Forms::PictureBox());
			this->timer1 = (gcnew System::Windows::Forms::Timer(this->components));
			this->labelSkeletonConsole = (gcnew System::Windows::Forms::Label());
			this->textBox2 = (gcnew System::Windows::Forms::TextBox());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar_thresh))->BeginInit();
			this->groupBox1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar_camthresh))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->BeginInit();
			this->statusStrip1->SuspendLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->BeginInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->BeginInit();
			this->SuspendLayout();
			// 
			// button_start
			// 
			this->button_start->Location = System::Drawing::Point(24, 430);
			this->button_start->Name = L"button_start";
			this->button_start->Size = System::Drawing::Size(206, 92);
			this->button_start->TabIndex = 3;
			this->button_start->Text = L"Start";
			this->button_start->UseVisualStyleBackColor = true;
			this->button_start->Click += gcnew System::EventHandler(this, &Form1::button_start_Click);
			// 
			// trackBar_thresh
			// 
			this->trackBar_thresh->Location = System::Drawing::Point(24, 297);
			this->trackBar_thresh->Maximum = 255;
			this->trackBar_thresh->Name = L"trackBar_thresh";
			this->trackBar_thresh->Size = System::Drawing::Size(301, 45);
			this->trackBar_thresh->TabIndex = 4;
			this->trackBar_thresh->TickFrequency = 8;
			this->trackBar_thresh->Scroll += gcnew System::EventHandler(this, &Form1::trackBar_thresh_Scroll);
			// 
			// label4
			// 
			this->label4->AutoSize = true;
			this->label4->Location = System::Drawing::Point(34, 327);
			this->label4->Name = L"label4";
			this->label4->Size = System::Drawing::Size(98, 12);
			this->label4->TabIndex = 8;
			this->label4->Text = L"threshold (depth) :";
			// 
			// label_thresh
			// 
			this->label_thresh->AutoSize = true;
			this->label_thresh->Location = System::Drawing::Point(169, 327);
			this->label_thresh->Name = L"label_thresh";
			this->label_thresh->Size = System::Drawing::Size(11, 12);
			this->label_thresh->TabIndex = 12;
			this->label_thresh->Text = L"0";
			// 
			// button_reset
			// 
			this->button_reset->Location = System::Drawing::Point(236, 430);
			this->button_reset->Name = L"button_reset";
			this->button_reset->Size = System::Drawing::Size(89, 92);
			this->button_reset->TabIndex = 13;
			this->button_reset->Text = L"Restore default";
			this->button_reset->UseVisualStyleBackColor = true;
			this->button_reset->Click += gcnew System::EventHandler(this, &Form1::button_reset_Click);
			// 
			// groupBox1
			// 
			this->groupBox1->Controls->Add(this->button_clear);
			this->groupBox1->Controls->Add(this->button_rb);
			this->groupBox1->Controls->Add(this->button_lb);
			this->groupBox1->Controls->Add(this->button_rt);
			this->groupBox1->Controls->Add(this->button_lt);
			this->groupBox1->Controls->Add(this->button_bgrefresh);
			this->groupBox1->Location = System::Drawing::Point(12, 12);
			this->groupBox1->Name = L"groupBox1";
			this->groupBox1->Size = System::Drawing::Size(329, 251);
			this->groupBox1->TabIndex = 14;
			this->groupBox1->TabStop = false;
			this->groupBox1->Text = L"Offsets";
			// 
			// button_clear
			// 
			this->button_clear->Location = System::Drawing::Point(145, 156);
			this->button_clear->Name = L"button_clear";
			this->button_clear->Size = System::Drawing::Size(130, 48);
			this->button_clear->TabIndex = 28;
			this->button_clear->Text = L"Clear";
			this->button_clear->UseVisualStyleBackColor = true;
			this->button_clear->Click += gcnew System::EventHandler(this, &Form1::button_clear_Click);
			// 
			// button_rb
			// 
			this->button_rb->Location = System::Drawing::Point(213, 101);
			this->button_rb->Name = L"button_rb";
			this->button_rb->Size = System::Drawing::Size(62, 49);
			this->button_rb->TabIndex = 23;
			this->button_rb->Text = L"RB";
			this->button_rb->UseVisualStyleBackColor = true;
			this->button_rb->Click += gcnew System::EventHandler(this, &Form1::button_rb_Click);
			// 
			// button_lb
			// 
			this->button_lb->Location = System::Drawing::Point(145, 101);
			this->button_lb->Name = L"button_lb";
			this->button_lb->Size = System::Drawing::Size(62, 49);
			this->button_lb->TabIndex = 22;
			this->button_lb->Text = L"LB";
			this->button_lb->UseVisualStyleBackColor = true;
			this->button_lb->Click += gcnew System::EventHandler(this, &Form1::button_lb_Click);
			// 
			// button_rt
			// 
			this->button_rt->Location = System::Drawing::Point(213, 46);
			this->button_rt->Name = L"button_rt";
			this->button_rt->Size = System::Drawing::Size(62, 49);
			this->button_rt->TabIndex = 21;
			this->button_rt->Text = L"RT";
			this->button_rt->UseVisualStyleBackColor = true;
			this->button_rt->Click += gcnew System::EventHandler(this, &Form1::button_rt_Click);
			// 
			// button_lt
			// 
			this->button_lt->Location = System::Drawing::Point(145, 46);
			this->button_lt->Name = L"button_lt";
			this->button_lt->Size = System::Drawing::Size(62, 49);
			this->button_lt->TabIndex = 20;
			this->button_lt->Text = L"LT";
			this->button_lt->UseVisualStyleBackColor = true;
			this->button_lt->Click += gcnew System::EventHandler(this, &Form1::button_lt_Click);
			// 
			// button_bgrefresh
			// 
			this->button_bgrefresh->Location = System::Drawing::Point(47, 46);
			this->button_bgrefresh->Name = L"button_bgrefresh";
			this->button_bgrefresh->Size = System::Drawing::Size(90, 158);
			this->button_bgrefresh->TabIndex = 13;
			this->button_bgrefresh->Text = L"Manual BG Refresh";
			this->button_bgrefresh->UseVisualStyleBackColor = true;
			this->button_bgrefresh->Click += gcnew System::EventHandler(this, &Form1::button_bgrefresh_Click);
			// 
			// trackBar_camthresh
			// 
			this->trackBar_camthresh->Location = System::Drawing::Point(24, 342);
			this->trackBar_camthresh->Maximum = 255;
			this->trackBar_camthresh->Name = L"trackBar_camthresh";
			this->trackBar_camthresh->Size = System::Drawing::Size(301, 45);
			this->trackBar_camthresh->TabIndex = 15;
			this->trackBar_camthresh->TickFrequency = 5;
			this->trackBar_camthresh->Scroll += gcnew System::EventHandler(this, &Form1::trackBar_camthresh_Scroll);
			// 
			// label5
			// 
			this->label5->AutoSize = true;
			this->label5->Location = System::Drawing::Point(34, 372);
			this->label5->Name = L"label5";
			this->label5->Size = System::Drawing::Size(117, 12);
			this->label5->TabIndex = 16;
			this->label5->Text = L"cam threshold (gray) :";
			// 
			// label_camthresh
			// 
			this->label_camthresh->AutoSize = true;
			this->label_camthresh->Location = System::Drawing::Point(169, 372);
			this->label_camthresh->Name = L"label_camthresh";
			this->label_camthresh->Size = System::Drawing::Size(11, 12);
			this->label_camthresh->TabIndex = 17;
			this->label_camthresh->Text = L"0";
			// 
			// pictureBox1
			// 
			this->pictureBox1->BackColor = System::Drawing::Color::Black;
			this->pictureBox1->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pictureBox1->Location = System::Drawing::Point(347, 12);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(352, 261);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 18;
			this->pictureBox1->TabStop = false;
			// 
			// pictureBox2
			// 
			this->pictureBox2->BackColor = System::Drawing::Color::Black;
			this->pictureBox2->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pictureBox2->Location = System::Drawing::Point(705, 12);
			this->pictureBox2->Name = L"pictureBox2";
			this->pictureBox2->Size = System::Drawing::Size(352, 261);
			this->pictureBox2->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox2->TabIndex = 19;
			this->pictureBox2->TabStop = false;
			// 
			// statusStrip1
			// 
			this->statusStrip1->Items->AddRange(gcnew cli::array< System::Windows::Forms::ToolStripItem^  >(2) {
				this->toolStripStatusLabel2,
					this->toolStripStatusLabel3
			});
			this->statusStrip1->Location = System::Drawing::Point(0, 591);
			this->statusStrip1->Name = L"statusStrip1";
			this->statusStrip1->Size = System::Drawing::Size(1068, 22);
			this->statusStrip1->SizingGrip = false;
			this->statusStrip1->TabIndex = 24;
			this->statusStrip1->Text = L"statusStrip1";
			// 
			// toolStripStatusLabel2
			// 
			this->toolStripStatusLabel2->Name = L"toolStripStatusLabel2";
			this->toolStripStatusLabel2->RightToLeft = System::Windows::Forms::RightToLeft::No;
			this->toolStripStatusLabel2->Size = System::Drawing::Size(34, 17);
			this->toolStripStatusLabel2->Text = L"Laser";
			// 
			// toolStripStatusLabel3
			// 
			this->toolStripStatusLabel3->Name = L"toolStripStatusLabel3";
			this->toolStripStatusLabel3->Size = System::Drawing::Size(36, 17);
			this->toolStripStatusLabel3->Text = L"Hand";
			// 
			// comboBox1
			// 
			this->comboBox1->FormattingEnabled = true;
			this->comboBox1->Items->AddRange(gcnew cli::array< System::Object^  >(3) { L"127.0.0.1", L"172.16.2.102", L"172.16.2.120" });
			this->comboBox1->Location = System::Drawing::Point(24, 404);
			this->comboBox1->Name = L"comboBox1";
			this->comboBox1->Size = System::Drawing::Size(239, 20);
			this->comboBox1->TabIndex = 25;
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(269, 405);
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(56, 19);
			this->textBox1->TabIndex = 26;
			this->textBox1->Text = L"8888";
			// 
			// pictureBox3
			// 
			this->pictureBox3->BackColor = System::Drawing::Color::Black;
			this->pictureBox3->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pictureBox3->Location = System::Drawing::Point(347, 279);
			this->pictureBox3->Name = L"pictureBox3";
			this->pictureBox3->Size = System::Drawing::Size(352, 261);
			this->pictureBox3->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox3->TabIndex = 30;
			this->pictureBox3->TabStop = false;
			// 
			// pictureBox4
			// 
			this->pictureBox4->BackColor = System::Drawing::Color::Black;
			this->pictureBox4->BackgroundImageLayout = System::Windows::Forms::ImageLayout::Zoom;
			this->pictureBox4->Location = System::Drawing::Point(705, 279);
			this->pictureBox4->Name = L"pictureBox4";
			this->pictureBox4->Size = System::Drawing::Size(352, 261);
			this->pictureBox4->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox4->TabIndex = 31;
			this->pictureBox4->TabStop = false;
			// 
			// timer1
			// 
			this->timer1->Tick += gcnew System::EventHandler(this, &Form1::timer1_Tick);
			// 
			// labelSkeletonConsole
			// 
			this->labelSkeletonConsole->AutoSize = true;
			this->labelSkeletonConsole->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 18, System::Drawing::FontStyle::Regular, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->labelSkeletonConsole->Location = System::Drawing::Point(20, 553);
			this->labelSkeletonConsole->Name = L"labelSkeletonConsole";
			this->labelSkeletonConsole->Size = System::Drawing::Size(172, 24);
			this->labelSkeletonConsole->TabIndex = 32;
			this->labelSkeletonConsole->Text = L"Skeleton Output";
			// 
			// textBox2
			// 
			this->textBox2->Font = (gcnew System::Drawing::Font(L"MS UI Gothic", 15.75F, System::Drawing::FontStyle::Bold, System::Drawing::GraphicsUnit::Point,
				static_cast<System::Byte>(128)));
			this->textBox2->Location = System::Drawing::Point(492, 549);
			this->textBox2->Name = L"textBox2";
			this->textBox2->Size = System::Drawing::Size(564, 28);
			this->textBox2->TabIndex = 33;
			// 
			// Form1
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 12);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(1068, 613);
			this->Controls->Add(this->textBox2);
			this->Controls->Add(this->labelSkeletonConsole);
			this->Controls->Add(this->pictureBox4);
			this->Controls->Add(this->pictureBox3);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->comboBox1);
			this->Controls->Add(this->statusStrip1);
			this->Controls->Add(this->pictureBox2);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label_camthresh);
			this->Controls->Add(this->label5);
			this->Controls->Add(this->trackBar_camthresh);
			this->Controls->Add(this->groupBox1);
			this->Controls->Add(this->button_reset);
			this->Controls->Add(this->label_thresh);
			this->Controls->Add(this->label4);
			this->Controls->Add(this->trackBar_thresh);
			this->Controls->Add(this->button_start);
			this->FormBorderStyle = System::Windows::Forms::FormBorderStyle::FixedSingle;
			this->MaximizeBox = false;
			this->Name = L"Form1";
			this->Text = L"Control Panel - Prophet Cursor";
			this->FormClosing += gcnew System::Windows::Forms::FormClosingEventHandler(this, &Form1::Form1_FormClosing);
			this->Load += gcnew System::EventHandler(this, &Form1::Form1_Load);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar_thresh))->EndInit();
			this->groupBox1->ResumeLayout(false);
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->trackBar_camthresh))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox2))->EndInit();
			this->statusStrip1->ResumeLayout(false);
			this->statusStrip1->PerformLayout();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox3))->EndInit();
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox4))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::String ^formTitle;
	private: System::Void Form1_Load(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button_reset_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button_start_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void Form1_FormClosing(System::Object^  sender, System::Windows::Forms::FormClosingEventArgs^  e);
	private: System::Void button_bgrefresh_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button_lt_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button_lb_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button_rt_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button_rb_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void button_clear_Click(System::Object^  sender, System::EventArgs^  e);
	private: System::Void trackBar_camthresh_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void trackBar_thresh_Scroll(System::Object^  sender, System::EventArgs^  e);
	private: System::Void timer1_Tick(System::Object^  sender, System::EventArgs^  e);
};
}

