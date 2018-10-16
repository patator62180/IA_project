#pragma once
#include "Node.h"

class Selector : public Node {
	NodeState run(unsigned int npcId) {
		for (auto node : children) {
			if (node->run(npcId) == SUCCESS)
				return SUCCESS;
		}
		return FAIL;
	}
};
