#pragma once
#include <iostream>
#include <string.h>

using namespace std;

class Post
{
	private:
		string postCaption;
		string postUserName;
		string postDate;
		int postLikeAmount;
		char* postImage;


	public:
		Post()
		{
			postCaption = "";
			postUserName = "";
			postDate = "";
			postLikeAmount = 0;
			postImage = nullptr;
		}

		Post(string newCaption, string newName, string newDate, int newAmount, char* newImage)
		{
			postCaption = newCaption;
			postUserName = newName;
			postDate = newDate;
			postLikeAmount = newAmount;
			postImage = newImage;
		}

		~Post()
		{
			if(postImage)
				delete[] postImage;
		}

		void setCaption(string newCaption) // Post Caption
		{
			postCaption = newCaption;
		}

		string getCaption()
		{
			return postCaption;
		}

		void setName(string newName) // Post User Name
		{
			postUserName = newName;
		}

		string getName()
		{
			return postUserName;
		}

		void setDate(string newDate) // Post Date
		{
			postDate = newDate;
		}

		string getDate()
		{
			return postDate;
		}

		void setLikeAmount(int newLike) // Post LikeAmount
		{
			postLikeAmount = newLike;
		}

		int getLikeAmount()
		{
			return postLikeAmount;
		}

		void setImage(char* newImage) // Post Image
		{
			postImage = newImage;
		}

		char* getImage()
		{
			return postImage;
		}
	
};
