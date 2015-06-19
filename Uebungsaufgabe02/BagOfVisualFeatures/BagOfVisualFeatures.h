#ifndef BAGOFVISUALFEATURES
#define BAGOFVISUALFEATURES

//#define USEFRAMEPTRS
#define SVM
//#define KNN
//#define TREE
//#define MYKNN


#include "stdafx.h"
#include <vector>
#include <opencv2\opencv.hpp>
#include <opencv2\imgproc\imgproc.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>
#include <opencv2\nonfree\features2d.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\nonfree\nonfree.hpp>

#include "VideoContainer.h"
#include "FeatureDictionary.h"
#include "VideoMetaData.h"
#include "ConfusionMatrix.h"

//void PrintMemoryInfo( DWORD processID );
//void PrintMemoryUsage();

cv::Mat *BuildVocabulary(std::vector<cv::Mat *> &trainingFrames);

cv::Mat *BuildVocabulary(std::vector<cv::Mat> &trainingFrames);

cv::Mat *BuildVocabulary(std::vector<VideoContainer *> &videoContainers);

std::vector<VideoMetaData *> *BuildVideoMetaData(std::vector<VideoContainer *> &videoContainers, FeatureDictionary &dictionary);

std::map<int, std::string> * GetHashClassificationMap(std::vector<VideoMetaData *> &videoMetaDataSet);

void PrintFileClassList(std::vector<std::string> &fileClassList);

//cv::CvStatModel *TrainClassifier(std::vector<VideoMetaData *> &videoMetaDataSet, std::map<int, std::string> &hashClassificationMap);

//ConfusionMatrix *ClassifyVideos(std::vector<VideoContainer *> &videoContainers, FeatureDictionary &dictionary, cv::CvStatModel *classifier);

int main(int argc, char *argv[]);


#endif