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
#ifdef USEFRAMEPTRS
	this->m_videoFrames = GetFramePtrs(videoFileName);
	this->m_spatialTemporalFrames = new vector<Mat *>();
	GetSpatialTemporalFramePtrs(*(this->m_videoFrames), this->m_spatialTemporalFrames);
#else
	this->m_videoFrames = GetFramesCPP(videoFileName);
	this->m_spatialTemporalFrames = new vector<Mat>();
	this->m_videoFrames = GetSpatialTemporalFrames(this->m_spatialTemporalFrames);
#endif

}

string VideoContainer::getVideoFileName()
{
	return this->m_videoFileName;
}

string VideoContainer::getClassification()
{
	return this->m_classification;
}

#ifdef USEFRAMEPTRS
vector<Mat *> *VideoContainer::getFramePtrs()
{
	return this->m_videoFrames;
}

vector<Mat *> *VideoContainer::getSpatialTemporalFramePtrs()
{
	return this->m_spatialTemporalFrames;
}
#else
vector<Mat> *VideoContainer::getFrames()
{
	return this->m_videoFrames;
}

vector<Mat> *VideoContainer::getSpatialTemporalFrames()
{
	return this->m_spatialTemporalFrames;
}
#endif