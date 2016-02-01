#ifndef MESSAGE_H
#define MESSAGE_H
/////////////////////////////////////////////////////////////////////////////
// // Message.h   transfer a message between message type and string type  //
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
*   Message.h Message.cpp
*
* Maintanence History:
* ver 1.1 - 30 Mar 2014
* - changes to error handling
* ver 1.0 - 29 Mar 2014
* - first release
*/
#include <string>
#include <sstream>
#include <iostream>
#include <vector>
/////////////////////////////////////////////////////////
//class Message
class Message
{
public:
	Message(){};
	~Message(){};
	void MakeMessage(std::string head, std::string sourcecom, std::string sourceport, int size1, std::string file, std::string content);
	std::string MakeString();	
	void Analyze(std::string message);
	std::string& Header(); 
	std::string& SourceCom(); 
	std::string& SourcePort(); 
	std::string& Body(); 
	std::string& Filename();
	std::string& StartTime();
	//std::string& ThreadNum();
	int Size(); 
private:
	std::string startTime;
	//std::string threadNum;
	std::string header;
	std::string sourceCom;
	std::string sourcePort;
	std::string body;
	int size;
	std::string filename;
};
#endif
