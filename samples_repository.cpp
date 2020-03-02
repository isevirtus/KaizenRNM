#include "samples_repository.h"
#include <vector>
#include "basic_sample.h"
#include "iostream"

std::vector<BasicSample> SamplesRepository::basicSampleContainer;

SamplesRepository::SamplesRepository()
{
}

SamplesRepository::~SamplesRepository()
{
}

void SamplesRepository::addBasicSample(double mean, int states, std::vector<double> samples)
{
	BasicSample bs;
	bs.setMean(mean);
	bs.setStates(states);
	bs.setSample(samples);
	SamplesRepository::basicSampleContainer.push_back(bs);
}

std::vector<double> SamplesRepository::reuseBasicSample(double mean, int states)
{
	int z = SamplesRepository::basicSampleContainer.size();
	for (int i = 0; i < z; i++) {
		if (SamplesRepository::basicSampleContainer.at(i).mean == mean) {
			if (SamplesRepository::basicSampleContainer.at(i).states == states) {
				return SamplesRepository::basicSampleContainer.at(i).samples;
			}
		}
	}
	std::vector<double> x;
	return x;
}