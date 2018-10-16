#pragma once
#include "../Node.h"
#include "../../GameManager.h"


class Deplacement : public Node {
	Tile::ETilePosition dir ;
	Deplacement() = delete;

public:
	Deplacement(Tile::ETilePosition _dir) : dir(_dir) {};
	NodeState run(unsigned int npcId) {
		std::map<unsigned int, Movement>&  turnMove = GameManager::getInstance().getTurnMove();
		Movement mv = Movement();
		mv.direction = dir;
		turnMove[dir];
		return SUCCESS;
	}
};

