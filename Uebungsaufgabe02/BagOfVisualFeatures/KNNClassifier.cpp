#include "stdafx.h"
#include "KNNClassifier.h"
#include <opencv2\opencv.hpp>

#include "StringHelpers.h"
#include "FrameHelpers.h"
#include "NumberHelpers.h"

using namespace cv;
using namespace std;

KNNClassifier::KNNClassifier()
{

}

KNNClassifier::KNNClassifier(cv::Mat &trainingData, cv::Mat &trainingClassification, int K)
{
	train(trainingData, trainingClassification, K);
}

bool KNNClassifier::train(cv::Mat &trainingData, cv::Mat &trainingClassification, int K)
{
	// no training done but data saved
	this->m_trainingData = new Mat(trainingData.size(), trainingData.type());
	trainingData.copyTo(*(this->m_trainingData));
	this->m_trainingClassification = new Mat(trainingClassification.size(), trainingClassification.type());
	trainingClassification.copyTo(*(this->m_trainingClassification));

	return true;
}

typedef struct KeyValue 
{
	int key;
	float value;
} KeyValue;

bool keySort (KeyValue i,KeyValue j) { return (i.key<j.key); }
bool valueSort (KeyValue i,KeyValue j) { return (i.value<j.value); }

//classLabelHit

float KNNClassifier::find_nearest(cv::Mat &testData, int K)
{
	// stupid search through all trainingDatas
	vector<KeyValue> classLabelDistanceVector;

	//// TEST
	//string trainingDataFileName = ExecutionPath() + "TrainingData.yml";
	//FileStorage fs(trainingDataFileName, FileStorage::WRITE);
	//fs << "trainingData" << *(this->m_trainingData);
	//fs.release();
	//string trainingDataRowsFileName = ExecutionPath() + "TrainingDataRows.yml";
	//FileStorage fs2(trainingDataRowsFileName, FileStorage::WRITE);
	//fs2 << "trainingData" << "[" ;

	for(int row=0; row<this->m_trainingData->rows; row++)
	{
		// determine class label
		int classLabel = this->m_trainingClassification->at<int>(row,0);

		// determine train data
		//Mat trainingDataRow(1,this->m_trainingData->cols, this->m_trainingData->type());
		//cleanMatF32(trainingDataRow);
		//trainingDataRow.copyTo((*(this->m_trainingData))(Rect(0, row, this->m_trainingData->cols, 1)));

		Mat trainingDataRow = (*this->m_trainingData)(Rect(0, row, this->m_trainingData->cols, 1));

		//// TEST
		//fs2 << "trainingDataRow" << trainingDataRow;
		
		// calculate distance
		double sum = 0;
		for(int d=0; d<trainingDataRow.cols; d++)
			sum += abs(trainingDataRow.at<float>(0,d) - testData.at<float>(0,d));
		float distance = sqrt(sum);

		// add both in vector to sort
		KeyValue classLabelDistance;
		classLabelDistance.key = classLabel;
		classLabelDistance.value = distance;
		classLabelDistanceVector.push_back(classLabelDistance);
	}
	//fs2 << "]" ;
	//fs2.release();

	// sort map according to keys
	std::sort(classLabelDistanceVector.begin(), classLabelDistanceVector.end(), valueSort);

	// get the first K ones and do a majority vote
	map<int, int> classLabelHitMap;
	int K_ = min(K,classLabelDistanceVector.size());
	for(int k=0; k< K_; k++)
	{
		if ( classLabelHitMap.find(classLabelDistanceVector[k].key) == classLabelHitMap.end() ) 
		{
			// not found
			classLabelHitMap[classLabelDistanceVector[k].key] = 1;
		} 
		else 
		{
			// found
			classLabelHitMap[classLabelDistanceVector[k].key] += 1;
		}
	}

	// return label with the biggest hitcount

	int maxHit = 0;
	int foundLabel = -1;
	for(map<int, int>::iterator iter = classLabelHitMap.begin(); iter != classLabelHitMap.end(); ++iter)
	{
		if (maxHit < iter->second)
		{
			foundLabel = iter->first;
			maxHit = iter->second;
		}
	}

	return foundLabel;
}

bool KNNClassifier::save(string classifierFileName)
{
	FileStorage fs(classifierFileName, FileStorage::WRITE);

	fs << "trainingData" << *(this->m_trainingData);
	fs << "trainingClassification" << *(this->m_trainingClassification); 
	fs.release();

	return true;
}

bool KNNClassifier::load(string classifierFileName)
{
	FileStorage fs(classifierFileName, FileStorage::READ);
	Mat trainingData;
	fs["trainingData"] >> trainingData;
	this->m_trainingData = new Mat(trainingData.size(), trainingData.type());

	Mat trainingClassification;
	fs["trainingClassification"] >> trainingClassification;
	this->m_trainingClassification = new Mat(trainingClassification.size(), trainingClassification.type());

	fs.release();

	return true;
}
