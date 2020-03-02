#include "b_network_legacy.h"

namespace KaizenLibrary
{
	class BNetwork:
		public BNetworkLegacy,
		public DSL_network
	{

	public:
		static BNetwork* Instance();
		BNetwork();
		~BNetwork();
		RNode* createNode();
		RNode* createNode(int id);
		RNode* createNode(int id, int size);
		RNode* getNodeById(int id);
		std::vector<NodeToImprove> getMostInfluentialNodes(RNode* childNode);
		void deleteNode(RNode* node);
		void calculateTPN();
		void updateBeliefs();
		void createRelation(RNode* child, RNode* parent, double parentWeight);
		void removeRelation(RNode* child, RNode* parent);
		void setNodeEvidence(RNode* rNode, short evidence);
		void dataToRNode();
		void exportXDSLFile(std::string filename);
		void createRelation(int childId, int parentId, double parentWeight);
		void setNodeName(RNode *node, std::string name);
		void syncNode(RNode* rNode);
	private:
		static BNetwork* _instance;
	};
}