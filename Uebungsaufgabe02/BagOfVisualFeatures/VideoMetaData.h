#ifndef VIDEODESCRIPTOR
#define VIDEODESCRIPTOR

#include "stdafx.h"
#include <string.h>
#include <opencv2\opencv.hpp>


class VideoMetaData
{
private:
	cv::Mat *m_histogram;
	std::string m_classification;

public:
	VideoMetaData(cv::Mat *histogram, std::string classification);

	cv::Mat *getHistogram();
	std::string getClassification();
	//int ClassHash();	
};

#endif