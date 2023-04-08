#pragma once
#include <iostream>
#include <windows.networking.sockets.h>
#include <string>
#include <fstream>
#include "../Client/PktDef.h"

using namespace std;

string beginUserLogin(SOCKET ConnectionSocket);
string loginUser(SOCKET ConnectionSocket, bool& userLoggedIn);
bool checkForUser(string userName, string password);
