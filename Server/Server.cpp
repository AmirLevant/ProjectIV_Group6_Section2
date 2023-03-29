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
    bool isLoggedIn = false;
    while (!isLoggedIn) {
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
                cout << "Login failed. Please try again." << endl;
            }
            else {
                isLoggedIn = true;
                cout << "Logged in successfully." << endl;
            }
        }
        else {
            // Invalid credentials
            cout << "Invalid username or password. Please try again." << endl;
        }
    }

    // Receive .jpeg with caption
    while (true) {
        char buffer[BUFFER_SIZE];
        memset(buffer, 0, BUFFER_SIZE);
        int bytesReceived = recv(ServerSocket, buffer, BUFFER_SIZE, 0);
        if (bytesReceived > 0) {
            std::cout << "Received: " << buffer << std::endl;

            // Track time of received data
            auto receivedTime = std::chrono::system_clock::now();
            std::time_t receivedTimeT = std::chrono::system_clock::to_time_t(receivedTime);
            std::string timeString = std::ctime(&receivedTimeT);
            std::cout << "Data received on " << timeString << std::endl;

            // Save data to file
            std::ofstream outFile;
            outFile.open("received_data.txt", std::ios::out | std::ios::app);
            if (outFile.is_open()) {
                outFile << timeString << " " << buffer << std::endl;
                outFile.close();
            }

            // Send confirmation to client
            const char* confirmation = "Data received";
            send(ServerSocket, confirmation, strlen(confirmation), 0);
        }
    }

    // Receive request to show all saved data
    char request[BUFFER_SIZE];
    recv(ServerSocket, request, BUFFER_SIZE, 0);
    if (strcmp(request, "SHOW_DATA\n") == 0) {
        // Open file and read all saved data
        std::ifstream inFile;
        inFile.open("received_data.txt", std::ios::in);
        if (inFile.is_open()) {
            std::string line;
            while (std::getline(inFile, line)) {
                send(ServerSocket, line.c_str(), line.size(), 0);
                recv(ServerSocket, response, BUFFER_SIZE, 0);
                if (strcmp(response, "OK\n") != 0) {
                    cout << "Failed to send data. Continuing..." << endl;
                }
            }
            inFile.close();
        }
    }

    // Logout
    string logoutMsg = "LOGOUT\n";
    send(ServerSocket, logoutMsg.c_str(), logoutMsg.size(), 0);
    recv(ServerSocket, response, BUFFER_SIZE, 0);
    if (strcmp(response, "OK\n") != 0) {
        cout << "Logout failed. Continuing..." << endl;
    }

    // Close connection and socket
    closesocket(ServerSocket);

    // Free Winsock DLL resources

    WSACleanup();

    int garbage;
    cin >> garbage;

    return 1;

}


