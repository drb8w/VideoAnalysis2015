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
		for(int j=0; j < classifications.size(); j++)
			this->m_confusionMatrix[i][j] = 0;

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

string ConfusionMatrix::ToString()
{
	string str = "ConfusionMatrix: \n";

	vector<Classification *> classifications = *(this->getClassifications());

	for(int i=0; i<classifications.size(); i++)
	{
		str += classifications[i]->getName() + "\n";
		int * confusionRow = this->m_confusionMatrix[i];
		for(int j=0; j<classifications.size(); j++)
		{
			stringstream ss;
			ss << confusionRow[j];
			string iStr = ss.str();
			str += iStr + " ";	
		}
		str += "\n";
	}
	return str;
}


void PrintConfusionMatrix(ConfusionMatrix &confusionMatrix)
{
	cout << confusionMatrix.ToString();

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
		std::string sensStr(ss2.str());
		sensStr = "Sinsitivity: " + sensStr  + "\n";

		cout << sensStr;
	}
}

