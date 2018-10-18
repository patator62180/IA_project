#pragma once
#include "../Node.h"
#include "../../GameManager.h"


class NbTiles : public Node {
	int nbTiles;
	NbTiles() = delete;
public:
	NbTiles(int _nbTiles) : nbTiles(_nbTiles) {};
	virtual NodeState run(unsigned int npcId) {
		GameManager& gm = GameManager::getInstance();
		if (gm.getMap().getLayout().size() == nbTiles)
			return SUCCESS;
		return FAIL;
	}
};