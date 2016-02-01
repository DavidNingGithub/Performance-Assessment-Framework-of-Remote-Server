#ifndef DISPLAYER_H
#define DISPLAYER_H

//////////////////////////////////////////////////////////////////////////
//  Displayer.h - declares MNode displayer					 		    //
//  ver 1.1																//
//  Language:           C++11				                            //
//  Platform:           Lenovo Y410p, Windows 8.1				        //
//  Application:        Comparer for CSE687 Pr2, Sp2014					//
//  Author:		        Shishi Fu									    //
//////////////////////////////////////////////////////////////////////////

/*
Module Operations:
==================
This module gives an ordinary print and an XML representation of MNodes.

Public Interface:
=================
Displayer();
~Displayer();
void DisplayNode(Node* node);
void XMLWrite(std::string type, Node* node);
void XMLDisplay(std::string type, Node* node);
void DisplaySimilarCodes(std::vector<std::pair<Node *, Node *>> similarCodes);

Build Process:
==============
Required files
- XmlWriter.h,ActionsAndRules.h, sstream.h, DataStructure.h

Build commands (either one)
- devenv Comparer.sln


Maintenance History:
====================
ver 1.1 : 21 March 14
- added new displaying interfaces.
ver 1.0 : 09 Feb 14
- Created to give an ordinary print and an XML representation of MNodes.
*/

#include <sstream>
#include <iostream>
#include <fstream>
#include "DataStructure.h"
#include "XmlWriter.h"


class Displayer
{
public:
	Displayer();
	~Displayer();
	void DisplayNode(Node* node);
	void XMLWrite(std::string type, Node* node);
	void XMLDisplay(std::string type, Node* node);
	std::string DisplaySimilarCodes(std::vector<std::pair<Node *, Node *>> similarCodes);
private:
	Node* root;
	XmlWriter wtr;
};



#endif