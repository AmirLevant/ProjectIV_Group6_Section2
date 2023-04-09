#include "MyForm.h"
#include <cstdio>

using namespace System;
using namespace System::Windows::Forms;


void createForm(int* imagesToDelete);

[STAThreadAttribute]
void main(cli::array<String^>^ args)
{
	int* imagesToDelete = new int;
	createForm(imagesToDelete);
	
	if (imagesToDelete > 0)
	{
		for (int i = 1; i <= *imagesToDelete; i++)
		{
			ostringstream os;
			os << i << ".jpg";

			int result = remove(os.str().c_str());
		}
	}
	delete imagesToDelete;
}


void createForm(int* imagesToDelete)
{
	Application::EnableVisualStyles();
	Application::SetCompatibleTextRenderingDefault(false);
	Client::MyForm form(imagesToDelete);
	Application::Run(% form);

}