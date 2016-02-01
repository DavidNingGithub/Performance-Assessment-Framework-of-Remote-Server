
#include "Compare.h"
#ifdef TEST_COMPARE
int main(int argc, char* argv[])
{
	Compare *comp = new Compare();
	if (argc < 3)
	{
		std::cout << "No input commands\n";
		return 0;
	}
	comp->CompareFiles(argv[1],argv[2]);
	comp->DisplaySimilarCodes();
	std::cout << "\n \t End of Processing files ";
	std::cout << "\n  " << std::string(60, '-') << std::endl;


	return 0;
}

#endif