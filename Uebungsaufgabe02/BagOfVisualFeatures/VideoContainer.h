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

public:
	VideoContainer(std::string videoFileName, std::string classification);

	std::string GetVideoFileName();
	std::string GetClassification();
	std::vector<cv::Mat> *GetFrames();

};


#endif