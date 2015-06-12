#include "stdafx.h"
#include "Classification.h"

#include "StringHelpers.h"

Classification::Classification(std::string name)
{
	this->m_name = name;
	this->m_hash = str_hash(name);
}

std::string Classification::getName()
{
	return this->m_name;
}

int Classification::getHash()
{
	return this->m_hash;
}