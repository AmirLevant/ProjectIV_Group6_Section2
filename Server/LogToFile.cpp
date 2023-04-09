#include "LogToFile.h"

void writePostToFile(Post* post)
{
	ofstream ofs;

	ofs.open("LogFile.txt", ios::app);

	if (ofs.is_open())
	{
		ofs << "Saved Post Data: " << std::endl;
		ofs << post->getDate() << std::endl;
		ofs << "User Name: " << post->getName() << std::endl;
		ofs << "Caption: " << post->getCaption() << std::endl;
		ofs << "File Path: " << post->getFilePath() << std::endl;
		ofs << "Like Amout: " << post->getLikeAmount() << std::endl << std::endl;
	}

	ofs.close();
}

void writePacketRawDataToFile(char* buffer, int size, string status) // Where size is the size of the packet
{
	ofstream ofs;

	ofs.open("LogFile.txt", ios::app);

	if (ofs.is_open())
	{
		ofs << status << " Packet Data: " << std::endl;
		ofs.write(buffer, size);
		ofs << std::endl << std::endl;
	}

	ofs.close();
}