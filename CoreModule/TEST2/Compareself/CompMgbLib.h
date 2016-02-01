#ifndef CPMGDLIB_H
#define CPMGDLIB_H
#include "Comparer.h"
using namespace System;

namespace CPMgdLib
{
	public ref class Comparer{
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

}
#endif