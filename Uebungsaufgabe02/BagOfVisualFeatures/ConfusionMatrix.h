#ifndef CONFUSIONMATRIX
#define CONFUSIONMATRIX

#include "stdafx.h"
#include <vector>
#include <string>
#include <map>
#include "Classification.h"

class ConfusionMatrix
{
private:
	std::vector<Classification *> *m_classifications;
	std::map<int, int> *m_classHashIndexMap;
	int **m_confusionMatrix;

	int indexOfClassHash(int classificationHash);

public:
	ConfusionMatrix(std::vector<Classification *> &classifications);

	std::vector<Classification *> *getClassifications();

	void add(int groundTruthHash, int predictionHash);

	float Precision(Classification &classification);
	float Sensitivity(Classification &classification);

	float TruePositives(Classification &classification);
	float FalsePositives(Classification &classification);
	float TrueNegatives(Classification &classification);
	float FalseNegatives(Classification &classification);

	std::string ToString();

};

void PrintConfusionMatrix(ConfusionMatrix &confusionMatrix);

#endif