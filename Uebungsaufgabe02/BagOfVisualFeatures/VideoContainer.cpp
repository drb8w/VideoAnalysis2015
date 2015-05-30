#include "stdafx.h"
//#include <stdio.h>
//#include <string.h>
//#include <list>
//#include <vector>
//#include <cmath>
//#include <opencv2\opencv.hpp> 

#include "VideoContainer.h"
#include "VideoHelpers.h"

using namespace cv;
using namespace std; 

VideoContainer::VideoContainer(string videoFileName, string classification)
{
	this->m_videoFileName = videoFileName;
	this->m_classification = classification;

	// read frames once
	this->m_videoFrames = GetFrames(videoFileName);
	this->m_spatialTemporalFrames = new vector<Mat>();
	GetSpatialTemporalFrames(this->m_videoFrames, this->m_spatialTemporalFrames);
}

string VideoContainer::getVideoFileName()
{
	return this->m_videoFileName;
}

string VideoContainer::getClassification()
{
	return this->m_classification;
}

vector<Mat> *VideoContainer::getFrames()
{
	return this->m_videoFrames;
}

vector<Mat> *VideoContainer::getSpatialTemporalFrames()
{
	return this->m_spatialTemporalFrames;
}
