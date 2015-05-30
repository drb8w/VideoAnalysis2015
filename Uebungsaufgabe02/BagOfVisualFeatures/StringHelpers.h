#ifndef STRINGHELPERS
#define STRINGHELBERS

#include "stdafx.h"
#include <vector>
#include <string>

std::vector<std::string> splitString(std::string s, std::string delimiter);

char *replace(char *st, char *orig, char *repl);

std::string ArgumentPath(int argc, char *argv[], int argNo, std::string &path);

#endif