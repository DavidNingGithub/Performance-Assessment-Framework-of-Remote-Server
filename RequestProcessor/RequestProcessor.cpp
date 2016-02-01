/////////////////////////////////////////////////////////////////
// RequestProcessor.cpp - Emit programmed requests to Server   //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014   //
/////////////////////////////////////////////////////////////////
/*
 *  What you need to add/change to be compliant with requirements:
 *  - replace the mock channel with your real channel.
 *  - Add processing communicators for TextSearch and/or Scope Analysis
 *    to your server.
 *  - Modify the processing requests as needed to match your server
 *    syntax.
 *  - Modify the information returned to the Tester to provide
 *    performance information instead of the debugging data I've used
 *    here.
 *  - 
 */
#define IN_DLL
#include "IComm.h"
#include "Message.h"
#include "Cpp11-BlockingQueue.h"
#include <thread>
#include <functional>

class RequestProcessor : public IComm
{
public:
  RequestProcessor();
  virtual ~RequestProcessor() { }
  virtual void PostMessage(const Message& msg);
  virtual Message GetMessage();
  bool OutQueueEmpty();
  void doRequest(const Message& msg, size_t count);
  void doRequest(int argc, char* argv[]);
  void processing(int argc, char* argv[]);
private:
  BlockingQueue<Message> inQ;
  BlockingQueue<Message> outQ;
  std::thread rpThread;
};
//----< factory for RequestProcessors >--------------------------------

IComm* IComm::Create() { return new RequestProcessor(); }

//----< Constructor creates mock channel thread >----------------------

RequestProcessor::RequestProcessor()
{
  std::function<void()> f = [&]()
  {
    Message msg, reply;
    while (true)
    {
      msg = inQ.deQ();

      if (msg.cmd() == "stop")
      {
        reply.clear();
        reply.add("stop");
      }
      else
      {
        reply.clear();
        reply.add("reply : time = 354 millisec");
      }
      reply.add("dstMachine : 127.0.0.1 : 8085").add("dstName : display");

      outQ.enQ(reply);
    }
  };
  rpThread = std::thread(f);
  rpThread.detach();
}
//----< make multiple requests for specified message >-----------------

void RequestProcessor::doRequest(const Message& msg, size_t count)
{
  for (size_t i = 0; i < count; ++i)
    PostMessage(msg);
}
//----< parse commandline to make message and request >----------------

void RequestProcessor::doRequest(int argc, char* argv[])
{
  if (argc < 3)
    throw std::exception("pass command type and parameters on commandline");
  if (argc < 4 && argv[1] == "TextSearch")
    throw std::exception("TextSearch needs count and search string arguments");

  // valid commandline args
  // - TextSearch 3 "a search string"
  // - FileUpload RequestProcessor.cpp

  if (std::string(argv[1]) == "TextSearch" || std::string(argv[1]) == "FileUpload")
  {
    Message msg(std::string(argv[1]) + " : " + std::string(argv[2]));
    if (std::string(argv[1]) == "TextSearch")
    {
      msg.add("SearchString : " + std::string(argv[3]));
      msg.add("dstName : TextSearch");
    }
    else
    {
      msg.add("dstName : FileUpload");
    }
    msg.add("dstMachine : 127.0.0.1 : 8080");
    std::cout << "\n  posting message:" << msg.show();
    PostMessage(msg);
  }
}
//----< post message to mock channel >---------------------------------

void RequestProcessor::PostMessage(const Message& msg)
{
  inQ.enQ(msg);
}
//----< get message from mock channel >--------------------------------

Message RequestProcessor::GetMessage()
{
  return outQ.deQ();
}
//----< is the output queue empty so I can shut down? >----------------

bool RequestProcessor::OutQueueEmpty()
{
  bool test = outQ.size() == 0;
  return test;
}
//----< demo the RequestProcessor >------------------------------------

void RequestProcessor::processing(int argc, char* argv[])
{
  std::cout << "\n  commandline arguments are:";
  for (int i = 0; i < argc; ++i)
    std::cout << "\n  " << std::string(argv[i]);
  std::cout << "\n";

  try
  {
    doRequest(argc, argv);
    Message msg = GetMessage();
    std::cout << msg.show();
    Message stop("stop");
    PostMessage(stop);
    Message reply = GetMessage();
    std::cout << reply.show();
  }
  catch (std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what();
  }
  while (OutQueueEmpty() == false)
    GetMessage();
}
//----< demonstrate RequestProcessor >---------------------------------

int main(int argc, char* argv[])
{
  std::cout << "\n  Testfdsafdasfasdfsadfsdafsdafsdafing RequestProcessor with Mock Channel";
  std::cout << "\n ============================================";

  RequestProcessor rp;
  rp.processing(argc, argv);

  /////////////////////////////////////////////////////////
  // use these statements for TestSpawn::TestInWindow()
  //----------------------------------------------------
  //std::cout << "\n\n" << "RequestProcessor Completed\n";
  //std::cout << "\n  press key and Enter to exit: ";
  //char ch;
  //std::cin >> ch;
}

