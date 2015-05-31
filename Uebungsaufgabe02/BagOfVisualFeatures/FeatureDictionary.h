#ifndef FEATUREDICTIONARY
#define FEATUREDICTIONARY

#include "stdafx.h"
#include <string.h>
#include <vector>
#include <opencv2\opencv.hpp> 

class FeatureDictionary
{
private:
	cv::Mat *m_vocabulary;
	cv::flann::Index *m_kdTree;
public:
	FeatureDictionary(cv::Mat *vocabulary);

	cv::Mat *getVocabulary();
	cv::flann::Index *getKDTree();

};

#endif