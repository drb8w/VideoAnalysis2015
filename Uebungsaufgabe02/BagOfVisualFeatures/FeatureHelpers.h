#ifndef FEATUREHELPERS
#define FEATUREHELPERS

#define NFEATURES 100
//#define NFEATURES 75
//#define NFEATURES 50

#define NUMCLUSTERS 480 // 50*50 from CompVis or 480 from paper
//#define NUMCLUSTERS 360
//#define NUMCLUSTERS 240

#include "stdafx.h"
#include <vector>
#include <opencv2\opencv.hpp>


std::vector<cv::Mat> *ExtractFeatures(std::vector<cv::Mat> &frames, int nfeatures = NFEATURES);
std::vector<cv::Mat *> *ExtractFeaturePtrs(std::vector<cv::Mat *> &frames, int nfeatures = NFEATURES);
cv::Mat *AppendFeatures(std::vector<cv::Mat> &features);
cv::Mat *AppendFeaturePtrs(std::vector<cv::Mat *> &features);
cv::Mat *ClusterFeatures(std::vector<cv::Mat> &features, int numClusters = NUMCLUSTERS); // 50*50 from CompVis or 480 from paper
cv::Mat *ClusterFeaturePtrs(std::vector<cv::Mat *> &features, int numClusters = NUMCLUSTERS); // 50*50 from CompVis or 480 from paper

#endif