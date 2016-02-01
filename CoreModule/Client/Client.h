#ifndef CLIENT_H
#define CLIENT_H
#include <string>
#include <vector>
#ifdef IN_DLL
#define DLL_DECL __declspec(dllexport)
#else
#define DLL_DECL __declspec(dllimport)
#endif

class absClient
{
public:
	absClient(){};
	virtual DLL_DECL ~absClient(){}
	static DLL_DECL absClient* Create();
	virtual DLL_DECL void clientMain(int commandport, int checktemp, int senderid, int loopNum, std::string test, std::string tempport) = 0;
	virtual DLL_DECL std::string& returnQ() = 0;
};
#endif