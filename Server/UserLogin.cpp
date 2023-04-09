#include "UserLogin.h"
#include "LogToFile.h"
#include <sstream>
#include <Windows.h>

string beginUserLogin(SOCKET ConnectionSocket)
{
	bool userLoggedIn;
	string userName;
	do
	{
		userLoggedIn = false;
		userName = loginUser(ConnectionSocket, userLoggedIn);

		Post* loginStatus = new Post();
		PktDef newPacket;
		newPacket.setMessageType(4);

		if (userLoggedIn)
		{
			newPacket.setPostFinishFlag(true);
			cout << "Logging user in..." << endl << endl;
		}
		else
		{
			cout << "Incorrect login information" << endl << endl;
		}

		char* TxBuffer;

		char garbageData = { '\0' };
		char* garbagePtr = &garbageData;
		int dataSize = newPacket.setData(loginStatus, garbagePtr, 1);

		int size = 0;
		TxBuffer = newPacket.SerializeData(size, dataSize);
		writePacketRawDataToFile(TxBuffer, size, "Sent");

		send(ConnectionSocket, TxBuffer, size, 0);

		char RxBuffer[1024];
		recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);

		PktDef recPacket(RxBuffer);

		if (recPacket.getMessageType() != 4)
		{
			delete loginStatus;
			exit(1);
		}

		delete loginStatus;
	} while (!userLoggedIn);

	return userName;
}

string loginUser(SOCKET ConnectionSocket, bool& userLoggedIn)
{
	string userName;
	char RxBuffer[1024];
	recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);

	PktDef newPacket(RxBuffer);

	if (newPacket.getMessageType() == 3)
	{
		userName = signUpUser(ConnectionSocket, RxBuffer);
		userLoggedIn = true;

		return userName;
	}

	Post* loginData = new Post();

	newPacket.parseData(loginData);

	userName = loginData->getName();
	string password = loginData->getCaption();

    cout << "User Name: " << userName << endl;
    cout << "Password: " << password << endl << endl;

	userLoggedIn = checkForUser(userName, password);

	if (loginData)
		delete loginData;

	return userName;
}

bool checkForUser(string userName, string password)
{
    ifstream file("Users.txt");
    string line;
    while (getline(file, line))
    {
        // Split the line into username and password
        size_t pos = line.find(",");
        if (pos == string::npos)
        {
            continue; // invalid line
        }
        string name = line.substr(0, pos);
        string pass = line.substr(pos + 1);

        name = name + '\0';
        pass = pass + '\0';

        // Check if the username and password match
        if (name == userName && pass == password)
        {
            file.close();
            return true;
        }
    }
    file.close();
    return false;
}

string signUpUser(SOCKET ConnectionSocket, char* RxBuffer)
{
	PktDef newPacket(RxBuffer);
	Post* loginData = new Post();

	newPacket.parseData(loginData);

	string name = loginData->getName();
	string password = loginData->getCaption();

	cout << "User Name: " << name << endl;
	cout << "Password: " << password << endl << endl;

	createUser(name, password);

	if (loginData)
		delete loginData;

	return name;
}

void createUser(string userName, string password)
{
	ofstream ofs;
	ofs.open("Users.txt", ios::app);

	ostringstream os;
	removeTerminatingChar(userName);
	removeTerminatingChar(password);
	os << endl << userName << ',' << password;
	addTerminatingChar(userName);
	addTerminatingChar(password);

	if (ofs.is_open())
	{
		ofs << os.str();
	}

	ofs.close();

	std::wstring wFolderName(userName.begin(), userName.end());

	BOOL success = CreateDirectory(wFolderName.c_str(), NULL);
	if (success) {
		std::cout << "Folder created successfully." << std::endl;
	}
	else {
		std::cout << "Folder creation failed" << std::endl;
	}

	removeTerminatingChar(userName);

	ostringstream posts;
	posts << userName << "/posts.txt";

	ofstream outFile;

	outFile.open(posts.str());
	if (outFile.is_open())
	{
		cout << "posts.txt file created";
	}
	else 
	{
		std::cout << "posts.txt file creation failed" << std::endl;
	}

	outFile.close();
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