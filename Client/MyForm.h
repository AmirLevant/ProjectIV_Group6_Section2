#pragma once

#include "Client.h"
#include "LoginPage.h"
#include "NewPost.h"
#include "LogToFile.h"

#define MAX_PACKET_SIZE 1024
#define HEADER_SIZE 7

namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;

	/// <summary>
	/// Summary for MyForm
	/// </summary>
	public ref class MyForm : public System::Windows::Forms::Form
	{
	private:
		SOCKET ClientSocket;

	public:
		MyForm(void)
		{
			InitializeComponent();
			ClientSocket = InitClient();
			ConnectToServer(ClientSocket);
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~MyForm()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	protected:
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Button^ button1;
	private: System::Windows::Forms::Button^ button2;
	private: System::Windows::Forms::Button^ newPost_button;

	private: System::Windows::Forms::Button^ button4;
	private: System::Windows::Forms::Button^ button5;
	private: System::Windows::Forms::Button^ button6;

	private:
		/// <summary>
		/// Required designer variable.
		/// </summary>
		System::ComponentModel::Container ^components;

#pragma region Windows Form Designer generated code
		/// <summary>
		/// Required method for Designer support - do not modify
		/// the contents of this method with the code editor.
		/// </summary>
		void InitializeComponent(void)
		{
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->button1 = (gcnew System::Windows::Forms::Button());
			this->button2 = (gcnew System::Windows::Forms::Button());
			this->newPost_button = (gcnew System::Windows::Forms::Button());
			this->button4 = (gcnew System::Windows::Forms::Button());
			this->button5 = (gcnew System::Windows::Forms::Button());
			this->button6 = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 28));
			this->label1->ImageAlign = System::Drawing::ContentAlignment::TopCenter;
			this->label1->Location = System::Drawing::Point(278, 24);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(211, 44);
			this->label1->TabIndex = 0;
			this->label1->Text = L"FotoFrenzy";
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(272, 104);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(226, 242);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 1;
			this->pictureBox1->TabStop = false;
			// 
			// button1
			// 
			this->button1->Location = System::Drawing::Point(403, 395);
			this->button1->Name = L"button1";
			this->button1->Size = System::Drawing::Size(86, 23);
			this->button1->TabIndex = 2;
			this->button1->Text = L"next post";
			this->button1->UseVisualStyleBackColor = true;
			// 
			// button2
			// 
			this->button2->Location = System::Drawing::Point(286, 395);
			this->button2->Name = L"button2";
			this->button2->Size = System::Drawing::Size(86, 23);
			this->button2->TabIndex = 3;
			this->button2->Text = L"previous post";
			this->button2->UseVisualStyleBackColor = true;
			// 
			// newPost_button
			// 
			this->newPost_button->Location = System::Drawing::Point(572, 117);
			this->newPost_button->Name = L"newPost_button";
			this->newPost_button->Size = System::Drawing::Size(75, 23);
			this->newPost_button->TabIndex = 4;
			this->newPost_button->Text = L"new post";
			this->newPost_button->UseVisualStyleBackColor = true;
			this->newPost_button->Click += gcnew System::EventHandler(this, &MyForm::newPost_button_Click);
			// 
			// button4
			// 
			this->button4->Location = System::Drawing::Point(572, 164);
			this->button4->Name = L"button4";
			this->button4->Size = System::Drawing::Size(75, 23);
			this->button4->TabIndex = 5;
			this->button4->Text = L"edit post";
			this->button4->UseVisualStyleBackColor = true;
			// 
			// button5
			// 
			this->button5->Location = System::Drawing::Point(572, 210);
			this->button5->Name = L"button5";
			this->button5->Size = System::Drawing::Size(75, 23);
			this->button5->TabIndex = 6;
			this->button5->Text = L"delete post";
			this->button5->UseVisualStyleBackColor = true;
			// 
			// button6
			// 
			this->button6->Location = System::Drawing::Point(58, 117);
			this->button6->Name = L"button6";
			this->button6->Size = System::Drawing::Size(75, 23);
			this->button6->TabIndex = 7;
			this->button6->Text = L"Login";
			this->button6->UseVisualStyleBackColor = true;
			this->button6->Click += gcnew System::EventHandler(this, &MyForm::button6_Click);
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 450);
			this->Controls->Add(this->button6);
			this->Controls->Add(this->button5);
			this->Controls->Add(this->button4);
			this->Controls->Add(this->newPost_button);
			this->Controls->Add(this->button2);
			this->Controls->Add(this->button1);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void button6_Click(System::Object^ sender, System::EventArgs^ e) {
		LoginPage lp;
		lp.ShowDialog();
	}

	private: System::Void newPost_button_Click(System::Object^ sender, System::EventArgs^ e) {
		Post* newPost = new Post();
		
		NewPost np(newPost);
		np.ShowDialog();

		newPost->setName("Andrew");

		writePostToFile(newPost);

		char* buffer;
		ifstream ifs;
		ifs.open(newPost->getFilePath(), ios::binary);
		char* TxBuffer;
		bool firstPacket = true;

		ofstream ofs;
		ofs.open("image.jpeg", ios::binary);
		if (ifs.is_open())
		{
			while (!ifs.eof())
			{
				PktDef newPacket;

				if (!firstPacket)
					newPacket.setFirstPacket(false);

				int imageDataSize = MAX_PACKET_SIZE - newPacket.getHeaderSize() - newPost->getPostSize();

				buffer = new char[imageDataSize];

				ifs.read(buffer, imageDataSize);

				int dataSize = newPacket.setData(newPost, buffer, imageDataSize);

				delete[] buffer;

				int size = 0;
				TxBuffer = newPacket.SerializeData(size, dataSize);

				writePacketRawDataToFile(TxBuffer, size);

				PktDef* recPkt = new PktDef(TxBuffer);
				Post* newPost = new Post();

				char* imageStartingPoint = recPkt->parseData(newPost);
				ofs.write(imageStartingPoint, imageDataSize);

				firstPacket = false;

				/*delete newPost;
				delete recPkt;*/
			}
		}
	}
};
}
