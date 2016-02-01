/////////////////////////////////////////////////////////////////////////////
// // AbstractCommunicator.cpp                                             //
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
*   BlockingQueue.h, BlockingQueue.cpp,AbstractCommunicator.h,AbstractCommunicator.cpp
*
* Maintanence History:
* ver 1.1 - 30 Mar 2014
* - changed Sendthread from terminating to default
* - minor changes to error handling
* ver 1.0 - 29 Mar 2014
* - first release
*/
#ifndef ABSTRACTCOMMUNICATOR_H
#define ABSTRACTCOMMUNICATOR_H
#include <string>
#include <sstream>
#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <mutex>
#include <direct.h>
#include <atlstr.h>
#include "../sockets/Sockets.h"
#include "../Threads/Threads.h"
#include "../Threads/Locks.h"
#include "../BlockingQueue/Cpp11-BlockingQueue.h"
#include "../Display/Display.h"
#include"../Message/Message.h"
#include"../TEST2/Compareself/Exec.h"

class Sender;
/////////////////////////////////////////////////////////
//class ICOM
class ICOM
{
public:
	ICOM(){};
	virtual ~ICOM(){};
	virtual void PostMessage1(std::string Msg) = 0;
	virtual std::string GetMessage1()=0;
};
/////////////////////////////////////////////////////////
//class AbstractCommunicator
class AbstractCommunicator:public ICOM
{
public:
	AbstractCommunicator(std::string name):Name(name){};
	virtual ~AbstractCommunicator(){};
	void PostMessage1(std::string Msg);
	std::string GetMessage1();
	BlockingQueue<std::string>& returnQ();
	std::string returnName();
	virtual void ProcessMsg()=0;
	virtual Socket& returnSocket();
	
private:
	BlockingQueue<std::string> queue;
	std::string Name;
	Socket s_;
};
/////////////////////////////////////////////////////////
//class AbstractDispatcher
class AbstractDispatcher :public AbstractCommunicator
{
public:
	AbstractDispatcher(std::string name):AbstractCommunicator(name){};
	virtual ~AbstractDispatcher(){};
	virtual void Register(AbstractCommunicator* test) = 0;
};
/////////////////////////////////////////////////////////
//class VaultDispatcher
class VaultDispatcher : public AbstractDispatcher
{
public:
	VaultDispatcher(std::string name) :AbstractDispatcher(name){};
	~VaultDispatcher(){};
	void Register(AbstractCommunicator* test);
	void ProcessMsg();
	void ProcessMsg1();
	void LOCK(std::mutex* lock);
	int& returnport();
	BlockingQueue<std::string>& DisplayQ();
private:
	std::map<std::string, ICOM*> map;
	std::vector<std::string> ComName;
	std::map<std::string, int> CheckEnd;
	std::mutex* lock1;
	Sender* Dispsender;
	BlockingQueue<std::string> Displayqueue;
	int port=0;	 
};

/////////////////////////////////////////////////////////
//class SendThread
class SendThread
{
public:
	SendThread(Socket s, BlockingQueue<std::string>& q) : s_(s), q_(q) {}
	std::string& status() { return status_; }
	void run();
	
private:
	std::string status_;
	Socket s_;
	BlockingQueue<std::string>& q_;
	int port = 0;
};
/////////////////////////////////////////////////////////
//class Sender
class Sender : public AbstractCommunicator
{
public:
	Sender(std::string name, int numMsgs, int ID,int check,std::string recpor, std::string Msg) : AbstractCommunicator(name),Recport(recpor), Check(check),numMsgs_(numMsgs), message(Msg){ myCount = ID; }
	~Sender(){};
	int id();
	void start(std::string ip, int port);
	void search(std::vector<std::string>& allfiles, int n, std::string command);
	void SendFile(std::vector<std::string>& allfiles);
	void ProcessMsg();
	void simi(std::string msg,int Num);
	void echoMsg(std::string msg, int Num);
	void textSearchMsg(std::string msg, int Num);
	void DownloadMsg(std::string msg, int Num);
	void begin(std::string ip, int port);
	void LOCK(std::mutex* lock);
	Socket& returnSocket();
	std::mutex*& retLock();
	void Dispatcher(VaultDispatcher* test);
	
private:
	Socket s_;
	BlockingQueue<std::string> q_;
	SendThread* pSt;
	std::string message;
	int myCount;
	int numMsgs_;
	//std::vector<std::string> AllFiles;
	std::mutex* lock3;
	VaultDispatcher* DISP;
	Display temp;
	std::string Recport;
	int Check;
	
	
};
///////////////////////////////////////////////////////
//class DownloadCommunicator
//class DownloadCommunicator
class DownloadCommunicator : public AbstractCommunicator
{
public:
	DownloadCommunicator(std::string name) :AbstractCommunicator(name){};
	~DownloadCommunicator(){};
	void search(std::vector<std::string>& allfiles, std::string command);
	void SendFile(std::vector<std::string>& allfiles, std::string starttime, std::string senderName);
	void ProcessMsg();
	void LOCK(std::mutex* lock);
	std::mutex*& retLock();
	void Dispatcher(VaultDispatcher* test);
private:
	//Socket s_;
	//BlockingQueue<std::string> q_;
	//SendThread* pSt;
	//std::string message;
	//int myCount;
	//int numMsgs_;
	//std::vector<std::string> AllFiles;
	std::mutex* lock3;
	VaultDispatcher* DISP;
	Display tempD;
	std::string Recport;
};




/////////////////////////////////////////////////////////
//class EchoCommunicator
class EchoCommunicator :public AbstractCommunicator
{
public:
	EchoCommunicator(std::string name) :AbstractCommunicator(name){};
	~EchoCommunicator(){};
	void Dispatcher(VaultDispatcher* test);
	void ProcessMsg();
	void ProcessMsg1();
	void LOCK(std::mutex* lock);
private:
	VaultDispatcher* DISP;
	 std::mutex* lock2;
	 Display tempD;
};
class TextSearchthread
{
public:
	TextSearchthread(std::map<std::string,std::vector<std::string>>& Result, std::vector<std::string>& KEY, BlockingQueue<std::string>& q,int& RUN) : result(Result), keys(KEY), q_(q),runing(RUN) {}
	void run();
private:
	//std::string status_;
	std::vector<std::string>& keys;
	BlockingQueue<std::string>& q_;
	std::map<std::string, std::vector<std::string>> & result;
	int& runing;
};
/////////////////////////////////////////////////////////
//class TextSearchCommunicator
class TextSearchCommunicator :public AbstractCommunicator
{
public:
	TextSearchCommunicator(std::string name) :AbstractCommunicator(name){};
	~TextSearchCommunicator(){};
	void Dispatcher(VaultDispatcher* test);
	void Analyses(Message& temp, std::vector<std::string>& strs);
	void search(std::vector<std::string>& AllFiles, std::string command);
	void ProcessMsg();
	void LOCK(std::mutex* lock);
private:
	VaultDispatcher* DISP;
	std::mutex* lock2;
	Display tempD;
	BlockingQueue<std::string> q_;
	
	
};
/////////////////////////////////////////////////////////
//class TextSearchCommunicator
class Similaritythread
{
public:
	Similaritythread( Executer* exec, BlockingQueue<std::string>& q, int& RUN) : /*result(Result),*/ Exec(exec), q_(q), runing(RUN) {}
	void run();
private:
	//std::string status_;
	//std::vector<std::string>& keys;
	BlockingQueue<std::string>& q_;
	//std::map<std::string, std::vector<std::string>> & result;
	int& runing;
	Executer* Exec;
};
class SimilarityCommunicator :public AbstractCommunicator
{
public:
	SimilarityCommunicator(std::string name) :AbstractCommunicator(name){};
	~SimilarityCommunicator(){};
	void Dispatcher(VaultDispatcher* test);
	void Analyses(Message& temp, std::vector<std::string>& strs);
	void search(std::vector<std::string>& AllFiles, std::string command);
	void ProcessMsg();
	void LOCK(std::mutex* lock);
private:
	VaultDispatcher* DISP;
	std::mutex* lock2;
	Display tempD;
	//::vector<std::string> keywords;
	BlockingQueue<std::string> q_;
	/*std::vector<std::string> AllFiles;*/
};


/////////////////////////////////////////////////////////
//class FileCommunicator
class FileCommunicator :public AbstractCommunicator
{
public:
	FileCommunicator(std::string name) :AbstractCommunicator(name){};
	~FileCommunicator(){};
	void Dispatcher(VaultDispatcher* test);
	void checkdirectory(std::string senderid);	
	void checkfile(std::string filename);

	void ProcessMsg();
	void ProcessMsg1();
	void LOCK(std::mutex* lock);
private:
	VaultDispatcher* DISP;
	std::mutex* lock2;
	std::vector<std::string> directory;
	std::vector<std::string> recordfile;
	Display tempD;
};



///////////////////////////////////////////////////
// ClientHandlerThread thread

class ClientHandlerThread
{
public:
	ClientHandlerThread(Socket s,int &a, BlockingQueue<std::string>& q) : s_(s), q_(q),A(a) {}
	void run();
	void run1();
private:
	Socket s_;
	BlockingQueue<std::string>& q_;
	int & A;
};
///////////////////////////////////////////////////
// listenThread thread
class ListenThread
{
public:
	ListenThread(int port, BlockingQueue<std::string>& q,int& a, VaultDispatcher* disp) : listener_(port),A(a), q_(q), stop_(false), Disp(disp) {}
	void stop(bool end);
	void run();
	void LOCK(std::mutex* lock);
private:
	bool stop_;
	BlockingQueue<std::string>& q_;
	VaultDispatcher* Disp;
	SocketListener listener_;
	std::mutex* lock5;
	int& A;
};
/////////////////////////////////////////////////////////
//class Receiver
class Receiver
{
public:
	void DISP(VaultDispatcher* dispa);
	void start(int port);
	

	void Listenthread(ListenThread* temp);
	void LOCK(std::mutex* lock);
private:
	BlockingQueue<std::string> q_;
	ListenThread* pLt;
	VaultDispatcher* Disp;
	std::mutex* lock4;
};
	
#endif