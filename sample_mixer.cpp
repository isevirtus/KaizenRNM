#include "sample_mixer.h"
#include "r_node.h"
#include "truncated_normal.hpp"
#include <iostream>

KaizenLibrary::SampleMixer::SampleMixer()
{
}

KaizenLibrary::SampleMixer::~SampleMixer()
{
}

KaizenLibrary::WMean::WMean()
{
}

KaizenLibrary::WMean::~WMean()
{
}

double KaizenLibrary::WMean::mix(RNode *rNode)
{
	double div = 0;
	double sum = 0;
	double mean = 0;
	double aux = 0;
	int seed = 123456789;

	rNode->cleanStateValues();
	
	for (int i = 0; i < rNode->getParentsPtr()->size(); i++) {
		div += rNode->getParentsWeightPtr()->at(i);
	}

	int counter = 0;
	
	for (int i = 0; i < SAMPLESIZE; i+=EQUIDISTANT_PASS) {
		for (int j = 0; j < rNode->getParentsPtr()->size(); j++) {
			sum += rNode->getParentsPtr()->at(j)->getSamplesPtr()->at(i) * rNode->getParentsWeightPtr()->at(j);
		}
		
		mean = sum / div;
		
		sum = 0; 

		aux = truncated_normal_ab_sample(mean, rNode->getVariance(), 0, 1, seed);

		rNode->getSamplesPtr()->at(counter) = aux;
		for (int z = 0; z < rNode->getNumberOfStates(); z++) {
			if (rNode->getSamplesPtr()->at(counter) < rNode->getStateIntervalsPtr()->at(z)) {
				rNode->getStateValuesPtr()->at(z) += 1;
				break;
			}
		}

		counter++;
	}

	for (int w = 0; w < rNode->getNumberOfStates(); w++) {
		rNode->getStateValuesPtr()->at(w) = (double) rNode->getStateValuesPtr()->at(w) / EQUIDISTANT_SAMPLESIZE;
	}

	for (int w = 0; w < rNode->getNumberOfStates(); w++) {
		rNode->getTpnPtr()->push_back(rNode->getStateValuesPtr()->at(w));
	}

	return aux;
}

KaizenLibrary::WMin::WMin()
{
}

KaizenLibrary::WMin::~WMin()
{
}

double KaizenLibrary::WMin::mix(RNode *rNode)
{
	double highestValue;
	double div = 0;
	double sum = 0;
	double aux = 0;
	double min = 123456789;
	double a = 0;
	double currentMin = 0;
	int seed = 123456789;

	rNode->cleanStateValues();
	int counter = 0;
	for (int i = 0; i < SAMPLESIZE; i+=EQUIDISTANT_PASS) {
		for (int j = 0; j < rNode->getParentsPtr()->size(); j++) {
			div = rNode->getParentsWeightPtr()->at(j) + rNode->getParentsPtr()->size() - 1;
			a = rNode->getParentsPtr()->at(j)->getSamplesPtr()->at(i) * rNode->getParentsWeightPtr()->at(j); 
			for (int k = 0; k < rNode->getParentsPtr()->size(); k++) {
				if (k != j) {
					sum += rNode->getParentsPtr()->at(k)->getSamplesPtr()->at(i);
				}
			}
			
			currentMin = (a + sum) / div;
			if (currentMin < min) {
				min = currentMin;
			}
			sum = 0; 			
		}
		
		aux = truncated_normal_ab_sample(min, rNode->getVariance(), 0, 1, seed);

		
		rNode->getSamplesPtr()->at(counter) = aux;

		for (int z = 0; z < rNode->getNumberOfStates(); z++) {
			if (rNode->getSamplesPtr()->at(counter) < rNode->getStateIntervalsPtr()->at(z)) {
				rNode->getStateValuesPtr()->at(z) += 1;
				break;
			}
		}

		counter++;
	}

	for (int w = 0; w < rNode->getNumberOfStates(); w++) {
		rNode->getStateValuesPtr()->at(w) = (double) rNode->getStateValuesPtr()->at(w) / EQUIDISTANT_SAMPLESIZE;
	}
	

	for (int w = 0; w < rNode->getNumberOfStates(); w++) {
		rNode->getTpnPtr()->push_back(rNode->getStateValuesPtr()->at(w));
	}

	return aux;
}

KaizenLibrary::WMax::WMax()
{
}

KaizenLibrary::WMax::~WMax()
{
}

double KaizenLibrary::WMax::mix(RNode *rNode)
{
	double highestValue;
	double div = 0;
	double sum = 0;
	double aux = 0;
	double max = 0;
	double a = 0;
	double currentMax = 0;	
	int seed = 123456789;

	rNode->cleanStateValues();
	int counter = 0;
	for (int i = 0; i < SAMPLESIZE; i+=EQUIDISTANT_PASS) {
		for (int j = 0; j < rNode->getParentsPtr()->size(); j++) {
			div = rNode->getParentsWeightPtr()->at(j) + rNode->getParentsPtr()->size() - 1;
			a = rNode->getParentsPtr()->at(j)->getSamplesPtr()->at(i) * rNode->getParentsWeightPtr()->at(j); 
			for (int k = 0; k < rNode->getParentsPtr()->size(); k++) {
				if (k != j) {
					sum += rNode->getParentsPtr()->at(k)->getSamplesPtr()->at(i);
				}
			}
			
			currentMax = (a + sum) / div;
			if (currentMax > max) {
				max = currentMax;
			}
			sum = 0; 			
		}
		
		aux = truncated_normal_ab_sample(max, rNode->getVariance(), 0, 1, seed);

		
		rNode->getSamplesPtr()->at(counter) = aux;

		for (int z = 0; z < rNode->getNumberOfStates(); z++) {
			if (rNode->getSamplesPtr()->at(counter) < rNode->getStateIntervalsPtr()->at(z)) {
				rNode->getStateValuesPtr()->at(z) += 1;
				break;
			}
		}

		counter++;
	}
	
	for (int w = 0; w < rNode->getNumberOfStates(); w++) {
		rNode->getStateValuesPtr()->at(w) = (double) rNode->getStateValuesPtr()->at(w) / EQUIDISTANT_SAMPLESIZE;
	}
	
	for (int w = 0; w < rNode->getNumberOfStates(); w++) {
		rNode->getTpnPtr()->push_back(rNode->getStateValuesPtr()->at(w));
	}

	return aux;
}

KaizenLibrary::EmptyMixer::EmptyMixer()
{
}

KaizenLibrary::EmptyMixer::~EmptyMixer()
{
}

double KaizenLibrary::EmptyMixer::mix(RNode *rNode) 
{
	return 0;
}