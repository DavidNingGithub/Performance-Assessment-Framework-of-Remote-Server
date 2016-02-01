/////////////////////////////////////////////////////////////////
// sever.cpp - Demonstration of socket sever with              //
//                concurrent sever                             //
//                                                             //
// Tan Ning, CSE687 - Object Oriented Design, Spring 2013      //
/////////////////////////////////////////////////////////////////
/*
 * This Reciever expects to read lines of text only.
 * So message framing is done by lines.  
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
 * - Reciever.cpp, Sockets.h, Sockets.cpp, 
 *   Threads.h, Threads.cpp, Locks.h, Locks.cpp
 *   BlockingQueue.h, BlockingQueue.cpp,abstractcommunicator.cpp,abstractcommuncator.h
 *F
 * Maintanence History:
 * ver 1.1 - 30 Mar 2014
 * - changes to error handling
 * ver 1.0 - 29 Mar 2014
 * - first release
 */


#include "../Communicator/AbstractComunicator.h"
#include <string>
#include <thread>
#include <iostream>
#include <sstream>
#include <mutex>
#ifdef SEVER
int main()
{
  int ret = 0;
  try
  {
	std::mutex lock2;
    Receiver* rcvr2=new Receiver;	//create dispatcher
	VaultDispatcher* Disp2;
	EchoCommunicator* Echo2;
	FileCommunicator* File2;
	DownloadCommunicator* Download2;
	TextSearchCommunicator* Textsearch2;
	SimilarityCommunicator* Simi2;
	Sender* Send2;
	Disp2 = new VaultDispatcher("disp");//create a dispatcher
	Echo2 = new EchoCommunicator("echo");//create a echocommunicator
	File2 = new FileCommunicator("file");//create a filecommunicator
	Download2 = new DownloadCommunicator("down");
	Textsearch2 = new TextSearchCommunicator("text");
	Simi2 = new SimilarityCommunicator("simi");
	Send2 = new Sender("send",1,1,1,"8080","OOD");//create a sender
	rcvr2->LOCK(&lock2);//assign lock for each of the communicator containning receiver and dispatcher
	Disp2->LOCK(&lock2);
	File2->LOCK(&lock2);
	Echo2->LOCK(&lock2);
	Send2->LOCK(&lock2);
	Download2->LOCK(&lock2);
	Textsearch2->LOCK(&lock2);
	Simi2->LOCK(&lock2);
	Disp2->Register(Echo2);//register abstractcommunicator in the dispatcher
	Disp2->Register(File2);
	Disp2->Register(Send2);
	Disp2->Register(Download2);
	Disp2->Register(Textsearch2);
	Disp2->Register(Simi2);
	Echo2->Dispatcher(Disp2);//assign dispatcher to each of the communicator 
	File2->Dispatcher(Disp2);
	Send2->Dispatcher(Disp2);
	Download2->Dispatcher(Disp2);
	Textsearch2->Dispatcher(Disp2);
	Simi2->Dispatcher(Disp2);
	std::thread AD(&VaultDispatcher::ProcessMsg, Disp2);//create thread for each processMsg function
	std::thread SC(&Sender::ProcessMsg, Send2);
	std::thread EC(&EchoCommunicator::ProcessMsg, Echo2);
	std::thread FC(&FileCommunicator::ProcessMsg, File2);
	std::thread DC(&DownloadCommunicator::ProcessMsg, Download2);
	std::thread TC(&TextSearchCommunicator::ProcessMsg, Textsearch2);
	std::thread SI(&SimilarityCommunicator::ProcessMsg, Simi2);
	AD.detach();
	EC.detach();
	FC.detach();
	SC.detach();
	DC.detach();
	TC.detach();
	SI.detach();
	rcvr2->DISP(Disp2);
	rcvr2->start(8080);//start the receiver
	while (true);
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what();
    ret = 1;
  }
  sout << "\n\n";
  return ret;
}
#endif