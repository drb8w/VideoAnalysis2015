#ifndef FRAMEHELPERS
#define FRAMEHELPERS

#include "stdafx.h"
#include <vector>
#include <opencv2\opencv.hpp>


void convertToGrayscale(const cv::Mat &img, cv::Mat &imgGray);

std::vector<cv::Mat> *convertToGrayscales(std::vector<cv::Mat> &rgbFrames, std::vector<cv::Mat> *grayFrames);

std::vector<cv::Mat> *convertToGrayscales(std::vector<cv::Mat> &rgbFrames);

std::vector<cv::Mat *> *convertToGrayscalePtrs(std::vector<cv::Mat *> &rgbFrames, std::vector<cv::Mat *> &grayFrames);

std::vector<cv::Mat *> *convertToGrayscalePtrs(std::vector<cv::Mat *> &rgbFrames);

void cleanMatF32(cv::Mat &mat);

void cleanMatS32(cv::Mat &mat);

#endif