#pragma once
#include "Node.h"

struct Selector : public Node {
	virtual NodeState run(unsigned int npcId) {
		for (auto node : children) {
			if (node->run(npcId) == SUCCESS)
				return SUCCESS;
		}
		return FAIL;
	}
};
