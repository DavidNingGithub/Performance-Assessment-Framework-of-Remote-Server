// This is the main DLL file.

#include "stdafx.h"
#include<string>
#include "CLRclient.h"
using namespace	System;

namespace CLRclient {
	ManagedClient::ManagedClient()
	{
		abstractClient=abstractClient->Create();
	}
	void ManagedClient::start(int commandport, int checktemp, int senderid, int loopNum, String^ test1, String^ tempport1)
	{
		int q = (int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(test1);
		char* p = (char*)q;
		std::string test = std::string(p);
		int i = (int)System::Runtime::InteropServices::Marshal::StringToHGlobalAnsi(tempport1);
		char* k = (char*)i;
		std::string tempport = std::string(k);
		abstractClient->clientMain(commandport, checktemp, senderid, loopNum, test, tempport);
	}
	String^ ManagedClient::getResult()
	{
		std::string displaystring = abstractClient->returnQ();
		String^ display = gcnew String(displaystring.c_str());
		return display;
	}	
}