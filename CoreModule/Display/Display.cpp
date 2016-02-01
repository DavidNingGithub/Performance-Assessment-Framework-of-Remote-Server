/////////////////////////////////////////////////////////////////////////////
// // Display.cpp               Display the received message information   //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
//                                                                         //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2013                             //
// Platform:    Dell XPS 8300, Core i7, Windows 7 SP1                      //
// Application: Summer Projects, 2012                                      //
// Author:      Tao Ning   , CST 4-187, Syracuse University                //
//              (315) 443-3948, jfawcett@twcny.rr.com                      //
/////////////////////////////////////////////////////////////////////////////
/*
* This Display expects to Display the information in the message from server or client 
* 
*
* For HTTP like protocols the Reciever should expect
* lines in the header and bytes in the body, if there is one,
* specified by a header line something like:
*    content_length : 1024
* where 1024 is a stand-in for whatever you want your block
* size to be.
*
*/
/*
* Required files:
* - 
*   
*   Message.h Message.cpp,Display.h
*F
* Maintanence History:
* ver 1.1 - 30 Mar 2014
* - changes to error handling
* ver 1.0 - 29 Mar 2014
* - first release
*/
#include "../Display/Display.h"
#include <iostream>
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DisplayMessage function  can display message information
void Display::DisplayMessage(Message test)
{
	std::cout << "   Communicator:     " << test.Header() << "\n";
	std::cout << "   SourcePort:       " << test.SourcePort() << "\n";
	std::cout << "   SourceIp:         " << test.SourceCom() << "\n";
	
	std::cout << "   FileName:         " << test.Filename() << "\n";
	std::cout << "   Size:             " << test.Size() << "\n";
	if (test.Header() == "file")
		std::cout << "   Content:          The content of the message is the code of file.\n                     It's too large to display.\n";
	else	
		std::cout << "   Content:          " << test.Body() << "\n";
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//sender function  can display message information in sender 
void Display::sender(int id, std::string test)
{
	std::cout << "=========================================================================\n";
	std::cout << "---The Client send a download request to Sever through sender# " << id << "\n";
	std::cout << "---The Client want to download the file or files under the path: \"" << test.c_str()<<"\"\n";
	std::cout << "=========================================================================\n\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//sender function  can display message information in sender 
void Display::TextS(std::vector<std::string> keyword, std::vector<std::string> filename)
{
	std::cout << "=========================================================================\n";
	std::cout << "---The server received a request to search\n";
	for (size_t i = 0; i < keyword.size(); i++)
	{
		std::cout << "---"<<keyword[i]<<"\n";
	}
	std::cout << "---In the file:\n";
	for (size_t i = 0; i < filename.size();i++)
	{
		int pos = filename[i].find_last_of("\\");
		std::string temp = filename[i].substr(0, pos);
		int pos2 = temp.find_last_of("\\");
		std::string Filename = filename[i].substr(pos2);
		std::cout << "---" << Filename << "\n";
	}
	std::cout << "==========================================================================\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//sender1 function  can display message information in sender 

void Display::sender1(int id, std::string test)
{
	std::cout << "=========================================================================\n";
	std::cout << "---The Client send a message to Sever through sender# " << id << "\n";
	std::cout << "---The content of the message:\n---" << test.c_str()<<"\n";
	std::cout << "=========================================================================\n\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//sendfile function  can display message information in sender
void Display::sendfile(int ID,std::string FILE)
{
	std::cout << "=========================================================================\n";
	std::cout << "---The Client send a file to Sever through sender# " << ID << "\n";
	std::cout << "---The name of the file: " << FILE<<"\n";
	std::cout << "---The file is originally stored in the folder \"test\"\n";
	std::cout << "=========================================================================\n\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//sendfile function  can display message information in sender
void Display::sendfile2(std::string FILE)
{
	std::cout << "=========================================================================\n";
	std::cout << "---The Sever send a file named "<< FILE<<" to Client\n";
	std::cout << "=========================================================================\n\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dpro function  can display message information in echo communicator
void Display::Dpro(std::string senderid, std::string content)
{
	std::cout << "=========================================================================\n";
	std::cout << "----Sever received a message from " << senderid << "\n";
	std::cout << "----The content of the message is " <<content << "\n";
	std::cout << "=========================================================================\n\n";
	std::cout << "\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dpro1 function  can display message information in echo communicator in client
void Display::Dpro4(int Duration,std::string content)
{
	std::cout << "=========================================================================\n";
	std::cout << "---This is a message received from sever.\n---The content of the message:\n";
	std::cout <<"---" <<content;
	std::cout << "---The duration of time from sending request to receive the reply is " << Duration<< "ms\n";
	std::cout << "=========================================================================";
	std::cout << "\n";
}
void Display::Dpro1(int Duration, std::string content)
{
	std::cout << "=========================================================================\n";
	std::cout << "---This is a message received from sever.\n---The content of the message:\n";
	std::cout <<  content;
	std::cout << "The duration of time from sending request to receive the reply is " << Duration << "ms\n";
	//std::cout << "=========================================================================";
	std::cout << "\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dpro function  can display message information in echo communicator
void Display::Dpro2(std::string senderid, std::vector<std::string> content)
{
	std::cout << "=========================================================================\n";
	std::cout << "----Sever received a download request from " << senderid << "\n";
	for (size_t i = 0; i < content.size(); i++)
	{
		int pos1 = content[i].find_last_of("\\");
		std::string temp = content[i].substr(pos1+1);
		std::cout << "----The client want to download the file named \" " << temp << "\"\n";
	}
	std::cout << "=========================================================================\n\n";
	std::cout << "\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//file function  can display message information in file communicator 
void Display::file(std::string filename, std::string folder)
{
	std::cout << "=========================================================================\n";
	std::cout << "----Sever receiverd a file named \"" << filename << "\"\n";
	std::cout << "----The file is stored in the folder \""<<folder << "\"\n";
	std::cout << "=========================================================================\n\n";
	std::cout << "\n";
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//file1 function  can display message information in file communicator
void Display::file1(int Duration,std::string filename, std::string folder)
{
	std::cout << "=========================================================================\n";
	std::cout << "----Client receiverd a file named \"" << filename << "\"\n";
	std::cout << "----The file is stored in the folder \""<<folder << "\"\n";
	std::cout << "---The duration of time from sending request to receive the reply is " << Duration << "ms\n";
	std::cout << "=========================================================================";
	std::cout << "\n";
	std::cout << "=========================================================================\n\n";
	std::cout << "\n";
}
std::string Display::file12(int Duration,std::string filename, std::string folder)
{
	std::string use= "====================================================\n";
	use+="----Client receiverd a file named \"" + filename + "\"\n";
	use+= "----The file is stored in the folder \""+folder + "\"\n";
	use+= "====================================================\n\n";
	return use;
}
#ifdef TEST_DISPLAY
void main()
{
	std::cout << "\n  Test Display class";
	std::cout << "\n ==========================================================\n";
	try
	{
		std::cout << "\n  Test function DisplayMessage()";
		std::cout << "\n ==========================================================\n";
		Message test1;
		test1.MakeMessage("Destination", "Myip", "Myport", 11, "FileName", "Content");
		Display test;
		test.DisplayMessage(test1);
		std::cout << "\n  Test DisplayMessage() while the head of the message is file";
		std::cout << "\n ==========================================================\n";

		test1.Header() = "file";
		test.DisplayMessage(test1);
		std::cout << "\n  Test function Dpro(),Dpro1(),file() and sender()";
		std::cout << "\n ==========================================================\n\n\n";
		std::cout << "sender()\n";
		test.sender(1, "content\n");
		std::cout << "Dpro()\n";
		test.Dpro("Sender1", "Message from Sender1");
		std::cout << "Dpro1()\n";
		test.Dpro1(87,"Sever received a message from client\n");
		std::cout << "file()\n";
		test.file("filename", "folder1");
	}
	catch (std::exception& e)
	{
		std::cout << "\n  " << e.what() << "\n\n";
	}
	std::cout << "\n\n";
}


#endif