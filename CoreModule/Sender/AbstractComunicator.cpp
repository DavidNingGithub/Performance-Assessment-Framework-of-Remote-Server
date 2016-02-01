/////////////////////////////////////////////////////////////////////////////
// AbstractCommunicator.cpp - Demonstration process message                //
// ver 2                                                                  //
// Tao Ning, CSE687 - Object Oriented Design, Spring 2013                 //
////////////////////////////////////////////////////////////////////////////
/*
 * 
 * So message framing is done by lines.  
 *
 * For HTTP like protocols the Sender should send lines for each
 * header attribute and bytes in the body, if there is one,
 * specified by a last header line something like:
 *    content_length : 1024
 * where 1024 is a stand-in for whatever you want your block 
 * size to be.
 *
 */
/*
 * Required files:
 * - Sender.cpp, Sockets.h, Sockets.cpp, 
 *   Threads.h, Threads.cpp, Locks.h, Locks.cpp
 *   BlockingQueue.h, BlockingQueue.cpp....
 *
 * Maintanence History:
 * ver 1.1 - 30 Mar 2014
 * - changed Sendthread from terminating to default
 * - minor changes to error handling
 * ver 1.0 - 29 Mar 2014
 * - first release
 */
#include "../Communicator/AbstractComunicator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <vector>
#include <utility>
#include <chrono>
#include <mutex>
#include "../FileManager/FileSystem.h"
#include "../FileManager/FileMgr.h"
//#include"../Exec.h"
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//PostMessage1 function can post message
void AbstractCommunicator::PostMessage1(std::string Msg)
{
	queue.enQ(Msg); 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//GetMessage1 function can get a  message from queue
std::string AbstractCommunicator::GetMessage1()
{ 
	return queue.deQ(); 
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//returnQ can return the private member queue
BlockingQueue<std::string>& AbstractCommunicator::returnQ()
{ 
	return queue;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//returnName function can return communicator name 
std::string AbstractCommunicator::returnName()
{
	return Name;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//returnSocket function can return socket
Socket& AbstractCommunicator::returnSocket()
{
	return s_;
};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Register function can register communicator in the map
void VaultDispatcher::Register(AbstractCommunicator* test)
{
	if (test->returnName() == this->returnName())//if the name of a AbstractCommunicator is itself, it won't register
		return;
	map[test->returnName()] = test;
	ComName.push_back(test->returnName());
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg function can process message and transfer the message to a specific communicator
void VaultDispatcher::ProcessMsg()
{	while (true)
	{	std::string Msg = returnQ().deQ();
		Message temp;
		temp.Analyze(Msg);
		if (temp.SourceCom().length() < 4)
		{	std::map<std::string, int>::iterator test2;
			test2 = CheckEnd.find(temp.SourcePort());
			if (test2 == CheckEnd.end())
				CheckEnd[temp.SourcePort()] = atoi(temp.SourceCom().c_str());
		}
		std::map<std::string, ICOM*>::iterator test;
		test = map.find(temp.Header());
		if (test != map.end())//if the dispatcher has the communicator in its map, it will transfer the message to the communicator
		{
			map[temp.Header()]->PostMessage1(Msg);
			if (temp.SourceCom().length() == 4 && temp.Header() == "send")
			{
				if (CheckEnd[temp.SourcePort()] == 1)
				{
					if (temp.SourceCom() == "down")
					{
						if (temp.Body().length() < 2048)
						{	std::string quit = "disp,stop," + temp.SourcePort() + ",stop,quit,quit";
							map[temp.Header()]->PostMessage1(quit);
						}
					}
					else
					{
						std::string quit = "disp,stop," + temp.SourcePort() + ",stop,quit,quit";
						map[temp.Header()]->PostMessage1(quit);
					}
				}
				else if (temp.SourceCom() == "down")
				{
					if (temp.Body().length() < 2048)
						CheckEnd[temp.SourcePort()] -= 1;
				}
				else
					CheckEnd[temp.SourcePort()] -= 1;
			}
		}
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg function can process message and transfer the message to a specific communicator
void VaultDispatcher::ProcessMsg1()
{	
	while (true)
	{
		std::string Msg = returnQ().deQ();
		Message temp;
		temp.Analyze(Msg);
		std::map<std::string, ICOM*>::iterator test;
		test = map.find(temp.Header());
		if (test != map.end())//if the dispatcher has the communicator in its map, it will transfer the message to the communicator
		{
			map[temp.Header()]->PostMessage1(Msg);
		}
		else if (temp.Header() == this->returnName() && temp.Body() == "quit")//shut down a communicator
		{
			while (true)
			{
				if (returnQ().size() == 0)
				{
					for (size_t i = 0; i < ComName.size(); i++)
					{
						Message test;
						test.MakeMessage(ComName[i], "", "", 0, "quit", "quit");
						std::string Msg2 = test.MakeString();
						map[ComName[i]]->PostMessage1(Msg2);
					}
					break;
				}
			}
			break;
		}
		else
			std::cout << "Can't find the coummunicator\n";
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOCK function can return lock
void VaultDispatcher::LOCK(std::mutex* lock)
{
	lock1 = lock;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//returnport function can return port
int& VaultDispatcher::returnport()
{
	return port;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//returnport function can return port
BlockingQueue<std::string>& VaultDispatcher::DisplayQ()
{
	return Displayqueue;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//run function can make sendthread run, and it will put the message into the socket
 void SendThread::run()
 {	
    status_ = "good";  
    std::string msg;
    do
    {
      msg = q_.deQ();
	  if (msg == "stop")
		  break;
	  Message temp;
	  temp.Analyze(msg);
	  int portinput = atoi(temp.SourcePort().c_str());	  
	  if (portinput != port)//judge the port number if its not the current port, disconnect current port and connect the new port
	  {
		  if (port != 0)
		  {
			  ::Sleep(10);//new
			  s_.disconnect();
		  }
		  s_.connect("127.0.0.1", portinput);
		  port = portinput;//record the port 
	  }
      if(!s_.writeLine(msg))//put the message into the socket
      {
        sout << "\n  bad status in sending thread";
        status_ = "bad";
        break;
      }
	  if (temp.Header() == "disp")
	  {
		  s_.disconnect();
		  s_.connect("127.0.0.1", port);
	  }
    } while(msg != "stop");
    s_.disconnect();
 }
 ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
 //ToString function can transfer a int type value to string type 
std::string ToString(int i)
{
  std::ostringstream conv;
  conv << i;
  return conv.str();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//id function can return id of the sender 
int Sender::id() 
{
	return myCount; 
}
std::string ReturnStarttime()
{
	std::chrono::time_point<std::chrono::high_resolution_clock> tstart = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(tstart.time_since_epoch());
	int start = ms.count() % 10000000;
	std::string starttime = ToString(start);
	return starttime;
}
void Sender::simi(std::string msg,int Num)
{
	std::string starttime = ReturnStarttime();
	std::string numLoop = ToString(Num);
	msg = "simi||" + starttime + ',' + numLoop + ',' + Recport + ",16,filename," + message;
	Message tempory1;
	tempory1.Analyze(msg);
	tempory1.Filename() = "sender#" + ToString(id());
	msg = tempory1.MakeString();
	lock3->lock();
	temp.sender1(id(), message);
	lock3->unlock();
	q_.enQ(msg);
	::Sleep(10 * id());  // sleep time increases with each addition Sender
}

void Sender::echoMsg(std::string msg, int Num)
{
	std::string starttime = ReturnStarttime();
	std::string numLoop = ToString(numMsgs_);
	msg = "echo||" + starttime + ',' + numLoop + ',' + Recport + ",16,filename," + message;
	Message tempory1;
	tempory1.Analyze(msg);
	tempory1.Filename() = "sender#" + ToString(id());
	msg = tempory1.MakeString();
	lock3->lock();
	temp.sender1(id(), message);
	lock3->unlock();
	q_.enQ(msg);
	::Sleep(10 * id());   // sleep time increases with each addition Sender
}
void Sender::textSearchMsg(std::string msg, int Num)
{
	std::string starttime = ReturnStarttime();
	std::string numLoop = ToString(numMsgs_);
	msg = "text||" + starttime + ',' + numLoop + ',' + Recport + ",16,filename," + message;
	Message tempory1;
	tempory1.Analyze(msg);
	tempory1.Filename() = "sender#" + ToString(id());
	msg = tempory1.MakeString();
	lock3->lock();
	temp.sender1(id(), message);
	lock3->unlock();
	q_.enQ(msg);
	::Sleep(10 * id());  // sleep time increases with each addition Sender
}
void Sender::DownloadMsg(std::string msg, int Num)
{
	std::string starttime = ReturnStarttime();
	size_t count = 0;
	std::string tempMsg = message;
	while (tempMsg.find("||") != -1)
	{
		int temppos = tempMsg.find_last_of("||");
		tempMsg = tempMsg.substr(0, temppos - 1);
		count++;
	}
	int tempNumMsg = numMsgs_*(count + 1);
	std::string numLoop = ToString(tempNumMsg);
	msg = "down||" + starttime + ',' + numLoop + ',' + Recport + ",16,filename," + message;
	Message tempory1;
	tempory1.Analyze(msg);
	tempory1.Filename() = "sender#" + ToString(id());
	msg = tempory1.MakeString();
	lock3->lock();
	temp.sender(id(), message);
	lock3->unlock();
	q_.enQ(msg);
	::Sleep(10 * id());  // sleep time increases with each addition Sender
}
void Sender::begin(std::string ip, int port)
{
	retLock()->lock();
	std::cout << "\n Sender #" << id() << " started\n";
	retLock()->unlock();
	if (!s_.connect(ip, port))//connect the socket to a specific port
	{
		delete pSt;
		return;
	}
	pSt = new SendThread(s_, q_);//make the senderthread and sender share a unique queue
	std::thread ST(&SendThread::run, pSt);
	ST.detach();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//start function can start sender, enqueue message to the queue shared by sender and senderthread
void Sender::start(std::string ip, int port)
 {	begin(ip, port);
    std::string msg;
	if (Check == 6)//check whether send messge to a server
	{	for (int i = 0; i < numMsgs_; ++i)
		{	simi(msg, numMsgs_);
			if (pSt->status() == "bad")
				break;
		}
	}
	if (Check == 1)//check whether send messge to a server
	{	for (int i = 0; i < numMsgs_; ++i)
		{	echoMsg(msg, numMsgs_);
			if (pSt->status() == "bad")
				break;
		}
	}
	if (Check == 5)//check whether send messge to a server
	{	for (int i = 0; i < numMsgs_; ++i)
		{	textSearchMsg(msg, numMsgs_);
			if (pSt->status() == "bad")
				break;
		}
	}
	if (Check == 4)//check whether send messge to a server
	{	for (int i = 0; i < numMsgs_; ++i)
		{	DownloadMsg(msg, numMsgs_);
			if (pSt->status() == "bad")
				break;
		}
	}
	if (Check == 2)//check whether send all files in a directory to the server 
	{	for (int i = 0; i < numMsgs_; ++i)
		{	std::string test = message;
			std::vector<std::string> AllFiles;
			search(AllFiles, 1, test);
			SendFile(AllFiles);
		}
	}
	::Sleep(10);
	q_.enQ("stop");//demo that the communicator can be closed
	lock3->lock();
    msg = "sender#" + ToString(id()) + ": stop\n";
	lock3->unlock();
    sout << "\n  " + msg;
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//search function can put the file name under a directory into a vector
  void Sender::search(std::vector<std::string>& AllFiles, int n, std::string command)
  {
	  int test = 0;
	  string path;
	  FileMgr::patterns patts;
	  int check = 0;
	  std::vector<std::string> tempV;
	  if (command.find("||") != -1)
	  {
		  check = 1;
		  while (command.find("||") != -1)
		  {
			  int pos = command.find_last_of("||");
			  tempV.push_back(command.substr(pos + 1));
			  command = command.substr(0, pos - 1);
		  }
		  tempV.push_back(command);
		  path = tempV[0];
	  }
	  else
		  path = command;
	  FileMgr::files foundFiles;
	  FileMgr FindFile;
	  std::string mainpath;
	  mainpath = FileSystem::Path::getFullFileSpec(path);
	  FileSystem::FileInfo test1(mainpath);//check whether the url is a path or a file
	  bool fileOrNot = test1.isDirectory();
	  if (fileOrNot)
	  {
		  if (patts.size() == 0)
			  patts.push_back("*.*");
		  AllFiles = FindFile.findfile(foundFiles, mainpath, patts, test);//record the file into a vector
	  }
	  else
	  {
		  if (check == 1)
		  {
			  for (size_t i = 0; i < tempV.size(); i++)
			  {
				  std::string UseTemp = FileSystem::Path::getFullFileSpec(tempV[i]);
				  AllFiles.push_back(UseTemp);
			  }
		  }
		  else
			  AllFiles.push_back(mainpath);
	  }
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SendFile function can send file
  void Sender::SendFile(std::vector<std::string>& AllFiles)
  {
	  Message use;
	  std::string msg;
	  std::string filename;
	  for (size_t i = 0; i < AllFiles.size(); i++)
	  {
		  std::stringstream ss;
		  ss << myCount;
		  std::string senderid;
		  ss >> senderid;//transfer a string to int
		  filename = ".\\ClientSender#" + senderid + "\\";//modified
		  std::ifstream input(AllFiles[i], std::ios::binary);//open the file 
		  char stream[2048];
		  size_t pos = AllFiles[i].find_last_of("\\");
		  filename += AllFiles[i].substr(pos + 1);
		  std::streamsize Num = 0;
		  int pos11 = filename.find_last_of("\\");
		  std::string displayFILE = filename.substr(pos11 + 1);
		  lock3->lock();
		  temp.sendfile(id(), displayFILE);
		  lock3->unlock();
		  while (!input.eof())
		  {
			  input.read(stream, 2048);//write the content into the a block
			  Num = input.gcount();
			  unsigned int contentsize = unsigned int(input.gcount());
			  std::string body(stream, contentsize);
			  int ad = body.length();
			  std::chrono::time_point<std::chrono::high_resolution_clock> tstart = std::chrono::high_resolution_clock::now();
			  std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(tstart.time_since_epoch());
			  int start = ms.count() % 10000000;
			  std::string starttime = ToString(start);
			  std::string numLoop = ToString(numMsgs_*AllFiles.size());
			  use.MakeMessage("file||" + starttime, numLoop, Recport, body.length(), filename, body);
			  use.StartTime() = starttime;
			  msg = use.MakeString();
			  q_.enQ(msg);//send the block
			  ::Sleep(10 * id());
		  }
	  }
  }
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg function can Process reply message to client 
void Sender::ProcessMsg()
{
	lock3->lock();
	std::cout << "\n Sender #" << id() << " started\n";
	lock3->unlock();
	pSt = new SendThread(s_, q_);
	std::thread ST1(&SendThread::run, pSt);
	ST1.detach();
	while (true)
	{
		std::string Msg = returnQ().deQ();
		Message temp;
		temp.Analyze(Msg);
		if (temp.SourceCom() == "down")
			temp.Header() = "file";
		//temp.Header() = "echo";//change the header
		else if (temp.Filename()!="quit")
			temp.Header() = "echo";
		std::string reply = temp.MakeString();
		q_.enQ(reply);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//lock function can return lock 
void Sender::LOCK(std::mutex* lock)
{
	lock3 = lock;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//returnSocket function can reutrn  socket
Socket& Sender::returnSocket()
{
	return s_;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//retLock function can return lock 
std::mutex*& Sender::retLock()
{
	return lock3;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dispatcher function can give dispathcer value
void Sender::Dispatcher(VaultDispatcher* test)
{
	DISP = test;
};



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//start function can start sender, enqueue message to the queue shared by sender and senderthread
void DownloadCommunicator::ProcessMsg()
{
	while (true)
	{
		std::string Msg = returnQ().deQ();
		Message temp;
		temp.Analyze(Msg);
		if (temp.Body() == "quit")//if the message want to close the EchoCommunicator
		{
			while (true)
			{
				if (returnQ().size() == 0)
				{
					break;
				}
			}
			lock3->lock();
			std::cout << "Close " << temp.Header() << "\n";
			lock3->unlock();
			break;
		}
		else//if not
		{
			lock3->lock();
			std::string path = temp.Body();
			std::string senderID = temp.Filename();
			Recport = temp.SourcePort();			
			std::vector<std::string> AllFiles;
			search(AllFiles, path);
			tempD.Dpro2(temp.Filename(), AllFiles);
			SendFile(AllFiles, temp.StartTime(), senderID);
			lock3->unlock();
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//search function can put the file name under a directory into a vector
void DownloadCommunicator::search(std::vector<std::string>& AllFiles, std::string command)
{
	int test = 0;
	string path;
	FileMgr::patterns patts;
	int check = 0;
	std::vector<std::string> tempV;
	if (command.find("||") != -1)
	{
		check = 1;
		while (command.find("||") != -1)
		{
			int pos = command.find_last_of("||");
			tempV.push_back(command.substr(pos + 1));
			command = command.substr(0, pos - 1);
		}
		tempV.push_back(command);
		path = tempV[0];
	}
	else
		path = command;
	FileMgr::files foundFiles;
	FileMgr FindFile;
	std::string mainpath;
	mainpath = FileSystem::Path::getFullFileSpec(path);
	FileSystem::FileInfo test1(mainpath);//check whether the url is a path or a file
	bool fileOrNot = test1.isDirectory();
	if (fileOrNot)
	{
		if (patts.size() == 0)
			patts.push_back("*.*");
		AllFiles = FindFile.findfile(foundFiles, mainpath, patts, test);//record the file into a vector
	}
	else
	{
		if (check == 1)
		{
			for (size_t i = 0; i < tempV.size(); i++)
			{
				std::string UseTemp = FileSystem::Path::getFullFileSpec(tempV[i]);
				AllFiles.push_back(UseTemp);
			}
		}
		else
			AllFiles.push_back(mainpath);
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//SendFile function can send file
void DownloadCommunicator::SendFile(std::vector<std::string>& AllFiles, std::string starttime, std::string senderName)
{
	Message use;
	std::string msg;
	std::string filename;
	for (size_t i = 0; i < AllFiles.size(); i++)
	{
		filename = ".\\" + senderName + "\\";
		std::ifstream input(AllFiles[i], std::ios::binary);//open the file 
		char stream[2048];
		size_t pos = AllFiles[i].find_last_of("\\");
		filename += AllFiles[i].substr(pos + 1);
		std::streamsize Num = 0;
		int pos11 = filename.find_last_of("\\");
		std::string displayFILE = filename.substr(pos11 + 1);
		size_t ID = 0;
		//lock3->lock();
		tempD.sendfile2(displayFILE);
		//lock3->unlock();
		while (!input.eof())
		{
			ID++;
			input.read(stream, 2048);//write the content into the a block
			Num = input.gcount();
			unsigned int contentsize = unsigned int(input.gcount());
			std::string body(stream, contentsize);
			int ad = body.length();
			use.MakeMessage("send", "down", Recport, body.length(), filename, body);
			use.StartTime() = starttime;
			msg = use.MakeString();
			DISP->returnQ().enQ(msg);
			::Sleep(10 * ID);
		}
	}
	for (size_t i = 0; i < AllFiles.size(); i++)
	{
		AllFiles.pop_back();
	}
}

void DownloadCommunicator::LOCK(std::mutex* lock)
{
	lock3 = lock;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dispatcher function can give dispathcer value
void DownloadCommunicator::Dispatcher(VaultDispatcher* test)
{
	DISP = test;
};



////////////////////////////////=============================
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dispatcher function can give dispathcer value
void  EchoCommunicator::Dispatcher(VaultDispatcher* test)
{
	DISP = test;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg function can Process Msg in echo communicator in sever
void  EchoCommunicator::ProcessMsg()
{
	while (true)
	{
		std::string Msg = returnQ().deQ();
		Message temp;
		temp.Analyze(Msg);
		if (temp.Body() == "quit")//if the message want to close the EchoCommunicator
		{
			while (true)
			{
				if (returnQ().size() == 0)
				{
					break;
				}
			}
			lock2->lock();
			std::cout << "Close " << temp.Header() << "\n";
			lock2->unlock();
			break;
		}
		else//if not
		{
			lock2->lock();
			tempD.Dpro(temp.Filename(), temp.Body());
			std::string reply = "send||"+temp.StartTime()+",echo,"+temp.SourcePort()+",Contentdefination,"+temp.Filename()+",The sever received a message : " + temp.Body();
			Message replyMessage;//create reply message
			replyMessage.Analyze(reply);
			replyMessage.StartTime() = temp.StartTime();
			std::string sendmsg = replyMessage.MakeString();
			DISP->returnQ().enQ(sendmsg);
			lock2->unlock();
		}
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg1 function can Process Msg1 in echo in client 
void  EchoCommunicator::ProcessMsg1()
{
	while (true)
	{
		std::string Msg = returnQ().deQ();
		int pos2 = Msg.find_last_of(",");
		std::string checkMsg = Msg.substr(pos2+1);
		if (checkMsg == "quit\n")
		{
			int pos = Msg.find_last_of("\n");
			Msg = Msg.substr(0, pos);
		}
		Message temp;
		temp.Analyze(Msg);
		if (temp.Body() == "quit")
		{
			lock2->lock();
			std::cout << "Close " << temp.Header() << "\n";
			lock2->unlock();
			break;
		}
		else
		{
			lock2->lock();
			int start=atoi(temp.StartTime().c_str());
			std::chrono::time_point<std::chrono::high_resolution_clock> tstart = std::chrono::high_resolution_clock::now();
			std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(tstart.time_since_epoch());
			int end = ms.count() % 10000000;	
			int duration = end - start;
			if (temp.SourceCom() == "simi")
			{
				tempD.Dpro4(duration, temp.Body());//display the message received by receiver in client
			}
			else
				tempD.Dpro1(duration, temp.Body());
			int endmilli= ms.count() % 1000;
			std::string endmilliseconds = ToString(endmilli);
			std::string Displayresult = temp.Body() + '&' + ToString(duration) + '&' + endmilliseconds;
			DISP->DisplayQ().enQ(Displayresult);
			lock2->unlock();
		}
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOCK function can return lock 
void  EchoCommunicator::LOCK(std::mutex* lock)
{
	lock2 = lock;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////======================


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//run function can make TextSearchthread run, and it will put the message into the socket
void TextSearchthread::run()
{
	while (q_.size() > 0)
	{
		std::string fileAddress = q_.deQ();
		size_t pos = fileAddress.find_last_of("\\");
		std::string tempstring = fileAddress.substr(0, pos);
		size_t pos2 = tempstring.find_last_of("\\");
		std::string foldername = tempstring.substr(pos2 + 1);
		std::string filename = fileAddress.substr(pos+1);
		filename = foldername + "\\" + filename;
		FileSystem::File newfile(fileAddress);
		newfile.open(FileSystem::File::in);
		std::string Content;
		Content = newfile.getwholetext();
		std::vector<string> foundkey;
		for (size_t i = 0; i < keys.size(); i++)
		{
			if (Content.find(keys[i]) != -1)
				foundkey.push_back(keys[i]);
		}
		if (foundkey.size()>0)
			result[filename]= foundkey;		
	}
	runing--;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dispatcher function can give dispathcer value
void  TextSearchCommunicator::Dispatcher(VaultDispatcher* test)
{
	DISP = test;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//search function can put the file name under a directory into a vector
void TextSearchCommunicator::search(std::vector<std::string>& AllFiles, std::string command)
{
	int test = 0;
	string path;
	FileMgr::patterns patts;
	path = command;
	FileMgr::files foundFiles;
	FileMgr FindFile;
	string mainpath;
	mainpath = FileSystem::Path::getFullFileSpec(path);
	FileSystem::FileInfo test1(mainpath);//check whether the url is a path or a file
	bool fileOrNot = test1.isDirectory();
	if (fileOrNot)
	{
		if (patts.size() == 0)
		{
			patts.push_back("*.txt");
			patts.push_back("*.cpp");
			patts.push_back("*.h");
		}
		FileMgr::files Result;
		Result = FindFile.findfile(foundFiles, mainpath, patts, test);//record the file into a vector
		for (size_t i = 0; i < Result.size(); i++)
			AllFiles.push_back(Result[i]);
	}
	else
		AllFiles.push_back(mainpath);
}
void TextSearchCommunicator::Analyses(Message& temp, std::vector<std::string>& strs)
{
	std::string Content = temp.Body();
	std::string flag = "||";
	int lastPosition = 0, index = -1;
	while (Content.find(flag, lastPosition) != std::string::npos)//split the string with the symbol
	{
		index = Content.find(flag, lastPosition);
		if (index == lastPosition)
			strs.push_back(" ");
		else
			strs.push_back(Content.substr(lastPosition, index - lastPosition));
		lastPosition = index + 2;
	}
	std::string lastString = Content.substr(lastPosition);
	strs.push_back(lastString);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg function can Process Msg in echo communicator in sever
void  TextSearchCommunicator::ProcessMsg()
{	while (true)
	{	std::string Msg = returnQ().deQ();
		Message temp;
		temp.Analyze(Msg);
		std::vector<std::string> strs;
		lock2->lock();
		Analyses(temp, strs);
		std::vector<std::string> Paths;
		std::vector<std::string> keywords;	
		size_t Num = atoi(strs[0].c_str());
		for (size_t i = 0; i < strs.size() - 1; i++)
		{	if (strs[i + 1].find("#") != -1)
				keywords.push_back(strs[1 + i].substr(1));
			else
				Paths.push_back(strs[1 + i]);
		}
		std::vector<std::string> AllFiles;
		for (size_t k = 0; k < Paths.size();k++)
					search(AllFiles,Paths[k]);
		for (size_t i = 0; i < AllFiles.size(); i++)
			q_.enQ(AllFiles[i]);
		tempD.TextS(keywords, AllFiles);
		std::map<std::string, std::vector<std::string>> result;
		int runingthread = 0;
		for (size_t i = 0; i < Num; i++)
		{	runingthread++;
			TextSearchthread textSearch(result, keywords, q_,runingthread);
			std::thread TS(&TextSearchthread::run, &textSearch);
			TS.detach();				
		}
		::Sleep(500);
		while (runingthread != 0);
		std::string replytemp;
		std::map<std::string, std::vector<std::string>>::iterator it = result.begin();
		for (size_t i = 0; i < result.size(); i++)
		{	replytemp += "====================================================\n";
			replytemp += "---In the file " + it->first + "\n---The sever found keyword:\n";
			for (size_t k = 0; k < it->second.size(); k++)
			replytemp += "---"+it->second.at(k) + "\n";
		    it++;
		}
		std::string reply = "send||"+temp.StartTime()+",text," + temp.SourcePort() + ",Contentdefination," + temp.Filename() + "," + replytemp;
		Message replyMessage;//create reply message
		replyMessage.Analyze(reply);
		std::string sendmsg = replyMessage.MakeString();
		DISP->returnQ().enQ(sendmsg);
		lock2->unlock();
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOCK function can return lock 
void  TextSearchCommunicator::LOCK(std::mutex* lock)
{
	lock2 = lock;
}
////=====================================///////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//run function can make TextSearchthread run, and it will put the message into the socket
void Similaritythread::run()
{
	while (q_.size() > 0)
	{
		std::string fileAddress = q_.deQ();
		Exec->analyze(fileAddress);
	}
	runing--;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dispatcher function can give dispathcer value
void  SimilarityCommunicator::Dispatcher(VaultDispatcher* test)
{
	DISP = test;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//search function can put the file name under a directory into a vector
void SimilarityCommunicator::search(std::vector<std::string>& AllFiles, std::string command)
{
	int test = 0;
	string path;
	FileMgr::patterns patts;
	path = command;
	FileMgr::files foundFiles;
	FileMgr FindFile;
	string mainpath;
	mainpath = FileSystem::Path::getFullFileSpec(path);
	FileSystem::FileInfo test1(mainpath);//check whether the url is a path or a file
	bool fileOrNot = test1.isDirectory();
	if (fileOrNot)
	{
		if (patts.size() == 0)
		{
			patts.push_back("*.txt");
			patts.push_back("*.cpp");
			patts.push_back("*.h");
		}
		FileMgr::files Result;
		Result = FindFile.findfile(foundFiles, mainpath, patts, test);//record the file into a vector
		for (size_t i = 0; i < Result.size(); i++)
			AllFiles.push_back(Result[i]);
	}
	else
		AllFiles.push_back(mainpath);
}
void SimilarityCommunicator::Analyses(Message& temp, std::vector<std::string>& strs)
{
	std::string Content = temp.Body();
	std::string flag = "||";
	int lastPosition = 0, index = -1;
	while (Content.find(flag, lastPosition) != std::string::npos)//split the string with the symbol
	{
		index = Content.find(flag, lastPosition);
		if (index == lastPosition)
			strs.push_back(" ");
		else
			strs.push_back(Content.substr(lastPosition, index - lastPosition));
		lastPosition = index + 2;
	}
	std::string lastString = Content.substr(lastPosition);
	strs.push_back(lastString);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg function can Process Msg in echo communicator in sever
void  SimilarityCommunicator::ProcessMsg()
{	while (true)
	{	std::string Msg = returnQ().deQ();
		Message temp;
		temp.Analyze(Msg);
		std::vector<std::string> strs;
		lock2->lock();
		Analyses(temp, strs);
		size_t Num = atoi(strs[0].c_str());
		std::vector<std::string> Paths;
		for (size_t i = 0; i < strs.size() - 1; i++)
			Paths.push_back(strs[1 + i]);
		std::vector<std::string> AllFiles;
		for (size_t k = 0; k < Paths.size(); k++)
			search(AllFiles, Paths[k]);
		for (size_t i = 0; i < AllFiles.size(); i++)
			q_.enQ(AllFiles[i]);
		char* test1[] = { "test"};
		int test2=1;
		Executer* exec = new Executer(test2, test1);
		int runingthread = 0;
		for (size_t i=0; i < Num; i++)
		{	runingthread++;
			Similaritythread simiSearch(exec, q_, runingthread);
			std::thread TS(&Similaritythread::run, &simiSearch);
			TS.detach();
		}
		::Sleep(100);
		while (runingthread != 0);
		exec->Compare();
		std::cout << "================================================================\n";
		std::cout << "---The sever is requested to compare file: \n";
		for (size_t i = 0; i < AllFiles.size(); i++)
		{
			int pos = AllFiles[i].find_last_of("\\");
			std::string FileName = AllFiles[i].substr(pos + 1);
			std::cout << "---" << FileName << "\n";
		}
		std::string result=exec->DisplaySimilarCodes();
		std::string reply = "send||"+temp.StartTime()+",simi," + temp.SourcePort() + ",Contentdefination," + temp.Filename() + "," + result;
		Message replyMessage;//create reply message
		replyMessage.Analyze(reply);
		std::string sendmsg = replyMessage.MakeString();
		DISP->returnQ().enQ(sendmsg);
		lock2->unlock();		
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOCK function can return lock 
void  SimilarityCommunicator::LOCK(std::mutex* lock)
{
	lock2 = lock;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Dispatcher function return dispatcher
void FileCommunicator::Dispatcher(VaultDispatcher* test)
{
	DISP = test;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//checkdirectory function can check directory whether exists
void FileCommunicator::checkdirectory(std::string senderid)
{
	bool check = false;
	if (directory.size() != 0)
	{
		size_t test = 0;
		while (test < directory.size())
		{
			if (directory[test] == senderid)
				check = true;
			test++;
		}
	}
	if (!check)//if there is no directory with the same name,create one
	{
		directory.push_back(senderid);
		std::string name = ".\\" + senderid;
		if (_mkdir(name.c_str()) == 0)
		{
			lock2->lock();
			std::cout << "Directory " << name << " was successfully created\n";
			lock2->unlock();
		}
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//checkfile function can check file whether is created
void FileCommunicator::checkfile(std::string filename)
{
	bool check = false;
	size_t test = 0;
	if (recordfile.size() != 0)
	{

		while (test < recordfile.size())
		{
			if (recordfile[test] == filename)
				check = true;
			test++;
		}
	}
	if (check)//if there is already a file with same name in the directory,delete it
	{
		CString file = filename.c_str();
		LPCTSTR Dfile = file;
		DeleteFile(Dfile);
		std::vector<std::string>::iterator best = find(recordfile.begin(), recordfile.end(), filename);
		recordfile.erase(best);
	}
}
int DURATION(std::string starttime, std::string& endmilliseconds)
{
	int start = atoi(starttime.c_str());
	std::chrono::time_point<std::chrono::high_resolution_clock> tstart = std::chrono::high_resolution_clock::now();
	std::chrono::milliseconds ms = std::chrono::duration_cast<std::chrono::milliseconds>(tstart.time_since_epoch());
	int end = ms.count() % 10000000;
	int duration = end - start;
	endmilliseconds = ToString(ms.count() % 1000);
	return duration;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg1 function can ProcessMsg in file communicator in client part
void FileCommunicator::ProcessMsg1()
{	while (true)
	{	std::string Msg = returnQ().deQ();
		int pos2 = Msg.find_last_of(",");
		std::string checkMsg = Msg.substr(pos2 + 1);
		if (checkMsg == "quit\n")
		{	int pos = Msg.find_last_of("\n");
			Msg = Msg.substr(0, pos);
		}
		int pos = Msg.find_last_of("\n");
		Msg = Msg.substr(0, pos);
		Message temp;		
		temp.Analyze(Msg);
		if (temp.Body() == "quit")
		{	while (true)
				if (returnQ().size() == 0)
					break;
			lock2->lock();
			std::cout << "Close " << temp.Header() << "\n";
			lock2->unlock();
			break;
		}
		else
		{	std::string filename = temp.Filename();
			std::string rest = filename.substr(2);
			int Bpos = rest.find("\\");
			std::string senderid = rest.substr(0, Bpos);
			checkdirectory(senderid);//check directory whether exist
			checkfile(filename);//check file whether exist
			lock2->lock();
			std::ofstream out(filename, std::ios::app | std::ios::binary);
			if (out.is_open())//open the file
			{	out.write(temp.Body().c_str(), temp.Size());//write the block into the file
				if (temp.Size() < 2048)//when its size smaller than 2048, it means the file is nearly finished
				{	recordfile.push_back(filename);//register the file name when its transfermation is finished
					filename = rest.substr(Bpos + 1);
					std::string folder = rest.substr(0, Bpos);
					std::string endmilliseconds;
					int duration = DURATION(temp.StartTime(), endmilliseconds);
					tempD.file1(duration, filename, folder);
					std::string WPFre = tempD.file12(duration, filename, folder);
					std::string Displayresult = WPFre + '&' + ToString(duration) + '&' + endmilliseconds;
					DISP->DisplayQ().enQ(Displayresult);
				}out.close();
			}lock2->unlock();
		}
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg function can ProcessMsg in file communicator in sever part
void FileCommunicator::ProcessMsg()
{
	while (true)
	{
		std::string Msg = returnQ().deQ();
		Message temp;
		std::string folder;
		temp.Analyze(Msg);
		std::string filename = temp.Filename();
		std::string rest = filename.substr(2);
		int Bpos = rest.find("\\");
		std::string senderid = rest.substr(0, Bpos);
		checkdirectory(senderid);//check directory whether exist
		checkfile(filename);//check file whether exist	
		lock2->lock();
		std::ofstream out(filename, std::ios::app | std::ios::binary);
		if (out.is_open())//open the file
		{
			out.write(temp.Body().c_str(), temp.Size());//write the block into the file
			if (temp.Size() < 2048)//when its size smaller than 2048, it means the file is nearly finished
			{
				recordfile.push_back(filename);//register the file name when its transfermation is finished
				filename = rest.substr(Bpos + 1);
				folder = rest.substr(0, Bpos);
				tempD.file(filename, folder);
				std::string reply = "send,file," + temp.SourcePort() + ",Contentdefination," + temp.Filename() + ",---The sever received a file named " + filename + "\n---Through sender: " + senderid + "\n---And the sever stored the file in " + senderid + " folder";
				Message replyMessage;
				replyMessage.Analyze(reply);
				replyMessage.StartTime() = temp.StartTime();
				std::string send = replyMessage.MakeString();//create reply message
				DISP->returnQ().enQ(send);//send reply message
			}
			out.close();
		}
		lock2->unlock();
	}
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//ProcessMsg function can return lock 
void FileCommunicator::LOCK(std::mutex* lock)
{
	lock2 = lock;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//run function can run client thread
void ClientHandlerThread::run()
{
	std::string msg;
	std::string line;
	do
	{
		std::mutex lockcode;
		//lockcode.lock();
		line = s_.readMsg();//read message from the socket
		if (line == "\n")
			break;
		q_.enQ(line);
		if (line.substr(0, 4) == "disp")
		{	
			s_.disconnect();
			s_.~Socket();
			break;			
		}		
		//lockcode.unlock();
	} while (line != "stop");
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//stop function can stop thread 
void ListenThread::stop(bool end) 
{
	stop_ = end;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//run function can run listenthread
void ListenThread::run()
{
	while (!stop_)
	{
		Socket s = listener_.waitForConnect();//try to connect the socket
		ClientHandlerThread* pCh = new ClientHandlerThread(s, A,q_);
		std::thread CHT(&ClientHandlerThread::run, pCh);//assign a new thread "client handle" to a socket
		std::string msg;
		CHT.detach();		
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOCK function can reurn lock  message
void ListenThread::LOCK(std::mutex* lock)
{
	lock5 = lock;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//DISP function can return dispatcher 
void Receiver::DISP(VaultDispatcher* dispa)
{
	Disp = dispa;
};
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//start function can start receiver 
void Receiver::start(int port)
{
	lock4->lock();
	std::cout << "\n Reciever started\n";
	lock4->unlock();
	int a = 1;
	pLt = new ListenThread(port, q_,a, Disp);//run listenthread
	pLt->LOCK(lock4);
	try
	{
		std::thread LT(&ListenThread::run, pLt);
		LT.detach();
		std::string msg;
		size_t count = 0;
		do
		{
			msg = q_.deQ();//get message from the queue
			if (msg == "")
				continue;

			size_t pos = msg.find_last_of("\n");
			msg = msg.erase(pos, 1);//process message
			Disp->returnQ().enQ(msg);//give the message to the dispatcher
			if (msg.substr(0, 4) == "disp")
			{
				bool stopL = true;
				pLt->stop(stopL);
				a = 0;
				break;
			}
		} while (true);
	}
	catch (std::exception&)
	{
		delete pLt;
	}
	catch (...)
	{
		delete pLt;
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Listenthread function 
void Receiver::Listenthread(ListenThread* temp)
{
	pLt = temp;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//LOCK function can return lock 
void Receiver::LOCK(std::mutex* lock)
{
	lock4 = lock;
}
#ifdef TEST_ABSTRACTCOMMUNICATOR
void main()
{		std::mutex lock2;
		Receiver* rcvr2 = new Receiver;
		VaultDispatcher* Disp2;
		EchoCommunicator* Echo2;
		FileCommunicator* File2;
		Sender* Send2;
		Disp2 = new VaultDispatcher("disp");
		Echo2 = new EchoCommunicator("echo");
		File2 = new FileCommunicator("file");
		Send2 = new Sender("send", 1, 1, "OOD");
		rcvr2->LOCK(&lock2);
		Disp2->LOCK(&lock2);
		File2->LOCK(&lock2);
		Echo2->LOCK(&lock2);
		Send2->LOCK(&lock2);
		Disp2->Register(Echo2);
		Disp2->Register(File2);
		Disp2->Register(Send2);
		Echo2->Dispatcher(Disp2);
		File2->Dispatcher(Disp2);
		Send2->Dispatcher(Disp2);		
}
#endif