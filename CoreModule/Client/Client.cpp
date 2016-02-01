/////////////////////////////////////////////////////////////////////
//  Client.pp -  Client for project 3						       //
//  ver 1.0                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2013                                 //
//  Application:   CSE687, Pr3					                   //
//  Author:        Tao Ning								           //
/////////////////////////////////////////////////////////////////////
/*
* This is the client for project 3 which has a sender for sending
* message and a reciever for reponded message
*
* Required files :
* - Sockets.h, Sockets.cpp,
*   BlockingQueue.h, BlockingQueue.cpp,
*   Reciever.h, Reciever.cpp, Sender.h, Sender.cpp
*   FileMgr.h, FileMgr.cpp, FileSystem.h, FileSystem.cpp
*
* Build Commands
*   devenv Project3S14.sln /rebuild Debug
* Command arguments
*   Client: path, pattern, file type(1 for text, 2 for binary), server port, client port
*   Server: server port
*
* Maintanence History :
*ver 1.0 - 10 April 2014
* -first release
*/
#define IN_DLL
#include "Client.h"
#include "../Communicator/AbstractComunicator.h"
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <mutex>
#include <vector>

class client : public absClient
{
private:
	std::string display;
public:
	client();
	virtual ~client(){};
	void clientMain(int commandport, int checktemp, int senderid, int loopNum, std::string test, std::string tempport);
	std::string& returnQ();	
};
client::client() :absClient(){}
absClient* absClient::Create(){ return new client(); }
void client::clientMain(int commandport, int checktemp, int senderid, int loopNum, std::string test,std::string tempport)
{		
		std::mutex* lock = new std::mutex;
		Sender sndr1("send", loopNum, senderid * 2 + 1, checktemp, tempport, test);// sndr2("send", loopNum, senderid * 2 + 2, checktemp, tempport, test);
		Receiver* rcvr1 = new Receiver;//create sender and receiver 
		VaultDispatcher* Disp1;
		Disp1 = new VaultDispatcher("disp");
		EchoCommunicator* Echo1;
		FileCommunicator* File1;
		Echo1 = new EchoCommunicator("echo");
		File1 = new FileCommunicator("file");
		Disp1->Register(Echo1);
		Disp1->Register(File1);
		Echo1->Dispatcher(Disp1);
		File1->Dispatcher(Disp1);
		rcvr1->LOCK(lock);//assign lock to each of the communicator 
		Disp1->LOCK(lock);
		Echo1->LOCK(lock);
		File1->LOCK(lock);
		sndr1.LOCK(lock);
		std::thread sender1(&Sender::start, &sndr1, "127.0.0.1", 8080);
		std::thread AD(&VaultDispatcher::ProcessMsg1, Disp1);//open the thread to each of the processMsg function
		std::thread EC(&EchoCommunicator::ProcessMsg1, Echo1);
		std::thread FC(&FileCommunicator::ProcessMsg1, File1);
		rcvr1->DISP(Disp1);
		rcvr1->start(commandport);
		AD.join();
		EC.join();
		FC.join();
		sender1.join();
		while (Disp1->DisplayQ().size() != 0)
		{
			std::string getresult = Disp1->DisplayQ().deQ();
			display += '?' + getresult;
		}
}
std::string& client::returnQ()
{
	return display;
}
int main(int argc, char* argv[])
{
	int ret = 0;
	std::string tempport = argv[2];
	int commandport = atoi(tempport.c_str());
	std::string tempstring1 = argv[3];
	int checktemp = atoi(tempstring1.c_str());
	std::string tempstring2 = argv[4];
	int senderid = atoi(tempstring2.c_str());
	std::string loopnum = argv[5];
	int loopNum = atoi(loopnum.c_str());
	std::string test = argv[1];
	client* newOne= new client();
	newOne->clientMain(commandport,checktemp, senderid, loopNum, test, tempport);
	std::string resultget = newOne->returnQ();	
	return ret;
}