#include "basic_sample.h"

BasicSample::BasicSample()
{
	mean = 0.0;
	states = 0;
}

BasicSample::~BasicSample()
{
}

void BasicSample::setMean(double mean)
{
	this->mean = mean;
}

void BasicSample::setSample(std::vector<double> samples)
{
	this->samples = samples;
}

void BasicSample::setStates(int states)
{
	this->states = states;
}