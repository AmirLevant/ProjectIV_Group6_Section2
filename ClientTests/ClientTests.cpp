#include "pch.h"
#include "CppUnitTest.h"
#include"C:\Users\drash\OneDrive\Desktop\Project 4\Client\PktDef.h"
#include"C:\Users\drash\OneDrive\Desktop\Project 4\Client\LogToFile.cpp"
#include"C:\Users\drash\OneDrive\Desktop\Project 4\Client\Post.h"
//#include "../Project 4/Client/PktDef.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClientTests
{
	TEST_CLASS(PktDef_Tests)
	{
	public:

		TEST_METHOD(PktDefTests, Initialize) {
			PktDef pkt;
			char expected = '\0';
			Assert::AreEqual(pkt.getDateLength(), expected);
			Assert::AreEqual(pkt.getUserNameLength(), expected);
			Assert::AreEqual(pkt.getCaptionLength(), expected);
			Assert::AreEqual(pkt.getImageLength(), expected);
			Assert::AreEqual(pkt.getFirstPacket(), true);
			Assert::AreEqual(pkt.getPostFinishFlag(), false);
			// Assert::IsNull(pkt.parseData(data));
		}

	};
	TEST_CLASS(Post_Tests)
	{
	public:

		TEST_METHOD(PostTests)
		{
			// Initialize a new Post object
			Post post("Caption", "User", "2023-04-06", 10, "image.png");

			// Test getters
			Assert::AreEqual(post.getCaption(), std::string("Caption"));
			Assert::AreEqual(post.getName(), std::string("User"));
			Assert::AreEqual(post.getDate(), std::string("2023-04-06"));
			Assert::AreEqual(post.getLikeAmount(), 10);
			Assert::AreEqual(post.getFilePath(), std::string("image.png"));

			// Test setters
			post.setCaption("New Caption");
			Assert::AreEqual(post.getCaption(), std::string("New Caption"));
			post.setName("New User");
			Assert::AreEqual(post.getName(), std::string("New User"));
			post.setDate("2023-04-07");
			Assert::AreEqual(post.getDate(), std::string("2023-04-07"));
			post.setLikeAmount(20);
			Assert::AreEqual(post.getLikeAmount(), 20);
			post.setFilePath("new_image.png");
			Assert::AreEqual(post.getFilePath(), std::string("new_image.png"));

			// Test getPostSize method
			Assert::AreEqual(post.getPostSize(), 36); // 10 (date length) + 1 + 8 (username length) + 1 + 11 (caption length) + 1 + 4 (like amount)
			// Test reset method
			post.reset();
			Assert::AreEqual(post.getCaption(), std::string(""));
			Assert::AreEqual(post.getName(), std::string(""));
			Assert::AreEqual(post.getDate(), std::string(""));
			Assert::AreEqual(post.getLikeAmount(), 0);

			// Test getPostSize method
			Assert::AreEqual(post.getPostSize(), 7); //after resetting 1+1+1+4 = 7// 10 (date length) + 5 (date string) + 6 (username length) + 4 (username string) + 10 (caption length) + 4 (caption string) + 4 (like amount)
		}

	};
	TEST_CLASS(LogToFile_Tests)
	{
	public:
		

	};

}
