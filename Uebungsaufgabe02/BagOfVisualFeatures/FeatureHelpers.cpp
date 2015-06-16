#include "FeatureHelpers.h"

#include "stdafx.h"
//#define NFEATURES 100
#define NFEATURES 50

//#define NUMCLUSTERS 480 // 50*50 from CompVis or 480 from paper
#define NUMCLUSTERS 240

#include <stdio.h>
#include <list>
#include <cmath>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\nonfree\nonfree.hpp>

#include <iostream>

using namespace cv;
using namespace std;

vector<Mat> *ExtractFeatures(vector<Mat> &frames, int nfeatures = NFEATURES)
{
	cout << "Start: ExtractFeatures \n";
	// apply feature extraction on all collected frames
	vector<Mat> *descriptorsList = new vector<Mat>();

	// SIFT::SIFT(int nfeatures=0, int nOctaveLayers=3, double contrastThreshold=0.04, double edgeThreshold=10, double sigma=1.6)
	// void SIFT::operator()(InputArray img, InputArray mask, vector<KeyPoint>& keypoints, OutputArray descriptors, bool useProvidedKeypoints=false)
	SiftFeatureDetector detector(nfeatures);    
	
	for(int i=0; i<frames.size(); i++)
	{
		vector<KeyPoint> keypoints;
		detector.detect(frames[i], keypoints);
		Mat descriptors(nfeatures,128,CV_32FC1);
		detector.compute(frames[i], keypoints, descriptors);
		//// TEST
		//float test1 = descriptors.at<float>(0,0);
		//float test10 = descriptors.at<float>(10,10);
		descriptorsList->push_back(descriptors.clone());
	}

	cout << "End: ExtractFeatures \n";
	return descriptorsList;
}

vector<Mat *> *ExtractFeaturePtrs(vector<Mat *> &frames, int nfeatures = NFEATURES)
{
	// apply feature extraction on all collected frames
	vector<Mat *> *descriptorsList = new vector<Mat *>();

	// SIFT::SIFT(int nfeatures=0, int nOctaveLayers=3, double contrastThreshold=0.04, double edgeThreshold=10, double sigma=1.6)
	// void SIFT::operator()(InputArray img, InputArray mask, vector<KeyPoint>& keypoints, OutputArray descriptors, bool useProvidedKeypoints=false)
	SiftFeatureDetector detector(nfeatures);    
	
	for(int i=0; i<frames.size(); i++)
	{
		vector<KeyPoint> keypoints;
		detector.detect(*(frames[i]), keypoints);
		Mat *descriptors = new Mat(nfeatures,128,CV_32FC1);
		detector.compute(*(frames[i]), keypoints, *descriptors);
		//// TEST
		//float test1 = descriptors.at<float>(0,0);
		//float test10 = descriptors.at<float>(10,10);
		descriptorsList->push_back(descriptors);
	}

	return descriptorsList;
}

Mat *AppendFeatures(vector<Mat> &features)
{
	cout << "Start: AppendFeatures \n";
	// 1st: determine how many rows the resulting matrix must have
	int Rows = 0;
	for(int i=0; i<features.size(); i++)
		if (!features[i].empty())
			Rows += features[i].rows;
	int Cols = features[0].cols;
	int type = features[0].type();
	// create new Mat
	Mat *featureMat = new Mat(Rows, Cols, type);

	// 2nd: copy data with the right row offset to matrix
	int rowOffset = 0;
	for(int i=0; i<features.size(); i++)
	{
		if (!features[i].empty())
		{
			features[i].copyTo( (*featureMat)( Rect(0, rowOffset, Cols, features[i].rows) ) );
			rowOffset += features[i].rows;
		}
	}
	cout << "End: AppendFeatures \n";
	return featureMat;
}

Mat *AppendFeaturePtrs(vector<Mat *> &features)
{
	// 1st: determine how many rows the resulting matrix must have
	int Rows = 0;
	for(int i=0; i<features.size(); i++)
		if (features[i] != NULL && !features[i]->empty())
			Rows += features[i]->rows;
	int Cols = features[0]->cols;
	int type = features[0]->type();
	// create new Mat
	Mat *featureMat = new Mat(Rows, Cols, type);

	// 2nd: copy data with the right row offset to matrix
	int rowOffset = 0;
	for(int i=0; i<features.size(); i++)
	{
		if (features[i] != NULL && !features[i]->empty())
		{
			features[i]->copyTo( (*featureMat)( Rect(0, rowOffset, Cols, features[i]->rows) ) );
			rowOffset += features[i]->rows;
		}
	}

	return featureMat;
}

Mat *ClusterFeatures(vector<Mat> &features, int numClusters = NUMCLUSTERS) // 50*50 from CompVis or 480 from paper
{
	cout << "Start: ClusterFeatures \n";
	// 1st: determine how many rows the resulting matrix must have
	int Rows = 0;
	for(int i=0; i<features.size(); i++)
		if (!features[i].empty())
			Rows += features[i].rows;
	int Cols = features[0].cols;
	int type = features[0].type();
	// create new Mat
	Mat featureMat(Rows, Cols, type);

	// 2nd: copy data with the right row offset to matrix
	int rowOffset = 0;
	for(int i=0; i<features.size(); i++)
	{
		if (!features[i].empty())
		{
			features[i].copyTo( featureMat( Rect(0, rowOffset, Cols, features[i].rows) ) );
			rowOffset += features[i].rows;
		}
	}

	// 3rd: cluster features
	Mat bestLabels;
	int attempts = 5;
	Mat centers;
	kmeans(featureMat, numClusters, bestLabels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers );
	
	Mat *clusters = new Mat(centers.rows, centers.cols, centers.type());
	centers.copyTo(*clusters);//,Rect(0,0,centers.cols, centers.rows));

	cout << "End: ClusterFeatures \n";
	return clusters;
}

Mat *ClusterFeaturePtrs(vector<Mat *> &features, int numClusters = NUMCLUSTERS) // 50*50 from CompVis or 480 from paper
{
	// 1st: determine how many rows the resulting matrix must have
	int Rows = 0;
	for(int i=0; i<features.size(); i++)
		if (features[i] != NULL && !features[i]->empty())
			Rows += features[i]->rows;
	int Cols = features[0]->cols;
	int type = features[0]->type();
	// create new Mat
	Mat featureMat(Rows, Cols, type);

	// 2nd: copy data with the right row offset to matrix
	int rowOffset = 0;
	for(int i=0; i<features.size(); i++)
	{
		if(features[i] != NULL && !features[i]->empty())
		{
			features[i]->copyTo( featureMat( Rect(0, rowOffset, Cols, features[i]->rows) ) );
			rowOffset += features[i]->rows;
		}
	}

	// 3rd: cluster features
	Mat bestLabels;
	int attempts = 5;
	Mat centers;
	kmeans(featureMat, numClusters, bestLabels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers );
	
	Mat *clusters = new Mat(centers.rows, centers.cols, centers.type());
	centers.copyTo(*clusters);//,Rect(0,0,centers.cols, centers.rows));

	return clusters;
}