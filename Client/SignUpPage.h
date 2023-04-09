#pragma once
#include "LogToFile.h"
#include <msclr/marshal_cppstd.h>

namespace Client {

	using namespace System;
	using namespace System::ComponentModel;
	using namespace System::Collections;
	using namespace System::Windows::Forms;
	using namespace System::Data;
	using namespace System::Drawing;

	/// <summary>
	/// Summary for SignUpPage
	/// </summary>
	public ref class SignUpPage : public System::Windows::Forms::Form
	{
	private: String^ userName;
	private: String^ password;
		   SOCKET ClientSocket;
	private: string* returnUserName;

	public:
		SignUpPage(void)
		{
			InitializeComponent();
			//
			//TODO: Add the constructor code here
			//
		}

		SignUpPage(SOCKET ClientSocket, string* returnUserName)
		{
			InitializeComponent();
			this->ClientSocket = ClientSocket;
			this->returnUserName = returnUserName;
			//
			//TODO: Add the constructor code here
			//
		}

	protected:
		/// <summary>
		/// Clean up any resources being used.
		/// </summary>
		~SignUpPage()
		{
			if (components)
			{
				delete components;
			}
		}
	private: System::Windows::Forms::Label^ label1;
	private: System::Windows::Forms::TextBox^ userName_box;
	protected:

	private: System::Windows::Forms::Label^ label2;
	private: System::Windows::Forms::TextBox^ password_Box;

	private: System::Windows::Forms::Button^ signUp_buton;

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
			this->userName_box = (gcnew System::Windows::Forms::TextBox());
			this->label2 = (gcnew System::Windows::Forms::Label());
			this->password_Box = (gcnew System::Windows::Forms::TextBox());
			this->signUp_buton = (gcnew System::Windows::Forms::Button());
			this->SuspendLayout();
			// 
			// label1
			// 
			this->label1->AutoSize = true;
			this->label1->Location = System::Drawing::Point(93, 49);
			this->label1->Name = L"label1";
			this->label1->Size = System::Drawing::Size(60, 13);
			this->label1->TabIndex = 0;
			this->label1->Text = L"User Name";
			// 
			// userName_box
			// 
			this->userName_box->Location = System::Drawing::Point(74, 75);
			this->userName_box->Name = L"userName_box";
			this->userName_box->Size = System::Drawing::Size(100, 20);
			this->userName_box->TabIndex = 1;
			this->userName_box->TextChanged += gcnew System::EventHandler(this, &SignUpPage::userName_changed);
			// 
			// label2
			// 
			this->label2->AutoSize = true;
			this->label2->Location = System::Drawing::Point(93, 166);
			this->label2->Name = L"label2";
			this->label2->Size = System::Drawing::Size(53, 13);
			this->label2->TabIndex = 2;
			this->label2->Text = L"Password";
			// 
			// password_Box
			// 
			this->password_Box->Location = System::Drawing::Point(74, 200);
			this->password_Box->Name = L"password_Box";
			this->password_Box->Size = System::Drawing::Size(100, 20);
			this->password_Box->TabIndex = 3;
			this->password_Box->TextChanged += gcnew System::EventHandler(this, &SignUpPage::password_Changed);
			// 
			// signUp_buton
			// 
			this->signUp_buton->Location = System::Drawing::Point(87, 262);
			this->signUp_buton->Name = L"signUp_buton";
			this->signUp_buton->Size = System::Drawing::Size(75, 23);
			this->signUp_buton->TabIndex = 4;
			this->signUp_buton->Text = L"Sign Up";
			this->signUp_buton->UseVisualStyleBackColor = true;
			this->signUp_buton->Click += gcnew System::EventHandler(this, &SignUpPage::signUp_Click);
			// 
			// SignUpPage
			// 
			this->AutoScaleDimensions = System::Drawing::SizeF(6, 13);
			this->AutoScaleMode = System::Windows::Forms::AutoScaleMode::Font;
			this->ClientSize = System::Drawing::Size(259, 336);
			this->Controls->Add(this->signUp_buton);
			this->Controls->Add(this->password_Box);
			this->Controls->Add(this->label2);
			this->Controls->Add(this->userName_box);
			this->Controls->Add(this->label1);
			this->Name = L"SignUpPage";
			this->Text = L"SignUpPage";
			this->ResumeLayout(false);
			this->PerformLayout();

		}
#pragma endregion

	private: System::Void signUp_Click(System::Object^ sender, System::EventArgs^ e) {

		std::string userString;			// Convert from String^ to string
		for (int i = 0; i < userName->Length; i++) {
			userString.push_back((char)userName[i]);
		}

		std::string passwordString;
		for (int i = 0; i < password->Length; i++) {
			passwordString.push_back((char)password[i]);
		}

		sendSignUp(userString, passwordString);

		receiveUserLogin();

		*returnUserName = userString;

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

		receiveUserLogin();

		send(ClientSocket, TxBuffer, size, 0);

		this->Close();
	}

	private: System::Void password_Changed(System::Object^ sender, System::EventArgs^ e) {
		this->password = password_Box->Text;
	}

	private: System::Void userName_changed(System::Object^ sender, System::EventArgs^ e) {
		this->userName = userName_box->Text;
	}

	void sendSignUp(string userString, string passwordString)
	{
		Post* signUpData = new Post();
		PktDef newPacket;

		signUpData->setName(userString);
		signUpData->setCaption(passwordString);
		newPacket.setMessageType(3);

		char* TxBuffer;

		char garbageData = { '\0' };
		char* garbagePtr = &garbageData;
		int dataSize = newPacket.setData(signUpData, garbagePtr, 1);

		int size = 0;
		TxBuffer = newPacket.SerializeData(size, dataSize);
		writePacketRawDataToFile(TxBuffer, size, "Sent");

		send(ClientSocket, TxBuffer, size, 0);
		if (signUpData)
			delete signUpData;

	}

	void receiveUserLogin()
	{
		char RxBuffer[1024];
		recv(ClientSocket, RxBuffer, sizeof(RxBuffer), 0);

		PktDef newPacket(RxBuffer);

		if (newPacket.getMessageType() != 4 && newPacket.getMessageType() != 8)
			exit(1);
	
	}

};
}
