#ifndef KNNCLASSIFIER
#define KNNCLASSIFIER

#include "stdafx.h"

#include <opencv2\opencv.hpp>

class KNNClassifier
{
private:
	cv::Mat *m_trainingData;
	cv::Mat *m_trainingClassification;

public:

	KNNClassifier();
	KNNClassifier(cv::Mat &trainingData, cv::Mat &trainingClassification, int K);

	bool train(cv::Mat &trainingData, cv::Mat &trainingClassification, int K );

	float find_nearest(cv::Mat &testData, int K );

	bool save(std::string classifierFileName);
	bool load(std::string classifierFileName);

};

#endif