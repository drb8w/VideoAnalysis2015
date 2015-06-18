#ifndef VIDEODESCRIPTOR
#define VIDEODESCRIPTOR

#include "stdafx.h"
#include <string.h>
#include <opencv2\opencv.hpp>
#include "Classification.h"

typedef std::map<int, std::string>::iterator it_type;

class VideoMetaData
{
private:
	cv::Mat *m_histogram;
	//std::string m_classification;
	Classification *m_classification;

	std::string m_videoFileName;

public:
	VideoMetaData(cv::Mat *histogram, std::string classificationName, std::string videoFileName);

	cv::Mat *getHistogram();
	//std::string getClassification();
	Classification *getClassification();

	std::string getVideoFileName();
	
	//int ClassHash();	
};

std::vector<Classification *> *GetUniqueVideoMetaDataClassifications(std::vector<VideoMetaData *> &videoMetaDataSet);

std::vector<Classification *> *GetVideoMetaDataClassifications(std::map<int, std::string> &hashClassificationMap);

#endif