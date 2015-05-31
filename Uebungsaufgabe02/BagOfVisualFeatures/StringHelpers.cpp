#include "stdafx.h"
#include "StringHelpers.h"

//#include "stdafx.h"

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

std::string ArgumentPath(int argc, char *argv[], int argNo, std::string &path)
{
	if (argc >argNo)
		path = argv[argNo];

	for (int i=0; i<100; i++)
		path = replace((char *)(path.c_str()), "\\", "/");

	if (strcmp((const char *)&(path[path.size()-1]),"/"))
		path = path + "/";

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