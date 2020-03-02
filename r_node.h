#include <vector>
#include "global.h"
#include "smile.h"

namespace KaizenLibrary
{

	class SampleMixer;

	enum WExpression
	{
		WMEAN, WMIN, WMAX, MIXMINMAX, NONE
	};

	
	class RNode
	{
	public:
		
		static int current_id;  

		RNode();				
		RNode(int id);			
		~RNode();

		
		// @deprecated
		void Init3StateNode(std::string state1, std::string state2, std::string state3, double inVar = DEFAULT_VAR, double inMu = DEFAULT_MU);
	
		int smileHandler;

		WExpression wExpression;
	
		void init();

		void addParent(RNode* parentNode, double parentWeight);
	
		void setChild(RNode* childNode);

		void setState(int pos, std::string name);

		void setStateValue(int pos, double value);

		void setParent(RNode* parentNode);
	
		void setParentWeight(double parentWeight);
	
		void vertigo(int parentIndex = 0); 
	
		void removeParent(RNode* parentNode);

		void removeParentOnly(RNode* parentNode);

		void removeChild(RNode* childNode);

		void resetStateValues();

		void fillSmileFileCPTValues(DSL_sysCoordinates *coordinates);

		void setMean(double inMu);

		void setVariance(double inVar);

		void setTpn(std::vector<double> tpn);

		void setSamples(std::vector<double> samples);
	
		void setName(std::string name);

		void setEvidence(short stateEvidence);

		void setWMinValue(double wminValue = 1);

		void setWMaxValue(double wmaxValue = 1);

		bool isChild();
	
		std::vector<RNode*>* getParentsPtr();

		std::vector<RNode*>* getChildsPtr();

		std::vector<double>* getTpnPtr();

		std::vector<std::string>* getStateTitlesPtr();
	
		std::vector<double>* getStateValuesPtr();

		std::vector<double>* getStateIntervalsPtr();

		std::vector<double>* getSamplesPtr();

		std::vector<double>* getParentsWeightPtr();

		std::vector<RNode*> getParents();

		std::vector<int> getParentsId();
	
		int getNumberOfStates();

		double getVariance();

		int size();

		std::string getName();

		short getEvidence();

		int getId();

		void weigthedMean();

		double weigthedMin();

		double weigthedMax();

		void fillSmileCPT(DSL_sysCoordinates *coordinates, int parentIndex);

		void generateEvidence(double inMu);
	
		void print();

		void addParentId(int id);

		void addChildId(int id);

		int getParentId(int parentIndex);

		int getChildId(int childIndex);

		double getWMinValue();
	
		double getWMaxValue();

		void fillEmptyNPT(int parentIndex = 0);

		void numberOfStatesChanged();

		void setNptNeedUpdate(bool nptOutdated);

		bool manualNptNeedUpdate();

		void notifyChildOnDelete();

		RNode *withState(std::string name);

		RNode* withVariance(double inVar);

		RNode* withMean(double inMu);
	
		RNode* withName(std::string name);

		void setId(int id);

		void setSampleMixer(SampleMixer *mixer);

		void setSampleMixer();

		void wmean();

		void cleanStateValues();

		void notifyChanges();

		void setFunction(std::string function);

		void addNPTEntry(double entry);

		void generateNormalDistribution();

		void generatePseudoNormalDistribution();

		void truncateMe();

	private:
		std::vector<double> _samples;			
		std::vector<double> _tpn;				
		std::vector<double> _parentsWeight;		
		std::vector<RNode*> _parents;			
		std::vector<int> _parentsId;			
		std::vector<RNode*> _childNodes;		 
		std::vector<int> _childrenId;		    
		std::vector<std::string> _stateTitles;	
		std::vector<double> _stateValues;      
		std::vector<double> _stateIntervals;    
		std::string _name;						
		double _mu;								
		double _var;						
		int _nStates;							
		int _evidence;							
		int _id;								
		bool _manualNptneedUpdate;				
		SampleMixer *_sampleMixer;

	};
}