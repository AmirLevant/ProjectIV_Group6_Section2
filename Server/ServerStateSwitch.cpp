#include "ServerStateSwtich.h"

using namespace std;

string serverSwitch(char* RxBuffer, SOCKET ConnectionSocket, User user)
{
	string returnString;
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
	case 7:
	{
		cout << "Logging user out..." << endl << endl;
		returnString = "logout";

		char* TxBuffer;

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

		break;
	}
	case 9:
	{
		cout << "Terminating connection to server" << endl << endl;
		returnString = "quit";
		break;
	}
	}
	return returnString;
}