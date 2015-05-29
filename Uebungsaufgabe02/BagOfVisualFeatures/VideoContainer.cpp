#include "stdafx.h"
//#include <stdio.h>
//#include <string.h>
//#include <list>
//#include <vector>
//#include <cmath>
//#include <opencv2\opencv.hpp> 

#include "VideoContainer.h"

using namespace cv;
using namespace std; 

VideoContainer::VideoContainer(string videoFileName, string classification){
	this->m_videoFileName = videoFileName;
	this->m_classification = classification;

	// read frames once
}

string VideoContainer::GetVideoFileName()
{
	return this->m_videoFileName;
}

string VideoContainer::GetClassification()
{
	return this->m_classification;
}

vector<Mat> *VideoContainer::GetFrames()
{
	return NULL;
}
