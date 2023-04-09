#pragma once
#include "LogToFile.h"
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

	void receivePost(SOCKET ConnectionSocket, char* RxBuffer)
	{
		char* TxBuffer;
		char buffer[1024];
		Post* newPost = new Post();
		PktDef recPacket(RxBuffer);
		writePacketRawDataToFile(RxBuffer, MAX_PACKET_SIZE, "Received");
		
		removeTerminatingChar(userName);
		ostringstream os;

		for (int i = 1; i <= MAX_PACKET_SIZE; i++)
		{
			os << userName << '/' << i << ".jpg";
			ifstream ifs;
			ifs.open(os.str());

			if (ifs.is_open() != true)
			{
				break;
			}
			os.str("");
		}
		addTerminatingChar(userName);
		ofstream ofs;

		bool firstPacket = true;

		ofs.open(os.str(), ios::binary);
		if (ofs.is_open())
		{
			while (recPacket.getPostFinishFlag() != true)
			{
				char* imageStartingPoint;
				if (!firstPacket)
				{
					recv(ConnectionSocket, buffer, sizeof(buffer), 0);

					PktDef newPacket(buffer);
					writePacketRawDataToFile(buffer, MAX_PACKET_SIZE, "Received");

					if (newPacket.getPostFinishFlag() == true)
						break;

					imageStartingPoint = newPacket.parseData(newPost);

					int imageDataSize = newPacket.getImageLength();
					ofs.write(imageStartingPoint, imageDataSize);

				}
				else
				{
					imageStartingPoint = recPacket.parseData(newPost);

					int imageDataSize = recPacket.getImageLength();

					ofs.write(imageStartingPoint, imageDataSize);

					if (recPacket.getFirstPacket() == true)
					{
						Post tempPost = *newPost;
						tempPost.setFilePath(os.str());

						int randLikes = (rand() % 200) + 1;
						tempPost.setLikeAmount(randLikes);

						saveToServer(tempPost);

						posts->push_back(tempPost);
					}
					firstPacket = false;
				}

				Post* ackPost = new Post();
				PktDef ackPkt;
				ackPkt.setMessageType(4);

				char garbageData = { '\0' };
				char* garbagePtr = &garbageData;
				int dataSize = ackPkt.setData(ackPost, garbagePtr, 1);

				int size = 0;
				TxBuffer = ackPkt.SerializeData(size, dataSize);
				writePacketRawDataToFile(TxBuffer, size, "Sent");

				send(ConnectionSocket, TxBuffer, size, 0);

				delete ackPost;
			}
			
		}
		ofs.close();

		Post* ackPost = new Post();
		PktDef ackPkt;
		ackPkt.setMessageType(4);

		char garbageData = { '\0' };
		char* garbagePtr = &garbageData;
		int dataSize = ackPkt.setData(ackPost, garbagePtr, 1);

		int size = 0;
		TxBuffer = ackPkt.SerializeData(size, dataSize);
		writePacketRawDataToFile(TxBuffer, size, "Sent");

		send(ConnectionSocket, TxBuffer, size, 0);

		delete ackPost;
	}

	void saveToServer(Post post)
	{
		ofstream ofs;

		ostringstream os;
		removeTerminatingChar(userName);
		os << userName << "/posts.txt";
		ofs.open(os.str(), ios::app);
		addTerminatingChar(userName);

		string date = post.getDate();
		removeTerminatingChar(date);

		string name = post.getName();
		removeTerminatingChar(name);

		string caption = post.getCaption();
		removeTerminatingChar(caption);

		if (ofs.is_open())
		{
			ostringstream postString;
			postString << endl << date << '$' << name << '$' << caption << '$' << post.getFilePath() << '$' << post.getLikeAmount();

			ofs << postString.str();
		}

		ofs.close();
	}

	void deletePost(SOCKET ConnectionSocket, char* RxBuffer)
	{
		char* TxBuffer;
		Post* deletePost = new Post();
		PktDef recPacket(RxBuffer);
		writePacketRawDataToFile(RxBuffer, MAX_PACKET_SIZE, "Received");

		recPacket.parseData(deletePost);

		removeTerminatingChar(userName);
		ostringstream os;
		os << userName << "/posts.txt";
		ifstream file(os.str());
		string line;
		int lineNumber = 0;
		addTerminatingChar(userName);
		
		string date = deletePost->getDate();
		removeTerminatingChar(date);
		removeTerminatingChar(date);

		string name = deletePost->getName();
		removeTerminatingChar(name);
		removeTerminatingChar(name);

		string caption = deletePost->getCaption();
		removeTerminatingChar(caption);
		removeTerminatingChar(caption);

		if (file.is_open()) {
			while (getline(file, line)) {
				++lineNumber;
				istringstream ss(line);
				string field;
				bool allFieldsMatch = true;


				getline(ss, field, '$'); // postDate
				if (field.find(date) == string::npos) 
				{
					allFieldsMatch = false;
				}

				getline(ss, field, '$'); // postUserName
				if (field != name)
				{
					allFieldsMatch = false;
				}

				getline(ss, field, '$'); // postCaption
				if (field != caption)
				{
					allFieldsMatch = false;
				}

				getline(ss, field, '$'); // imageFilePath

				getline(ss, field, '$'); // postLikeAmount

				if (allFieldsMatch) 
				{
					string filePath = removeLineFromFile(os.str(), lineNumber);
					removePictureFromFile(lineNumber, filePath);
					posts->erase(posts->begin() + (lineNumber - 1));
				}
			}
			file.close();
		}
		else {
			cout << "Unable to open file" << endl;
		}
		delete deletePost;


		Post* ackPost = new Post();
		PktDef ackPkt;
		ackPkt.setMessageType(4);

		char garbageData = { '\0' };
		char* garbagePtr = &garbageData;
		int dataSize = ackPkt.setData(ackPost, garbagePtr, 1);

		int size = 0;
		TxBuffer = ackPkt.SerializeData(size, dataSize);
		writePacketRawDataToFile(TxBuffer, size, "Sent");

		send(ConnectionSocket, TxBuffer, size, 0);

		delete ackPost;
	}

	string removeLineFromFile(string filename, int lineNum) {
		// Open the file
		ifstream inFile(filename);

		// Read the file into a vector of strings
		vector<string> lines;
		string line;
		while (getline(inFile, line)) {
			lines.push_back(line);
		}

		string specificLine = lines.at(lineNum - 1);

		lines.erase(lines.begin() + (lineNum - 1));

		string delimiter = "$";
		size_t pos = 0;
		string token;
		int field_num = 0;
		while ((pos = specificLine.find(delimiter)) != string::npos) {
			token = specificLine.substr(0, pos);
			if (field_num == 3) {  // fourth field contains the image filepath
				cout << token << endl;  // do something with the filepath
				break;
			}
			specificLine.erase(0, pos + delimiter.length());
			field_num++;
		}

		inFile.close();

		// Write the updated data back to the file
		ofstream outFile(filename);
	
		for (int i = 0; i < lines.size(); i ++) {
			outFile << lines.at(i);
			if (i != (lines.size() - 1))
				outFile << endl;
		}

		outFile.close();

		return token;
	}

	void removePictureFromFile(int lineNumber, string filePath)
	{
		int result = remove(filePath.c_str());
	}
};

