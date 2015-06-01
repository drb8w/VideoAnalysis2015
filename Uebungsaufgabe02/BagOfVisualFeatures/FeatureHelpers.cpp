#include "FeatureHelpers.h"

#include "stdafx.h"
#include <stdio.h>
#include <list>
#include <cmath>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\nonfree\nonfree.hpp>

using namespace cv;
using namespace std;

vector<Mat *> *ExtractFeatures(vector<Mat *> &frames, int nfeatures = 100)
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

Mat *AppendFeatures(vector<Mat *> &features)
{
	// create new Mat
	int rows = features[0]->rows;
	int cols = features[0]->cols;
	int Rows = rows * features.size();
	int Cols = cols;
	int type = features[0]->type();
	Mat *featureMat = new Mat(Rows, Cols, type);

	// move every row of Mats in combined Mat
	for(int i=0; i<features.size(); i++)
	{
		//// TEST
		//float test1 = (*features)[i].at<float>(0,0);
		//float test10 = (*features)[i].at<float>(10,10);

		// Rect(int x, int y, int width, int height) 
		features[i]->copyTo( (*featureMat)( Rect(0, i*rows, cols, rows) ) );
		
		//// TEST
		//float test2 = featureMat.at<float>(i*rows,0);
		//float test20 = featureMat.at<float>(i*rows+10,10);
		//std::vector<float> myArray;
		//myArray.assign((float*)featureMat.datastart, (float*)featureMat.dataend);
		//int s=0;
	}
	return featureMat;
}


Mat *ClusterFeatures(vector<Mat *> &features, int numClusters = 500) // 50*50 from CompVis or 480 from paper
{
	// create new Mat
	int rows = features[0]->rows;
	int cols = features[0]->cols;
	int Rows = rows * features.size();
	int Cols = cols;
	int type = features[0]->type();
	Mat featureMat(Rows, Cols, type);

	// move every row of Mats in combined Mat
	for(int i=0; i<features.size(); i++)
	{
		//// TEST
		//float test1 = (*features)[i].at<float>(0,0);
		//float test10 = (*features)[i].at<float>(10,10);

		// Rect(int x, int y, int width, int height) 
		features[i]->copyTo( featureMat( Rect(0, i*rows, cols, rows) ) );
		
		//// TEST
		//float test2 = featureMat.at<float>(i*rows,0);
		//float test20 = featureMat.at<float>(i*rows+10,10);
		//std::vector<float> myArray;
		//myArray.assign((float*)featureMat.datastart, (float*)featureMat.dataend);
		//int s=0;
	}
	

	Mat bestLabels;
	int attempts = 5;
	Mat centers;
	kmeans(featureMat, numClusters, bestLabels, TermCriteria(CV_TERMCRIT_ITER|CV_TERMCRIT_EPS, 10000, 0.0001), attempts, KMEANS_PP_CENTERS, centers );
	
	Mat *clusters = new Mat(centers.rows, centers.cols, centers.type());
	centers.copyTo(*clusters);//,Rect(0,0,centers.cols, centers.rows));

	return clusters;
}