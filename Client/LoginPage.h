#pragma once
#include "Client.h"
#include "LogToFile.h"
#include <msclr/marshal_cppstd.h>

#define MAX_PACKET_SIZE 1024

namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for LoginPage
	/// </summary>
	public ref class LoginPage : public System::Windows::Forms::Form
	{
	private:
		string* returnUserName;
		String^ userName;
	private: System::Windows::Forms::Button^ login_button;
		   String^ password;
	private: System::Windows::Forms::Label^ loginStatusLabel;

		   SOCKET ClientSocket;

	public:
		LoginPage(void)
		{
			InitializeComponent();
			loginStatusLabel->Visible = false;
		}

		LoginPage(SOCKET ClientSocket, string* returnUserName)
		{
			InitializeComponent();
			loginStatusLabel->Visible = false;
			this->ClientSocket = ClientSocket;
			this->returnUserName = returnUserName;
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~LoginPage()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	protected:
	private: System::Windows::Forms::TextBox^ userName_Box;
	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ password_Box;

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
			this->userName_Box = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->password_Box = (gcnew System::Windows::Forms::TextBox());
			this->login_button = (gcnew System::Windows::Forms::Button());
			this->loginStatusLabel = (gcnew System::Windows::Forms::Label());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(56, 46);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(66, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"User Name: ";
			// 
			// userName_Box
			// 
			this->userName_Box->Location = System::Drawing::Point(59, 74);
			this->userName_Box->Name = L"userName_Box";
			this->userName_Box->Size = System::Drawing::Size(100, 20);
			this->userName_Box->TabIndex = 1;
			this->userName_Box->TextChanged += gcnew System::EventHandler(this, &LoginPage::userName_Changed);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(56, 143);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(59, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Password: ";
			// 
			// password_Box
			// 
			this->password_Box->Location = System::Drawing::Point(59, 175);
			this->password_Box->Name = L"password_Box";
			this->password_Box->Size = System::Drawing::Size(100, 20);
			this->password_Box->TabIndex = 3;
			this->password_Box->TextChanged += gcnew System::EventHandler(this, &LoginPage::password_Changed);
			// 
			// login_button
			// 
			this->login_button->Location = System::Drawing::Point(70, 234);
			this->login_button->Name = L"login_button";
			this->login_button->Size = System::Drawing::Size(75, 23);
			this->login_button->TabIndex = 4;
			this->login_button->Text = L"Login";
			this->login_button->UseVisualStyleBackColor = true;
			this->login_button->Click += gcnew System::EventHandler(this, &LoginPage::login_button_Click);
			// 
			// loginStatusLabel
			// 
			this->loginStatusLabel->AutoSize = true;
			this->loginStatusLabel->Location = System::Drawing::Point(31, 272);
			this->loginStatusLabel->Name = L"loginStatusLabel";
			this->loginStatusLabel->Size = System::Drawing::Size(35, 13);
			this->loginStatusLabel->TabIndex = 5;
			this->loginStatusLabel->Text = L"label3";
			// 
			// LoginPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(224, 304);
			this->Controls->Add(this->loginStatusLabel);
			this->Controls->Add(this->login_button);
			this->Controls->Add(this->password_Box);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->userName_Box);
			this->Controls->Add(this->label1);
			this->Name = L"LoginPage";
			this->Text = L"LoginPage";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void userName_Changed(System::Object^ sender, System::EventArgs^ e) {
		this->userName = userName_Box->Text;
	}

	private: System::Void password_Changed(System::Object^ sender, System::EventArgs^ e) {
		this->password = password_Box->Text;
	}

	private: System::Void login_button_Click(System::Object^ sender, System::EventArgs^ e) {

		bool userLoggedIn = false;

		std::string userString;			// Convert from String^ to string
		for (int i = 0; i < userName->Length; i++) {
			userString.push_back((char)userName[i]);
		}

		std::string passwordString;
		for (int i = 0; i < password->Length; i++) {
			passwordString.push_back((char)password[i]);
		}

		sendUserLogin(userString, passwordString);

		userLoggedIn = receiveUserLogin();

		Post* ackPost = new Post();
		PktDef ackPkt;
		ackPkt.setMessageType(4);

		char* TxBuffer;

		char garbageData = { '\0' };
		char* garbagePtr = &garbageData;
		int dataSize = ackPkt.setData(ackPost, garbagePtr, 1);

		int size = 0;
		TxBuffer = ackPkt.SerializeData(size, dataSize);
		writePacketRawDataToFile(TxBuffer, size, "Sent");

		send(ClientSocket, TxBuffer, size, 0);

		delete ackPost;

		if (userLoggedIn)
		{
			*returnUserName = userString;
			this->Close();
		}
		else
		{
			System::String^ loginState = msclr::interop::marshal_as<System::String^>("Username or password incorrect");
			loginStatusLabel->Text = loginState;
			loginStatusLabel->Visible = true;
		}
	}

		   void sendUserLogin(string userString, string passwordString)
		   {
			   Post* loginData = new Post();
			   PktDef newPacket;

			   loginData->setName(userString);
			   loginData->setCaption(passwordString);
			   newPacket.setMessageType(1);

			   char* TxBuffer;

			   char garbageData = { '\0' };
			   char* garbagePtr = &garbageData;
			   int dataSize = newPacket.setData(loginData, garbagePtr, 1);

			   int size = 0;
			   TxBuffer = newPacket.SerializeData(size, dataSize);
			   writePacketRawDataToFile(TxBuffer, size, "Sent");

			   send(ClientSocket, TxBuffer, size, 0);
			   if (loginData)
				   delete loginData;
		   }

		   bool receiveUserLogin()
		   {
			   char RxBuffer[1024];
			   recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);

			   PktDef newPacket(RxBuffer);

			   bool status = newPacket.getPostFinishFlag();

			   return status;
		   }
};
}
