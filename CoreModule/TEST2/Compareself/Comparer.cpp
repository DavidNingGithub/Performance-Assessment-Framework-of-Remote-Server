//////////////////////////////////////////////////////////////////////////
//  Language:           C++11				                            //
//  Platform:           Lenovo Y410p, Windows 8.1				        //
//  Application:        Comparer for CSE687 Pr2, Sp2014					//
//  Author:		        Shishi Fu									    //
//////////////////////////////////////////////////////////////////////////
//#include <stdafx.h>
//#include "stdafx.h"
#include "Comparer.h"
Comparer::~Comparer()
{
	similarnodes.clear();
}

void Comparer::SortTree(Node *p, std::multiset<Node *, NCompare> &setNodes)
{
	std::vector<Node *> temp;
	setNodes.insert(p);
	temp = p->getChildren();
	for (auto node : temp)
	{
		SortTree(node, setNodes);
	}
	temp.clear();
}


bool Comparer::IsPair(Node *first, Node *secondp)
{
	for (auto pair : similarnodes)
	{
		if (pair.first == first && pair.second->getPar() == secondp) return true;
	}
	return false;
}

void Comparer::DeletePair(Node *first)
{
	std::vector<std::pair<Node *, Node *>>::iterator pair;
	for (pair = similarnodes.begin(); pair != similarnodes.end();)
	{
		if ((*pair).first == first) 
			pair = similarnodes.erase(pair); 
		else pair++;
	}

}
bool Comparer::IsSimilar(Node *n1, Node *n2)
{
	element e1, e2;
	int linedif;
	e1 = n1->value();
	e2 = n2->value();
	if (e1.type != e2.type) return false;
	linedif = e1.lineCount - e2.lineCount;
	if (linedif < 5 && linedif > -5)
	{
		std::vector<Node *> temp1, temp2;
		temp1 = n1->getChildren();
		temp2 = n2->getChildren();
		if (temp1.size() == 0 && temp2.size() == 0) return true;
		if (temp2.size() != 0)
		for (auto node1 : temp1)
		if (IsPair(node1, n2)) return true;

	}
	return false;
}

void Comparer::Compare(Tree *tree1, Tree *tree2)
{
	std::multiset<Node *, NCompare> SNodes1, SNodes2;
	std::multiset<Node *, NCompare>::iterator SNIter1, SNIter2;
	std::vector<Node *> child;
	element e1, e2;
	bool delpair=false;
	std::pair<std::multiset<Node *, NCompare>::iterator, std::multiset<Node *, NCompare>::iterator> p;
	SortTree(tree1->getTRoot(), SNodes1);
	SortTree(tree2->getTRoot(), SNodes2);
	SNodes1.erase(tree1->getTRoot());
	SNodes2.erase(tree2->getTRoot());
	for (SNIter1 = SNodes1.begin(); SNIter1 != SNodes1.end(); SNIter1++)
	{

		p = SNodes2.equal_range(*SNIter1);

		//for (SNIter2 = SNodes2.begin(); SNIter2 != SNodes2.end(); SNIter2++)		
		for (SNIter2 = p.first; SNIter2 != p.second; SNIter2++)
		{
			
			if (IsSimilar(*SNIter1, *SNIter2))
			{
				delpair = true;
				similarnodes.push_back(std::make_pair(*SNIter1, *SNIter2));
			}
			else continue;

		}
			//similarnodes.push_back(std::make_pair(*SNIter1, *SNIter2));
			//delete children pairs
		child = (*SNIter1)->getChildren();
		if (delpair)
		{
			for (auto first : child)
				DeletePair(first);
			delpair = false;
		}
				
	}


}
std::vector<std::pair<Node *, Node *>> Comparer::getSimilarCodes()
{
	std::vector<std::pair<Node *, Node *>>::iterator it;
	for (it = similarnodes.begin(); it != similarnodes.end();)
	{
		if ((*it).first->getComplexity() == 1)
		{
			it = similarnodes.erase(it);

		}
		else it++;
	}
	return similarnodes;
}

void Comparer::ClearSimilarCodes()
{
	similarnodes.clear();
}

#ifdef TEST_COMPARER
#include <iostream>
int main(int argc, char* argv[])
{
	std::cout << "\n  Testing Displayer class";
	std::cout << "\n ========================\n";

	element elem[5];
	elem[0].name = "root";
	elem[0].type = "function";
	elem[0].lineCount = 10;
	elem[1].name = "if";
	elem[1].type = "condition";
	elem[1].lineCount = 3;
	elem[2].name = "for";
	elem[2].type = "loop";
	elem[2].lineCount = 3;
	elem[3].name = "else";
	elem[3].type = "condition";
	elem[3].lineCount = 5;
	elem[4].name = "while";
	elem[4].type = "loop";
	elem[4].lineCount = 10;
	pNode1->addChild(pNode2);
	pNode1->addChild(pNode3);
	pNode1->setComplexity(4);

	pNode2->addChild(pNode4);
	pNode2->addChild(pNode5);
	pNode2->setComplexity(2);

	pNode6->addChild(pNode7);
	pNode6->addChild(pNode8);
	pNode6->setComplexity(4);

	pNode7->addChild(pNode9);
	pNode7->addChild(pNode10);
	pNode7->setComplexity(2);

	Tree *tree1, *tree2;
	Comparer comparer;
	std::vector<std::pair<Node *, Node *>> similarcodes;
	pNode1->addChild(pNode2);
	pNode1->addChild(pNode3);
	pNode2->addChild(pNode4);
	pNode2->addChild(pNode5);

	pNode6->addChild(pNode7);
	pNode6->addChild(pNode8);
	pNode7->addChild(pNode9);
	pNode7->addChild(pNode10);

	tree1 = new Tree();
	tree2 = new Tree();
	tree1->makeRoot(pNode1);
	tree2->makeRoot(pNode6);

	comparer.Compare(tree1, tree2);
	similarcodes = comparer.getSimilarCodes();
	std::cout << "\nDisplay similar codes:\n";
	for (auto pair : similarcodes)
		std::cout << pair.first->value().show() << "\t" << pair.second->value().show() << "\n";

}
#endif