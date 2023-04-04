#pragma once

#include "Post.h"
#include <msclr\marshal_cppstd.h>
#include <iomanip>
#include <ctime> 
#include <sstream>

namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;
	using namespace System::IO;
	using namespace msclr::interop;
	using namespace System::Drawing::Imaging;

	/// <summary>
	/// Summary for NewPost
	/// </summary>
	public ref class NewPost : public System::Windows::Forms::Form
	{
	private:
		Post* newPost;

	public:
		NewPost(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		NewPost(Post* newPost)
		{
			InitializeComponent();
			this->newPost = newPost;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~NewPost()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	protected:
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ captionBox;

	private: System::Windows::Forms::Button^ addPicture_button;
	private: System::Windows::Forms::Button^ createPost_button;

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
			this->pictureBox1 = (gcnew System::Windows::Forms::PictureBox());
			this->label1 = (gcnew System::Windows::Forms::Label());
			this->captionBox = (gcnew System::Windows::Forms::TextBox());
			this->addPicture_button = (gcnew System::Windows::Forms::Button());
			this->createPost_button = (gcnew System::Windows::Forms::Button());
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->BeginInit();
			this->SuspendLayout();
			// 
			// pictureBox1
			// 
			this->pictureBox1->Location = System::Drawing::Point(37, 56);
			this->pictureBox1->Name = L"pictureBox1";
			this->pictureBox1->Size = System::Drawing::Size(208, 277);
			this->pictureBox1->SizeMode = System::Windows::Forms::PictureBoxSizeMode::StretchImage;
			this->pictureBox1->TabIndex = 0;
			this->pictureBox1->TabStop = false;
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(297, 123);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(76, 13);
			this->label1->TabIndex = 1;
			this->label1->Text = L"Add a caption:";
			// 
			// captionBox
			// 
			this->captionBox->Location = System::Drawing::Point(300, 139);
			this->captionBox->Name = L"captionBox";
			this->captionBox->Size = System::Drawing::Size(104, 20);
			this->captionBox->TabIndex = 2;
			this->captionBox->TextChanged += gcnew System::EventHandler(this, &NewPost::captionBox_Changed);
			// 
			// addPicture_button
			// 
			this->addPicture_button->Location = System::Drawing::Point(300, 56);
			this->addPicture_button->Name = L"addPicture_button";
			this->addPicture_button->Size = System::Drawing::Size(75, 23);
			this->addPicture_button->TabIndex = 3;
			this->addPicture_button->Text = L"Add Picture";
			this->addPicture_button->UseVisualStyleBackColor = true;
			this->addPicture_button->Click += gcnew System::EventHandler(this, &NewPost::addPicture_button_Click);
			// 
			// createPost_button
			// 
			this->createPost_button->Location = System::Drawing::Point(300, 310);
			this->createPost_button->Name = L"createPost_button";
			this->createPost_button->Size = System::Drawing::Size(75, 23);
			this->createPost_button->TabIndex = 4;
			this->createPost_button->Text = L"Create Post";
			this->createPost_button->UseVisualStyleBackColor = true;
			this->createPost_button->Click += gcnew System::EventHandler(this, &NewPost::createPost_button_Click);
			// 
			// NewPost
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(495, 383);
			this->Controls->Add(this->createPost_button);
			this->Controls->Add(this->addPicture_button);
			this->Controls->Add(this->captionBox);
			this->Controls->Add(this->label1);
			this->Controls->Add(this->pictureBox1);
			this->Name = L"NewPost";
			this->Text = L"NewPost";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void captionBox_Changed(System::Object^ sender, System::EventArgs^ e) {
		std::string caption;

		for (int i = 0; i < captionBox->Text->Length; i++) {
			caption.push_back((char)captionBox->Text[i]);
		}
		newPost->setCaption(caption);
	}

	private: System::Void addPicture_button_Click(System::Object^ sender, System::EventArgs^ e) {

		OpenFileDialog^ openFileDialog1 = gcnew OpenFileDialog();
		openFileDialog1->Filter = "Image Files(*.bmp;*.jpg;*.jpeg;*.png)|*.bmp;*.jpg;*.jpeg;*.png|All files (*.*)|*.*";

		if (openFileDialog1->ShowDialog() == System::Windows::Forms::DialogResult::OK) {
			std::string filePath = marshal_as<std::string>(openFileDialog1->FileName);
			Image^ image = Image::FromFile(openFileDialog1->FileName);
			// Do something with the selected file

			pictureBox1->Image = image;

			newPost->setFilePath(filePath);
		}

		
	}

	private: System::Void createPost_button_Click(System::Object^ sender, System::EventArgs^ e) {

		auto t = std::time(nullptr);
		auto time = *std::localtime(&t);

		std::ostringstream oss;
		oss << std::put_time(&time, "%d-%m-%Y %H-%M-%S");
		std::string date = oss.str();

		newPost->setDate(date);

		this->Close();
	}
};
}
