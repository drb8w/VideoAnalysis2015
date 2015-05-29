#ifndef VIDEOHELPERS
#define VIDEOHELPERS

#include "stdafx.h"
#include <vector>
#include <opencv2\opencv.hpp> 

std::vector<cv::Mat> *GetFrames(std::string videoFileName, std::vector<cv::Mat> *videoFrames);

std::vector<cv::Mat> *GetFrames(std::string videoFileName);

std::vector<cv::Mat *> *GetFramePtrs(std::string videoFileName, std::vector<cv::Mat *> *videoFramePtrs);

std::vector<cv::Mat *> *GetFramePtrs(std::string videoFileName);

std::vector<cv::Mat> *GetSpatialTemporalFramesXt(std::vector<cv::Mat> *frames, std::vector<cv::Mat> *framesXt);

std::vector<cv::Mat> *GetSpatialTemporalFramesXt(std::vector<cv::Mat> *frames);

std::vector<cv::Mat> * GetSpatialTemporalFrames(std::vector<cv::Mat> *videoFrames, std::vector<cv::Mat> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true);

std::vector<cv::Mat> * GetSpatialTemporalFrames(std::vector<cv::Mat> *videoFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true);

std::vector<cv::Mat> * GetSpatialTemporalFramesSet(std::vector<std::vector<cv::Mat>> *videoFramesSet, std::vector<cv::Mat> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true);


#endif