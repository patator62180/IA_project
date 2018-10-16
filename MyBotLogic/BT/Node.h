#pragma once
#include <vector>
#include "../GameObject/Npc.h"


using namespace std;

enum NodeState { SUCCESS, FAIL, RUNNING };

struct Node {
	vector<Node*> children;
	virtual NodeState run(unsigned int npcId) = 0;
};