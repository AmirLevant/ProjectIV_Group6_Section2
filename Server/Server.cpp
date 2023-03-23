#define _CRT_SECURE_NO_WARNINGS
#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")


#include <iostream>
#include<chrono>
#include<ctime>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#define PORT 8080
#define BUFFER_SIZE 1024

using namespace std;

int main()
{
    //starts Winsock DLLs		
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        return -1;

    //create server socket
    SOCKET ServerSocket;
    ServerSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (ServerSocket == INVALID_SOCKET) {
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
        closesocket(ServerSocket);
        WSACleanup();
        return -1;
    }

    std::cout << "Waiting for client packet" << std::endl;

    char response[BUFFER_SIZE];
   // Hard-coded username and password
    string hardcodedUsername = "john";
    string hardcodedPassword = "password";

    // Login
    cout << "Enter your username: ";
    string username;
    cin >> username;
    cout << "Enter your password: ";
    string password;
    cin >> password;

    if (username == hardcodedUsername && password == hardcodedPassword) {
        // Valid credentials
        string loginMsg = "LOGIN " + username + "\n";
        send(ServerSocket, loginMsg.c_str(), loginMsg.size(), 0);
        char response[BUFFER_SIZE];
        recv(ServerSocket, response, BUFFER_SIZE, 0);
        if (strcmp(response, "OK\n") != 0) {
            cout << "Login failed. Exiting..." << endl;
            closesocket(ServerSocket);
            WSACleanup();
            return 0;
        }
    }
    else {
        // Invalid credentials
        cout << "Invalid username or password. Exiting..." << endl;
        closesocket(ServerSocket);
        WSACleanup();
        return 0;
    }

    cout << "Logged in successfully." << endl;
    bool isLoggedIn = true; // Assuming that the user is already logged in
    //TO DO: Receive .jpeg / caption
    char buffer[BUFFER_SIZE];
    memset(buffer, 0, BUFFER_SIZE);
    int bytesReceived = recv(ServerSocket, buffer, BUFFER_SIZE, 0);
    if (bytesReceived > 0) {
        std::cout << "Received: " << buffer << std::endl;
    }

    // TO DO: Track time of received data so that user can refresh his/her memories
    auto receivedTime = std::chrono::system_clock::now();
    std::time_t receivedTimeT = std::chrono::system_clock::to_time_t(receivedTime);
    std::string timeString = std::ctime(&receivedTimeT);
    std::cout << "Data received on " << timeString << std::endl;

    //TO DO: Save it to file
    std::ofstream outFile;
    outFile.open("received_data.txt", std::ios::out | std::ios::app);
    if (outFile.is_open()) {
        outFile << timeString << " " << buffer << std::endl;
        outFile.close();
    }

    //TO DO: send confirmation to client
    const char* confirmation = "Data received";
    send(ServerSocket, confirmation, strlen(confirmation), 0);

    // Logout
    string logoutMsg = "LOGOUT\n";
    send(ServerSocket, logoutMsg.c_str(), logoutMsg.size(), 0);
    recv(ServerSocket, response, BUFFER_SIZE, 0);
    if (strcmp(response, "OK\n") != 0) {
        cout << "Logout failed. Continuing..." << endl;
    }

    //closes connection and socket
    closesocket(ServerSocket);

    //frees Winsock DLL resources
    WSACleanup();

    //This code has been added to simply keep the console window open until you
    //type a character.

    int garbage;
    cin >> garbage;

    return 1;

}
