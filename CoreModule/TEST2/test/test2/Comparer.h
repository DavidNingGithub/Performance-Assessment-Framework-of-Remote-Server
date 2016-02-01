#ifndef COMPARE_H
#define	COMPARE_H

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


class Comparer{
private:
	std::vector<std::pair<Node *, Node *>> similarnodes;	
public:
	Comparer(){}
	~Comparer();
	void SortTree(Node *p, std::multiset<Node *, NCompare> &setNodes);
	bool IsSimilar(Node *n1, Node *n2);
	bool IsPair(Node *first, Node *secondp);
	void DeletePair(Node *first);
	void Compare(Tree *tree1, Tree *tree2);
	std::vector<std::pair<Node *, Node *>> getSimilarCodes();
	void ClearSimilarCodes();
};

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
		if ((*pair).first == first) pair = similarnodes.erase(pair);
		else pair++;
	}
	
}
bool Comparer::IsSimilar(Node *n1, Node *n2)
{
	element e1, e2;
	int linedif;
	e1 = n1->value();
	e2 = n2->value();
	linedif = e1.lineCount - e2.lineCount;
	if (e1.type != e2.type) return false;
	if (e1.lineCount == e2.lineCount)
	{
		std::vector<Node *> temp1, temp2;
		temp1 = n1->getChildren();
		temp2 = n2->getChildren();
		if (temp1.size() == 0 && temp2.size()==0) return true;
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
	std::pair<std::multiset<Node *, NCompare>::iterator, std::multiset<Node *, NCompare>::iterator> p;
	SortTree(tree1->getTRoot(), SNodes1);
	SortTree(tree2->getTRoot(), SNodes2);
	SNodes1.erase(tree1->getTRoot());
	SNodes2.erase(tree2->getTRoot());
	similarnodes.push_back(std::make_pair(tree1->getTRoot(), tree2->getTRoot()));
	for (SNIter1 = SNodes1.begin(); SNIter1 != SNodes1.end(); SNIter1++)
	{

		p = SNodes2.equal_range(*SNIter1);

		//for (SNIter2 = SNodes2.begin(); SNIter2 != SNodes2.end(); SNIter2++)
		for (SNIter2 = p.first; SNIter2 != p.second; SNIter2++)
		{

			if (IsSimilar(*SNIter1, *SNIter2))
			{
				similarnodes.push_back(std::make_pair(*SNIter1, *SNIter2));
				//delete children pairs
				child = (*SNIter1)->getChildren();
				for (auto first : child)
					DeletePair(first);
			}
			else continue;
			
		}
		
	}
	
	
}
std::vector<std::pair<Node *, Node *>> Comparer::getSimilarCodes()
{
	return similarnodes;
}

void Comparer::ClearSimilarCodes()
{
	similarnodes.clear();
}

#endif