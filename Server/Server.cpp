#include "UserLogin.h"
#include "LogToFile.h"
#include "ServerStateSwtich.h"
#include "User.h"
#include <time.h>

SOCKET createSocket();
SOCKET listenForConnection(SOCKET ServerSocket);

int main()
{
	srand(time(NULL));

	SOCKET ServerSocket = createSocket();
	if (ServerSocket < 0)
		return 1;

	SOCKET ConnectionSocket;
	string quitStatus;
	do
	{
		ConnectionSocket = listenForConnection(ServerSocket);
		if (ConnectionSocket < 0)
			return 1;

		string userName = beginUserLogin(ConnectionSocket);

		User newUser;
		newUser.setUserName(userName);
		newUser.loadPosts();
		newUser.sendAllPosts(ConnectionSocket);

		do
		{
			char RxBuffer[1024];

			recv(ConnectionSocket, RxBuffer, sizeof(RxBuffer), 0);

			quitStatus = serverSwitch(RxBuffer, ConnectionSocket, newUser);

		} while (quitStatus != "logout" && quitStatus != "quit");
	} while (quitStatus != "quit");
	
	closesocket(ConnectionSocket);	//closes incoming socket
	closesocket(ServerSocket);	    //closes server socket	
	WSACleanup();					//frees Winsock resources
	return 0;
}

SOCKET createSocket()
{
	//starts Winsock DLLs		
	WSADATA wsaData;
	if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{
		std::cout << "Failed to initialize Winsock library" << std::endl;
		return -1;
	}

	//create server socket
	SOCKET ServerSocket;
	ServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (ServerSocket == INVALID_SOCKET)
	{
		std::cout << "Failed to create socket" << std::endl;
		WSACleanup();
		return -1;
	}

	//binds socket to address
	sockaddr_in SvrAddr;
	SvrAddr.sin_family = AF_INET;
	SvrAddr.sin_addr.s_addr = INADDR_ANY;
	SvrAddr.sin_port = htons(27000);
	if (bind(ServerSocket, (struct sockaddr*)&SvrAddr, sizeof(SvrAddr)) == SOCKET_ERROR)
	{
		std::cout << "Failed to bind socket to port " << 27000 << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return -1;
	}

	return ServerSocket;
}

SOCKET listenForConnection(SOCKET ServerSocket)
{
	//listen on a socket
	if (listen(ServerSocket, 1) == SOCKET_ERROR) {
		std::cout << "Failed to listen for incoming connections" << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return -1;
	}

	std::cout << "Waiting for client connection\n" << std::endl;

	//accepts a connection from a client
	SOCKET ConnectionSocket;
	ConnectionSocket = SOCKET_ERROR;
	if ((ConnectionSocket = accept(ServerSocket, NULL, NULL)) == SOCKET_ERROR) {
		std::cout << "Failed to accept incoming connection" << std::endl;
		closesocket(ServerSocket);
		WSACleanup();
		return -1;
	}

	std::cout << "Connected to client" << endl << endl;

	return ConnectionSocket;
}






