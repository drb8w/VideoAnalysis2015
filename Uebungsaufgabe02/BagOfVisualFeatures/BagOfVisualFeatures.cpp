#include "stdafx.h"
#include <stdio.h>
#include <list>
#include <vector>
#include <cmath>
#include <opencv2\opencv.hpp> 
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\nonfree\nonfree.hpp>

#include "FrameHelpers.h"
#include "VideoHelpers.h"
#include "FeatureHelpers.h"
#include "VideoContainer.h"
#include "FileHelpers.h"
#include "StringHelpers.h"

using namespace cv;
using namespace std;

//VideoCapture cap("C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/training/1_Kiss/Kiss_001.avi"); 


Mat * BuildVocabulary(vector<Mat> *trainingFrames)
{
	vector<Mat> *features = ExtractFeatures(trainingFrames);
	Mat *featureClusters = ClusterFeatures(features);

	return featureClusters;
}

void BuildKNN(vector<VideoContainer *> *videoContainers, Mat *vocabulary)
{
	// output featurepoints and labels for videos

}

int main(int argc, char *argv[])
{
	//string videoFileName = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/training/1_Kiss/Kiss_001.avi";
	//string videoFileName = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/training/1_Kiss/Kiss_002.avi";

	//string videoFileDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/training/1_Kiss/";
	string videoFileDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/training/";
	ArgumentPath(argc, argv, 1, videoFileDir);
	

	vector<string> videoFileNames = getFilesPathWithinFolder(videoFileDir);
	
	//vector<string> *videoFileNames = new vector<string>();
	//videoFileNames->push_back(videoFileName);

	vector<Mat> *trainingFrames = new vector<Mat>();
	vector<VideoContainer *> *videoContainers = new vector<VideoContainer *>();
	for(int i=0; i<videoFileNames.size(); i++)
	{
		//vector<Mat> *videoFrames = GetFrames(videoFileName);	
		//GetSpatialTemporalFrames(videoFrames, trainingFrames);

		vector<string> tokens = splitString(videoFileNames[i], "/");
		string classification = tokens[tokens.size()-1];

		VideoContainer *videoContainer = new VideoContainer(videoFileNames[i], classification);
		videoContainers->push_back(videoContainer);
		trainingFrames->insert(trainingFrames->end(), videoContainer->getSpatialTemporalFrames()->begin(), videoContainer->getSpatialTemporalFrames()->end());
	}
	// Learning phase
	// ==============
	// apply feature extraction on all collected frames

	Mat *vocabulary = BuildVocabulary(trainingFrames);

	// build feature representation of every video in the trainingset
	BuildKNN(videoContainers, vocabulary);

	int j=0;

	// ==============

}

