
//////////////////////////////////////////////////////////////////////////
//  Language:           C++11				                            //
//  Platform:           Lenovo Y410p, Windows 8.1				        //
//  Application:        Comparer for CSE687 Pr2, Sp2014					//
//  Author:		        Shishi Fu									    //
//////////////////////////////////////////////////////////////////////////
////#include "stdafx.h"
#include "Displayer.h"
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
			XMLWrite(type, child);
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
		{
			std::cout << "\n" << std::string(80, '=') << std::endl;
			std::cout << "\n Comparing File (first) " << pair.first->value().name << " and File (second) " << pair.second->value().name << "\n\n\n";
		}
		else
		{
			if (pair.first->getComplexity() >= 3)
			std::cout << "Pair:\n  "<<pair.first->value().showNum() << " Complexity: " << pair.first->getComplexity() << "\n  " << pair.second->value().showNum() << " Complexity= " << pair.second->getComplexity() << "\n\n";
		}
	}
}
// test stub for displaying
#ifdef TEST_DISPLAYER

int main(int argc, char* argv[])
{

	std::cout << "\n  Testing Displayer class";
	std::cout << "\n ========================\n";

	element eroot, ecf, ecs, egcf;
	eroot.name = "root";
	eroot.type = "class";
	eroot.lineCount = 0;
	ecf.name = "function1";
	ecf.type = "function";
	ecf.lineCount = 0;
	ecs.name = "function2";
	ecs.type = "function";
	ecs.lineCount = 0;
	egcf.name = "if";
	egcf.type = "specialkeyword";
	egcf.lineCount = 0;
	Node* pNode1 = new Node(eroot);
	Node* pNode2 = new Node(ecf);
	Node* pNode3 = new Node(ecs);
	Node* pNode4 = new Node(egcf);

	pNode1->addChild(pNode2);
	pNode2->addChild(pNode4);
	pNode1->addChild(pNode3);
	Displayer displayer;
	std::cout << "\nDisplay all nodes:\n";
	displayer.XMLDisplay("all", pNode1);
	std::cout << "\n\nDisplay all functions:\n";
	displayer.XMLDisplay("function", pNode1);
}

#endif