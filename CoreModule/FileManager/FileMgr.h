/////////////////////////////////////////////////////////////////////////////
// // fileMgr.h                                                            //
// ver 2.2                                                                 //
// ----------------------------------------------------------------------- //
// copyright ?Jim Fawcett, 2012                                            //
// All rights granted provided that this notice is retained                //
// ----------------------------------------------------------------------- //
// Language:    Visual C++, Visual Studio 2010                             //
// Platform:    Dell XPS 8300, Core i7, Windows 7 SP1                      //
// Application: Summer Projects, 2012                                      //
// Author:      Tao Ning   , CST 4-187, Syracuse University                //
//              (315) 443-3948, tning.rr.com                               //
/////////////////////////////////////////////////////////////////////////////
/*
* Module Operations:
* ==================
* This module provides classes, File, FileInfo, Path, Directory, and
* FileSystemSearch.
*
* The File class supports opening text and binary files for either input
* or output.  File objects have names, get and put lines of text, get and
* put blocks of bytes if binary, can be tested for operational state,
* cleared of errors, and output File objects can be flushed to their streams.
*
* FileInfo class accepts a fully qualified filespec and supports queries
* concerning name, time and date, size, and attributes.  You can compare
* FileInfo objects by name, date, and size.
*
* Path class provides static methods to turn a relative filespec into an
* absolute filespec, return the path, name, or extension of the filespec,
* and build filespecs from path and name.  Path also provides methods to
* convert filespecs to uppercase and to lowercase.
*
* The Directory class supports getting filenames or directories from a
* fully qualified filespec, e.g., path + filename pattern using static
* methods.  It also provides non-static methods to get and set the current
* directory.
*
* Public Interface:
* =================
* File f(filespec,File::in,File::binary);
* File g(filespec,File::out,File::binary);
* while(f.isGood()) {
*   block b = f.getBlock();
*   g.putBlock(b);
* }
* FileInfo fi("..\foobar.txt");
* if(fi.good())
*   ...
* std::string filespec = "..\temp.txt";
* std::string fullyqualified = Path::getFullFileSpec(filespec);
* std::string path = Path::getPath(fullyqualified);
* std::string name = Path::getName(fullyqualified);
* std::string extn = Path::getExt(fullyqualified);
*
* Directory d;
* std::string dir = d.getCurrentDirectory();
* d.setCurrentDirectory(dir);
* std::vector<std::string> files = Directory::getFiles(path, pattern);
* std::vector<std::string> dirs = Directory::getDirectories(path);
*
* Required Files:
* ===============
* FileMgr.h, FileMgr.cpp
*
* Build Command:
* ==============
* cl /EHa /DTEST_FILESYSTEM FileSystem.cpp
*
* Maintenance History:
* ====================
* ver 2.2 : 23 Feb 13
* - fixed bug in Path::getExt(...) discovered by Yang Zhou and Kevin Kong
* ver 2.1 : 07 Jun 12
* - moved FileSystemSearch to cpp file to make private
* - added bool withExt parameter to Path::getName
* - made Directory's getCurrentDirectory and setCurrentDirectory static
* - added exists, create, and remove to Directory class
* ver 2.0 : 04 Jun 12
* - made a few small changes to provide same interface as
*   Linux version.
* ver 1.0 : 31 Jan 09
* - first release
*/
#ifndef FILEMGR_H
#define FILEMGR_H
// FileMgr.h - help demo

#include <vector>
#include <string>

using namespace std;
/////////////////////////////////////////////////////////
//class FileMgr
class FileMgr
{
public:
	typedef std::vector<std::string> files;
	typedef std::vector<std::string> patterns;
	files& search(const std::string& path, patterns patts);
	files findfile(FileMgr::files&, string&, vector<string>, int);
private:
	std::string path;
	files _files;
	patterns _patterns;
};
#endif
