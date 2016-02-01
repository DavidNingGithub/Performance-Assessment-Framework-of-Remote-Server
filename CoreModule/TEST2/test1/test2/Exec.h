#ifndef	EXEC_H
#define EXEC_H
//////////////////////////////////////////////////////////////////////////
//  Exec.h - declares new executer									    //
//  ver 1.0																//
//  Language:           C++11				                            //
//  Platform:           Lenovo Y410p, Windows 8.1				        //
//  Application:        Code analyzer for CSE687 Pr1, Sp2014			//
//  Author:		        Shishi Fu									    //
//  Source Author:      Jim Fawcett, CST 4-187, Syracuse University     //
//                      (315) 443-3948, jfawcett@twcny.rr.com           //
//////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module analyzes code file and show results.

Public Interface:
=================
FileMgr::files readFiles();
Tree* analyze(std::string pFileName);
std::vector<Node *> getMaxFuncNode(Tree* tree);

Build Process:
==============
Required files
- FileSystem.h, FileSystem.h, Parser.h, ConfigureParser.h, Displayer.h

Build commands (either one)
- devenv CodeAnalyzer.sln
- command parameters: test *.h *.cpp 0 /s /b

Maintenance History:
====================
ver 1.0 : 09 Feb 14
- Created to analyse code file and display result.
*/

#include <vector>
#include "FileMgr.h"
#include "FileSystem.h"
#include "Parser.h"
#include "ConfigureParser.h"
#include "Displayer.h"
#include "Comparer.h"


class Executer
{
public:
	Executer(int margc, char** margv);
	~Executer();
	FileMgr::files readFiles();
	void analyze(std::string pFileName);
	void getMaxFuncNode(Tree* tree);
	void Display(Tree *tree);
	void Compare();
	void DisplaySimilarCodes();
private:
	int   argc;
	char **argv;
	FileMgr fm;
	std::vector<Tree *> trees;
	std::vector<Node *> MaxFunc;
	std::vector<std::pair<Node *, Node *>> similarCodes;
	Displayer printer;
	Comparer comparer;

};

Executer::Executer(int margc, char** margv)
{
	argc = margc;
	argv = margv;

}

Executer::~Executer()
{
	delete argv;


}

// Read files from certain path and return found files of certain pattern
FileMgr::files Executer::readFiles()
{
	int i;
	//std::cout << "\n Read Files:";
	FileMgr::patterns patts;
	FileMgr::files foundFiles;
	std::string path, pattern, select;
	path = argv[1];
	//read patterns
	for (i = 2; i < argc; i++)
	{
		pattern = argv[i];
		if (pattern == "0") break;
		patts.push_back(pattern);
	}
	select = argv[++i];
	if (select == "/s")
		foundFiles = fm.searchAllDirs(path, patts);
	else
		foundFiles = fm.search(path, patts);
	//fm.showFiles();
	return foundFiles;
}

// Analyze the code file and add result tree to trees
void Executer::analyze(std::string pFileName)
{
	Tree *tree;
	//std::cout << "\nAnalyze file: "<< std::string(22, '=') << std::endl;
	// collecting tokens from files, named on the command line
	if (pFileName.length() == 0)
	{
		std::cout << "\n  Wrong file input\n\n";
		return;
	}
	ConfigParseToConsole configure;
	Parser* pParser = configure.Build();
	try
	{
		if (pParser)
		{
			if (!configure.Attach(pFileName))
			{
				std::cout << "\n  could not open file " << pFileName << std::endl;
				return;
			}
		}
		else
		{
			std::cout << "\n\n  Parser not built\n\n";
			return;
		}
		// now that parser is built, use it
		while (pParser->next())
			pParser->parse();
		std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}

	tree = configure.getMTRoot();
	Node* root = tree->getTRoot();
	if (root == NULL) return;
	root->value().name = pFileName;
	root->value().startLineNum = 1;
	trees.push_back(tree);
	//return trees;
}

// Get functions with maximum node count
void Executer::getMaxFuncNode(Tree* tree)
{
	std::vector<Node *> temp;
	int t, maxFunc = 0;
	temp = tree->getTRoot()->getChildren();
	for (auto node : temp)
	{
		t = node->getComplexity();
		if (node->value().type == "function" && t >= maxFunc)
			maxFunc = t;
	}
	for (auto node : temp)
	{
		if (node->value().type == "function" && node->getComplexity() == maxFunc)
			MaxFunc.push_back(node);
	}
	temp.clear();

}

// Display results of file analysis
void Executer::Display(Tree *tree)
{
	std::string s = argv[argc - 1];
	if (s == "/b")
	{
		std::cout << "\n\tXMLDisplay the functions:" << std::endl;
		printer.XMLDisplay("function", tree->getTRoot());
	}
	else
	{
		std::cout << "\n\tXMLDisplay the file tree:" << std::endl;
		printer.XMLDisplay("all", tree->getTRoot());
	}

	getMaxFuncNode(tree);
	std::cout << "\n\n\tXMLDisplay MaxFunctions num=" << MaxFunc.size() << std::endl;
	for (auto node : MaxFunc)
	{
		printer.XMLDisplay("all", node);
	}
	MaxFunc.clear();
}
void Executer::Compare()
{
	
	int i, j, size;
	size = trees.size();
	for (i = 0; i < size; i++) 
	{
		for (j = size-1; j != i;j--)
		{
			int ccdif;
			ccdif = trees[i]->getTRoot()->getComplexity() - trees[j]->getTRoot()->getComplexity();
			if (ccdif > 30 || ccdif < -30) continue;
			comparer.Compare(trees[i], trees[j]);
			/*
			std::vector<std::pair<Node *, Node *>> similarCode;
			similarCode = comparer.getSimilarCodes();
			for (auto pair : similarCode)
				similarCodes.push_back(pair);
			comparer.ClearSimilarCodes();
			*/
		}
	}
	
	
}

void Executer::DisplaySimilarCodes()
{

	
	printer.DisplaySimilarCodes(similarCodes);
}

#endif