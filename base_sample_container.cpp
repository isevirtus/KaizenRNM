#include "base_sample_container.h"
#include "truncated_normal.hpp"
#include "global.h"

using namespace std;

BaseSampleContainer::BaseSampleContainer(int stateQuantity, int evidence)
{
	_stateQuantity = stateQuantity;
	_evidence = evidence;
	_samples.reserve(SAMPLESIZE);
	_samples.resize(SAMPLESIZE);
	setUpBounds();
	generateRandomSamples();
}

int BaseSampleContainer::getStateQuantity()
{
	return _stateQuantity;
}

int BaseSampleContainer::Get_evidence()
{
	return _evidence;
}

vector<double>* BaseSampleContainer::getSamplesPTR()
{
	return &_samples;
}

void BaseSampleContainer::generateRandomSamples()
{
	int seed = 123456789;
	double aux;						
	
	double mean = (_lowerBound + _upperBound) / 2;

	for (int i = 0; i < SAMPLESIZE; i++) {
		aux = truncated_normal_ab_sample(mean, DEFAULT_BASE_VARIANCE, _lowerBound, _upperBound, seed);
		_samples.at(i) = aux;
	}
}

void BaseSampleContainer::setUpBounds()
{
	if (_evidence == 0)	{
		_lowerBound = 0;
		_upperBound = 1.0/_stateQuantity;
	} else if (_evidence == 1 && _stateQuantity >= 2) {
		_lowerBound = 1.0 / _stateQuantity;
		_upperBound = 2.0 / _stateQuantity;
	} else if (_evidence == 2 && _stateQuantity >= 3) {
		_lowerBound = 2.0 / _stateQuantity;
		_upperBound = 3.0 / _stateQuantity;
	} else if (_evidence == 3 && _stateQuantity >= 4) {
		_lowerBound = 3.0 / _stateQuantity;
		_upperBound = 4.0 / _stateQuantity;
	} else if (_evidence == 4 && _stateQuantity == 5) {
		_lowerBound = 4.0 / _stateQuantity;
		_upperBound = 5.0 / _stateQuantity;
	}
		
}