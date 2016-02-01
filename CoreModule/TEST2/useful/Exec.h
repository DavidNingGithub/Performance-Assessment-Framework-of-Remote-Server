#ifndef	EXEC_H
#define EXEC_H
//////////////////////////////////////////////////////////////////////////
//  Exec.h - declares new executer									    //
//  ver 1.1																//
//  Language:           C++11				                            //
//  Platform:           Lenovo Y410p, Windows 8.1				        //
//  Application:        Comparer for CSE687 Pr2, Sp2014					//
//  Author:		        Shishi Fu									    //
//////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module analyzes code file and show results.

Public Interface:
=================
FileMgr::files readFiles();
void analyze(std::string pFileName);
void getMaxFuncNode(Tree* tree);
void Display(Tree *tree);
void Compare();
void DisplaySimilarCodes();

Build Process:
==============
Required files
- FileSystem.h, FileSystem.h, Parser.h, ConfigureParser.h, Displayer.h, Comparer.h

Build commands (either one)
- devenv Comparer.sln
- command parameters: test *.h *.cpp 0 /s 

Maintenance History:
====================
ver 1.1 : 12 March 14
- Added file comparison interfaces.
ver 1.0 : 09 Feb 14
- Created to analyse code file and display result.
*/

#include <vector>

#include "Parser.h"
#include "ConfigureParser.h"
#include "Displayer.h"
#include "Comparer.h"


class Executer
{
public:
	Executer(int margc, char** margv);
	~Executer();
	//FileMgr::files readFiles();
	void analyze(std::string pFileName);
	void getMaxFuncNode(Tree* tree);
	void Display(Tree *tree);
	void Compare();
	void DisplaySimilarCodes();
private:
	int   argc;
	char **argv;
	//FileMgr fm;
	std::vector<Tree *> trees;
	std::vector<Node *> MaxFunc;
	std::vector<std::pair<Node *, Node *>> similarCodes;
	Displayer printer;
	Comparer comparer;

};



#endif