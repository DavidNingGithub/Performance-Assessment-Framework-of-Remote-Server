/////////////////////////////////////////////////////////////////////
//  FileMgr.cpp - get files from given path and patterns           //
//  ver 1.0                                                        //
//                                                                 //
//  Lanaguage:     Visual C++ 2013                                 //
//  Application:   CSE687 Pr2					                   //
//  Author:        Tao Ning								           //
/////////////////////////////////////////////////////////////////////
#include "FileMgr.h"
#include <iostream>
#include "FileSystem.h"

using namespace std;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//search function is used to return the vector storing full name of each file in the specific directory

FileMgr::files& FileMgr::search(const std::string& path, FileMgr::patterns patts)
{
	for (auto patt : patts)
	{
		files temp = FileSystem::Directory::getFiles(path, patt);
		for (auto file : temp)
		{
			file = path + "\\"+file;
          _files.push_back(file);
		}
			
	}
	// recursive search is an exercise left to students
	return _files;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//findfile function can process depth search.
FileMgr::files FileMgr::findfile(FileMgr::files& foundFiles, string& path, vector<string> patts, int rtest)
{
	FileMgr fm;
	if (rtest == 0)
	{
		foundFiles = fm.search(path, patts);
	}
	else//command line have a /r option, we should do depth search
	{
		FileMgr::files files;
		vector<string> paths;
		string fullpath = FileSystem::Path::getFullFileSpec(path);
		paths = FileSystem::Directory::getDirectories(fullpath);
		files = fm.search(fullpath, patts);
		foundFiles.insert(foundFiles.end(), files.begin(), files.end());
		for (size_t i = 2; i<paths.size(); i++)
		{
			string fpath = FileSystem::Path::getFullFileSpec(paths.at(i));
			findfile(foundFiles, fpath, patts, rtest);
		}
	}
	return foundFiles;
}
#ifdef TEST_FILEMGR

int main(int argc, char* argv[])
{
	int test = 0;
	string path;
	vector<string> paths;
	FileMgr::files AllFiles;
	FileMgr::patterns patts;
	for (int i = 0; i < argc; ++i)
	{
		string compare = argv[i];
		int no = compare.find("*");
		if (strcmp("/s", argv[i]) == 0)
			test = 1;
		else if (no >= 0)
			patts.push_back(argv[i]);
		else
			path = argv[i];
	}
	FileMgr::files foundFiles;
	FileMgr FindFile;
	string mainpath;
	mainpath = FileSystem::Path::getFullFileSpec(path);
	AllFiles = FindFile.findfile(foundFiles, mainpath, patts, test);
	for (auto file : AllFiles)
		std::cout << "\n  " << file;
	std::cout << "\n\n";
}
#endif
