#ifndef DISPLAYER_H
#define DISPLAYER_H

//////////////////////////////////////////////////////////////////////////
//  Displayer.h - declares MNode displayer					 		    //
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
This module gives an ordinary print and an XML representation of MNodes.

Public Interface:
=================
void DisplayNode(Node* node);
void XMLWrite(std::string type, Node* node);
void XMLDisplay(std::string type, Node* node);

Build Process:
==============
Required files
- XmlWriter.h,ActionsAndRules.h, sstream.h

Build commands (either one)
- devenv CodeAnalyzer.sln


Maintenance History:
====================
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
	void DisplaySimilarCodes(std::vector<std::pair<Node *, Node *>> similarCodes);
private:
	Node* root;
	XmlWriter wtr;
};

Displayer::Displayer()
{
	wtr.indent();
	wtr.addDeclaration();
	wtr.addComment("Max Function Display");
}

Displayer::~Displayer()
{
	delete root;
}

// Give an ordinary presentation of MNode and its children
void Displayer::DisplayNode(Node* node)
{
	std::vector<Node*> temp;
	temp = node->getChildren();
	if (temp.size() != 0)
	{
		for (auto child : temp)
			DisplayNode(child);
	}
	std::cout << node->value().show();
	if (node->value().type == "function")
		std::cout << "Node Count:" << node->getComplexity();
	std::cout << std::endl;
		
}

// Write node information into XML. Type is used to select node type to write
void Displayer::XMLWrite(std::string type, Node* node)
{
	std::vector<Node*> temp;
	element elem;
	std::stringstream ss, dd;
	elem = node->value();
	if (type == "all" || elem.type == type)
	{

		wtr.start(elem.name);
		wtr.addAttribute("Type", elem.type);
		ss << elem.lineCount;
		wtr.addAttribute("LineCount", ss.str());
		dd << node->getComplexity();
		wtr.addAttribute("Complexity", dd.str());
		//wtr.end();
	}
		
	
	temp = node->getChildren();
	if (temp.size() != 0)
	{
		for (auto child : temp)
			XMLWrite(type,child);
	}
	if (type == "all" || elem.type == type) wtr.end();
}

// Give an XML presentation of MNode and its children and write it into a xml file
void Displayer::XMLDisplay(std::string type, Node* node)
{
	XMLWrite(type, node);
	std::cout << wtr.xml();
	std::ofstream out("MaxFunc.xml");
	if (out.good())
	{
		out << wtr.xml().c_str();
		out.close();
	}
	wtr.clear();
}

void Displayer::DisplaySimilarCodes(std::vector<std::pair<Node *, Node *>> similarCodes)
{
	std::cout << "Similar codes:\n";
	for (auto pair : similarCodes)
	{
		if (pair.first->value().type == "file")
			std::cout << "\n\n Comparing File (left) " << pair.first->value().name << " and File (right) " << pair.second->value().name << "\n\n";
		else 
			std::cout << pair.first->value().showNum() << " Complexity= " << pair.first->getComplexity() << "\t" << pair.second->value().showNum() << " Complexity= " << pair.second->getComplexity() << "\n";
	}
}

#endif