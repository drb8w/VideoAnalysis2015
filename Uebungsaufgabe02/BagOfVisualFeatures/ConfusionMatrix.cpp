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

void ConfusionMatrix::add(int groundTruthHash, int predictionHash)
{
	this->m_confusionMatrix[indexOfClassHash(groundTruthHash)][indexOfClassHash(predictionHash)] +=1;
}