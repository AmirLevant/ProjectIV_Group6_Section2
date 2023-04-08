#pragma once
#include "../Client/PktDef.h"
#include <sstream>

#define MAX_PACKET_SIZE 1024

class User
{
	string userName;
	vector<Post>* posts;

public:
	User()
	{
		userName = "";
		posts = new vector<Post>();
	}

	void setUserName(string userName)
	{
		this->userName = userName;
	}

	string getUserName()
	{
		return userName;
	}

	void loadPosts()
	{	
		removeTerminatingChar(userName);
		ostringstream os;
		os << userName << "/posts.txt";
		addTerminatingChar(userName);
		ifstream inputFile(os.str());
		string line;

		if (!inputFile.is_open()) 
			std::cerr << "Error opening input file: " << os.str() << std::endl;
		
		while (std::getline(inputFile, line)) {
			
			Post newPost;
			size_t pos = 0;
			string token;
			string delimiter = "$";

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			line.erase(0, pos + delimiter.length());
			newPost.setDate(token);

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			line.erase(0, pos + delimiter.length());
			newPost.setName(token);

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			line.erase(0, pos + delimiter.length());
			newPost.setCaption(token);

			pos = line.find(delimiter);
			token = line.substr(0, pos);
			line.erase(0, pos + delimiter.length());
			newPost.setFilePath(token);

			token = line;
			newPost.setLikeAmount(stoi(token));

			posts->push_back(newPost);
		}

		inputFile.close();
	}

	void removeTerminatingChar(std::string& str) {
		if (str.back() == '\0') {
			str.pop_back();
		}
	}

	void addTerminatingChar(std::string& str) {
		if (str.back() != '\0') {
			str.push_back('\0');
		}
	}

	void sendAllPosts(SOCKET ConnectionSocket)
	{
		char* TxBuffer;
		char RxBuffer[1024];
		Post* postPtr;
		for (int i = 0; i < posts->size(); i++)
		{
			postPtr = &posts->at(i);

			writePostToFile(postPtr);

			char* buffer;		// For reading in from image file
			ifstream ifs;
			ifs.open(postPtr->getFilePath(), ios::binary);

			bool firstPacket = true;

			if (ifs.is_open())
			{
				while (!ifs.eof())
				{
					PktDef newPacket;
					newPacket.setMessageType(2);

					if (!firstPacket)
						newPacket.setFirstPacket(false);

					int imageDataSize = MAX_PACKET_SIZE - newPacket.getHeaderSize() - postPtr->getPostSize();

					buffer = new char[imageDataSize];

					ifs.read(buffer, imageDataSize);
					imageDataSize = ifs.gcount();

					int dataSize = newPacket.setData(postPtr, buffer, imageDataSize);

					delete[] buffer;

					int size = 0;
					TxBuffer = newPacket.SerializeData(size, dataSize);

					writePacketRawDataToFile(TxBuffer, size, "Sent");

					send(ConnectionSocket, TxBuffer, size, 0);

					
					recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);

					PktDef recPacket(RxBuffer);

					if (recPacket.getMessageType() != 4)
						exit(1);

					firstPacket = false;
				}
			}
			Post* postFinished = new Post();
			PktDef finalPacket;
			finalPacket.setPostFinishFlag(true);

			char garbageData = { '\0' };
			char* garbagePtr = &garbageData;
			int dataSize = finalPacket.setData(postFinished, garbagePtr, 1);

			int size = 0;
			TxBuffer = finalPacket.SerializeData(size, dataSize);
			writePacketRawDataToFile(TxBuffer, size, "Sent");

			send(ConnectionSocket, TxBuffer, size, 0);

			recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);

			PktDef newPacket(RxBuffer);

			if (newPacket.getMessageType() != 4)
			{
				delete postFinished;
				exit(1);
			}

			delete postFinished;
		}

		Post* allFinished = new Post();
		PktDef allSent;
		allSent.setPostFinishFlag(true);
		allSent.setMessageType(8);

		char garbageData = { '\0' };
		char* garbagePtr = &garbageData;
		int dataSize = allSent.setData(allFinished, garbagePtr, 1);

		int size = 0;
		TxBuffer = allSent.SerializeData(size, dataSize);
		writePacketRawDataToFile(TxBuffer, size, "Sent");

		send(ConnectionSocket, TxBuffer, size, 0);

		recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);

		PktDef newPacket(RxBuffer);

		if (newPacket.getMessageType() != 4)
		{
			delete allFinished;
			exit(1);
		}

		if (allFinished)
			delete allFinished;
	
	}
};