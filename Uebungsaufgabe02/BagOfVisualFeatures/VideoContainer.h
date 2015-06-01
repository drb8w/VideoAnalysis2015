#ifndef VIDEOCONTAINER
#define VIDEOCONTAINER

#include "stdafx.h"
#include <string.h>
#include <vector>
#include <opencv2\opencv.hpp> 

class VideoContainer
{
private:
	std::string m_videoFileName;
	std::string m_classification;

	std::vector<cv::Mat *> *m_videoFrames;
	std::vector<cv::Mat *> *m_spatialTemporalFrames;

public:
	VideoContainer(std::string videoFileName, std::string classification);

	std::string getVideoFileName();
	std::string getClassification();
	std::vector<cv::Mat *> *getFrames();
	std::vector<cv::Mat *> *getSpatialTemporalFrames();
};


#endif