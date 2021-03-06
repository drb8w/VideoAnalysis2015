//#define USEFRAMEPTRS
//#define SVM
//#define KNN
#define TREE
//#define MYKNN

#include "stdafx.h"

#include "BagOfVisualFeatures.h"

#include "windows.h"
#include "psapi.h"
#include<tchar.h>

#include <iostream>
#include <fstream>

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
#include "KNNClassifier.h"

using namespace cv;
using namespace std;

void PrintMemoryInfo( DWORD processID )
{
    HANDLE hProcess;
    PROCESS_MEMORY_COUNTERS pmc;

    // Print the process identifier.

    printf( "\nProcess ID: %u\n", processID );

    // Print information about the memory usage of the process.

    hProcess = OpenProcess(  PROCESS_QUERY_INFORMATION |
                                    PROCESS_VM_READ,
                                    FALSE, processID );
    if (NULL == hProcess)
        return;

    if ( GetProcessMemoryInfo( hProcess, &pmc, sizeof(pmc)) )
    {
        printf( "\tPageFaultCount: 0x%08X\n", pmc.PageFaultCount );
        printf( "\tPeakWorkingSetSize: 0x%08X\n", 
                  pmc.PeakWorkingSetSize );
        printf( "\tWorkingSetSize: 0x%08X\n", pmc.WorkingSetSize );
        printf( "\tQuotaPeakPagedPoolUsage: 0x%08X\n", 
                  pmc.QuotaPeakPagedPoolUsage );
        printf( "\tQuotaPagedPoolUsage: 0x%08X\n", 
                  pmc.QuotaPagedPoolUsage );
        printf( "\tQuotaPeakNonPagedPoolUsage: 0x%08X\n", 
                  pmc.QuotaPeakNonPagedPoolUsage );
        printf( "\tQuotaNonPagedPoolUsage: 0x%08X\n", 
                  pmc.QuotaNonPagedPoolUsage );
        printf( "\tPagefileUsage: 0x%08X\n", pmc.PagefileUsage ); 
        printf( "\tPeakPagefileUsage: 0x%08X\n", 
                  pmc.PeakPagefileUsage );
    }

    CloseHandle( hProcess );
}

void PrintMemoryUsage()
{
	PrintMemoryInfo( GetCurrentProcessId() );
}

//void PrintMemoryUsage()
//{
//	// Use to convert bytes to MB
//	int DIV = 1048576;
//
//
//	// Specify the width of the field in which to print the numbers. 
//	// The asterisk in the format specifier "%*I64d" takes an integer 
//	// argument and uses it to pad and right justify the number.
//
//	int WIDTH =  7;
//	
//	MEMORYSTATUSEX statex;
//
//	statex.dwLength = sizeof (statex);
//
//	GlobalMemoryStatusEx (&statex);
//
//	_tprintf (TEXT("There is  %*ld percent of memory in use.\n"),WIDTH, statex.dwMemoryLoad);
//	_tprintf (TEXT("There are %*I64d total Mbytes of physical memory.\n"),WIDTH,statex.ullTotalPhys/DIV);
//	_tprintf (TEXT("There are %*I64d free Mbytes of physical memory.\n"),WIDTH, statex.ullAvailPhys/DIV);
//	_tprintf (TEXT("There are %*I64d total Mbytes of paging file.\n"),WIDTH, statex.ullTotalPageFile/DIV);
//	_tprintf (TEXT("There are %*I64d free Mbytes of paging file.\n"),WIDTH, statex.ullAvailPageFile/DIV);
//	_tprintf (TEXT("There are %*I64d total Mbytes of virtual memory.\n"),WIDTH, statex.ullTotalVirtual/DIV);
//	_tprintf (TEXT("There are %*I64d free Mbytes of virtual memory.\n"),WIDTH, statex.ullAvailVirtual/DIV);
//	_tprintf (TEXT("There are %*I64d free Mbytes of extended memory.\n"),WIDTH, statex.ullAvailExtendedVirtual/DIV);
//
//}


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
	cout << "Start: BuildVocabulary \n";
#ifdef USEFRAMEPTRS
	vector<Mat *> *fullFeatures = new vector<Mat *>();
#else
	vector<Mat> *fullFeatures = new vector<Mat>();
#endif
	for (int i=0; i<videoContainers.size(); i++)
	{
		VideoContainer *videoContainer = videoContainers[i];

		// TEST
		cout << "video " + videoContainer->getVideoFileName() + " \n";

#ifdef USEFRAMEPTRS
		vector<Mat *> *trainingFrames = videoContainer->getSpatialTemporalFramePtrs();
		vector<Mat *> *features = ExtractFeaturePtrs(*trainingFrames);
#else
		vector<Mat> *trainingFrames = videoContainer->getSpatialTemporalFrames();
		vector<Mat> *features = ExtractFeatures(*trainingFrames);
#endif	
		videoContainer->release();
		//trainingFrames->clear(); // TODO: check double deletion?
		//delete trainingFrames;

		//PrintMemoryUsage();

		cout << "create fullFeatures \n";
#ifdef USEFRAMEPTRS
		for(int j=0; j<features->size(); j++)
			if ((*features)[j] != NULL && !(*features)[j]->empty() )
				fullFeatures->push_back((*features)[j]);
#else
		for(int j=0; j<features->size(); j++)
			if (!(*features)[j].empty())
				fullFeatures->push_back((*features)[j].clone());
#endif
		cout << "clear features \n";
		features->clear();
		delete features;
	}
	// TEST
	//cout << "ClusterFeatures \n";

#ifdef USEFRAMEPTRS
	Mat *featureClusters = ClusterFeaturePtrs(*fullFeatures);
#else
	Mat *featureClusters = ClusterFeatures(*fullFeatures);
#endif
	cout << "End: BuildVocabulary \n";
	return featureClusters;
}

vector<VideoMetaData *> *BuildVideoMetaData(vector<VideoContainer *> &videoContainers, FeatureDictionary &dictionary)//, vector<Mat> &histograms, vector<string> &classifications)
{
	cout << "Start: BuildVideoMetaData \n";
	// output featurepoints and labels for videos
	vector<VideoMetaData *> *videoMetaDataSet = new vector<VideoMetaData *>();

	int historamBins = dictionary.getVocabulary()->rows;
	
	for (int i=0; i<videoContainers.size(); i++)
	{
		Mat *histogram = new Mat(1, historamBins, CV_32F);
		cleanMatF32(*histogram);

		VideoContainer *videoContainer = videoContainers[i];

		// TEST
		cout << "video " + videoContainer->getVideoFileName() + " \n";

		int nfeatures = 3*NFEATURES; 	// denser sampling of videos
#ifdef USEFRAMEPTRS
		vector<Mat *> *features = ExtractFeaturePtrs(*(videoContainer->getSpatialTemporalFramePtrs()),nfeatures);
		Mat *collectedFeatures = AppendFeaturePtrs(*features);
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

		// create video meta data
		VideoMetaData *videoMetaData = new VideoMetaData(histogram, videoContainer->getClassification(), videoContainer->getVideoFileName());
		videoMetaDataSet->push_back(videoMetaData);
	}

	cout << "End: BuildVideoMetaData \n";
	return videoMetaDataSet;
}

#ifdef MYKNN
KNNClassifier *TrainClassifier(vector<VideoMetaData *> &videoMetaDataSet, map<int, string> &hashClassificationMap)
#else
CvStatModel *TrainClassifier(vector<VideoMetaData *> &videoMetaDataSet, map<int, string> &hashClassificationMap)
#endif
{
	cout << "Start: TrainClassifier \n";
	
#ifdef TREE
	CvRTParams params;
	CvRTrees *classifier = new CvRTrees();
#endif
#ifdef SVM
	CvSVMParams params;
	CvSVM *classifier = new CvSVM();
#endif
#ifdef KNN
	int K = 5;
	CvKNearest *classifier = new CvKNearest();
#endif
#ifdef MYKNN
	int K = 5;
	KNNClassifier *classifier = new KNNClassifier();
#endif

	// assemble trainingData out of histogram-entries of videoMetaDataSet
	
	// Copy Data
	// ===================================================
	// create new Mat
	int rows = videoMetaDataSet[0]->getHistogram()->rows;
	int cols = videoMetaDataSet[0]->getHistogram()->cols;
	int Rows = rows * videoMetaDataSet.size();
	int Cols = cols;
	int type = CV_32FC1;
	Mat trainingData(Rows,Cols, CV_32FC1);
	// TEST
	cleanMatF32(trainingData);
	
	// move every row of Mats in combined Mat
	for(int i=0; i<videoMetaDataSet.size(); i++)
		videoMetaDataSet[i]->getHistogram()->copyTo( trainingData( Rect(0, i*rows, cols, rows) ) );

	//// TEST
	//string trainingDataFileName = ExecutionPath() + "TrainingData.yml";
	//FileStorage fs(trainingDataFileName, FileStorage::WRITE);
	//fs << "trainingData" << trainingData;
	//fs.release();
	// ===================================================

	// Copy Data
	Mat trainingClassification(videoMetaDataSet.size(),1, CV_32SC1);
	// TEST
	cleanMatS32(trainingClassification);
	
	for(int j=0; j<videoMetaDataSet.size(); j++)
	{
		int hash = videoMetaDataSet[j]->getClassification()->getHash();
		hashClassificationMap[hash] = videoMetaDataSet[j]->getClassification()->getName();
		trainingClassification.at<int>(j,0) = hash;
	}

	//// TEST
	//string trainingClassificationFileName = ExecutionPath() + "TrainingClassification.yml";
	//FileStorage fs2(trainingClassificationFileName, FileStorage::WRITE);
	//fs2 << "trainingClassification" << trainingClassification;
	//fs2.release();

#ifdef TREE
	classifier->train(trainingData, CV_ROW_SAMPLE, trainingClassification,
				cv::Mat(), cv::Mat(), cv::Mat(), cv::Mat(), params);
#endif
#ifdef SVM
	bool succ = classifier->train(trainingData, trainingClassification, Mat(), Mat(), params );

	//// TEST if learned trainingData is classified correctly
	//int classification = (int)classifier->predict(*(videoMetaDataSet[0]->getHistogram()));
	//int hash = videoMetaDataSet[0]->getClassification()->getHash();
	//CvSVM myClassifier(trainingData, trainingClassification, Mat(), Mat(), params );
	//int myClassification = (int)myClassifier.predict(*(videoMetaDataSet[0]->getHistogram()));

	//// TEST - try idiotic classification
	//Mat myTrainingData(10,1, CV_32FC1);
	//Mat myTrainingClassification(10,1, CV_32SC1);
	//for(int k=0; k<10; k++)
	//{
	//	myTrainingData.at<float>(k,0) = k;
	//	if (k < 5)
	//		myTrainingClassification.at<float>(k,0)=0;
	//	else
	//		myTrainingClassification.at<float>(k,0)=1;		
	//}
	//CvSVM myClassifier(myTrainingData, myTrainingClassification, Mat(), Mat(), params );
	//Mat testMat(1,1,CV_32FC1);
	//testMat.at<float>(0,0)=3;
	//float myClassification = myClassifier.predict(testMat);
	//testMat.at<float>(0,0)=7;
	//float myClassification2 = myClassifier.predict(testMat);	

#endif
#ifdef KNN
	classifier->train(trainingData, trainingClassification, Mat(), false, K );
#endif
#ifdef MYKNN
	classifier->train(trainingData, trainingClassification, K );
#endif

	cout << "End: TrainClassifier \n";
	return classifier;
}

#ifdef MYKNN
ConfusionMatrix *ClassifyVideos(vector<VideoContainer *> &videoContainers, FeatureDictionary &dictionary, KNNClassifier *classifier, map<int, string> &hashClassificationMap, vector<string> &fileClassList)
#else
ConfusionMatrix *ClassifyVideos(vector<VideoContainer *> &videoContainers, FeatureDictionary &dictionary, CvStatModel *classifier, map<int, string> &hashClassificationMap, vector<string> &fileClassList)
#endif
{
	cout << "Start: ClassifyVideos \n";
#ifdef TREE
	CvRTrees *classifierPtr = (CvRTrees *)(classifier);
#endif
#ifdef SVM
	CvSVM *classifierPtr = (CvSVM *)(classifier);
#endif
#ifdef KNN
	int K = 5;
	CvKNearest *classifierPtr = (CvKNearest *)(classifier);
#endif
#ifdef MYKNN
	int K = 5;
	KNNClassifier *classifierPtr = classifier;
#endif
	// generate ConfusionMatrix that shows how often a classification of the videoContainers is hit by the learning algorithm
	vector<VideoMetaData *> *videoMetaDataSet = BuildVideoMetaData(videoContainers, dictionary);

	ConfusionMatrix *confusionMatrix = new ConfusionMatrix(*GetVideoMetaDataClassifications(hashClassificationMap));

	for(int i=0; i<videoMetaDataSet->size(); i++)
	{
		Mat *histogram = (*videoMetaDataSet)[i]->getHistogram();
#if defined(KNN) || defined(MYKNN)
		int classification = (int)classifierPtr->find_nearest(*histogram, K );
#else
		int classification = (int)classifierPtr->predict(*histogram);
#endif

		try{
			// try to find in map
			string classificationStr = hashClassificationMap[classification];

			if (strcmp(classificationStr.c_str(),"") == 0 )
				classificationStr = "NO CLASS";
		
			// compact fileName to relative path without extension
			string relativeFileName = RelativeFileName((*videoMetaDataSet)[i]->getVideoFileName());
			string cleanClassificationStr = CleanString(classificationStr);
			string outputStr = relativeFileName+"\t"+cleanClassificationStr+"\n";
			fileClassList.push_back(outputStr);

			int hash = (*videoMetaDataSet)[i]->getClassification()->getHash();
			confusionMatrix->add(hash, classification);
		}
		catch(Exception ex)
		{

		}
	}
	cout << "End: ClassifyVideos \n";
	return confusionMatrix;
}

map<int, string> * GetHashClassificationMap(vector<VideoMetaData *> &videoMetaDataSet)
{
	map<int, string> *hashClassificationMap = new map<int, string>();
	for(int j=0; j<videoMetaDataSet.size(); j++)
	{
		int hash = videoMetaDataSet[j]->getClassification()->getHash();
		(*hashClassificationMap)[hash] = videoMetaDataSet[j]->getClassification()->getName();
	}

	return hashClassificationMap;
}

void PrintFileClassList(vector<string> &fileClassList)
{
	string resultTextFileStr = ExecutionPath() + "result.txt";
	ofstream out(resultTextFileStr);
	for(int k=0;k<fileClassList.size(); k++)
		out << fileClassList[k];
	out.close();
}

int main(int argc, char *argv[])
{
	string mode = "LEARNCLASSIFY";
	ArgumentString(argc, argv, 1, mode);

	Mat *vocabulary = NULL;
	FeatureDictionary *dictionary = NULL;
#ifdef MYKNN
	KNNClassifier *classifier = NULL;
#else
	CvStatModel *classifier = NULL;
#endif
	map<int, string> *hashClassificationMap = NULL;

	string videoTestFileDir = "";
	vector<string> videoTestFileNames;

	// ======================================================================================================
	
	if (strcmp(mode.c_str(),"FULL") == 0 || strcmp(mode.c_str(),"TRAIN") == 0 )
	{
		//string videoFileDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/training/";
		//string videoFileDir = "C:/Users/Christian/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/Assignment2/training";
		string videoFileDir = "./training/";
		ArgumentPath(argc, argv, 2, videoFileDir);
	
		vector<string> videoFileNames = getFilesPathWithinFolder(videoFileDir, true, ".avi");

		if (videoFileNames.size() == 0)
		{
			cout << "No training video files found.";
			return -1;
		}

		if (strcmp(mode.c_str(),"FULL") == 0)
		{
			//videoTestFileDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/test/";
			videoTestFileDir = "./test/";
			ArgumentPath(argc, argv, 3, videoTestFileDir);
			videoTestFileNames = getFilesPathWithinFolder(videoTestFileDir, true, ".avi");

			if (videoTestFileNames.size() == 0)
			{
				cout << "No test video files found.";
				return -1;
			}
		}

		vector<VideoContainer *> *videoContainers = new vector<VideoContainer *>();
		for(int i=0; i<videoFileNames.size(); i++)
		{
			vector<string> tokens = splitString(videoFileNames[i], "/");
			string classification = tokens[tokens.size()-1];

			VideoContainer *videoContainer = new VideoContainer(videoFileNames[i], classification);
			videoContainers->push_back(videoContainer);
		}

		// ------------------------------------------------
		// Learning phase
		// ------------------------------------------------
		// apply feature extraction on all collected frames

		Mat *vocabulary = BuildVocabulary(*videoContainers);

		// save vocabulary
		//string vocabularyDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/";
		string vocabularyDir = "./";
		ArgumentPath(argc, argv, 4, vocabularyDir);
		string vocabularyFileName = vocabularyDir + "Vocalulary.yml";

		FileStorage fs(vocabularyFileName, FileStorage::WRITE);
		fs << "vocabulary" << *vocabulary;
		fs.release();

		dictionary = new FeatureDictionary(vocabulary);

		// build feature representation of every video in the trainingset
		vector<VideoMetaData *> *videoMetaDataSet = BuildVideoMetaData(*videoContainers, *dictionary);

		string videoMetaDataSetDir = "./";
		ArgumentPath(argc, argv, 5, videoMetaDataSetDir);
		string videoMetaDataSetFileName = videoMetaDataSetDir + "VideoMetaDataSet.yml";

		FileStorage fs2(videoMetaDataSetFileName, FileStorage::WRITE);
		fs2 << "videoMetaDataSet" << "[" ;
		for(int k=0; k<videoMetaDataSet->size(); k++)
		{
			VideoMetaData *videMetaData = (*videoMetaDataSet)[k];
			fs2 << "{";
			fs2 << "histogram" << *(videMetaData->getHistogram());
			fs2 << "classification" << videMetaData->getClassification()->getName();
			fs2 << "videoFileName" << videMetaData->getVideoFileName();
			fs2 << "}";
		}
		fs2 << "]";
		fs2.release();
		
		hashClassificationMap = new map<int, string>();
		classifier = TrainClassifier(*videoMetaDataSet, *hashClassificationMap);

		string hashClassificationMapDir = "./";
		ArgumentPath(argc, argv, 6, hashClassificationMapDir);
		string hashClassificationMapFileName = hashClassificationMapDir + "HashClassificationMap.yml";

		FileStorage fs3(hashClassificationMapFileName, FileStorage::WRITE);
		fs3 << "hashClassificationMap" << "[" ;
		for(it_type iterator = hashClassificationMap->begin(); iterator != hashClassificationMap->end(); iterator++) {
			// iterator->first = key
			// iterator->second = value
			fs3 << "{";
			fs3 << "key" << iterator->first;
			fs3 << "value" << iterator->second;
			fs3 << "}";
		}
		fs3 << "]";
		fs3.release();

		//string classifierDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/";
		string classifierDir = "./";
		ArgumentPath(argc, argv, 7, classifierDir);
		string classifierFileName = classifierDir + "Classifier";
		classifier->save(classifierFileName.c_str());
	}

	// ======================================================================================================

	if (strcmp(mode.c_str(),"CLASSIFY") == 0)
	{
		// load vocabulary
		//string vocabularyDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/";
		string vocabularyDir = "./";
		ArgumentPath(argc, argv, 2, vocabularyDir);
		string vocabularyFileName = vocabularyDir + "Vocalulary.yml";

		Mat voc;
		FileStorage fs2(vocabularyFileName, FileStorage::READ);
		fs2["vocabulary"] >> voc;

		if (voc.empty())
		{
			cout << "No vocabulary found.";
			return -1;
		}

		vocabulary = new Mat(voc.size(), voc.type());
		voc.copyTo(*vocabulary);
		dictionary = new FeatureDictionary(vocabulary);

		string hashClassificationMapDir = "./";
		ArgumentPath(argc, argv, 3, hashClassificationMapDir);
		string hashClassificationMapFileName = hashClassificationMapDir + "HashClassificationMap.yml";

		FileStorage fs3(hashClassificationMapFileName, FileStorage::READ);
		FileNode hashClassificationMapNode = fs3["hashClassificationMap"];
		FileNodeIterator it = hashClassificationMapNode.begin(), it_end = hashClassificationMapNode.end();
		int idx = 0;
		
		hashClassificationMap = new map<int, string>();

		// iterate through a sequence using FileNodeIterator
		for( ; it != it_end; ++it, idx++ )
		{
			int hash;
			(*it)["key"] >> hash;
			string classificationStr = "";
			(*it)["value"] >> classificationStr;
			(*hashClassificationMap)[hash] = classificationStr;
		}		
		fs3.release();

		// load classifier
		//string classifierDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/";
		string classifierDir = "./";
		ArgumentPath(argc, argv, 4, classifierDir);
		string classifierFileName = classifierDir + "Classifier";
#ifdef TREE
		classifier = new CvRTrees();
#endif
#ifdef SVM
		classifier = new CvSVM();
#endif
#ifdef KNN
		classifier = new CvKNearest();
#endif
#ifdef MYKNN
		classifier = new KNNClassifier();
#endif
		classifier->load(classifierFileName.c_str());
		// TODO: verification missing

		// load test file dir
		//videoTestFileDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/test/";
		videoTestFileDir = "./test/";
		ArgumentPath(argc, argv, 5, videoTestFileDir);
		videoTestFileNames = getFilesPathWithinFolder(videoTestFileDir, true, ".avi");

		if (videoTestFileNames.size() == 0)
		{
			cout << "No test video files found.";
			return -1;
		}
	}
	
	// ======================================================================================================

	if (strcmp(mode.c_str(),"LEARNCLASSIFY") == 0)
	{
		// load vocabulary
		//string vocabularyDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/";
		string vocabularyDir = "./";
		ArgumentPath(argc, argv, 2, vocabularyDir);
		string vocabularyFileName = vocabularyDir + "Vocalulary.yml";

		Mat voc;
		FileStorage fs2(vocabularyFileName, FileStorage::READ);
		fs2["vocabulary"] >> voc;
		fs2.release();

		if (voc.empty())
		{
			cout << "No vocabulary found.";
			return -1;
		}

		vocabulary = new Mat(voc.size(), voc.type());
		voc.copyTo(*vocabulary);
		dictionary = new FeatureDictionary(vocabulary);

		// load VideoMetaDataSet
		string videoMetaDataSetDir = "./";
		ArgumentPath(argc, argv, 3, videoMetaDataSetDir);
		string videoMetaDataSetFileName = videoMetaDataSetDir + "VideoMetaDataSet.yml";

		FileStorage fs3(videoMetaDataSetFileName, FileStorage::READ);
		FileNode videoMetaDataSetNode = fs3["videoMetaDataSet"];
		FileNodeIterator it = videoMetaDataSetNode.begin(), it_end = videoMetaDataSetNode.end();
		int idx = 0;

		vector<VideoMetaData *> *videoMetaDataSet = new vector<VideoMetaData *>();

		// iterate through a sequence using FileNodeIterator
		for( ; it != it_end; ++it, idx++ )
		{
			Mat histogram;
			(*it)["histogram"] >> histogram;
			string classificationStr = "";
			(*it)["classification"] >> classificationStr;
			string videoFileNameStr = "";
			(*it)["videoFileName"] >> videoFileNameStr;

			// create videoMetaData
			Mat *histogramPtr = new Mat(histogram.size(), histogram.type());
			histogram.copyTo(*histogramPtr);
			VideoMetaData *videoMetaData = new VideoMetaData(histogramPtr, classificationStr, videoFileNameStr);

			videoMetaDataSet->push_back(videoMetaData);
		}
		fs3.release();
		
		// create classifier
		hashClassificationMap = new map<int, string>();
		classifier = TrainClassifier(*videoMetaDataSet, *hashClassificationMap);

		// load test file dir
		//videoTestFileDir = "C:/Users/braendlc/Documents/TU_Wien/2_Semester/VideoAnalysis/UE/UE02/test/";
		videoTestFileDir = "./test/";
		ArgumentPath(argc, argv, 4, videoTestFileDir);
		videoTestFileNames = getFilesPathWithinFolder(videoTestFileDir, true, ".avi");

		if (videoTestFileNames.size() == 0)
		{
			cout << "No test video files found.";
			return -1;
		}
	}
	
	// ======================================================================================================


	if (strcmp(mode.c_str(),"FULL") == 0 ||strcmp(mode.c_str(),"CLASSIFY") == 0 || strcmp(mode.c_str(),"LEARNCLASSIFY") == 0)
	{
		vector<VideoContainer *> *videoTestContainers = new vector<VideoContainer *>();
		for(int i=0; i<videoTestFileNames.size(); i++)
		{
			vector<string> tokens = splitString(videoTestFileNames[i], "/");
			string classification = tokens[tokens.size()-1];

			VideoContainer *videoContainer = new VideoContainer(videoTestFileNames[i], classification);
			videoTestContainers->push_back(videoContainer);
		}

		// , map<int, string> &hashClassificationMap, vector<string> &fileClassList
		vector<string> *fileClassList = new vector<string>();
		ConfusionMatrix *confusionMatrix = ClassifyVideos(*videoTestContainers, *dictionary, classifier, *hashClassificationMap, *fileClassList);

		// print out Precision and Sensitifity for all classes
		PrintConfusionMatrix(*confusionMatrix);

		PrintFileClassList(*fileClassList);
	}
	
	// ======================================================================================================
}

