#ifndef FILEHELPERS
#define FILEHELPERS

#include <Windows.h>
#include <vector>
#include <string>

std::vector<std::string> get_all_files_names_within_folder(std::string folder);

std::vector<std::string> getFilesPathWithinFolder(std::string folder, bool recursive = true);

#endif