//#define USEFRAMEPTRS
#define ONLYSPACIALFRAMES
//#define TESTWRITE

#ifndef VIDEOCONTAINER
#define VIDEOCONTAINER

#include "stdafx.h"
#include <string.h>
#include <vector>
#include <opencv2\opencv.hpp>


class VideoContainer
{
private:
	bool m_lazy;
	std::string m_videoFileName;
	std::string m_classification;

#ifdef USEFRAMEPTRS
	std::vector<cv::Mat *> *m_videoFrames;
	std::vector<cv::Mat *> *m_spatialTemporalFrames;
#else
	std::vector<cv::Mat> *m_videoFrames;
	std::vector<cv::Mat> *m_spatialTemporalFrames;
#endif

public:
	VideoContainer(std::string videoFileName, std::string classification, bool lazy = true);

	std::string getVideoFileName();
	std::string getClassification();

#ifdef USEFRAMEPTRS
	std::vector<cv::Mat *> *getFramePtrs();
	std::vector<cv::Mat *> *getSpatialTemporalFramePtrs();
#else
	std::vector<cv::Mat> *getFrames();
	std::vector<cv::Mat> *getSpatialTemporalFrames();
#endif

	void release();

};


#endif