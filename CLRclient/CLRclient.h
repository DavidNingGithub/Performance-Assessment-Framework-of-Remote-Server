// CLRclient.h

#pragma once
#include "../TEST - ����/Client/Client.h"
#include<string>
using namespace System;

namespace CLRclient {

	public ref class ManagedClient
	{
	public:
		ManagedClient();
		void start(int commandport, int checktemp, int senderid, int loopNum, String^ test, String^ tempport);
		String^ getResult();
	private:
		absClient* abstractClient;
	};
}
