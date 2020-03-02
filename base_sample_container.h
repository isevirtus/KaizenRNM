#pragma once
#include <vector>

class BaseSampleContainer
{
public:
	BaseSampleContainer(int stateQuantity, int evidence);
	std::vector<double>* getSamplesPTR();
	int getStateQuantity();
	int Get_evidence();
private:
	std::vector<double> _samples;
	int _stateQuantity;			
	int _evidence;				
	double _lowerBound;
	double _upperBound;
	void setUpBounds();			
	void generateRandomSamples();
};