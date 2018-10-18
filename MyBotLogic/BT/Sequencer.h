#pragma once
#include "Node.h"

struct Sequencer : public Node {
	virtual NodeState run(unsigned int npcId) {
		for (auto node : children) {
			if (node->run(npcId) == FAIL)
				return FAIL;
		}
		return SUCCESS;
	}
};