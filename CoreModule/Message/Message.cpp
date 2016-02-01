/////////////////////////////////////////////////////////////////////////////
// // Message.cpp transfer a message between message type and string type  //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
//                                                                         //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2014                             //
// Platform:    Dell XPS 8300, Core i7, Windows 7 SP1                      //
// Application: Summer Projects, 2012                                      //
// Author:      Tao Ning   , CST 4-187, Syracuse University                //
//              (315) 443-3948, tning.rr.com                               //
/////////////////////////////////////////////////////////////////////////////
/*
* This Message expects to analyze lines of text only.
* So message framing is done by lines.
*
* For HTTP like protocols the Message should expect
* lines in the header and bytes in the body, if there is one,
* specified by a header line something like:
*    content_length : 1024
* where 1024 is a stand-in for whatever you want your block
* size to be.
*
*/
/*
* Required files:  
*   Message.h
*
* Maintanence History:
* ver 1.1 - 30 Mar 2014
* - changes to error handling
* ver 1.0 - 29 Mar 2014
* - first release
*/
#include "../Message/Message.h"
#include <string>
#include <sstream>
#include <iostream>
#include <vector>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MakeMessage function  can make message
void Message::MakeMessage(std::string head, std::string sourceip, std::string sourceport, int size1, std::string file, std::string content)
{
	header = head;//assign value a message
	sourceCom = sourceip;
	sourcePort = sourceport;
	filename = file;
	size = content.length();
	body = content;
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//MakeString function  can transfer a message to a string
std::string Message::MakeString()
{
	std::stringstream ss;
	ss << size;
	std::string size1;
	ss >> size1;//transfer a string into a int value
	//std::string message = header +',' + sourceIp + ',' + sourcePort + ',' + size1 + ','+ filename + ',' + body + '\n';
	std::string message = header + "||" + startTime + ',' + sourceCom + ',' + sourcePort + ',' + size1 + ',' + filename + ',' + body + '\n';
	//std::string message = header + ',' + sourceIp + ',' + sourcePort + ',' + size1 + ',' + filename + ',' + body + '\n';

	return message;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Analyze function  can analyze a string and set value of the message 
void Message::Analyze(std::string message)
{
	std::string flag = ",";
	std::vector<std::string> strs;
	int lastPosition = 0, index = -1;
	for (size_t i = 0; i < 5; i++)//split the string with the symbol
	{
		index = message.find(flag, lastPosition);
		if (index == lastPosition)
			strs.push_back(" ");
		else
			strs.push_back(message.substr(lastPosition, index - lastPosition));
		lastPosition = index + 1;
	}
	std::string lastString = message.substr(lastPosition);
	if (!lastString.empty())
	{
		strs.push_back(lastString);
	}//get content 
	else
		strs.push_back(" ");
	header = strs[0].substr(0,4);//assign value
	if (strs[0].length() > 4)
	{
		int pos = strs[0].find_last_of("||");
		startTime = strs[0].substr(pos + 1);
	}
	sourceCom = strs[1];
	sourcePort = strs[2];
	size = atoi(strs[3].c_str());
	filename = strs[4];
	body = strs[5];
	size = body.length();
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// functions  can return private members
std::string& Message::Header(){ return header; };
std::string& Message::SourceCom(){ return sourceCom; }
std::string& Message::SourcePort(){ return sourcePort; }
std::string& Message::Body(){ return body; };
std::string& Message::Filename(){ return filename; };
std::string& Message::StartTime(){ return startTime; };
//std::string& Message::ThreadNum(){ return threadNum; };
int Message::Size(){ return size; };
#ifdef TEST_MESSAGE
void main()
{
	std::cout << "\n  Test Message class";
	std::cout << "\n ==========================================================\n";
	try
	{   
		std::cout << "\n  Test function MakeMessage() and MakeString()";
		std::cout << "\n ==========================================================\n";
		Message test1;
		test1.MakeMessage("Destination", "Myip", "Myport", 11, "FileName", "Content");
		std::cout << "  This is the Message Content:\n";
		std::string output = test1.MakeString();
		std::cout << "  "<<output << "\n";
		std::cout << "\n  Test function Analyze() and MakeString()";
		std::cout << "\n ==========================================================\n";
		test1.Analyze(output);
		std::string output1 = test1.MakeString();
		std::cout << "  This is the Message Content:\n";	
		std::cout <<"  "<< output1;
		std::cout << "  Notice:\n";
		std::cout << "  You can see that the Analyze() function \n  has the ability to correct the size of content\n\n";	
		std::cout << "\n  Test function Header(), SourceIp(), SourcePort(),Body() and Filename";
		std::cout << "\n ==========================================================\n";
		std::cout << "   Communicator:     " << test1.Header() << "\n";
		std::cout << "   SourcePort:       " << test1.SourcePort() << "\n";
		std::cout << "   SourceCom:         " << test1.SourceCom() << "\n";
		std::cout << "   FileName:         " << test1.Filename() << "\n";
		std::cout << "   Size:             " << test1.Size() << "\n";
		std::cout << "   Content:          " << test1.Body() << "\n";
		
	}
	catch(std::exception& e)
	{
		std::cout << "\n  " << e.what() << "\n\n";
	}
	std::cout << "\n\n";
}


#endif