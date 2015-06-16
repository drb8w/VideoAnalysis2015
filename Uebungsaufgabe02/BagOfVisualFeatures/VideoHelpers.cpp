#include "VideoHelpers.h"

#include "stdafx.h"
//#include <stdio.h>
//#include <string.h> 
//#include <list>
//#include <vector>
//#include <cmath>
//#include <opencv2\opencv.hpp> 

#include "FrameHelpers.h"

#define FRAMEMODULO 4

using namespace cv;
using namespace std;


vector<Mat> *GetFramesCPP(string videoFileName, vector<Mat> *videoFrames, int frameModulo = FRAMEMODULO)
{
	VideoCapture capture;
	capture.open(videoFileName);
	if(! capture.isOpened()) {
		fprintf(stderr, "Could not initialize capturing..\n");
		return NULL;
	}

	long frameCtr = 0;
	while(true) {
		Mat frame;
		// get a new frame
		capture >> frame;
		if(frame.empty())
			break;

		//// TEST
		//namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
		//imshow( "Display window", frame);  
		//imwrite("C:\\test.png",frame);
		
		frameCtr++;
		if (frameCtr % frameModulo == 0)
			videoFrames->push_back(frame.clone());
	}

	return videoFrames;
}

vector<Mat> *GetFrames(string videoFileName, vector<Mat> *videoFrames, int frameModulo = FRAMEMODULO)
{
	// http://answers.opencv.org/question/5768/how-can-i-get-one-single-frame-from-a-video-file/
	VideoCapture cap(videoFileName); 
		
    if( !cap.isOpened()){
         cout << "Cannot open the video file" << endl;
         return NULL;
    }

    double count = cap.get(CV_CAP_PROP_FRAME_COUNT); //get the frame count
    double width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
    double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT);
	
	for (int i=0; i<count; i++)
	{
		if (i % frameModulo == 0)
		{
			cap.set(CV_CAP_PROP_POS_FRAMES,i); //Set index to frame i
			//Mat *frame = new Mat(height, width, CV_8UC3);
			//bool success = cap.read(*frame);
			//if (success)
			//	videoFrames->push_back(frame->clone());
			Mat frame;
			bool success = cap.read(frame);

			//// TEST
			//namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
			//imshow( "Display window", frame);  

			if (success)
				videoFrames->push_back(frame.clone());
		}
    }
	//cap.release();

	return videoFrames;
}

vector<Mat *> *GetFramePtrs(string videoFileName, vector<Mat *> *videoFramePtrs, int frameModulo = FRAMEMODULO)
{
	// http://answers.opencv.org/question/5768/how-can-i-get-one-single-frame-from-a-video-file/
	VideoCapture cap(videoFileName); 
		
    if( !cap.isOpened()){
         cout << "Cannot open the video file" << endl;
         return NULL;
    }

    double count = cap.get(CV_CAP_PROP_FRAME_COUNT); //get the frame count
    double width = cap.get(CV_CAP_PROP_FRAME_WIDTH); 
    double height = cap.get(CV_CAP_PROP_FRAME_HEIGHT); 
	
	//for (int i=0; i<count; i++)
	//{
	//	cap.set(CV_CAP_PROP_POS_FRAMES,i); //Set index to frame i
	//	Mat *frame = new Mat(height, width, CV_8UC3);
	//	try
	//	{
	//		bool success = cap.read(*frame);
	//		if (success)
	//			videoFramePtrs.push_back(&(frame->clone()));
	//	}
	//	catch(Exception ex)
	//	{
	//		int j=0;
	//	}
    //}

	for (int i=0; i<count; i++)
	{
		cap.set(CV_CAP_PROP_POS_FRAMES,i); //Set index to frame i
		try
		{
			if (i % frameModulo == 0)
			{
				Mat readFrame;
				bool success = cap.read(readFrame);
				if (success)
				{
					Mat *frame = new Mat(readFrame.size(), readFrame.type());
					readFrame.copyTo(*frame);
					videoFramePtrs->push_back(frame);
				}
			}
		}
		catch(Exception ex)
		{
			cout << "Exception in GetFramePtrs occoured.";
		}
    }

	//cap.release();

	return videoFramePtrs;
}

vector<Mat> *GetFramesCPP(string videoFileName, int frameModulo = FRAMEMODULO)
{	
	vector<Mat> *videoFrames = new vector<Mat>();
	return GetFramesCPP(videoFileName, videoFrames, frameModulo);
}

vector<Mat> *GetFrames(string videoFileName, int frameModulo = FRAMEMODULO)
{	
	vector<Mat> *videoFrames = new vector<Mat>();
	return GetFrames(videoFileName, videoFrames, frameModulo);
}

vector<Mat *> *GetFramePtrs(string videoFileName, int frameModulo = FRAMEMODULO)
{	
	vector<Mat *> *videoFrames = new vector<Mat *>();
	return GetFramePtrs(videoFileName, videoFrames, frameModulo);
}

//vector<Mat> *GetSpatialTemporalFramesXt(vector<Mat> *frames, vector<Mat> *framesXt, int frameModulo = FRAMEMODULO)
//{
//	// stack variable is y i.e. row
//
//	int timeStamps = frames->size();
//	for(int timeNr = 0; timeNr < timeStamps; timeNr ++)
//	{
//		Mat imgSpatioTemporal(timeStamps, (*frames)[0].cols, CV_8U);
//		framesXt->push_back(imgSpatioTemporal);
//	}
//
//	for(int timeNr = 0; timeNr < timeStamps; timeNr ++)
//	{
//		Mat img_i = (*frames)[timeNr];
//
//		for(int columnNr = 0; columnNr < (*frames)[0].cols; columnNr++ )
//		{		
//			for(int rowNr = 0; rowNr < (*frames)[0].rows; rowNr++ )
//			{
//				// transfer luminance
//				img_i.at<uchar>(timeNr, columnNr) = img_i.at<uchar>(rowNr, columnNr);				
//			}
//		}	
//	}
//
//	return framesXt;
//}

vector<Mat> *GetSpatialTemporalFramesXt(vector<Mat> &frames, vector<Mat> *framesXt, int frameModulo = FRAMEMODULO)
{
	// stack variable is y i.e. row
	int timeStamps = frames.size();
	int spacioTempFrames = frames[0].rows / frameModulo;
	for(int spacTempNr = 0; spacTempNr < spacioTempFrames; spacTempNr ++)
	{
		Mat imgSpatioTemporal(timeStamps, frames[0].cols, CV_8U);
		framesXt->push_back(imgSpatioTemporal);
	}


	for(int spacTempNr = 0; spacTempNr < spacioTempFrames; spacTempNr ++)
	{
		// a spacTempNr indicates a frame in Xt
		Mat targetFrame = (*framesXt)[spacTempNr];
		
		for(int timeNr = 0; timeNr < timeStamps; timeNr ++)
		{
			Mat sourceFrame = frames[timeNr];
			for(int columnNr = 0; columnNr < frames[0].cols; columnNr++ )
			{
				// transfer luminance
				//(*framesXt)[spacTempNr].at<uchar>(timeNr,columnNr) = sourceFrame.at<uchar>(spacTempNr*frameModulo,columnNr);
				targetFrame.at<uchar>(timeNr,columnNr) = sourceFrame.at<uchar>(spacTempNr*frameModulo,columnNr);
			}
		}
		//// TEST
		//namedWindow( "Display window", WINDOW_AUTOSIZE );// Create a window for display.
		//imshow( "Display window", targetFrame );  

		(*framesXt)[spacTempNr] = targetFrame;
	}

	return framesXt;
}

vector<Mat *> *GetSpatialTemporalFramePtrsXt(vector<Mat *> &frames, vector<Mat *> *framesXt, int frameModulo = FRAMEMODULO)
{
	// stack variable is y i.e. row
	int timeStamps = frames.size();
	int spacioTempFrames = frames[0]->rows / frameModulo;
	for(int spacTempNr = 0; spacTempNr < spacioTempFrames; spacTempNr ++)
	{
		Mat *imgSpatioTemporal = new Mat(timeStamps, frames[0]->cols, CV_8U);
		framesXt->push_back(imgSpatioTemporal);
	}

	for(int spacTempNr = 0; spacTempNr < spacioTempFrames; spacTempNr ++)
	{
		// a spacTempNr indicates a frame in Xt
		Mat *targetFrame = (*framesXt)[spacTempNr];
		
		for(int timeNr = 0; timeNr < timeStamps; timeNr ++)
		{
			Mat *sourceFrame = frames[timeNr];
			for(int columnNr = 0; columnNr < frames[0]->cols; columnNr++ )
			{
				// transfer luminance
				targetFrame->at<uchar>(timeNr,columnNr) = sourceFrame->at<uchar>(spacTempNr*frameModulo,columnNr);
			}
		}
	}

	return framesXt;
}

vector<Mat> *GetSpatialTemporalFramesXt(vector<Mat> &frames, int frameModulo = FRAMEMODULO)
{
	vector<Mat> *framesXt = new vector<Mat>();	
	return GetSpatialTemporalFramesXt(frames, framesXt, frameModulo);
}

vector<Mat *> *GetSpatialTemporalFramePtrsXt(vector<Mat *> &frames, int frameModulo = FRAMEMODULO)
{
	vector<Mat *> *framesXt = new vector<Mat *>();	
	return GetSpatialTemporalFramePtrsXt(frames, framesXt, frameModulo);
}

vector<Mat> *GetSpatialTemporalFrames(vector<Mat> &videoFrames, vector<Mat> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true, int frameModulo = FRAMEMODULO)
{	
	vector<Mat> *grayFrames = convertToGrayscales(videoFrames);
	if(useFramesXY)
	{
		trainingFrames->assign(grayFrames->begin(), grayFrames->end());
	}
	if (useFramesXt)
	{
		GetSpatialTemporalFramesXt(*grayFrames, trainingFrames, frameModulo);
	}
	if (useFramesYt)
	{
		//GetSpatialTemporalFramesYt(*grayFrames, trainingFrames);
	}
	return trainingFrames;
}

vector<Mat *> *GetSpatialTemporalFramePtrs(vector<Mat *> &videoFrames, vector<Mat *> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true, int frameModulo = FRAMEMODULO)
{	
	vector<Mat *> *grayFrames = convertToGrayscalePtrs(videoFrames);
	if(useFramesXY)
	{
		trainingFrames->assign(grayFrames->begin(), grayFrames->end());
	}
	if (useFramesXt)
	{
		GetSpatialTemporalFramePtrsXt(*grayFrames, trainingFrames, frameModulo);
	}
	if (useFramesYt)
	{
		//GetSpatialTemporalFramePtrsYt(grayFrames, trainingFrames, frameModulo);
	}
	return trainingFrames;
}

vector<Mat> *GetSpatialTemporalFrames(vector<Mat> &videoFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true, int frameModulo = FRAMEMODULO)
{
	vector<Mat> *trainingFrames = new vector<Mat>();
	return GetSpatialTemporalFrames(videoFrames, trainingFrames, useFramesXY, useFramesXt, useFramesYt, frameModulo);
}

vector<Mat *> *GetSpatialTemporalFramePtrs(vector<Mat *> &videoFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true, int frameModulo = FRAMEMODULO)
{
	vector<Mat *> *trainingFrames = new vector<Mat *>();
	return GetSpatialTemporalFramePtrs(videoFrames, trainingFrames, useFramesXY, useFramesXt, useFramesYt, frameModulo);
}

vector<Mat> * GetSpatialTemporalFramesSet(vector<vector<Mat>> &videoFramesSet, vector<Mat> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true)
{
	for(int i=0; i<videoFramesSet.size(); i++)
		GetSpatialTemporalFrames(videoFramesSet[i], trainingFrames);

	return trainingFrames;
}

vector<Mat *> *GetSpatialTemporalFramePtrsSet(vector<vector<Mat *>> &videoFramesSet, vector<Mat *> *trainingFrames, bool useFramesXY = true, bool useFramesXt = true, bool useFramesYt = true)
{
	for(int i=0; i<videoFramesSet.size(); i++)
		//GetSpatialTemporalFramePtrs(videoFramesSet[i], trainingFrames);

	return trainingFrames;
}