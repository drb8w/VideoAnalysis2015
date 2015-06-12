#ifndef VIDEODESCRIPTOR
#define VIDEODESCRIPTOR

#include "stdafx.h"
#include <string.h>
#include <opencv2\opencv.hpp>
#include "Classification.h"

class VideoMetaData
{
private:
	cv::Mat *m_histogram;
	//std::string m_classification;
	Classification *m_classification;

public:
	VideoMetaData(cv::Mat *histogram, std::string classificationName);

	cv::Mat *getHistogram();
	//std::string getClassification();
	Classification *getClassification();
	
	//int ClassHash();	
};

std::vector<Classification *> *GetUniqueVideoMetaDataClassifications(std::vector<VideoMetaData *> &videoMetaDataSet);

#endif