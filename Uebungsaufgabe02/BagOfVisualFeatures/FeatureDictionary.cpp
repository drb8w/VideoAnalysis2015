#include "stdafx.h"

#include "FeatureDictionary.h"

using namespace cv;

cv::flann::Index *BuildDictionary(Mat *vocabulary)
{
	// make KD Tree outward as dictionary for vocabulary
	// KdTree with 5 random trees
	cv::flann::KDTreeIndexParams indexParams(5);
	// Create the Index
	cv::flann::Index *kdtree = new cv::flann::Index(*vocabulary, indexParams);
	return kdtree;
}

FeatureDictionary::FeatureDictionary(Mat *vocabulary)
{
	this->m_vocabulary = vocabulary;
	this->m_kdTree = BuildDictionary(vocabulary);
}

cv::Mat *FeatureDictionary::getVocabulary()
{
	return this->m_vocabulary;
}

cv::flann::Index *FeatureDictionary::getKDTree()
{
	return this->m_kdTree;
}
