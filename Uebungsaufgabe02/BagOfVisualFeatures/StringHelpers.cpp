#include "stdafx.h"
#include "StringHelpers.h"

//#include "stdafx.h"

#include <iostream>
#include <Windows.h>

#include <locale>
#include <string>
#include <sstream>

using namespace std;

vector<string> splitString(string s, string delimiter)
{
	vector<string> tokens;
	size_t pos = 0;
	std::string token;
	while ((pos = s.find(delimiter)) != std::string::npos) {
		token = s.substr(0, pos);
		tokens.push_back(token);
		s.erase(0, pos + delimiter.length());
	}
	return tokens;
}

char *replace(char *st, char *orig, char *repl) 
{
  static char buffer[4096];
  char *ch;
  if (!(ch = strstr(st, orig)))
   return st;
  strncpy(buffer, st, ch-st);  
  buffer[ch-st] = 0;
  sprintf(buffer+(ch-st), "%s%s", repl, ch+strlen(orig));
  return buffer;
}

std::string ArgumentString(int argc, char *argv[], int argNo, std::string &str)
{
	if (argc >argNo)
		str = argv[argNo];

	return str;
}


wstring widen( const string& str )
{
    wostringstream wstm ;
    const ctype<wchar_t>& ctfacet = 
                        use_facet< ctype<wchar_t> >( wstm.getloc() ) ;
    for( size_t i=0 ; i<str.size() ; ++i ) 
              wstm << ctfacet.widen( str[i] ) ;
    return wstm.str() ;
}
string narrow( const wstring& str )
{
    ostringstream stm ;
    const ctype<char>& ctfacet = 
                         use_facet< ctype<char> >( stm.getloc() ) ;
    for( size_t i=0 ; i<str.size() ; ++i ) 
                  stm << ctfacet.narrow( str[i], 0 ) ;
    return stm.str() ;
}


std::string ExecutionPath()
{
	wchar_t buffer[MAX_PATH]; 
	GetModuleFileName(NULL, buffer, MAX_PATH);

	// convert to string
	string execPath = narrow(buffer);

	for (int i=0; i<100; i++)
		execPath = replace((char *)(execPath.c_str()), "\\", "/");

	// find last slash
	std::size_t found = execPath.find_last_of("/");
	execPath = execPath.substr(0,found+1);

	return execPath;
}

std::string ArgumentPath(int argc, char *argv[], int argNo, std::string &path)
{
	if (argc >argNo)
		path = argv[argNo];

	for (int i=0; i<100; i++)
		path = replace((char *)(path.c_str()), "\\", "/");

	if (strcmp((const char *)&(path[path.size()-1]),"/"))
		path = path + "/";

	if (path[0] =='.')
	{
		// convert to string
		string execPath = ExecutionPath();		
		// concat right parts
		path = execPath + path.substr(2,path.size()-2);
	}

	return path;
}

int str_hash( const string &key, int tableSize) {
   int hashVal = 0;

   for(int i = 0; i<key.length();  i++)
     hashVal = 37*hashVal+key[i];

   hashVal %= tableSize;

   if(hashVal<0)
     hashVal += tableSize;

   return hashVal;
 }