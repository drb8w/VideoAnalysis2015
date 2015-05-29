#ifndef FEATUREHELPERS
#define FEATUREHELPERS

#include "stdafx.h"
#include <vector>
#include <opencv2\opencv.hpp> 

std::vector<cv::Mat> *ExtractFeatures(std::vector<cv::Mat> *frames, int nfeatures = 100);
cv::Mat *ClusterFeatures(std::vector<cv::Mat> *features, int numClusters = 500); // 50*50 from CompVis or 480 from paper

#endif