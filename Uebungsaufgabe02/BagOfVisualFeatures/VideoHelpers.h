#ifndef VIDEOHELPERS
#define VIDEOHELPERS

#include "stdafx.h"
#include <vector>
#include <opencv2\opencv.hpp>

#define FRAMEMODULO 4

std::vector<cv::Mat> *GetFramesCPP(std::string videoFileName, std::vector<cv::Mat> *videoFrames, int frameModulo = FRAMEMODULO);
std::vector<cv::Mat> *GetFrames(std::string videoFileName, std::vector<cv::Mat> *videoFrames, int frameModulo = FRAMEMODULO);
std::vector<cv::Mat *> *GetFramePtrs(std::string videoFileName, std::vector<cv::Mat *> *videoFramePtrs, int frameModulo = FRAMEMODULO);

std::vector<cv::Mat> *GetFramesCPP(std::string videoFileName, int frameModulo = FRAMEMODULO);
std::vector<cv::Mat> *GetFrames(std::string videoFileName, int frameModulo = FRAMEMODULO);
std::vector<cv::Mat *> *GetFramePtrs(std::string videoFileName, int frameModulo = FRAMEMODULO);

std::vector<cv::Mat> *GetSpatialTemporalFramesXt(std::vector<cv::Mat> &frames, std::vector<cv::Mat> *framesXt, int frameModulo = FRAMEMODULO);
std::vector<cv::Mat *> *GetSpatialTemporalFramePtrsXt(std::vector<cv::Mat *> &frames, std::vector<cv::Mat *> *framesXt, int frameModulo = FRAMEMODULO);

std::vector<cv::Mat> *GetSpatialTemporalFramesXt(std::vector<cv::Mat> &frames, int frameModulo = FRAMEMODULO);
std::vector<cv::Mat *> *GetSpatialTemporalFramePtrsXt(std::vector<cv::Mat *> &frames, int frameModulo = FRAMEMODULO);

std::vector<cv::Mat> *GetSpatialTemporalFrames(std::vector<cv::Mat> &videoFrames, std::vector<cv::Mat> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true, int frameModulo = FRAMEMODULO);
std::vector<cv::Mat *> *GetSpatialTemporalFramePtrs(std::vector<cv::Mat *> &videoFrames, std::vector<cv::Mat *> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true, int frameModulo = FRAMEMODULO);

std::vector<cv::Mat> *GetSpatialTemporalFrames(std::vector<cv::Mat> &videoFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true, int frameModulo = FRAMEMODULO);
std::vector<cv::Mat *> *GetSpatialTemporalFramePtrs(std::vector<cv::Mat *> &videoFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true, int frameModulo = FRAMEMODULO);

std::vector<cv::Mat> *GetSpatialTemporalFramesSet(std::vector<std::vector<cv::Mat>> &videoFramesSet, std::vector<cv::Mat> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true);
std::vector<cv::Mat *> *GetSpatialTemporalFramePtrsSet(std::vector<std::vector<cv::Mat *>> &videoFramesSet, std::vector<cv::Mat &> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true);


#endif