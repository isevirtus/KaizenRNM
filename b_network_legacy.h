#include "smile.h"

namespace KaizenLibrary
{
	class RNode;

	struct NodeToImprove
	{
		int nodeId; 
		std::string node_name;
		std::string state_0;
		std::string state_1;
		int evidence;
		std::vector<double> outcome;

		friend bool operator<(NodeToImprove a, NodeToImprove b)
		{
			int last_state = a.outcome.size() - 1;
			return a.outcome.at(last_state) > b.outcome.at(last_state);
		}
	};

	class BNetworkLegacy
	{
	public:
		static BNetworkLegacy* getInstance();
		BNetworkLegacy();
		~BNetworkLegacy();
		RNode *createNodeLegacy();
		RNode *createNodeLegacy(int id);
		void setNetworkName(std::string networkName);
		std::string getNetworkName();
		void fillSmileCPT(DSL_sysCoordinates *coordinates, RNode* rNode);
		void deleteRNodeLegacy(RNode* rNode);
		RNode *getRNodeById(int id);
		void clear();
		std::vector<NodeToImprove> getMostInfluentialNodesLegacy(RNode* childNode);
		void nodesThatIWant(std::vector<RNode *> *nodeList, RNode* childNode);
		void calculateTPNLegacy();
		void updateInference();
		void exportXDSLFileLegacy(std::string filename);
		void dataToRNodeLegacy();
		bool nodeExists(int id);
		std::vector<RNode*> *getNodes();
	protected:
		static BNetworkLegacy* _instance;
		std::vector<RNode*> _nodes;
		std::string _name;
		DSL_network _theNet;
		DSL_simpleCase *_mainScenario;
	};
}