#pragma once
#include "../Node.h"
#include "../../GameManager.h"


class Deplacement : public Node {
	Tile::ETilePosition dir ;
	Deplacement() = delete;

public:
	Deplacement(Tile::ETilePosition _dir) : dir(_dir) {};
	virtual NodeState run(unsigned int npcId) {
		std::map<unsigned int, Movement>& turnMove = GameManager::getInstance().getTurnMove();
		Movement mv = Movement();
		mv.direction = dir;
		turnMove[npcId] = mv;
		return SUCCESS;
	}
};

