#pragma once
#include <windows.networking.sockets.h>
#pragma comment(lib, "Ws2_32.lib")

#include "Post.h"

class PktDef
{
	// ***** YOUR CODE GOES HERE ****** //
	struct Header
	{
		unsigned char messageType;			// 0 = Not set, 1 = User login, 2 = Posting to server, 3 = Requesting posts from server, 4 = Acknowledgement, 5 = Request to delete a post, 6 = Request to edit caption
		unsigned char dateLength;
		unsigned char userNameLength;
		unsigned char captionLength;
		unsigned short imageLength;			// Length of image data in this specific packet
		bool firstPacket;		// Set to true if is the first packet and so has more information that just the image data
		bool postFinishFlag;
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
		packet.Head.firstPacket = true;
		packet.Data = nullptr;
		pSerialBuff = nullptr;
	}

	PktDef(char* buffer)
	{
		int offset = 0;		// To skip characters in the buffer

		memcpy(&packet.Head.messageType, buffer, sizeof(packet.Head.messageType));
		offset += sizeof(packet.Head.messageType);

		memcpy(&packet.Head.dateLength, buffer + offset, sizeof(packet.Head.dateLength));
		offset += sizeof(packet.Head.dateLength);

		memcpy(&packet.Head.userNameLength, buffer + offset, sizeof(packet.Head.userNameLength));
		offset += sizeof(packet.Head.userNameLength);

		memcpy(&packet.Head.captionLength, buffer + offset, sizeof(packet.Head.captionLength));
		offset += sizeof(packet.Head.captionLength);

		memcpy(&packet.Head.imageLength, buffer + offset, sizeof(packet.Head.imageLength));
		offset += sizeof(packet.Head.imageLength);

		memcpy(&packet.Head.firstPacket, buffer + offset, sizeof(packet.Head.firstPacket));
		offset += sizeof(packet.Head.firstPacket);

		memcpy(&packet.Head.postFinishFlag, buffer + offset, sizeof(packet.Head.postFinishFlag));
		offset += sizeof(packet.Head.postFinishFlag);

		int size = packet.Head.dateLength + packet.Head.userNameLength + packet.Head.captionLength + sizeof(int) + packet.Head.imageLength;
		packet.Data = new char[size];
		memcpy(packet.Data, buffer + offset, size);
	}

	char* parseData(Post* post)		// Returning pointer to image data
	{
		int offset = 0;
		if (packet.Head.firstPacket)
		{
			string date;
			//memcpy(&date[0], packet.Data, packet.Head.dateLength);
			date.assign(packet.Data, packet.Head.dateLength);
			offset += packet.Head.dateLength;

			string userName;
			userName.assign(packet.Data + offset, packet.Head.userNameLength);
			offset += packet.Head.userNameLength;

			string caption;
			caption.assign(packet.Data + offset, packet.Head.captionLength);
			offset += packet.Head.captionLength;

			int likes;
			memcpy(&likes, packet.Data + offset, sizeof(int));
			offset += sizeof(int);

			post->setDate(date);
			post->setName(userName);
			post->setCaption(caption);
			post->setLikeAmount(likes);
		}
		else						// Don't set post data if not first packet as that data has been cleared
			offset = packet.Head.dateLength + packet.Head.userNameLength + packet.Head.captionLength + sizeof(int);

		return (packet.Data + offset);
	}

	void setMessageType(int messageType)
	{
		packet.Head.messageType = (char)messageType;
	}

	char getMessageType()
	{
		return packet.Head.messageType;
	}

	void setDateLength(int length)
	{
		packet.Head.dateLength = (char)length;
	}

	char getDateLength()
	{
		return packet.Head.dateLength;
	}

	void setUserNameLength(int length)
	{
		packet.Head.userNameLength = (char)length;
	}

	char getUserNameLength()
	{
		return packet.Head.userNameLength;
	}

	void setCaptionLength(int length)
	{
		packet.Head.captionLength = (char)length;
	}

	char getCaptionLength()
	{
		return packet.Head.captionLength;
	}

	void setImageLength(int length)
	{
		packet.Head.imageLength = (char)length;
	}

	char getImageLength()
	{
		return packet.Head.imageLength;
	}

	void setFirstPacket(bool boolean)
	{
		packet.Head.firstPacket = boolean;
	}

	bool getFirstPacket()
	{
		return packet.Head.firstPacket;
	}

	void setPostFinishFlag(bool boolean)
	{
		packet.Head.postFinishFlag = boolean;
	}

	bool getPostFinishFlag()
	{
		return packet.Head.postFinishFlag;
	}

	int setData(Post* post, char* imageData, int size)			// Where size is the length of image data being sent in the specific packet
	{
		if (packet.Data)
			delete[] packet.Data;

		int totalSize;

		packet.Head.dateLength = (unsigned char)(post->getDate().length() + 1);
		packet.Head.userNameLength = (unsigned char)(post->getName().length() + 1);
		packet.Head.captionLength = (unsigned char)(post->getCaption().length() + 1);
		packet.Head.imageLength = (unsigned short)size;

		totalSize = packet.Head.dateLength + packet.Head.userNameLength + packet.Head.captionLength + sizeof(int) + packet.Head.imageLength;		// totalSize is size of data;
		packet.Data = new char[totalSize];
		
		int offset = 0;

		memcpy(packet.Data, &post->getDate()[0], packet.Head.dateLength);
		offset += packet.Head.dateLength;

		memcpy(packet.Data + offset, &post->getName()[0], packet.Head.userNameLength);
		offset += packet.Head.userNameLength;

		memcpy(packet.Data + offset, &post->getCaption()[0], packet.Head.captionLength);
		offset += packet.Head.captionLength;

		*(int*)(packet.Data + offset) = post->getLikeAmount();
		/*int value = post->getLikeAmount();
		memcpy(packet.Data + offset, &post->getLikeAmount(), sizeof(int));*/
		offset += sizeof(int);

		memcpy(packet.Data + offset, imageData, packet.Head.imageLength);

		post->reset();

		return totalSize;
	}

	char* getAddressOfData()
	{
		return packet.Data;
	}

	char* SerializeData(int& size, int dataSize)		// Size will be size of total packet, dataSize is size of the data
	{
		if (pSerialBuff)
			delete[] pSerialBuff;

		size = getHeaderSize() + dataSize;

		pSerialBuff = new char[size];		// Character array should be able to hold data of entire packet
		int offset = 0;

		memcpy(pSerialBuff, &packet.Head.messageType, sizeof(packet.Head.messageType));
		offset += sizeof(packet.Head.messageType);

		memcpy(pSerialBuff + offset, &packet.Head.dateLength, sizeof(packet.Head.dateLength));		
		offset += sizeof(packet.Head.dateLength);

		memcpy(pSerialBuff + offset, &packet.Head.userNameLength, sizeof(packet.Head.userNameLength));
		offset += sizeof(packet.Head.userNameLength);

		memcpy(pSerialBuff + offset, &packet.Head.captionLength, sizeof(packet.Head.captionLength));
		offset += sizeof(packet.Head.captionLength);

		memcpy(pSerialBuff + offset, &packet.Head.imageLength, sizeof(packet.Head.imageLength));
		offset += sizeof(packet.Head.imageLength);

		memcpy(pSerialBuff + offset, &packet.Head.firstPacket, sizeof(packet.Head.firstPacket));
		offset += sizeof(packet.Head.firstPacket);

		memcpy(pSerialBuff + offset, &packet.Head.postFinishFlag, sizeof(packet.Head.postFinishFlag));
		offset += sizeof(packet.Head.postFinishFlag);

		memcpy(pSerialBuff + offset, packet.Data, dataSize);

		return pSerialBuff;
	}

	int getHeaderSize()
	{
		return sizeof(packet.Head.messageType) + sizeof(packet.Head.dateLength) + sizeof(packet.Head.userNameLength) + sizeof(packet.Head.captionLength) + sizeof(packet.Head.imageLength)
			+ sizeof(packet.Head.firstPacket) + sizeof(packet.Head.postFinishFlag);
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