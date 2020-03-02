#include "base_sample_service.h"
#include "base_sample_container.h"
#include "truncated_normal.hpp"
#include <vector>
#include "global.h"
#include <iostream> // For debug purposes

using namespace std;

BaseSampleService* BaseSampleService::_instance = 0;

BaseSampleService* BaseSampleService::Instance()
{
	if(_instance == 0)
	{
		_instance = new BaseSampleService;
	}
	return _instance;
}

BaseSampleService::BaseSampleService()
{
	generateBaseSamples();
}

vector<double> BaseSampleService::getContainerSamplesPTR(int stateQuantity, int evidence)
{
	for(BaseSampleContainer container : baseSampleContainerRepository)
	{
		
		if(container.getStateQuantity() == stateQuantity && container.Get_evidence() == evidence)
		{
			vector<double> t = *container.getSamplesPTR();
			return t;
			
		}
	}
	vector<double> x;
	return x;				
}

void BaseSampleService::generateBaseSamples()
{
	for(int i = 0; i < MAX_NUMBER_OF_STATES; i++)
	{
		if(i < 2)
		{
			BaseSampleContainer b2(2, i);
			baseSampleContainerRepository.push_back(b2);
		}

		if(i < 3)
		{
			BaseSampleContainer b3(3, i);			
			baseSampleContainerRepository.push_back(b3);
		}

		if(i < 4)
		{
			BaseSampleContainer b4(4, i);
			baseSampleContainerRepository.push_back(b4);
		}

		BaseSampleContainer b5(5, i);
		baseSampleContainerRepository.push_back(b5);
	}
		
}

// Debug
int BaseSampleService::getSize()
{
	return baseSampleContainerRepository.size();
}

// Debug
void BaseSampleService::print()
{
	for(BaseSampleContainer b : baseSampleContainerRepository)
	{
		for(int i = 0; i < 10; i++)
		{
			if(b.getStateQuantity() == 5){
				cout << b.getSamplesPTR()->at(i) << endl;
			}
		}
	}
	
}