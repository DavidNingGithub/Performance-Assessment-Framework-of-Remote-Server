#ifndef ACTIONSANDRULES_H
#define ACTIONSANDRULES_H
//////////////////////////////////////////////////////////////////////////
//  ActionsAndRules.h - declares new parsing rules and actions		    //
//  ver 3.0																//
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
  This module defines several action classes.  Its classes provide 
  specialized services needed for specific applications.  The modules
  Parser, SemiExpression, and Tokenizer, are intended to be reusable
  without change.  This module provides a place to put extensions of
  these facilities and is not expected to be reusable. 

  Public Interface:
  =================
  Toker t(someFile);              // create tokenizer instance
  SemiExp se(&t);                 // create a SemiExp attached to tokenizer
  Parser parser(se);              // now we have a parser
  Rule1 r1;                       // create instance of a derived Rule class
  Action1 a1;                     // create a derived action
  r1.addAction(&a1);              // register action with the rule
  parser.addRule(&r1);            // register rule with parser
  while(se.getSemiExp())          // get semi-expression
    parser.parse();               //   and parse it

  Build Process:
  ==============
  Required files
    - Parser.h, Parser.cpp, ScopeStack.h, ScopeStack.cpp,
      ActionsAndRules.h, ActionsAndRules.cpp, ConfigureParser.cpp,
      ItokCollection.h, SemiExpression.h, SemiExpression.cpp, tokenizer.h, tokenizer.cpp
  Build commands (either one)
    - devenv CodeAnalyzer.sln
    - cl /EHsc /DTEST_PARSER parser.cpp ActionsAndRules.cpp \
         semiexpression.cpp tokenizer.cpp /link setargv.obj

  Maintenance History:
  ====================
  ver 3.0 : 09 Feb 14
  - added new rules and actions to scope
  ver 2.0 : 01 Jun 11
  - added processing on way to building strong code analyzer
  ver 1.1 : 17 Jan 09
  - changed to accept a pointer to interfaced ITokCollection instead
    of a SemiExpression
  ver 1.0 : 12 Jan 06
  - first release

*/

#include <queue>
#include <string>
#include <sstream>
#include "Parser.h"
#include "ITokCollection.h"
#include "ScopeStack.h"
#include "Tokenizer.h"
#include "SemiExpression.h"
#include "DataStructure.h"

///////////////////////////////////////////////////////////////
// ScopeStack element is application specific

///////////////////////////////////////////////////////////////
// Repository instance is used to share resources
// among all actions.

class Repository  // application specific
{
  ScopeStack<element> stack;
 
  Toker* p_Toker;
  Node *p_Root;// node for MTree root 
  Node *p_Cur;// node record current node to be the parent of the next node
  Tree *p_Tree;// MTree to store found scopes 
public:
	
  Repository(Toker* pToker)
  {
    p_Toker = pToker;
	element e;
	e.name = "root";
	e.type = "file";
	e.lineCount = 0;
	p_Root = new Node(e);
	p_Root->setPar(NULL);
	p_Cur = p_Root;
	p_Tree = new Tree(new newOper);
	p_Tree->makeRoot(p_Root);
  }
  ScopeStack<element>& scopeStack()
  {
    return stack;
  }

  Toker* Toker()
  {
    return p_Toker;
  }
  size_t lineCount()
  {
    return (size_t)(p_Toker->lines()+1);
  }

  Node *getRoot(){ return p_Root; }
  Node *getCur(){ return p_Cur; }
  Tree *getTree(){ return p_Tree; }
  void setCur(Node *p) { p_Cur = p; }

};

///////////////////////////////////////////////////////////////
// rule to detect beginning of anonymous scope

class BeginningOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--BeginningOfScope rule";
    if(pTc->find("{") < pTc->length())
    {
      doActions(pTc);
      return false;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePush : public IAction
{
  Repository* p_Repos;
public:
  HandlePush(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    element elem;
    elem.type = "unknown";
    elem.name = "anonymous";
    elem.lineCount = p_Repos->lineCount();
    p_Repos->scopeStack().push(elem);
  }
};

///////////////////////////////////////////////////////////////
// rule to detect end of scope

class EndOfScope : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--EndOfScope rule";
    if(pTc->find("}") < pTc->length())
    {
      doActions(pTc);
      return false;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to handle scope stack at end of scope

class HandlePop : public IAction
{
  Repository* p_Repos;
  
public:
  HandlePop(Repository* pRepos)
  {
    p_Repos = pRepos;
  }
  void doAction(ITokCollection*& pTc)
  {
    if(p_Repos->scopeStack().size() == 0)
      return;
    element elem = p_Repos->scopeStack().pop();
    if(elem.name != "anonymous")
    {

	  Node *pCur;
	  pCur = p_Repos->getCur();

	  pCur->value().startLineNum = elem.lineCount;
	  elem.lineCount = p_Repos->lineCount() - elem.lineCount + 1;
	  pCur->value().endLineNum = p_Repos->lineCount();
	  pCur->value().lineCount = elem.lineCount;
	  //std::cout << "\n" << pCur->value().showNum();
	  int comp = pCur->getComplexity();

	  //change current parent node
	  pCur = pCur->getPar();
	  if (pCur == NULL)
		  pCur = p_Repos->getRoot();
	  pCur->setComplexity(comp);
	  p_Repos->setCur(pCur);

    }

	
	
  }
};

///////////////////////////////////////////////////////////////
// rule to detect preprocessor statements

class PreprocStatement : public IRule
{
public:
  bool doTest(ITokCollection*& pTc)
  {
    //std::cout << "\n--PreprocStatement rule";
    if(pTc->find("#") < pTc->length())
    { 
      doActions(pTc);
      return true;
    }
    return false;
  }
};

///////////////////////////////////////////////////////////////
// action to print preprocessor statement to console

class PrintPreproc : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  Preproc Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// rule to detect function definitions

class FunctionDefinition : public IRule
{
public:
  bool isSpecialKeyWord(const std::string& tok)
  {
    const static std::string keys[]
      = { "for", "while", "switch", "if", "catch" };
    for(int i=0; i<5; ++i)
      if(tok == keys[i])
        return true;
    return false;
  }
  bool doTest(ITokCollection*& pTc)
  {
    ITokCollection& tc = *pTc;
    if(tc[tc.length()-1] == "{")
    {
      size_t len = tc.find("(");
	  if (len < tc.length() && !isSpecialKeyWord(tc[len - 1]))
      {
			  //std::cout << "\n--FunctionDefinition rule";
			  doActions(pTc);
        return true;
      }
    }
    return false;
  }
};



///////////////////////////////////////////////////////////////
// action to push function name onto ScopeStack

class PushFunction : public IAction
{
  Repository* p_Repos;
  
public:
	PushFunction(Repository* pRepos)
  {
    p_Repos = pRepos;
	
  }
  void doAction(ITokCollection*& pTc)
  {
    // pop anonymous scope
	  element e;
    e=p_Repos->scopeStack().pop();
	if (e.name != "anonymous")
	{
		p_Repos->scopeStack().push(e);
	}
    // push function scope
    std::string name = (*pTc)[pTc->find("(") - 1];
    element elem;
    elem.type = "function";
    elem.name = name;
    elem.lineCount = p_Repos->lineCount();
	//add new node to current parent and set new current parent node
	Node  *temp = new Node(elem);
	p_Repos->getCur()->addChild(temp);
	p_Repos->setCur(temp);
    p_Repos->scopeStack().push(elem);		


  }
};

///////////////////////////////////////////////////////////////
// action to send semi-expression that starts a function def
// to console

class PrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    std::cout << "\n\n  FuncDef Stmt: " << pTc->show().c_str();
  }
};

///////////////////////////////////////////////////////////////
// action to send signature of a function def to console

class PrettyPrintFunction : public IAction
{
public:
  void doAction(ITokCollection*& pTc)
  {
    pTc->remove("public");
    pTc->remove(":");
    pTc->trimFront();
    int len = pTc->find(")");
    std::cout << "\n\n  Pretty Stmt:    ";
    for(int i=0; i<len+1; ++i)
      std::cout << (*pTc)[i] << " ";
  }
};

///////////////////////////////////////////////////////////////
// rule to detect special key word definitions, as for, while, switch, if, catch and else
class SpecialWordDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
		if (tok == keys[i])
			return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		
		
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("(");
			size_t lelse = tc.find("else");
			if (len < tc.length() && isSpecialKeyWord(tc[len - 1]))
			{
				//std::cout << "SpecialWordDefinition: tc=" <<tc[len-1]<< std::endl;
				doActions(pTc);
				return true;
			}
			if (lelse <tc.length())
			{
				doActions(pTc);
				return true;
			}
		}
		
		
		
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push special key word name onto ScopeStack
class PushSpecialKeyWord : public IAction
{
	Repository* p_Repos;
	
public:
	bool isLoop(std::string s)
	{
		const static std::string keys[]
			= { "for", "while"};
		for (int i = 0; i<2; ++i)
		if (s == keys[i])
			return true;
		return false;
	}

	bool isCondition(std::string s)
	{
		const static std::string keys[]
			= { "switch", "if", "else" };
		for (int i = 0; i<3; ++i)
		if (s == keys[i])
			return true;
		return false;
	}

	PushSpecialKeyWord(Repository* pRepos)
	{
		p_Repos = pRepos;
		
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		//p_Repos->scopeStack().pop();
		element e;
		e = p_Repos->scopeStack().pop();
		if (e.name!="anonymous")
		{
			p_Repos->scopeStack().push(e);
		}
		// push scope
		std::string name = (*pTc)[pTc->find("(") - 1];
		if (name == "{")
			name = "else";
		element elem;
		if (isLoop(name))
			elem.type = "loop";
		else 
			if (isCondition(name))
				elem.type = "condition";
			else 
				elem.type = name;
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
		//add new node to current parent and set new current parent node
		Node  *temp = new Node(elem);
		p_Repos->getCur()->addChild(temp);
		p_Repos->setCur(temp);
		
	}
};

///////////////////////////////////////////////////////////////
// rule to detect class definitions

class ClassDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
		if (tok == keys[i])
			return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("class");
			if (len < tc.length() && !isSpecialKeyWord(tc[len + 1]))
			{
				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};



///////////////////////////////////////////////////////////////
// action to push class name onto ScopeStack

class PushClass : public IAction
{
	Repository* p_Repos;
	
public:
	PushClass(Repository* pRepos)
	{
		p_Repos = pRepos;
		
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element e;
		e = p_Repos->scopeStack().pop();
		if (e.name != "anonymous")
			p_Repos->scopeStack().push(e);

		std::string name = (*pTc)[pTc->find("class") + 1];
		element elem;
		elem.type = "class";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
		//add new node to current parent and set new current parent node
		Node  *temp = new Node(elem);
		p_Repos->getCur()->addChild(temp);
		p_Repos->setCur(temp);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect namespace definitions

class NameSpaceDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
		if (tok == keys[i])
			return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("namespace");
			if (len < tc.length() && !isSpecialKeyWord(tc[len + 1]))
			{

				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};



///////////////////////////////////////////////////////////////
// action to push namespace name onto ScopeStack

class PushNameSpace : public IAction
{
	Repository* p_Repos;
	
public:
	PushNameSpace(Repository* pRepos)
	{
		p_Repos = pRepos;
		
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element e;
		e = p_Repos->scopeStack().pop();
		if (e.name != "anonymous")
			p_Repos->scopeStack().push(e);
		// push function scope
		std::string name = (*pTc)[pTc->find("namespace") + 1];
		element elem;
		elem.type = "namespace";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
		
		//add new node to current parent and set new current parent node
		Node  *temp = new Node(elem);
		p_Repos->getCur()->addChild(temp);
		p_Repos->setCur(temp);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect struct definitions

class StructDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch" };
		for (int i = 0; i<5; ++i)
		if (tok == keys[i])
			return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("struct");
			if (len < tc.length() && !isSpecialKeyWord(tc[len + 1]))
			{

				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};



///////////////////////////////////////////////////////////////
// action to push struct name onto ScopeStack

class PushStruct : public IAction
{
	Repository* p_Repos;
	
public:
	PushStruct(Repository* pRepos)
	{
		p_Repos = pRepos;
		
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element e;
		e = p_Repos->scopeStack().pop();
		if (e.name != "anonymous")
			p_Repos->scopeStack().push(e);

		std::string name = (*pTc)[pTc->find("struct") + 1];
		element elem;
		elem.type = "struct";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
		//add new node to current parent and set new current parent node
		Node  *temp = new Node(elem);
		p_Repos->getCur()->addChild(temp);
		p_Repos->setCur(temp);
	}
};

///////////////////////////////////////////////////////////////
// rule to detect struct definitions

class EnumDefinition : public IRule
{
public:
	bool isSpecialKeyWord(const std::string& tok)
	{
		const static std::string keys[]
			= { "for", "while", "switch", "if", "catch","try" };
		for (int i = 0; i<5; ++i)
		if (tok == keys[i])
			return true;
		return false;
	}
	bool doTest(ITokCollection*& pTc)
	{
		ITokCollection& tc = *pTc;
		if (tc[tc.length() - 1] == "{")
		{
			size_t len = tc.find("enum");
			if (len < tc.length() && !isSpecialKeyWord(tc[len + 1]))
			{

				doActions(pTc);
				return true;
			}
		}
		return false;
	}
};

///////////////////////////////////////////////////////////////
// action to push struct name onto ScopeStack

class PushEnum : public IAction
{
	Repository* p_Repos;
	
public:
	PushEnum(Repository* pRepos)
	{
		p_Repos = pRepos;
		
	}
	void doAction(ITokCollection*& pTc)
	{
		// pop anonymous scope
		element e;
		e = p_Repos->scopeStack().pop();
		if (e.name != "anonymous")
			p_Repos->scopeStack().push(e);

		std::string name = (*pTc)[pTc->find("enum") + 1];
		element elem;
		elem.type = "enum";
		elem.name = name;
		elem.lineCount = p_Repos->lineCount();
		p_Repos->scopeStack().push(elem);
		
		//add new node to current parent and set new current parent node
		Node  *temp = new Node(elem);
		p_Repos->getCur()->addChild(temp);
		p_Repos->setCur(temp);
	}
};

#endif
