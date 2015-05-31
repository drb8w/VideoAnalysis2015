#include "stdafx.h"

#include "VideoMetaData.h"
#include "StringHelpers.h"

VideoMetaData::VideoMetaData(cv::Mat *histogram, std::string classification)
{
	this->m_histogram = histogram;
	this->m_classification = classification;
}

cv::Mat *VideoMetaData::getHistogram()
{
	return this->m_histogram;
}

std::string VideoMetaData::getClassification()
{
	return this->m_classification;
}

//int VideoMetaData::ClassHash()
//{
//	return str_hash(this->m_classification);
//}
	
