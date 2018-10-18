#pragma once
#include <vector>
#include "../GameObject/Npc.h"


using namespace std;

enum NodeState { SUCCESS, FAIL, RUNNING };

class NodeException {};

struct Node {
	vector<Node*> children;
	virtual NodeState run(unsigned int npcId) {
			throw NodeException();
			return FAIL; };
};