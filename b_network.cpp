#include "b_network.h"
#include "smile.h"
#include "r_node.h"
#include "iostream"

using namespace std;

namespace KaizenLibrary {
	BNetwork* BNetwork::_instance = 0;

	BNetwork* BNetwork::Instance() 
	{
		if(_instance == 0)
		{
			_instance = new BNetwork();
		}
		return _instance;
	}

	BNetwork::BNetwork()
	{
		_mainScenario = AddCase("Case_1");
	}

	BNetwork::~BNetwork()
	{
	}

	RNode* BNetwork::createNode() 
	{
		RNode* theNode = new RNode();	

		theNode->withName("New Node")
			->withState("state 1")
			->withState("state 2")
			->withState("state 3")
			->init();
		DSL_stringArray states;
		int nodeHandler =  AddNode(DSL_CPT, theNode->getName().c_str());
		theNode->smileHandler = nodeHandler;

		for (int i = 0; i < theNode->getNumberOfStates(); i++)
			states.Add(theNode->getStateTitlesPtr()->at(i).c_str());

		GetNode(nodeHandler)->Definition()->SetNumberOfOutcomes(states);
		
		_nodes.push_back(theNode);
		return theNode;
	}

	RNode *BNetwork::createNode(int id, int size) 
	{
		RNode* theNode = new RNode();	
		
		theNode->setName("New Node");
		for (int i = 0; i < size; i++) {
			theNode->setState(i, "state_" + (i+1));
		} 
		theNode->init();
		
		DSL_stringArray states;
		int nodeHandler =  AddNode(DSL_CPT, theNode->getName().c_str());
		theNode->smileHandler = nodeHandler;

		for (int i = 0; i < theNode->getNumberOfStates(); i++)
			states.Add(theNode->getStateTitlesPtr()->at(i).c_str());

		GetNode(nodeHandler)->Definition()->SetNumberOfOutcomes(states);
		
		theNode->setId(id);
		_nodes.push_back(theNode);
		
		return theNode;
	}

	RNode *BNetwork::createNode(int id)
	{
		RNode* theNode = createNode();
		theNode->setId(id);
		
		return theNode;
	}

	void BNetwork::syncNode(RNode* theNode) 
	{
		int smileHandler =  theNode->smileHandler;
		int smileOutcomes = GetNode(smileHandler)->Definition()->GetNumberOfOutcomes(); 
		int rNodeOutcomes = theNode->getNumberOfStates(); 
		
		DSL_stringArray someNames; 
		DSL_doubleArray theProbs;
	
		GetNode(smileHandler)->Info().Header().SetName(theNode->getName().c_str());
	
		for (int i = 0; i < rNodeOutcomes; i++) {
			someNames.Add(theNode->getStateTitlesPtr()->at(i).c_str());
		}
	
		for (int i = rNodeOutcomes; i < smileOutcomes; i++) {
			GetNode(smileHandler)->Definition()->RemoveOutcome(i);
		}

		for (int i = smileOutcomes; i < rNodeOutcomes; i++) {
			GetNode(smileHandler)->Definition()->AddOutcome(theNode->getStateTitlesPtr()->at(i).c_str());
		}
	
		theProbs.SetSize(rNodeOutcomes);
		for (int i = 0; i < rNodeOutcomes; i++) {
			theProbs[i] = theNode->getStateValuesPtr()->at(i);
		}
	
		GetNode(smileHandler)->Definition()->SetDefinition(theProbs);
		
		GetNode(smileHandler)->Definition()->RenameOutcomes(someNames);
	}

	void BNetwork::deleteNode(RNode* node) {
		DeleteNode(node->smileHandler);		// DSL_net
		deleteRNodeLegacy(node);			// BNetworkLegacy
	}

	void BNetwork::calculateTPN()
	{
		if (GetNumberOfCases() == 0)
			_mainScenario = AddCase("Case_1");
		
		for (int i = 0; i < _nodes.size(); i++) {
			if (_nodes.at(i)->isChild()) {
				DSL_sysCoordinates theCoordinates(*GetNode(_nodes.at(i)->smileHandler)->Definition());
		
				if (_nodes.at(i)->wExpression == WExpression::NONE) {
					 _nodes.at(i)->fillSmileFileCPTValues(&theCoordinates);
				} else {
					_nodes.at(i)->getTpnPtr()->clear();
					_nodes.at(i)->fillSmileCPT(&theCoordinates, 0);
				}
			} 
			
			if (_nodes.at(i)->getEvidence() != -1) {
				_mainScenario->AddEvidence(_nodes.at(i)->smileHandler, _nodes.at(i)->getEvidence()); 
			} else {
				_mainScenario->RemoveEvidenceByHandle(_nodes.at(i)->smileHandler);
			}
		}
		_mainScenario->CaseToNetwork();
		UpdateBeliefs();
		dataToRNode();
	}

	void BNetwork::updateBeliefs()
	{
		if (GetNumberOfCases() == 0)
			_mainScenario = AddCase("Case_1");
	
		for (int i = 0; i < _nodes.size(); i++) {
			if (!_nodes.at(i)->isChild() && _nodes.at(i)->getEvidence() != -1) {
				_mainScenario->SetEvidenceByHandle(_nodes.at(i)->smileHandler, _nodes.at(i)->getEvidence());
			}
		}

		_mainScenario->CaseToNetwork();
		UpdateBeliefs();
		dataToRNode();
	}

	void BNetwork::createRelation(RNode* child, RNode* parent, double parentWeight)
	{
		child->addParent(parent, parentWeight);				// RNode
		AddArc(parent->smileHandler, child->smileHandler);	// DSL_net 
	}

	RNode* BNetwork::getNodeById(int id)
	{
		for (int i = 0; i < _nodes.size(); ++i) {
			if (_nodes.at(i)->getId() == id) {
				return _nodes.at(i);
			}
		}
		return NULL;
	}

	void BNetwork::createRelation(int childId, int parentId, double parentWeight)
	{
		RNode* child = getNodeById(childId);
		RNode* parent = getNodeById(parentId);
		child->addParent(parent, parentWeight);				// RNode
		AddArc(parent->smileHandler, child->smileHandler);	// DSL_net 
	}

	void BNetwork::removeRelation(RNode* child, RNode* parent)
	{
		child->removeParent(parent);						  // RNode	
		RemoveArc(parent->smileHandler, child->smileHandler); // DSL_net
	}

	void BNetwork::setNodeEvidence(RNode* rNode, short evidence)
	{
		rNode->setEvidence(evidence);
		_mainScenario->SetEvidenceByHandle(rNode->smileHandler, evidence);
	}

	void BNetwork::exportXDSLFile(string filename) 
	{
		WriteFile(filename.c_str());
	}

	void BNetwork::dataToRNode() 
	{
		for (int i = 0; i < _nodes.size(); i++) {
				DSL_sysCoordinates theCoord(*GetNode(_nodes.at(i)->smileHandler)->Value());
				theCoord.GoFirst();
				_nodes.at(i)->getStateValuesPtr()->at(0) = theCoord.UncheckedValue();
				if (_nodes.at(i)->getNumberOfStates() > 1) {
					for (int j = 1; j < _nodes.at(i)->getNumberOfStates(); ++j) {
						theCoord.Next();
						_nodes.at(i)->getStateValuesPtr()->at(j) = theCoord.UncheckedValue();
					}
				}
		}
	}

	vector<NodeToImprove> BNetwork::getMostInfluentialNodes(RNode* childNode)
	{
		bool hasNoEvidence = false;

		if (GetNumberOfNodes() == 0)
			throw "You have to update nodes first";

		vector<NodeToImprove> result;
	
		vector<RNode *> nodeWithEvidence;
		nodeWithEvidence.reserve(50); 
	
		nodesThatIWant(&nodeWithEvidence, childNode);

		DSL_sysCoordinates theCoord(*GetNode(childNode->smileHandler)->Value());

		for(int i = 0; i < nodeWithEvidence.size(); i++) {
			if (nodeWithEvidence.at(i)->smileHandler < 0)
				throw "You need to perform calculation before attempting this.";

			if (nodeWithEvidence.at(i)->getEvidence() == -1) {
				_mainScenario->SetEvidenceByHandle(nodeWithEvidence.at(i)->smileHandler, nodeWithEvidence.at(i)->getNumberOfStates()-1);
				hasNoEvidence = true;
			} else {
				_mainScenario->SetEvidenceByHandle(nodeWithEvidence.at(i)->smileHandler, nodeWithEvidence.at(i)->getEvidence()+1);
				hasNoEvidence = false;
			}

			_mainScenario->CaseToNetwork();
			UpdateBeliefs();

			theCoord.GoFirst();
			NodeToImprove nodeToImprove;
			for (int i = 0; i < childNode->getNumberOfStates(); i++) {
				nodeToImprove.outcome.push_back(theCoord.UncheckedValue());
				theCoord.Next();
			}

			nodeToImprove.nodeId = nodeWithEvidence.at(i)->getId();
			nodeToImprove.node_name = nodeWithEvidence.at(i)->getName();
			if (nodeWithEvidence.at(i)->getEvidence() == -1) {
				nodeToImprove.evidence = nodeWithEvidence.at(i)->getNumberOfStates()-1;
				nodeToImprove.state_0 = "No evidence";
				nodeToImprove.state_1 = nodeWithEvidence.at(i)->getStateTitlesPtr()->at(nodeWithEvidence.at(i)->getNumberOfStates()-1);  // Get last item maybe... see this later TODO
			} else {
				nodeToImprove.evidence = nodeWithEvidence.at(i)->getEvidence()+1;
				nodeToImprove.state_0 = nodeWithEvidence.at(i)->getStateTitlesPtr()->at(nodeWithEvidence.at(i)->getEvidence());
				nodeToImprove.state_1 = nodeWithEvidence.at(i)->getStateTitlesPtr()->at(nodeWithEvidence.at(i)->getEvidence()+1);
			}
			result.push_back(nodeToImprove);
		
			if (hasNoEvidence)
				_mainScenario->RemoveEvidenceByHandle(nodeWithEvidence.at(i)->smileHandler);
			else
				_mainScenario->SetEvidenceByHandle(nodeWithEvidence.at(i)->smileHandler, nodeWithEvidence.at(i)->getEvidence());
		}

		std::sort(result.begin(), result.end());
		return result;
	}

	void BNetwork::setNodeName(RNode *node, std::string name)
	{
		node->setName(name);
		GetNode(node->smileHandler)->Info().Header().SetName(node->getName().c_str());
	}

}