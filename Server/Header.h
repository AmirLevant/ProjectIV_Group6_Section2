#pragma once
#include <iostream>
#include<cstring>
#include<string>
class PktDef
{
	// ***** YOUR CODE GOES HERE ****** //
	struct Header
	{
		unsigned char ScrAddr : 4;
		unsigned char DestAddr : 4;
		unsigned char dataType;			// Type of data (0 = image, 1 = caption, 2 = username) 
		unsigned short messageSize;		// Total size of the message (data) being sent
		bool imageFinishFlag : 1;		// Set to true (1) when all bytes of the image have been sent
		bool postFinishFlag : 1;	// Set to true when an entire post has been sent
	};

	struct Packet
	{
		Header Head;
		char* Data;
	} packet;

	char* pSerialBuff;

public:
	PktDef()
	{
		memset(&packet.Head, 0, sizeof(packet.Head));
		packet.Data = nullptr;
		pSerialBuff = nullptr;
	}

	PktDef(char* buffer)
	{
		int offset = 0;		// To skip characters in the buffer

		memcpy(&packet.Head, buffer, sizeof(char));
		offset += sizeof(char);

		memcpy(&packet.Head.dataType, buffer + offset, sizeof(packet.Head.dataType));
		offset += sizeof(packet.Head.dataType);

		memcpy(&packet.Head.messageSize, buffer + offset, sizeof(packet.Head.messageSize));
		offset += sizeof(packet.Head.messageSize);

		memcpy(&packet.Head.messageSize + sizeof(packet.Head.messageSize), buffer + offset, sizeof(char));
		offset += sizeof(char);

		packet.Data = new char[packet.Head.messageSize];
		memcpy(packet.Data, buffer + offset, packet.Head.messageSize);
	}

	void setSourceValue(unsigned int source)
	{
		packet.Head.ScrAddr = (char)source;
	}

	void setDestinationValue(unsigned int dest)
	{
		packet.Head.DestAddr = (char)dest;
	}

	void setDataType(string type)
	{
		if (type == "image")
			packet.Head.dataType = 0;
		else if (type == "caption")
			packet.Head.dataType = 1;
		else if (type == "username")
			packet.Head.dataType = 2;
	}

	unsigned char getDataType()
	{
		return packet.Head.dataType;
	}

	void setMessageSize(unsigned short dataSize)
	{
		packet.Head.messageSize = dataSize;
	}

	unsigned short getMessageSize()
	{
		packet.Head.messageSize;
	}

	void setImageFinishFlag(bool flag)
	{
		packet.Head.imageFinishFlag = flag;
	}

	bool getImageFinishFlag()
	{
		return packet.Head.imageFinishFlag;
	}

	void setPostFinishFlag(bool flag)
	{
		packet.Head.postFinishFlag = flag;
	}

	bool getPostFinishFlag()
	{
		return packet.Head.postFinishFlag;
	}

	void swapDestination()
	{
		char temp;
		temp = packet.Head.ScrAddr;
		packet.Head.ScrAddr = packet.Head.DestAddr;
		packet.Head.DestAddr = temp;
	}

	void setData(char* buffer, int size)
	{
		if (packet.Data)
			delete[] packet.Data;

		packet.Data = new char[size];
		memcpy(packet.Data, buffer, size);
		packet.Head.messageSize = size;

	}

	char* getAddressOfData()
	{
		return packet.Data;
	}

	char* SerializeData(int& size)		// Size will be size of total packet
	{
		if (pSerialBuff)
			delete[] pSerialBuff;

		size = sizeof(char) + sizeof(packet.Head.dataType) + sizeof(packet.Head.messageSize) + sizeof(char) + packet.Head.messageSize;
		pSerialBuff = new char[size];		// Character array should be able to hold data of entire packet
		int offset = 0;

		memcpy(pSerialBuff, &packet.Head, sizeof(char));
		offset += sizeof(char);

		memcpy(pSerialBuff + offset, &packet.Head.dataType, sizeof(packet.Head.dataType));
		offset += sizeof(packet.Head.dataType);

		memcpy(pSerialBuff + offset, &packet.Head.messageSize, sizeof(packet.Head.messageSize));
		offset += sizeof(packet.Head.messageSize);

		memcpy(pSerialBuff + offset, &packet.Head.messageSize + sizeof(packet.Head.messageSize), sizeof(char));		// Copying the 3 bits for flags plus 5 bits of padding
		offset += sizeof(char);

		memcpy(pSerialBuff + offset, &packet.Data, packet.Head.messageSize);

		return pSerialBuff;
	}

	~PktDef()		// Destructor for packets to delete allocated memory
	{
		if (packet.Data)
		{
			delete[] packet.Data;
		}
		if (pSerialBuff)
		{
			delete[] pSerialBuff;
		}
	}
};