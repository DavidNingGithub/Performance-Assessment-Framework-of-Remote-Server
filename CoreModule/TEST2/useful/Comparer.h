#ifndef COMPARER_H
#define	COMPARER_H
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
This module compares code files to find similar codes.

Public Interface:
=================
Comparer();
~Comparer();
void SortTree(Node *p, std::multiset<Node *, NCompare> &setNodes);
bool IsSimilar(Node *n1, Node *n2);
bool IsPair(Node *first, Node *secondp);
void DeletePair(Node *first);
void Compare(Tree *tree1, Tree *tree2);
std::vector<std::pair<Node *, Node *>> getSimilarCodes();
void ClearSimilarCodes();

Build Process:
==============
Required files
- DataStructure.h, vector.h, set.h

Build commands (either one)
- devenv Comparer.sln
- command parameters: test *.h *.cpp 0 /s 

Maintenance History:
====================
ver 1.0 : 12 March 14
- Created to compare code files and get similar codes.
*/
#include <vector>
#include <set>
#include "DataStructure.h"

class NCompare
{
public:
	bool    operator () (Node *b1, Node *b2) const{
		return b1->getComplexity()<b2->getComplexity();
	}
};


class Comparer
{
private:
	std::vector<std::pair<Node *, Node *>> similarnodes;	
public:
	~Comparer();
	void SortTree(Node *p, std::multiset<Node *, NCompare> &setNodes);
	bool IsSimilar(Node *n1, Node *n2);
	bool IsPair(Node *first, Node *secondp);
	void DeletePair(Node *first);
	void Compare(Tree *tree1, Tree *tree2);
	std::vector<std::pair<Node *, Node *>> getSimilarCodes();
	void ClearSimilarCodes();
};



#endif