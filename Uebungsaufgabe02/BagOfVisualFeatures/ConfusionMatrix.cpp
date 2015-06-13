#include "stdafx.h"
#include "ConfusionMatrix.h"

//#include <vector>
//#include <string>
//#include <map>

#include "VideoMetaData.h"

using namespace std;

ConfusionMatrix::ConfusionMatrix(vector<Classification *> &classifications)
{
	this->m_classifications = new vector<Classification *>(classifications);

	this->m_classHashIndexMap = new map<int, int>();

	// assignments are done in matrix just in the row of classifications
	this->m_confusionMatrix = new int *[classifications.size()];
	for(int i=0; i < classifications.size(); i++)
	{
		this->m_confusionMatrix[i] = new int[classifications.size()];
		(*(this->m_classHashIndexMap))[classifications[i]->getHash()] = i;
	}

}

int ConfusionMatrix::indexOfClassHash(int classificationHash)
{
	return (*(this->m_classHashIndexMap))[classificationHash];
}

std::vector<Classification *> *ConfusionMatrix::getClassifications()
{
	return this->m_classifications;
}

void ConfusionMatrix::add(int groundTruthHash, int predictionHash)
{
	this->m_confusionMatrix[indexOfClassHash(groundTruthHash)][indexOfClassHash(predictionHash)] +=1;
}

float ConfusionMatrix::Precision(Classification &classification)
{
	int classIndex = indexOfClassHash(classification.getHash());

	float prec = TruePositives(classification)/(TruePositives(classification) + FalsePositives(classification));

	return prec;
}

float ConfusionMatrix::Sensitivity(Classification &classification)
{
	int classIndex = indexOfClassHash(classification.getHash());

	float sens = TruePositives(classification)/(TruePositives(classification) + FalseNegatives(classification));

	return sens;
}

float ConfusionMatrix::TruePositives(Classification &classification)
{
	int classIndex = indexOfClassHash(classification.getHash());

	return this->m_confusionMatrix[classIndex][classIndex];
}

float ConfusionMatrix::FalsePositives(Classification &classification)
{
	int classIndex = indexOfClassHash(classification.getHash());
	
	float fp = 0;
	for(int i=0; i < this->m_classifications->size(); i++)
		fp += this->m_confusionMatrix[i][classIndex]; // TODO: CHECK INDEXING!!!
	
	return fp;
}

float ConfusionMatrix::TrueNegatives(Classification &classification)
{
	int classIndex = indexOfClassHash(classification.getHash());

	float tn = 0;
	for(int i=0; i < this->m_classifications->size(); i++)
		tn += this->m_confusionMatrix[i][i];
	
	return tn;
}

float ConfusionMatrix::FalseNegatives(Classification &classification)
{
	int classIndex = indexOfClassHash(classification.getHash());

	float fn = 0;
	for(int i=0; i < this->m_classifications->size(); i++)
		fn += this->m_confusionMatrix[classIndex][i]; // TODO: CHECK INDEXING!!!
	
	return fn;
}


