#include "b_network_legacy.h"
#include <iostream>
#include <Windows.h>
#include <atlstr.h>
#include <vector>
#include "r_node.h"

using namespace std;

namespace KaizenLibrary
{
	BNetworkLegacy* BNetworkLegacy::_instance = 0;

	BNetworkLegacy* BNetworkLegacy::getInstance()
	{
		if(_instance == 0)
			_instance = new BNetworkLegacy();
		return _instance;
	}

	BNetworkLegacy::BNetworkLegacy()
	{
		DSL_network	theNet;
	}

	BNetworkLegacy::~BNetworkLegacy()
	{
	}

	void BNetworkLegacy::setNetworkName(string name)
	{
		_name = name + ".xdsl";
	}

	string BNetworkLegacy::getNetworkName()
	{
		return _name;
	}

	void BNetworkLegacy::fillSmileCPT(DSL_sysCoordinates *theCoordinates, RNode* node)
	{
		for(int i = 0; i < node->getTpnPtr()->size(); i++) {
			theCoordinates->UncheckedValue() = node->getTpnPtr()->at(i);
			theCoordinates->Next();
		}
	}

	void BNetworkLegacy::deleteRNodeLegacy(RNode* node)
	{
		node->notifyChildOnDelete();

		for (int i = 0; i < _nodes.size(); i++) {
			if(_nodes.at(i) == node)
				_nodes.erase(_nodes.begin()+i);	
		}
		delete node;
	}

	RNode *BNetworkLegacy::getRNodeById(int id)
	{
		for (int i = 0; i < _nodes.size(); ++i) {
			if (_nodes.at(i)->getId() == id)
				return _nodes.at(i);
		}
	}

	void BNetworkLegacy::clear()
	{
		RNode *toDelete;
		while (_nodes.size() > 0) {
			toDelete = _nodes.at(0);
			_nodes.erase(_nodes.begin());
			delete toDelete;
		}
		RNode::current_id = 0;
	}

	void BNetworkLegacy::nodesThatIWant(vector<RNode *> *nodeList, RNode* childNode)
	{
		for (int i = 0; i < childNode->getParentsPtr()->size(); i++) {
			if (childNode->getParentsPtr()->at(i)->isChild()) {
				nodesThatIWant(nodeList, childNode->getParentsPtr()->at(i));
			} else if (childNode->getParentsPtr()->at(i)->getEvidence() != (childNode->getParentsPtr()->at(i)->getNumberOfStates() - 1)) {
				nodeList->push_back(childNode->getParentsPtr()->at(i));
			}
		}
	}

	vector<NodeToImprove> BNetworkLegacy::getMostInfluentialNodesLegacy(RNode* childNode)
	{
		bool hasNoEvidence = false;

		if (_theNet.GetNumberOfNodes() == 0)
			throw "You have to update nodes first";

		vector<NodeToImprove> result;
	
		vector<RNode *> nodeWithEvidence;
		nodeWithEvidence.reserve(50); 
	
		nodesThatIWant(&nodeWithEvidence, childNode);

		DSL_sysCoordinates theCoord(*_theNet.GetNode(childNode->smileHandler)->Value());

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
			_theNet.UpdateBeliefs();

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

	void BNetworkLegacy::dataToRNodeLegacy()
	{
		for (int i = 0; i < _nodes.size(); i++) {
			if (_nodes.at(i)->isChild()) {
				DSL_sysCoordinates theCoord(*_theNet.GetNode(_nodes.at(i)->smileHandler)->Value());
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
	}

	void BNetworkLegacy::calculateTPNLegacy()
	{
		_theNet.CleanUp();
		if (_theNet.GetNumberOfCases() == 0) {
			_mainScenario = _theNet.AddCase("Case_1");
		} else {
			int numbetOfEvidence = _mainScenario->GetNumberOfEvidence();
			for(int i = 0; i < numbetOfEvidence; i++)
				_mainScenario->RemoveEvidence(0);
		}

		DSL_stringArray someNames;
	
		for (int i = 0; i < _nodes.size(); i++) {
			for (int j = 0; j < _nodes.at(i)->getNumberOfStates(); j++)
				someNames.Add(_nodes.at(i)->getStateTitlesPtr()->at(j).c_str());
		
			int nodeHandler =  _theNet.AddNode(DSL_CPT, _nodes.at(i)->getName().c_str());
			_nodes.at(i)->smileHandler = nodeHandler;

			_theNet.GetNode(nodeHandler)->Definition()->SetNumberOfOutcomes(someNames);
			someNames.Flush();
		}

		for (int i = 0; i < _nodes.size(); i++) {
			if (_nodes.at(i)->isChild()) {
				vector<RNode*> parents = _nodes.at(i)->getParents();
				for (int j = 0; j < parents.size(); j++)
					_theNet.AddArc(parents.at(j)->smileHandler, _nodes.at(i)->smileHandler); 
			
				DSL_sysCoordinates theCoordinates(*_theNet.GetNode(_nodes.at(i)->smileHandler)->Definition());
			
				if (_nodes.at(i)->wExpression == WExpression::NONE) { 
						_nodes.at(i)->fillSmileFileCPTValues(&theCoordinates);
				} else {
					_nodes.at(i)->getTpnPtr()->clear();
					_nodes.at(i)->fillSmileCPT(&theCoordinates, 0);
				}
			} else if (_nodes.at(i)->getEvidence() != -1) {
				_mainScenario->AddEvidence(_nodes.at(i)->smileHandler, _nodes.at(i)->getEvidence());
			}
		}

		_mainScenario->CaseToNetwork();
		_theNet.UpdateBeliefs();
		dataToRNodeLegacy();
	}

	void BNetworkLegacy::exportXDSLFileLegacy(string filename)
	{
		if (_theNet.GetNumberOfCases() == 0)
			_mainScenario = _theNet.AddCase("Case_1");

		for (int i = 0; i < _nodes.size(); i++) {
			if (!_nodes.at(i)->isChild()) {
				if (_nodes.at(i)->getEvidence() != -1)
					_mainScenario->SetEvidenceByHandle(_nodes.at(i)->smileHandler, _nodes.at(i)->getEvidence());
			}
		}

		_mainScenario->CaseToNetwork();
		_theNet.UpdateBeliefs();
		_theNet.WriteFile(filename.c_str());
	}

	RNode *BNetworkLegacy::createNodeLegacy()
	{
		RNode *n = new RNode();
		_nodes.push_back(n);
		return n;
	}

	RNode *BNetworkLegacy::createNodeLegacy(int id)
	{
		RNode *n = new RNode(id);
		_nodes.push_back(n);
		return n;
	}

	bool BNetworkLegacy::nodeExists(int id)
	{
		for (int i = 0; i < _nodes.size(); ++i) {
			if (_nodes.at(i)->getId() == id)
				return true;
		}
		return false;
	}

	void BNetworkLegacy::updateInference()
	{
		for (int i = 0; i < _nodes.size(); i++) {
			if (!_nodes.at(i)->isChild()) {
				if (_nodes.at(i)->getEvidence() != -1) {
					_mainScenario->AddEvidence(_nodes.at(i)->smileHandler, _nodes.at(i)->getEvidence());
				} else {
					_mainScenario->SetEvidenceByHandle(_nodes.at(i)->smileHandler, _nodes.at(i)->getEvidence());
				}
			}
		}

		_mainScenario->CaseToNetwork();
		_theNet.UpdateBeliefs();
		dataToRNodeLegacy();	
	}

	std::vector<RNode*> *BNetworkLegacy::getNodes() 
	{
		return &_nodes;
	}
}