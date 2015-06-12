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

	void add(int groundTruthHash, int predictionHash);

};

#endif