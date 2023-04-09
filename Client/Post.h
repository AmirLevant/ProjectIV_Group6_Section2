#pragma once
#include <iostream>
#include <string.h>
#include <fstream>
#include <stdio.h>
#include <string>
#include <vector>

using namespace std;

class Post
{
	private:
		string postDate;
		string postUserName;
		string postCaption;
		int postLikeAmount;
		string imageFilePath;

	public:
		Post()
		{
			postCaption = "";
			postUserName = "";
			postDate = "";
			postLikeAmount = 0;
			imageFilePath = "";
		}

		Post(string newCaption, string newName, string newDate, int newAmount, string filePath)
		{
			postCaption = newCaption;
			postUserName = newName;
			postDate = newDate;
			postLikeAmount = newAmount;
			imageFilePath = filePath;
		}

		/*Post(const Post& post) {
			this->postCaption = post.postCaption;
			this->postUserName = post.postUserName;
			this->postCaption = post.postCaption;
			this->postLikeAmount = post.postLikeAmount;
			this->imageFilePath = post.imageFilePath;
		}*/

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

		void setFilePath(string filePath) // Post Image
		{
			imageFilePath = filePath;
		}

		string getFilePath()
		{
			return imageFilePath;
		}

		void reset()
		{
			postCaption = "";
			postUserName = "";
			postDate = "";
			postLikeAmount = 0;
		}

		int getPostSize()		// Size of post data for packet
		{
			int size = postDate.length() + 1 + postUserName.length() + 1 + postCaption.length() + 1 + sizeof(int);
			return size;
		}

		~Post() = default;
};
