#pragma once
#include <vector>

class BasicSample;

class SamplesRepository {

public:
	SamplesRepository();
	virtual ~SamplesRepository();

	// Container for basic samples
	static std::vector<BasicSample> basicSampleContainer;
	
	// Add new sample to the basic samples container (for reuse)
	static void addBasicSample(double mean, int states, std::vector<double> samples);

	// Returns previously used samples
	// Returns an empty vector in case no matching sample has been used
	static std::vector<double> reuseBasicSample(double mean, int states);
	
};