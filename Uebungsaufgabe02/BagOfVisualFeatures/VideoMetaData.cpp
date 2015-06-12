#include "stdafx.h"

#include "VideoMetaData.h"
#include "StringHelpers.h"

VideoMetaData::VideoMetaData(cv::Mat *histogram, std::string classificationName)
{
	this->m_histogram = histogram;
	//this->m_classification = classification;
	this->m_classification = new Classification(classificationName);
}

cv::Mat *VideoMetaData::getHistogram()
{
	return this->m_histogram;
}

Classification *VideoMetaData::getClassification()
{
	return this->m_classification;
}

//std::string VideoMetaData::getClassification()
//{
//	return this->m_classification;
//}

//int VideoMetaData::ClassHash()
//{
//	return str_hash(this->m_classification);
//}
	

bool uniqueName(Classification *data1, Classification *data2)
{
	if (data1->getHash() == data2->getHash())
		return true;

	return false;
}

std::vector<Classification *> *GetUniqueVideoMetaDataClassifications(std::vector<VideoMetaData *> &videoMetaDataSet)
{
	// TODO: moveTo ConfusionMatrix
	std::list<Classification *> *classes = new std::list<Classification *>();

	for(int i=0; i<videoMetaDataSet.size(); i++)
	{
		VideoMetaData *videoMetaData = videoMetaDataSet[i];
		classes->push_back(videoMetaData->getClassification());
	}	
	classes->unique(uniqueName);
		
	return new std::vector<Classification *>(classes->begin(), classes->end());
}
