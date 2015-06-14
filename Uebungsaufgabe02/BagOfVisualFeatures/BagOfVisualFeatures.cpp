//#define USEFRAMEPTRS

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
#include "FeatureDictionary.h"
#include "VideoMetaData.h"
#include "ConfusionMatrix.h"

using namespace cv;
using namespace std;

Mat *BuildVocabulary(vector<Mat *> &trainingFrames)
{
	vector<Mat *> *features = ExtractFeaturePtrs(trainingFrames);
	Mat *featureClusters = ClusterFeaturePtrs(*features);

	return featureClusters;
}

Mat *BuildVocabulary(vector<Mat> &trainingFrames)
{
	vector<Mat> *features = ExtractFeatures(trainingFrames);
	Mat *featureClusters = ClusterFeatures(*features);

	return featureClusters;
}

Mat *BuildVocabulary(vector<VideoContainer *> &videoContainers)
{
	vector<Mat> *fullFeatures = new vector<Mat>();
	for (int i=0; i<videoContainers.size(); i++)
	{
		VideoContainer *videoContainer = videoContainers[i];
		vector<Mat> *trainingFrames = videoContainer->getSpatialTemporalFrames();
		vector<Mat> *features = ExtractFeatures(*trainingFrames);
		
		videoContainer->release();
		//trainingFrames->clear(); // TODO: check double deletion?
		//delete trainingFrames;

		for(int j=0; j<features->size(); j++)
			fullFeatures->push_back((*features)[i].clone());

		features->clear();
		delete features;
	}
	Mat *featureClusters = ClusterFeatures(*fullFeatures);		
	return featureClusters;
}

vector<VideoMetaData *> *BuildVideoMetaData(vector<VideoContainer *> &videoContainers, FeatureDictionary &dictionary)//, vector<Mat> &histograms, vector<string> &classifications)
{
	// output featurepoints and labels for videos
	vector<VideoMetaData *> *videoMetaDataSet = new vector<VideoMetaData *>();

	int historamBins = dictionary.getVocabulary()->rows;
	Mat *histogram = new Mat(1, historamBins, CV_32F);

	for (int i=0; i<videoContainers.size(); i++)
	{
		VideoContainer *videoContainer = videoContainers[i];
		int nfeatures = 300; 	// denser sampling of videos
#ifdef USEFRAMEPTRS
		vector<Mat *> *features = ExtractFeaturePtrs(*(videoContainer->getSpatialTemporalFramePtrs()),nfeatures);
		Mat *collectedFeatures = AppendFeaturePtrs(*features);
		//// TEST
		//Mat *collectedFeatures = &(*features)[0];
#else
		vector<Mat> *spatialTemporalFrames = videoContainer->getSpatialTemporalFrames();
		vector<Mat> *features = ExtractFeatures(*spatialTemporalFrames,nfeatures);
		
		videoContainer->release();
		//spatialTemporalFrames->clear(); // TODO: check double deletion?
		//delete spatialTemporalFrames;

		Mat *collectedFeatures = AppendFeatures(*features);
#endif

		// search represantive words in vocabluary that are closest to features
		for(int row=0; row<collectedFeatures->rows; row++)
		{
			// Perform single search for mean
			vector<float> singleQuery;
			vector<int> index(1);
			vector<float> dist(1);

			// Searching for the feature
			int numDimensions = collectedFeatures->cols;
			for(int col = 0 ; col < numDimensions ;col++)
				singleQuery.push_back((*collectedFeatures).at<float>(row, col));

			// Invoke the function
			dictionary.getKDTree()->knnSearch(singleQuery, index, dist, 1, cv::flann::SearchParams(64));

			// create histogram of hitcounts in dictionary
			histogram->at<float>(0,index[0]) = histogram->at<float>(0,index[0]) + 1;
		}

		// normalize histogram
		for (int histBin=0; histBin<historamBins; histBin++)
			histogram->at<float>(0,histBin) = histogram->at<float>(0,histBin)/collectedFeatures->rows;

		//histograms.push_back(histogram);

		// label for normalized histogram
		//classifications.push_back(videoContainer->getClassification());

		VideoMetaData *videoMetaData = new VideoMetaData(histogram, videoContainer->getClassification());
		videoMetaDataSet->push_back(videoMetaData);
	}

	return videoMetaDataSet;
}

CvRTrees *TrainClassifier(vector<VideoMetaData *> &videoMetaDataSet, map<int, string> &hashClassificationMap)
{
	CvRTParams params;
	CvRTrees *rtree = new CvRTrees();

	// assemble trainingData out of histogram-entries of videoMetaDataSet
	
	// Copy Data
	// ===================================================
	// create new Mat
	int rows = videoMetaDataSet[0]->getHistogram()->rows;
	int cols = videoMetaDataSet[0]->getHistogram()->cols;
	int Rows = rows * videoMetaDataSet.size();
	int Cols = cols;
	int type = CV_32FC1;
	//Mat trainingData(videoMetaDataSet.size(),videoMetaDataSet[0]->getHistogram()->cols, CV_32FC1);
	Mat trainingData(Rows,Cols, CV_32FC1);
	
	// move every row of Mats in combined Mat
	for(int i=0; i<videoMetaDataSet.size(); i++)
		videoMetaDataSet[i]->getHistogram()->copyTo( trainingData( Rect(0, i*rows, cols, rows) ) );
	// ===================================================

	// Copy Data
	Mat trainingClassification(videoMetaDataSet.size(),1, CV_32FC1);
	for(int j=0; j<videoMetaDataSet.size(); j++)
	{
		int hash = videoMetaDataSet[j]->getClassification()->getHash();
		hashClassificationMap[hash] = videoMetaDataSet[j]->getClassification()->getName();
		trainingClassification.at<float>(0,j) = hash;
	}
	rtree->train(trainingData, CV_ROW_SAMPLE, trainingClassification,
				cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(), params);

	return rtree;
}

ConfusionMatrix *ClassifyVideos(vector<VideoContainer *> &videoContainers, FeatureDictionary &dictionary, CvRTrees &rTreeClassifier)
{
	// generate ConfusionMatrix that shows how often a classification of the videoContainers is hit by the learning algorithm

	vector<VideoMetaData *> *videoMetaDataSet = BuildVideoMetaData(videoContainers, dictionary);

	//Mat ConfusionMatrix(,,CV_32FC1);
	ConfusionMatrix *confusionMatrix = new ConfusionMatrix(*GetUniqueVideoMetaDataClassifications(*videoMetaDataSet));

	for(int i=0; i<videoMetaDataSet->size(); i++)
	{
		Mat *histogram = (*videoMetaDataSet)[i]->getHistogram();
		float classification = rTreeClassifier.predict(*histogram);

		confusionMatrix->add((*videoMetaDataSet)[i]->getClassification()->getHash(), classification);
	}

	return confusionMatrix;
}

void PrintConfusionMatrix(ConfusionMatrix &confusionMatrix)
{
	// print out Precision and Sensitifity for all classes
	vector<Classification *> *classifications = confusionMatrix.getClassifications();
	for(int i=0; i<classifications->size(); i++)
	{
		Classification *classification = (*classifications)[i];

		string classStr = "Classification: " + classification->getName() + "\n";
		cout << classStr;

		std::ostringstream ss;
		ss << confusionMatrix.Precision(*classification);
		std::string precStr(ss.str());
		precStr = "Percision: " + precStr  + "\n";

		cout << precStr;

		std::ostringstream ss2;
		ss2 << confusionMatrix.Sensitivity(*classification);
		std::string sensStr(ss.str());
		sensStr = "Sinsitivity: " + sensStr  + "\n";

		cout << sensStr;
	}
}


int main(int argc, char *argv[])
{
	//string videoFileName = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/training/1_Kiss/Kiss_001.avi";
	string videoFileName = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/training/1_Kiss/Kiss_002.avi";

	string videoFileDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/training/";
	//string videoFileDir = "C:/Users/Christian/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/Assignment2/training";
	ArgumentPath(argc, argv, 1, videoFileDir);
	
	vector<string> videoFileNames = getFilesPathWithinFolder(videoFileDir);

	if (videoFileNames.size() == 0)
	{
		cout << "No training video files found.";
		return -1;
	}

	string videoTestFileDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/test/";
	ArgumentPath(argc, argv, 2, videoTestFileDir);
	vector<string> videoTestFileNames = getFilesPathWithinFolder(videoTestFileDir);

	if (videoTestFileNames.size() == 0)
	{
		cout << "No test video files found.";
		return -1;
	}

	//// TEST
	//vector<string> videoFileNames;
	//videoFileNames.push_back(videoFileName);

//#ifdef USEFRAMEPTRS
//	vector<Mat *> *trainingFrames = new vector<Mat *>();
//#else
//	vector<Mat> *trainingFrames = new vector<Mat>();
//#endif
	vector<VideoContainer *> *videoContainers = new vector<VideoContainer *>();
	for(int i=0; i<videoFileNames.size(); i++)
	{
		vector<string> tokens = splitString(videoFileNames[i], "/");
		string classification = tokens[tokens.size()-1];

		VideoContainer *videoContainer = new VideoContainer(videoFileNames[i], classification);
		videoContainers->push_back(videoContainer);
//#ifdef USEFRAMEPTRS
//		trainingFrames->insert(trainingFrames->end(), videoContainer->getSpatialTemporalFramePtrs()->begin(), videoContainer->getSpatialTemporalFramePtrs()->end());
//#else
//		trainingFrames->insert(trainingFrames->end(), videoContainer->getSpatialTemporalFrames()->begin(), videoContainer->getSpatialTemporalFrames()->end());
//#endif
	}
	// Learning phase
	// ==============
	// apply feature extraction on all collected frames

	//Mat *vocabulary = BuildVocabulary(*trainingFrames);
	Mat *vocabulary = BuildVocabulary(*videoContainers);

	FeatureDictionary *dictionary = new FeatureDictionary(vocabulary);
	
	// build feature representation of every video in the trainingset
	//vector<Mat> *histograms = new vector<Mat>();
	//vector<string> *classifications = new vector<string>();
	vector<VideoMetaData *> *videoMetaDataSet = BuildVideoMetaData(*videoContainers, *dictionary);//, *histograms, *classifications);

	map<int, string> *hashClassificationMap = new map<int, string>();
	CvRTrees *classifier = TrainClassifier(*videoMetaDataSet, *hashClassificationMap);

	//string videoTestFileDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/test/";
	//ArgumentPath(argc, argv, 2, videoTestFileDir);
	//vector<string> videoTestFileNames = getFilesPathWithinFolder(videoTestFileDir);

	vector<VideoContainer *> *videoTestContainers = new vector<VideoContainer *>();
	for(int i=0; i<videoTestFileNames.size(); i++)
	{
		vector<string> tokens = splitString(videoTestFileNames[i], "/");
		string classification = tokens[tokens.size()-1];

		VideoContainer *videoContainer = new VideoContainer(videoTestFileNames[i], classification);
		videoTestContainers->push_back(videoContainer);
	}

	ConfusionMatrix *confusionMatrix = ClassifyVideos(*videoTestContainers, *dictionary, *classifier);

	// print out Precision and Sensitifity for all classes
	PrintConfusionMatrix(*confusionMatrix);

	int j=0;

	// ==============

}

