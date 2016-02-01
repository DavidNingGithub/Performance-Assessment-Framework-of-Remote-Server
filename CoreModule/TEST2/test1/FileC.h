#ifndef FILEMGR_H
#define FILEMGR_H


#include <vector>
#include <string>
#include <iostream>
#include "FileSystem.h"

class FileMgr
{
public:
	typedef std::vector<std::string> files;
	typedef std::vector<std::string> patterns;
	FileMgr(){ std::cout << "\n constructing FileMgr"; }
	~FileMgr(){ std::cout << "o\n destroying FileMgr"; }
	files& search(const std::string & path, patterns patts);
	files& searchAllDirs(const std::string& path, patterns pattern);
	void showFiles();
private:
	std::string path;
	files _files;
	patterns _patterns;
};

FileMgr::files& FileMgr::search(const std::string& path, FileMgr::patterns patts)
{
	for (auto patt : patts)
	{
		std::string s;
		FileMgr::files temp = FileSystem::Directory::getFiles(path, patt);
		for (auto file : temp)
		{
			_files.push_back(file);
		}
	}
	return _files;
}
//search all files
FileMgr::files& FileMgr::searchAllDirs(const std::string& path, FileMgr::patterns patts)
{
	std::vector<std::string> dirs;
	std::string nextpath, dir;
	for (auto patt : patts)
	{
		FileMgr::files temp = FileSystem::Directory::getFiles(path, patt);
		for (auto file : temp)
		{
			file = path + "/" + file;
			_files.push_back(file);
		}
	}
	for (auto patt : patts)
		dirs = FileSystem::Directory::getDirectories(path, (std::string) patt);
	
	//stop when there are only "." and ".." stored in dirs
	if (dirs.size() == 2)
		{ return _files;}
	else
	{
		int a,b,c;
	}
	while (dirs.size() > 2)
	{
		dir = dirs.back();
		nextpath = path + "/" + dir;
		dirs.pop_back();
		searchAllDirs(nextpath, patts);
	}


	return _files;
}

void FileMgr::showFiles()
{
	for (auto file : _files)
		std::cout << "\n" << file;
	std::cout << "\n\n";
}
#endif