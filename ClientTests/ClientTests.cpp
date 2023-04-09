#include "pch.h"
#include "CppUnitTest.h"
#include"C:\Users\drash\OneDrive\Desktop\Project 4\Client\PktDef.h"
#include"C:\Users\drash\OneDrive\Desktop\Project 4\Client\LogToFile.cpp"
#include"C:\Users\drash\OneDrive\Desktop\Project 4\Client\Post.h"


using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace ClientTests
{
	TEST_CLASS(PktDef_Tests)
	{
	public:
		TEST_METHOD(Setup) {
			pkt = PktDef();
		}

		TEST_METHOD(Teardown) {
			pkt = PktDef();
		}

		TEST_METHOD(TestInitialDateLength) {
			char expected = '\0';
			Assert::AreEqual(pkt.getDateLength(), expected);
		}

		TEST_METHOD(TestInitialUserNameLength) {
			char expected = '\0';
			Assert::AreEqual(pkt.getUserNameLength(), expected);
		}

		TEST_METHOD(TestInitialCaptionLength) {
			char expected = '\0';
			Assert::AreEqual(pkt.getCaptionLength(), expected);
		}

		TEST_METHOD(TestInitialImageLength) {
			char expected = '\0';
			Assert::AreEqual(pkt.getImageLength(), expected);
		}

		TEST_METHOD(TestInitialFirstPacketFlag) {
			Assert::AreEqual(pkt.getFirstPacket(), true);
		}

		TEST_METHOD(TestInitialPostFinishFlag) {
			Assert::AreEqual(pkt.getPostFinishFlag(), false);
		}
	private:
		PktDef pkt;
	};

	TEST_CLASS(Posts_Tests)
	{
	public:
		TEST_METHOD(Post_Getters)
		{
			// Initialize a new Post object
			Post post("Caption", "User", "2023-04-06", 10, "image.png");

			// Test getters
			Assert::AreEqual(post.getCaption(), std::string("Caption"));
			Assert::AreEqual(post.getName(), std::string("User"));
			Assert::AreEqual(post.getDate(), std::string("2023-04-06"));
			Assert::AreEqual(post.getLikeAmount(), 10);
			Assert::AreEqual(post.getFilePath(), std::string("image.png"));
		}

		TEST_METHOD(Post_Setters)
		{
			// Initialize a new Post object
			Post post("Caption", "User", "2023-04-06", 10, "image.png");

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
		}

		TEST_METHOD(Post_Reset)
		{
			// Initialize a new Post object
			Post post("Caption", "User", "2023-04-06", 10, "image.png");

			// Test reset method
			post.reset();
			Assert::AreEqual(post.getCaption(), std::string(""));
			Assert::AreEqual(post.getName(), std::string(""));
			Assert::AreEqual(post.getDate(), std::string(""));
			Assert::AreEqual(post.getLikeAmount(), 0);
		}

		TEST_METHOD(Post_Size_Initial)
		{
			// Initialize a new Post object
			Post post("", "", "", 0, "");

			// Test getPostSize method
			Assert::AreEqual(post.getPostSize(), 7); // 1+1+1+4 = 7
		}

		TEST_METHOD(Post_Size_After_Setters)
		{
			// Initialize a new Post object
			Post post("Caption", "User", "2023-04-06", 10, "image.png");

			// Test getPostSize method
			Assert::AreEqual(post.getPostSize(), 28); // 10 (date length) + 1 + 5 (username length) + 1 + 8 (caption length) + 1 + 4 (like amount)
		}

		TEST_METHOD(Post_Size_After_Reset)
		{
			// Initialize a new Post object
			Post post("Caption", "User", "2023-04-06", 10, "image.png");

			// Reset the object
			post.reset();

			// Test getPostSize method
			Assert::AreEqual(post.getPostSize(), 7); // 1+1+1+4 = 7

		}
		TEST_METHOD(Post_Size_With_Empty_Image_Path)
		{
			// Initialize a new Post object with empty image path
			Post post("Caption", "User", "2023-04-06", 10, "");

			// Test getters
			Assert::AreEqual(post.getCaption(), std::string("Caption"));
			Assert::AreEqual(post.getName(), std::string("User"));
			Assert::AreEqual(post.getDate(), std::string("2023-04-06"));
			Assert::AreEqual(post.getLikeAmount(), 10);
			Assert::AreEqual(post.getFilePath(), std::string(""));

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
			Assert::AreEqual(post.getPostSize(), 36); // 10 (date length) + 1 + 8 (username length) + 1 + 11 (caption length)  + 4 (like amount)
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
		TEST_METHOD(WritePostToFile_NonEmptyFilePath)
		{
			// Initialize a new Post object with non-empty image path
			Post post("Caption", "User", "2023-04-06", 10, "image.png");

			// Write post data to file
			writePostToFile(&post);

			// Check if the post data is written to the file correctly
			std::ifstream file("LogFile.txt");
			std::string line;
			bool found = false;
			while (std::getline(file, line)) {
				if (line == "File Path: image.png") {
					found = true;
					break;
				}
			}
			Assert::IsTrue(found);
		}

	};

}
