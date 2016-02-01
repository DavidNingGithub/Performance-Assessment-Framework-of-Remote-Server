/////////////////////////////////////////////////////////////////////////////
// // Display.h                                                            //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright ?Jim Fawcett, 2012                                            //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Dell XPS 8300, Core i7, Windows 7 SP1                      //
// Application: Summer Projects, 2012                                      //
// Author:      Tao Ning   , CST 4-187, Syracuse University                //
//              (315) 443-3948, tning.rr.com                               //
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
*   Display.cpp,Display.h
*F
* Maintanence History:
* ver 1.1 - 30 Mar 2014
* - changes to error handling
* ver 1.0 - 29 Mar 2014
* - first release
*/
#ifndef DISPLAY_H
#define DISPLAY_H
#include "../Message/Message.h"
#include <iostream>
/////////////////////////////////////////////////////////
//class Display
class Display
{
public:
	Display(){};
	~Display(){};
	void DisplayMessage(Message test);
	void sender(int a, std::string test);
	void sender1(int id, std::string test);
	void sendfile(int ID,std::string FILE);
	void sendfile2(std::string FILE);
	void Dpro(std::string a, std::string b);
	void Dpro2(std::string senderid, std::vector<std::string> content);
	void Dpro1(int Duration,std::string content);
	void Dpro4(int Duration, std::string content);
	void file(std::string filename, std::string folder);
	void file1(int duration,std::string filename, std::string folder);
	std::string file12(int duration, std::string filename, std::string folder);
	void TextS(std::vector<std::string> keyword, std::vector<std::string> filename);
};
#endif
