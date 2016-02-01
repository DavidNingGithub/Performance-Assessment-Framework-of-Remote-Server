
//////////////////////////////////////////////////////////////////////////
//  Language:           C++11				                            //
//  Platform:           Lenovo Y410p, Windows 8.1				        //
//  Application:        Code analyzer for CSE687 Pr1, Sp2014			//
//  Author:		        Shishi Fu									    //
//  Source Author:      Jim Fawcett, CST 4-187, Syracuse University     //
//                      (315) 443-3948, jfawcett@twcny.rr.com           //
//////////////////////////////////////////////////////////////////////////

// test stub for displaying
#ifdef TEST_DISPLAYER
#include "Displayer.h"
int main(int argc, char* argv[])
{

	std::cout << "\n  Testing Displayer class";
	std::cout << "\n ========================\n";

	element eroot, ecf, ecs, egcf;
	eroot.name = "root";
	eroot.type = "class";
	eroot.lineCount = 0;
	ecf.name = "function1";
	ecf.type = "function";
	ecf.lineCount = 0;
	ecs.name = "function2";
	ecs.type = "function";
	ecs.lineCount = 0;
	egcf.name = "if";
	egcf.type = "specialkeyword";
	egcf.lineCount = 0;
	Node* pNode1 = new Node(eroot);
	Node* pNode2 = new Node(ecf);
	Node* pNode3 = new Node(ecs);
	Node* pNode4 = new Node(egcf);

	pNode1->addChild(pNode2);
	pNode2->addChild(pNode4);
	pNode1->addChild(pNode3);
	Displayer displayer;
	std::cout << "\nDisplay all nodes:\n";
	displayer.XMLDisplay("all", pNode1);
	std::cout << "\n\nDisplay all functions:\n";
	displayer.XMLDisplay("function", pNode1);
}

#endif