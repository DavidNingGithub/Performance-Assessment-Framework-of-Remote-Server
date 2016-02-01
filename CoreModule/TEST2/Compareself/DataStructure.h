//////////////////////////////////////////////////////////////////////////
//  Comparer.h - declares new executer								    //
//  ver 1.0																//
//  Language:           C++11				                            //
//  Platform:           Lenovo Y410p, Windows 8.1				        //
//  Application:        Comparer for CSE687 Pr2, Sp2014					//
//  Author:		        Shishi Fu									    //
//////////////////////////////////////////////////////////////////////////
/*
Module Operations:
==================
This module provides datastructure definition.

Maintenance History:
====================
ver 1.0 : 12 March 14
- Created to provide datastructure definition.
*/
#ifndef DATASTRUCTURE_H
#define DATASTRUCTURE_H
#include "MNode.h"
#include "MTree.h"


struct element
{
	std::string type;
	std::string name;
	size_t startLineNum;
	size_t endLineNum;
	// size_t lineCount = endLineNum - startLineNum + 1;
	size_t lineCount;
	std::string show()
	{
		std::ostringstream temp;
		temp << "(";
		temp << type;
		temp << ", ";
		temp << name;
		temp << ", ";
		temp << lineCount;
		temp << ")";
		return temp.str();
	}
	std::string showNum()
	{
		std::ostringstream temp;
		temp << "Type: ";
		temp << type;
		temp << ", Name: ";
		temp << name;
		temp << ", Start: ";
		temp << startLineNum;
		temp << ", End: ";
		temp << endLineNum;
		temp << ", Lines: ";
		temp << lineCount;
		return temp.str();
	}
};

typedef TMTree::MNode<element> Node;
typedef TMTree::MTree<Node> Tree;


class newOper : public TMTree::Operation<Node>
{
public:
	bool operator()(Node* pNode)
	{
		std::cout << "\n -- " << pNode->value().show();
		return false;
	}
};

#endif