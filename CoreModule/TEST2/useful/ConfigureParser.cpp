//////////////////////////////////////////////////////////////////////////
//  ConfigureParser.h - builds and configures parsers				    //
//  ver 3.0																//
//  Language:           C++11				                            //
//  Platform:           Lenovo Y410p, Windows 8.1				        //
//  Application:        Code analyzer for CSE687 Pr1, Sp2014			//
//  Author:		        Shishi Fu									    //
//  Source Author:      Jim Fawcett, CST 4-187, Syracuse University     //
//                      (315) 443-3948, jfawcett@twcny.rr.com           //
//////////////////////////////////////////////////////////////////////////
//#include "stdafx.h"
#include "Parser.h"
#include "SemiExpression.h"
#include "Tokenizer.h"
#include "ActionsAndRules.h"
#include "ConfigureParser.h"

//----< destructor releases all parts >------------------------------

ConfigParseToConsole::~ConfigParseToConsole()
{
  // when Builder goes out of scope, everything must be deallocated

  delete pHandlePush;
  delete pBeginningOfScope;
  delete pHandlePop;
  delete pEndOfScope;
  delete pPushFunction;
  delete pFunctionDefinition;
  delete pFR;
  delete pRepo;
  delete pParser;
  delete pSemi;
  delete pToker;
  //my code
  delete pSpecialKeyWordDefinition;
  delete pPushSpecialKeyWord;
  delete pSpecialTypeDefinition;
  delete pPushSpecialType;

}
//----< attach toker to a file stream or stringstream >------------

bool ConfigParseToConsole::Attach(const std::string& name, bool isFile)
{
  if(pToker == 0)
    return false;
  return pToker->attach(name, isFile);
}
//----< Here's where alll the parts get assembled >----------------

Parser* ConfigParseToConsole::Build()
{
  try
  {
	 
    // add Parser's main parts

    pToker = new Toker;
    pToker->returnComments();
    pSemi = new SemiExp(pToker);
    pParser = new Parser(pSemi);
    pRepo = new Repository(pToker);

	

    // add code folding rules

    pFR = new codeFoldingRules;
    pParser->addFoldingRules(pFR);

    // configure to manage scope
    // these must come first - they return true on match
    // so rule checking continues

    pBeginningOfScope = new BeginningOfScope();
    pHandlePush = new HandlePush(pRepo);
    pBeginningOfScope->addAction(pHandlePush);
    pParser->addRule(pBeginningOfScope);
    pEndOfScope = new EndOfScope();
    pHandlePop = new HandlePop(pRepo);
    pEndOfScope->addAction(pHandlePop);
    pParser->addRule(pEndOfScope);

	
	//configure special key words
	
	// configure to detect and act on function definitions
	// these will stop further rule checking by returning false

	pFunctionDefinition = new FunctionDefinition;
	pPushFunction = new PushFunction(pRepo);  // no action
	pFunctionDefinition->addAction(pPushFunction);
	pParser->addRule(pFunctionDefinition);
	
	//configure special scopes
	pSpecialTypeDefinition = new SpecialTypeDefinition;
	pPushSpecialType = new PushSpecialType(pRepo);
	pSpecialTypeDefinition->addAction(pPushSpecialType);
	pParser->addRule(pSpecialTypeDefinition);

	//configure special key words
	pSpecialKeyWordDefinition = new SpecialWordDefinition;
	pPushSpecialKeyWord = new PushSpecialKeyWord(pRepo);  // no action
	pSpecialKeyWordDefinition->addAction(pPushSpecialKeyWord);
	pParser->addRule(pSpecialKeyWordDefinition);



	
	
    return pParser;
  }
  catch(std::exception& ex)
  {
    std::cout << "\n\n  " << ex.what() << "\n\n";
    return 0;
  }
}

Tree* ConfigParseToConsole::getMTRoot()
{
	return pRepo->getTree();
}

#ifdef TEST_CONFIGUREPARSER

#include <queue>
#include <string>

int main(int argc, char* argv[])
{
  std::cout << "\n  Testing ConfigureParser module\n "
            << std::string(32,'=') << std::endl;

  // collecting tokens from files, named on the command line

  if(argc < 2)
  {
    std::cout 
      << "\n  please enter name of file to process on command line\n\n";
    return 1;
  }

  for(int i=1; i<argc; ++i)
  {
    std::cout << "\n  Processing file " << argv[i];
    std::cout << "\n  " << std::string(16 + strlen(argv[i]),'-');

    ConfigParseToConsole configure;
    Parser* pParser = configure.Build();
    try
    {
      if(pParser)
      {
        if(!configure.Attach(argv[i]))
        {
          std::cout << "\n  could not open file " << argv[i] << std::endl;
          continue;
        }
      }
      else
      {
        std::cout << "\n\n  Parser not built\n\n";
        return 1;
      }
      // now that parser is built, use it

      while(pParser->next())
        pParser->parse();
      std::cout << "\n\n";
    }
    catch(std::exception& ex)
    {
      std::cout << "\n\n    " << ex.what() << "\n\n";
    }
    std::cout << "\n\n";
  }
}

#endif
