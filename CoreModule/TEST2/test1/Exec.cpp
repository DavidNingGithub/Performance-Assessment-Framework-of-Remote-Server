#include "FileMgr.h"
#include "FileSystem.h"
#include "Parser.h"
#include "ConfigureParser.h"
class Test
{
public:
	int a;
private:
	int b;
};

bool Parser::parse()
{
  bool succeeded = false;
  for(size_t i=0; i<rules.size(); ++i)
  {
    if(rules[i]->doTest(pTokColl))
      succeeded = true;
  }
  return succeeded;
}
void readFile(FileMgr &fm,FileMgr::files &files)
{
	std::cout << std::endl;
	std::cout << "FileManager Test:";
	FileMgr::patterns patts;
	FileMgr::files foundFiles;
	std::string path, pattern, select;
	int i = 0;
	std::cout << " please input path:" << std::endl;
	std::cin >> path;
	std::cout << " please input patterns:" << std::endl;
	while (1)
	{

		std::cin >> pattern;
		if (pattern == "0") break;
		patts.push_back(pattern);

	}
	if (patts.size() == 0)
	{
		std::cout << "No pattern input" << std::endl;
		return;
	}
	std::cout << " If you want to search all files, please input /s. Otherwise, please input 0" << std::endl;
	std::cin >> select;
	if (select == "/s")
		foundFiles = fm.searchAllDirs(path, patts);
	else
		foundFiles = fm.search(path, patts);

	files = foundFiles;
}

void analyze(std::string pFileName)
{
	std::cout << "\n  Testing Parser class\n "
		<< std::string(22, '=') << std::endl;

	// collecting tokens from files, named on the command line

	if (pFileName.length() == 0)
	{
		std::cout
			<< "\n  Wrong file input\n\n";
		return;
	}


	std::cout << "\n  Processing file " << pFileName;
	std::cout << "\n  " << std::string(16, '-');

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
		std::cout << "\n\n";
	}
	catch (std::exception& ex)
	{
		std::cout << "\n\n    " << ex.what() << "\n\n";
	}
	std::cout << "\n\n";
}

int main(int argc, char* argv[])
{
	FileMgr fm;
	FileMgr::files files;
	readFile(fm,files);
	fm.showFiles();
	for (auto file : files)
		analyze(file);
	return 0;
}