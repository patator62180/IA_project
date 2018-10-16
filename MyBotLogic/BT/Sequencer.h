#pragma once
#include "Node.h"

class Sequencer : public Node {
	NodeState run(unsigned int npcId) {
		for (auto node : children) {
			if (node->run(npcId) == FAIL)
				return FAIL;
		}
		return SUCCESS;
	}
};