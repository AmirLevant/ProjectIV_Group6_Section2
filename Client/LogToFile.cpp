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

void writePacketRawDataToFile(char* buffer, int size) // Where size is the size of the packet
{
	ofstream ofs;

	ofs.open("LogFile.txt", ios::app);

	if (ofs.is_open())
	{
		ofs << "Packet Data: " << std::endl; 
		/*ofs << "Date Length: " << packet.getDateLength() << std::endl;
		ofs << "User Name Length: " << packet.getUserNameLength() << std::endl;
		ofs << "Caption Length: " << packet.getCaptionLength() << std::endl;
		ofs << "Image Length " << packet.getImageLength() << std::endl;
		ofs << "First Packet Status: " << packet.getFirstPacket() << std::endl;
		ofs << "Post Finish Status: " << packet.getPostFinishFlag() << std::endl;*/
		ofs.write(buffer, size);
		ofs << std::endl;
	}

	ofs.close();
}
