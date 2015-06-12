#ifndef CLASSIFICATION
#define CLASSIFICATION

#include "stdafx.h"
#include <string>

class Classification
{
private:
	std::string m_name;
	int m_hash;
public:
	Classification(std::string name);

	std::string getName();
	int getHash();

};

#endif