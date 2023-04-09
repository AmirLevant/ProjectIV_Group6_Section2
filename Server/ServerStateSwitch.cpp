#include "ServerStateSwtich.h"

using namespace std;

bool serverSwitch(char* RxBuffer, SOCKET ConnectionSocket, User user)
{
	bool logout = false;
	PktDef menuChoice(RxBuffer);
	switch (menuChoice.getMessageType())
	{
	case 0:
	{
		cout << "Message Type not set" << endl << endl;
		break;
	}
	case 1:
	{
		cout << "User not logged out" << endl << endl;
		break;
	}
	case 2:
	{
		cout << "Post incoming to server" << endl << endl;
		user.receivePost(ConnectionSocket, RxBuffer);
		break;

	}
	}
	return logout;
}