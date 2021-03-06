#define ONLYSPACIALFRAMES
//#define TESTWRITE

#include "stdafx.h"
//#include <stdio.h>
//#include <string.h>
//#include <list>
//#include <vector>
//#include <cmath>
//#include <opencv2\opencv.hpp> 

#include "VideoContainer.h"
#include "VideoHelpers.h"
#include "FrameHelpers.h"

using namespace cv;
using namespace std; 

VideoContainer::VideoContainer(string videoFileName, string classification, bool lazy)
{
	this->m_videoFileName = videoFileName;
	this->m_classification = classification;
	this->m_lazy = lazy;

	if (lazy)
	{
		this->m_videoFrames = NULL;
		this->m_spatialTemporalFrames = NULL;
	}
	else
	{
	// read frames once
#ifdef USEFRAMEPTRS
	#ifdef ONLYSPACIALFRAMES
			this->m_spatialTemporalFrames = GetFramePtrs(videoFileName);
	#else
		this->m_videoFrames = GetFramePtrs(videoFileName);
		this->m_spatialTemporalFrames = new vector<Mat *>();
		GetSpatialTemporalFramePtrs(*(this->m_videoFrames), this->m_spatialTemporalFrames);
	#endif
#else
	#ifdef ONLYSPACIALFRAMES
		//vector<Mat> *frames = GetFrames(videoFileName);
		vector<Mat> *frames = GetFramesCPP(videoFileName);
		vector<Mat> *grayFrames = convertToGrayscales(*frames);
		this->m_spatialTemporalFrames = grayFrames;
	#else
		this->m_videoFrames = GetFramesCPP(videoFileName);
		//this->m_videoFrames = GetFrames(videoFileName);

		//// TEST
		//for (int i=0; i<this->m_videoFrames->size(); i++)
		//{
		//    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
		//    imshow( "Display window", (*(this->m_videoFrames))[i] );  
		//}

		this->m_spatialTemporalFrames = new vector<Mat>();
		this->m_spatialTemporalFrames = GetSpatialTemporalFrames(*(this->m_videoFrames));
	#endif
#endif
	}
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
	if (this->m_lazy && this->m_videoFrames == NULL)
	{
		// read frames once
		this->m_videoFrames = GetFramePtrs(this->m_videoFileName);
	}	
	return this->m_videoFrames;
}

vector<Mat *> *VideoContainer::getSpatialTemporalFramePtrs()
{
	if (this->m_lazy && this->m_spatialTemporalFrames == NULL)
	{
	#ifdef ONLYSPACIALFRAMES
		// read frames once
		this->m_spatialTemporalFrames = getFramePtrs();
	#else
		// read frames once
		this->m_videoFrames = getFramePtrs();
		this->m_spatialTemporalFrames = new vector<Mat *>();
		GetSpatialTemporalFramePtrs(*(this->m_videoFrames), this->m_spatialTemporalFrames);
	#endif
	}
	return this->m_spatialTemporalFrames;
}

void VideoContainer::release()
{
	if (this->m_lazy)
	{
		if (this->m_videoFrames != NULL)
		{
			for(int i=0; i<this->m_videoFrames->size(); i++)
				if (!(*this->m_videoFrames)[i]->empty())
					(*this->m_videoFrames)[i]->release(); // check if memory is freed
		
			delete this->m_videoFrames;
			this->m_videoFrames = NULL;
		}

		if (this->m_spatialTemporalFrames != NULL)
		{
			for(int i=0; i<this->m_spatialTemporalFrames->size(); i++)
				if (!(*this->m_spatialTemporalFrames)[i]->empty())
					(*this->m_spatialTemporalFrames)[i]->release(); // check if memory is freed
		
			delete this->m_spatialTemporalFrames;
			this->m_spatialTemporalFrames = NULL;
		}
	}
}

#else
vector<Mat> *VideoContainer::getFrames()
{
	if (this->m_lazy && this->m_videoFrames == NULL)
	{
		// read frames once
		this->m_videoFrames = GetFramesCPP(this->m_videoFileName);
		//this->m_videoFrames = GetFrames(this->m_videoFileName);
	}
	return this->m_videoFrames;
}

vector<Mat> *VideoContainer::getSpatialTemporalFrames()
{
	cout << "Start: getSpatialTemporalFrames \n";
	if (this->m_lazy && this->m_spatialTemporalFrames == NULL)
	{
	#ifdef ONLYSPACIALFRAMES
		// read frames once
		vector<Mat> *frames = getFrames();
		vector<Mat> *grayFrames = convertToGrayscales(*frames);
		this->m_spatialTemporalFrames = grayFrames;
	#else
		// read frames once
		this->m_videoFrames = getFrames();

		//// TEST
		//for (int i=0; i<this->m_videoFrames->size(); i++)
		//{
		//    namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
		//    imshow( "Display window", (*(this->m_videoFrames))[i] );  
		//}

		this->m_spatialTemporalFrames = new vector<Mat>();
		this->m_spatialTemporalFrames = GetSpatialTemporalFrames(*(this->m_videoFrames));
	#endif
	}

#ifdef TESTWRITE
	for(int i=0; i<this->m_spatialTemporalFrames->size(); i++)
	{
		stringstream ss;
		ss << i;
		string str = ss.str();

		//string impFileName = "C:/" + this->getVideoFileName() + str + ".png";
		string impFileName = "C:/test.png";
		Mat mat = (*(this->m_spatialTemporalFrames))[i];
		imwrite(impFileName, (*(this->m_spatialTemporalFrames))[i]);

		int x=0;
	}
#endif
	cout << "End: getSpatialTemporalFrames \n";
	return this->m_spatialTemporalFrames;
}

void VideoContainer::release()
{
	cout << "Start: release \n";
	if (this->m_lazy)
	{
		if (this->m_videoFrames != NULL)
		{
			for(int i=0; i<this->m_videoFrames->size(); i++)
				if (!(*this->m_videoFrames)[i].empty())
					(*this->m_videoFrames)[i].release(); // check if memory is freed
		
			delete this->m_videoFrames;
			this->m_videoFrames = NULL;
		}

		if (this->m_spatialTemporalFrames != NULL)
		{
			for(int i=0; i<this->m_spatialTemporalFrames->size(); i++)
				if (!(*this->m_spatialTemporalFrames)[i].empty())
					(*this->m_spatialTemporalFrames)[i].release(); // check if memory is freed
		
			delete this->m_spatialTemporalFrames;
			this->m_spatialTemporalFrames = NULL;
		}
	}
	cout << "End: release \n";
}


#endif