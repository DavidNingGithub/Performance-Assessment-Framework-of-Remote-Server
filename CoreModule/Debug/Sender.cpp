/////////////////////////////////////////////////////////////////
// Sender.cpp - Demonstration of concurrent socket connectors  //
// ver 2                                                       //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2013   //
/////////////////////////////////////////////////////////////////
/*
 * This Sender expects to write lines of text only.
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
 *   BlockingQueue.h, BlockingQueue.cpp
 *
 * Maintanence History:
 * ver 1.1 - 30 Mar 2013
 * - changed Sendthread from terminating to default
 * - minor changes to error handling
 * ver 1.0 - 29 Mar 2013
 * - first release
 */


#include "Sender.h"
#include <string>
#include <iostream>
#include <sstream>
#include <thread>
#include "../FileManager/FileSystem.h"
#include "../FileManager/FileMgr.h"
///////////////////////////////////////////////////
// SendThread thread

  void SendThread::run()
  {
    status_ = "good";  
    std::string msg;
    do
    {
      msg = q_.deQ();
      if(!s_.writeLine(msg))
      {
        sout << "\n  bad status in sending thread";
        status_ = "bad";
        break;
      }
    } while(msg != "stop");
    s_.disconnect();
  }


std::string ToString(int i)
{
  std::ostringstream conv;
  conv << i;
  return conv.str();
}


void Sender::start(std::string ip, int port)
  {
    sout << locker << "\n Sender #" << id() << " started" << unlocker;
    /*if(numTestLines_ > 0)
      sout << locker << "\n  Will initially send " << numTestLines_ 
           << " messages that consist of only a single newline for testing" << unlocker;*/
    pSt = new SendThread(s_, q_);
	std::thread ST(&SendThread::run, pSt);
    if(!s_.connect(ip, port))
    {
      sout << locker << "\n  couldn't connect to " << ip << ":" << port << "\n\n" << unlocker;
      delete pSt;
      return;
    }
    else
    {
      std::string logMsg = "\n  connected to " + ip + ":" + ToString(port);    
    }
    std::string msg;
    for(int i=0; i<numMsgs_; ++i)
    {
      msg =message;
      sout << locker << "\n  " << msg.c_str() << unlocker;
      q_.enQ(msg);
      ::Sleep(10 * id());  // sleep time increases with each addition Sender
      if(pSt->status() == "bad")
        break;
    }
    q_.enQ("echo,hihi,hihi,123,dfds,erwef");
    msg = "sender#" + ToString(id()) + ": stop";
    sout << "\n  " + msg;
	ST.join();
    delete pSt;
  }
void Sender::search(int n, char* command[])
  {
	  int test = 0;
	  string path;
	  vector<string> paths;
	  FileMgr::patterns patts;
	  for (int i = 0; i < n; ++i)
	  {
		  string compare = command[i];
		  int pos = compare.find("*");
		  if (strcmp("/s", command[i]) == 0)
			  test = 1;
		  else if (pos >= 0)
		  {
			  patts.push_back(command[i]);//new
		  }
		  else
			  path = command[i];
	  }
	  FileMgr::files foundFiles;
	  FileMgr FindFile;
	  string mainpath;
	  mainpath = FileSystem::Path::getFullFileSpec(path);
	  FileSystem::FileInfo test1(mainpath);
	  bool fileOrNot = test1.isDirectory();
	  if (fileOrNot)
	  {
		  if (patts.size() == 0)
			  patts.push_back("*.cpp");
		  AllFiles = FindFile.findfile(foundFiles, mainpath, patts, test);
	  }
	  else
		  AllFiles.push_back(mainpath);
  }




int main()
{
	
////////////////////////////////////////////////////////////////////////
  int ret = 0;
  try
  {
    // run two senders concurrently
	  Display a;
	  Message dfd;
	  std::string sdfd = "echo,targetIp,sourceIp,2323,test test test,filename";
	  dfd.Analyze(sdfd);
	  a.DisplayMessage(dfd);
    Sender sndr1(10,sdfd), sndr2(10,sdfd);
	char* test[] = { "/s", "../Debug", "*.cpp" };
	sndr1.search(3, test);
	std::thread sender1(&Sender::start, &sndr1, "127.0.0.1", 8080);
	std::thread sender2(&Sender::start, &sndr2, "127.0.0.1", 8080);
    sender1.join();
    sender2.join();
  }
  catch(std::exception& ex)
  {
    sout << "\n\n  " << ex.what();
    ret = 1;
  }
  catch(...)
  {
    sout << "\n\n  something bad happend to a sender";
    ret = 1;
  }
  std::cout << "\n\n  That's all Folks!\n\n";
  return ret;
}
