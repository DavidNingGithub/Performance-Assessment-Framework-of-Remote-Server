/////////////////////////////////////////////////////////////////
// TextSearch.cpp - Demonstrate how to search for text in file //
//                                                             //
// Jim Fawcett, CSE687 - Object Oriented Design, Spring 2014   //
/////////////////////////////////////////////////////////////////

#include "TextSearch.h"
#include <sstream>
#include <fstream>
#include "FileSystem.h"

TextSearch::Files TextSearch::search(Files& files, const SearchString& sstr)
{
  Files results;
  for (auto file : files)
  {
    std::ostringstream stext;
    std::ofstream fs;
    fs.open(file, std::fstream::in);
    if (fs.good())
    {
      stext << fs.rdbuf();
      std::string text = stext.str();
      size_t pos = text.find(sstr);
      if (pos < text.size())
        results.push_back(file);
    }
  }
  return results;
}

#include <iostream>

void show(const std::string& msg, const std::string& text)
{
  std::cout << "\n  " << msg;
  std::cout << "\n " << std::string(msg.size() + 2, '-');
  std::cout << "\n  " << text << "\n";
}

void show(const std::string& msg, TextSearch::Files& files)
{
  std::cout << "\n  " << msg;
  std::cout << "\n " << std::string(msg.size() + 2, '-');
  for (auto file : files)
    std::cout << "\n  " << file;
  std::cout << "\n";
}

int main()
{
  std::cout << "\n  Test TextSearch";
  std::cout << "\n =================";

  std::string path = "..\\TextSearch\\";
  std::string sstr = "main";
  TextSearch::Files files;
  files = FileSystem::Directory::getFiles(path, "*.*");
  show("file set", files);
  show("search string", sstr);
  TextSearch ts;
  TextSearch::Files results = ts.search(files, sstr);
  show("search results", results);
  std::cout << "\n\n";
}