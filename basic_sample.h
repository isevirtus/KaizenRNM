#pragma once
#include <vector>

class BasicSample {
public:
	BasicSample();
	virtual ~BasicSample();

	std::vector<double> samples;
	int states;
	double mean;

	void setMean(double mean);
	void setStates(int states);
	void setSample(std::vector<double> samples);
};

