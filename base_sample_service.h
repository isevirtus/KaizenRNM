#pragma once
#include "base_sample_container.h"

class BaseSampleService
{
public:
	static BaseSampleService* Instance();
	std::vector<double> getContainerSamplesPTR(int stateQuantity, int evidence);		
	int getSize();				// Debug purpose
	void print();				// Debug purpose

protected:
	BaseSampleService();

private:
	static BaseSampleService* _instance;
	std::vector<BaseSampleContainer> baseSampleContainerRepository;
	void generateBaseSamples();
};