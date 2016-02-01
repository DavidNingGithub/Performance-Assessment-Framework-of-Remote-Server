///////////////////////////////////////////////////////////////
// TextSearch - Demonstrate simple text search in file       //
//                                                           //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014 //
///////////////////////////////////////////////////////////////
/*
*  What you need to add to be compliant:
*  - ability to execute multiple concurrent searches
*  - handle case where string wraps, so it may contain
*    unexpected new lines and other consecutive whitespace.
*/

#include <vector>
#include <string>

class TextSearch
{
public:
  using Files = std::vector<std::string>;
  using SearchString = std::string;

  Files search(Files& files, const SearchString& sstr);
};