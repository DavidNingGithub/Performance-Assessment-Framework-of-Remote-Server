#ifndef COMPARE_H
#define COMPARE_H
#include <vector>
#include "Parser.h"
#include "ConfigureParser.h"
#include "Displayer.h"
#include "Comparer.h"

class Compare
{
private:
	std::vector<Tree *> trees;
	std::vector<std::pair<Node *, Node *>> similarCodes;
	Displayer printer;
	Comparer comparer;
public:
	void analyze(std::string pFileName);
	void CompareFiles(std::string file1, std::string file2);
	std::string DisplaySimilarCodes();
	void Clear();
};

void Compare::Clear()
{
	trees.clear();
	similarCodes.clear();
}
// Analyze the code file and add result tree to trees
void Compare::analyze(std::string pFileName)
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
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}

	tree = configure.getMTRoot();
	Node* root = tree->getTRoot();
	if (root == NULL) return;
	root->value().name = pFileName;
	trees.push_back(tree);
}

void Compare::CompareFiles(std::string file1, std::string file2)
{
	analyze(file1);
	analyze(file2);
	int i, j, size;
	size = trees.size();
	for (i = 0; i < size; i++)
	{
		for (j = size - 1; j != i; j--)
		{
			int ccdif;
			ccdif = trees[i]->getTRoot()->getComplexity() - trees[j]->getTRoot()->getComplexity();
			comparer.Compare(trees[i], trees[j]);
			std::vector<std::pair<Node *, Node *>> similarCode;
			similarCode = comparer.getSimilarCodes();
			if (similarCode.size() != 0)
				similarCodes.push_back(std::make_pair(trees[i]->getTRoot(), trees[j]->getTRoot()));
			for (auto pair : similarCode)
				similarCodes.push_back(pair);
			comparer.ClearSimilarCodes();
		}
	}


}

std::string Compare::DisplaySimilarCodes()
{
	std::string result;
	for (auto pair : similarCodes)
	{
		if (pair.first->value().type == "file")
		{
			/*std::cout << "\n" << std::string(80, '=') << std::endl;
			std::cout << "\n Comparing File (first) " << pair.first->value().name << " and File (second) " << pair.second->value().name << "\n\n\n";
*/
			result = result + "\n Comparing File (first) " + pair.first->value().name + " and File (second) " + pair.second->value().name + "\n";
		}
		else
		{
			if (pair.first->getComplexity() >= 3)
				//std::cout << "Pair:\n  " << pair.first->value().showNum() << " Complexity: " << pair.first->getComplexity() << "\n  " << pair.second->value().showNum() << " Complexity= " << pair.second->getComplexity() << "\n\n";
				result = result + "Pair:\n  " + pair.first->value().showNum()  + "\n  " + pair.second->value().showNum() + "\n\n";
		}
	}
	return result;
}
#endif