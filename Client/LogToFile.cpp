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