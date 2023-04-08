#pragma once
#include "LoginPage.h"
#include "NewPost.h"
#include <map>

#define MAX_PACKET_SIZE 1024

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
	private: vector<Post>* posts;
	private: System::Windows::Forms::Label^ dateLabel;
	private: System::Windows::Forms::TextBox^ textBox1;
	private: System::Windows::Forms::Label^ userNameLabel;
	private: System::Windows::Forms::Label^ likeLabel;
	private: SOCKET ClientSocket;
	private: int nextButtonClicks;
	private: string* userName;
	    
	public:
		MyForm(void)
		{
			InitializeComponent();

			userNameLabel->Visible = false;
			textBox1->Visible = false;
			dateLabel->Visible = false;
			likeLabel->Visible = false;
			prevButton->Visible = false;
			nextButton->Visible = false;
			nextButtonClicks = 0;
			textBox1->ReadOnly = true;
			newPost_button->Visible = false;
			editPost_button->Visible = false;
			deletePost_button->Visible = false;

			posts = new vector<Post>();
			ClientSocket = InitClient();
			ConnectToServer(ClientSocket);

			userName = new string;
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
			if (posts)
			{
				delete posts;
			}
			if (userName)
			{
				delete userName;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	protected:
	private: System::Windows::Forms::PictureBox^ pictureBox1;
	private: System::Windows::Forms::Button^ nextButton;

	private: System::Windows::Forms::Button^ prevButton;

	private: System::Windows::Forms::Button^ newPost_button;
	private: System::Windows::Forms::Button^ editPost_button;
	private: System::Windows::Forms::Button^ deletePost_button;
	private: System::Windows::Forms::Button^ log_button;





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
			this->nextButton = (gcnew System::Windows::Forms::Button());
			this->prevButton = (gcnew System::Windows::Forms::Button());
			this->newPost_button = (gcnew System::Windows::Forms::Button());
			this->editPost_button = (gcnew System::Windows::Forms::Button());
			this->deletePost_button = (gcnew System::Windows::Forms::Button());
			this->log_button = (gcnew System::Windows::Forms::Button());
			this->dateLabel = (gcnew System::Windows::Forms::Label());
			this->textBox1 = (gcnew System::Windows::Forms::TextBox());
			this->userNameLabel = (gcnew System::Windows::Forms::Label());
			this->likeLabel = (gcnew System::Windows::Forms::Label());
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
			// nextButton
			// 
			this->nextButton->Location = System::Drawing::Point(403, 395);
			this->nextButton->Name = L"nextButton";
			this->nextButton->Size = System::Drawing::Size(86, 23);
			this->nextButton->TabIndex = 2;
			this->nextButton->Text = L"next post";
			this->nextButton->UseVisualStyleBackColor = true;
			this->nextButton->Click += gcnew System::EventHandler(this, &MyForm::nextButton_Click);
			// 
			// prevButton
			// 
			this->prevButton->Location = System::Drawing::Point(286, 395);
			this->prevButton->Name = L"prevButton";
			this->prevButton->Size = System::Drawing::Size(86, 23);
			this->prevButton->TabIndex = 3;
			this->prevButton->Text = L"previous post";
			this->prevButton->UseVisualStyleBackColor = true;
			this->prevButton->Click += gcnew System::EventHandler(this, &MyForm::prevButton_Click);
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
			// editPost_button
			// 
			this->editPost_button->Location = System::Drawing::Point(572, 164);
			this->editPost_button->Name = L"editPost_button";
			this->editPost_button->Size = System::Drawing::Size(75, 23);
			this->editPost_button->TabIndex = 5;
			this->editPost_button->Text = L"edit post";
			this->editPost_button->UseVisualStyleBackColor = true;
			// 
			// deletePost_button
			// 
			this->deletePost_button->Location = System::Drawing::Point(572, 210);
			this->deletePost_button->Name = L"deletePost_button";
			this->deletePost_button->Size = System::Drawing::Size(75, 23);
			this->deletePost_button->TabIndex = 6;
			this->deletePost_button->Text = L"delete post";
			this->deletePost_button->UseVisualStyleBackColor = true;
			// 
			// log_button
			// 
			this->log_button->Location = System::Drawing::Point(686, 13);
			this->log_button->Name = L"log_button";
			this->log_button->Size = System::Drawing::Size(75, 23);
			this->log_button->TabIndex = 7;
			this->log_button->Text = L"Login";
			this->log_button->UseVisualStyleBackColor = true;
			this->log_button->Click += gcnew System::EventHandler(this, &MyForm::log_button_Click);
			// 
			// dateLabel
			// 
			this->dateLabel->AutoSize = true;
			this->dateLabel->Font = (gcnew System::Drawing::Font(L"Microsoft Sans Serif", 11.5F));
			this->dateLabel->Location = System::Drawing::Point(58, 104);
			this->dateLabel->Name = L"dateLabel";
			this->dateLabel->Size = System::Drawing::Size(51, 20);
			this->dateLabel->TabIndex = 8;
			this->dateLabel->Text = L"label2";
			// 
			// textBox1
			// 
			this->textBox1->Location = System::Drawing::Point(62, 127);
			this->textBox1->Multiline = true;
			this->textBox1->Name = L"textBox1";
			this->textBox1->Size = System::Drawing::Size(155, 180);
			this->textBox1->TabIndex = 9;
			// 
			// userNameLabel
			// 
			this->userNameLabel->AutoSize = true;
			this->userNameLabel->Location = System::Drawing::Point(13, 13);
			this->userNameLabel->Name = L"userNameLabel";
			this->userNameLabel->Size = System::Drawing::Size(35, 13);
			this->userNameLabel->TabIndex = 10;
			this->userNameLabel->Text = L"label2";
			// 
			// likeLabel
			// 
			this->likeLabel->AutoSize = true;
			this->likeLabel->Location = System::Drawing::Point(59, 333);
			this->likeLabel->Name = L"likeLabel";
			this->likeLabel->Size = System::Drawing::Size(35, 13);
			this->likeLabel->TabIndex = 11;
			this->likeLabel->Text = L"label2";
			// 
			// MyForm
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(784, 450);
			this->Controls->Add(this->likeLabel);
			this->Controls->Add(this->userNameLabel);
			this->Controls->Add(this->textBox1);
			this->Controls->Add(this->dateLabel);
			this->Controls->Add(this->log_button);
			this->Controls->Add(this->deletePost_button);
			this->Controls->Add(this->editPost_button);
			this->Controls->Add(this->newPost_button);
			this->Controls->Add(this->prevButton);
			this->Controls->Add(this->nextButton);
			this->Controls->Add(this->pictureBox1);
			this->Controls->Add(this->label1);
			this->Name = L"MyForm";
			this->Text = L"MyForm";
			(cli::safe_cast<System::ComponentModel::ISupportInitialize^>(this->pictureBox1))->EndInit();
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion
	private: System::Void log_button_Click(System::Object^ sender, System::EventArgs^ e) {
		LoginPage lp(ClientSocket, userName);
		lp.ShowDialog();

		System::String^ sysUser = msclr::interop::marshal_as<System::String^>(*userName);
		userNameLabel->Text = sysUser;
		userNameLabel->Visible = true;

		newPost_button->Visible = true;
		editPost_button->Visible = true;
		deletePost_button->Visible = true;
		log_button->Text = "Log out";

		receiveAllPosts();

		setPageData((*posts)[posts->size() - 1]);
		if (posts->size() > 1)
			nextButton->Visible = true;
	}

	private: System::Void newPost_button_Click(System::Object^ sender, System::EventArgs^ e) {
		Post* newPost = new Post();
		
		NewPost np(newPost);
		np.ShowDialog();

		newPost->setName(*userName);

		writePostToFile(newPost);

		Post tempPost = *newPost;
		posts->push_back(tempPost);
		if (posts->size() > 1)
			nextButton->Visible = true;
		setPageData(tempPost);
		prevButton->Visible = false;
		nextButtonClicks = 0;

		char* buffer;		// For reading in from image file
		ifstream ifs;
		ifs.open(newPost->getFilePath(), ios::binary);
		char* TxBuffer;		// For transmitting entire packet to server
		bool firstPacket = true;
		char RxBuffer[1024];

		if (ifs.is_open())
		{
			while (!ifs.eof())
			{
				PktDef newPacket;
				newPacket.setMessageType(2);

				if (!firstPacket)
					newPacket.setFirstPacket(false);

				int imageDataSize = MAX_PACKET_SIZE - newPacket.getHeaderSize() - newPost->getPostSize();
				buffer = new char[imageDataSize];

				ifs.read(buffer, imageDataSize);
				imageDataSize = ifs.gcount();

				int dataSize = newPacket.setData(newPost, buffer, imageDataSize);

				delete[] buffer;

				int size = 0;
				TxBuffer = newPacket.SerializeData(size, dataSize);

				writePacketRawDataToFile(TxBuffer, size, "Sent");

				send(ClientSocket, TxBuffer, size, 0);

				recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);

				PktDef recPacket(RxBuffer);

				if (recPacket.getMessageType() != 4)
					exit(1);

				firstPacket = false;
			}
		}
		ifs.close();
		delete newPost;


		Post* postFinished = new Post();
		PktDef finalPacket;
		finalPacket.setPostFinishFlag(true);

		char garbageData = { '\0' };
		char* garbagePtr = &garbageData;
		int dataSize = finalPacket.setData(postFinished, garbagePtr, 1);

		int size = 0;
		TxBuffer = finalPacket.SerializeData(size, dataSize);
		writePacketRawDataToFile(TxBuffer, size, "Sent");

		send(ClientSocket, TxBuffer, size, 0);

		recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);

		PktDef newPacket(RxBuffer);

		if (newPacket.getMessageType() != 4)
		{
			delete postFinished;
			exit(1);
		}

		delete postFinished;
	}

	void setPageData(Post newPost)
	{
		Bitmap^ image = gcnew Bitmap(gcnew String(newPost.getFilePath().c_str()));		// Setting current page data
		pictureBox1->Image = image;
		System::String^ sysCaption = msclr::interop::marshal_as<System::String^>(newPost.getCaption());
		textBox1->Text = sysCaption;
		textBox1->Visible = true;

		dateLabel->Text = formatDate(newPost.getDate());
		dateLabel->Visible = true;

		ostringstream os;
		os << newPost.getLikeAmount() << " Likes";
		System::String^ sysLikes = msclr::interop::marshal_as<System::String^>(os.str());
		likeLabel->Text = sysLikes;
		likeLabel->Visible = true;
	}

	System::String^ formatDate(string dateTimeString)
	{
		std::string dateString = dateTimeString.substr(0, 10);  // Get the date part of the string
		std::string timeString = dateTimeString.substr(11);     // Get the time part of the string

		std::string formattedDate = convertDate(dateString);     // Use the formatDate function to format the date
		std::string formattedTime = militaryToStandardTime(timeString);  // Use the militaryToStandardTime function to format the time

		string combined = formattedDate + " " + formattedTime;             // Combine the two formatted strings
		System::String^ sysDate = msclr::interop::marshal_as<System::String^>(combined);
		return sysDate;
	}

	std::string convertDate(std::string dateString)
	{
		// Define a mapping of month numbers to month names
		std::map<std::string, std::string> monthNames = {
			{"01", "January"}, {"02", "February"}, {"03", "March"}, {"04", "April"},
			{"05", "May"}, {"06", "June"}, {"07", "July"}, {"08", "August"},
			{"09", "September"}, {"10", "October"}, {"11", "November"}, {"12", "December"}
		};

		// Split the date string into day, month, and year components
		std::stringstream ss(dateString);
		std::string dayStr, monthStr, yearStr;
		std::getline(ss, dayStr, '-');
		std::getline(ss, monthStr, '-');
		std::getline(ss, yearStr);

		// Convert the month number to a month name using the mapping
		std::string monthName = monthNames[monthStr];

		// Combine the components into the final string format
		std::stringstream result;
		result << monthName << " " << dayStr << ", " << yearStr;
		return result.str();
	}

	std::string militaryToStandardTime(const std::string& militaryTime)
	{
		// Split the string into its components
		int hour = std::stoi(militaryTime.substr(0, 2));
		int minute = std::stoi(militaryTime.substr(3, 2));
		int second = std::stoi(militaryTime.substr(6, 2));

		// Determine whether it's AM or PM
		std::string ampm = (hour < 12) ? "AM" : "PM";

		// Convert to 12-hour time
		hour %= 12;
		if (hour == 0) 
			hour = 12;

		// Format the string
		std::string result = std::to_string(hour) + ":" + std::to_string(minute) + " " + ampm;
		return result;
	}

	private: System::Void nextButton_Click(System::Object^ sender, System::EventArgs^ e) {
		nextButtonClicks++;

		setPageData((*posts)[posts->size() - nextButtonClicks - 1]);

		if (nextButtonClicks > 0)
			prevButton->Visible = true;

		if (nextButtonClicks == (posts->size() - 1))
			nextButton->Visible = false;
	}

	private: System::Void prevButton_Click(System::Object^ sender, System::EventArgs^ e) {
		nextButtonClicks--;

		setPageData((*posts)[posts->size() - nextButtonClicks - 1]);

		if (nextButtonClicks <= 0)
			prevButton->Visible = false;

		if (nextButtonClicks < (posts->size() - 1))
			nextButton->Visible = true;
	}


	void receiveAllPosts()
	{

		bool moreData = true;
		int numImagesReceived = 0;

		char RxBuffer[1024];
		Post* loadPost = new Post();

		do
		{
			bool firstPacket = true;
			recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);

			PktDef recPacket(RxBuffer);
			writePacketRawDataToFile(RxBuffer, MAX_PACKET_SIZE, "Received");

			if (recPacket.getMessageType() == 8)
			{
				moreData = false;
				break;
			}
			else
				numImagesReceived++;

			ostringstream os;
			os << numImagesReceived << ".jpg";
			ofstream ofs;

			ofs.open(os.str(), ios::binary);
			if (ofs.is_open())
			{
				while (recPacket.getPostFinishFlag() != true)
				{
					char* imageStartingPoint;
					if (!firstPacket)
					{
						recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);

						PktDef newPacket(RxBuffer);
						writePacketRawDataToFile(RxBuffer, MAX_PACKET_SIZE, "Received");

						if (newPacket.getPostFinishFlag() == true)
							break;

						imageStartingPoint = newPacket.parseData(loadPost);

						int imageDataSize = newPacket.getImageLength();
						ofs.write(imageStartingPoint, imageDataSize);
					}
					else
					{
						imageStartingPoint = recPacket.parseData(loadPost);

						int imageDataSize = recPacket.getImageLength();

						ofs.write(imageStartingPoint, imageDataSize);

						if (recPacket.getFirstPacket() == true)
						{
							Post tempPost = *loadPost;
							tempPost.setFilePath(os.str());
							posts->push_back(tempPost);
						}
						firstPacket = false;
					}

					sendAckPacket();
				}
			}
			ofs.close();

			sendAckPacket();

		} while (moreData);

		sendAckPacket();
	}

	void sendAckPacket()
	{
		char* TxBuffer;

		Post* ackPost = new Post();
		PktDef ackPkt;
		ackPkt.setMessageType(4);

		char garbageData = { '\0' };
		char* garbagePtr = &garbageData;
		int dataSize = ackPkt.setData(ackPost, garbagePtr, 1);

		int size = 0;
		TxBuffer = ackPkt.SerializeData(size, dataSize);
		writePacketRawDataToFile(TxBuffer, size, "Sent");

		send(ClientSocket, TxBuffer, size, 0);

		delete ackPost;
	}
};
}
