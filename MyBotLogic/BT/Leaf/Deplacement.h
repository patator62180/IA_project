#pragma once
#include "../Node.h"
#include "../../GameManager.h"
#include "../../AIHelper.h"
#include "../../Algorithm/BehaviourTree.h"



class Deplacement : public Node {
	Tile::ETilePosition dir ;
	Deplacement() = delete;

public:
	Deplacement(Tile::ETilePosition _dir) : dir(_dir) {};
	virtual NodeState run(unsigned int npcId) {
		/*std::map<unsigned int, Movement>& turnMove = GameManager::getInstance().getAIHelper().getTurnMove();
		Movement mv = Movement();
		mv.direction = dir;
		turnMove[npcId] = mv; */

		Movement mv = Movement();
		mv.direction = dir;
		AIHelper::npcsInfo[npcId].pathRecord.push_back(mv);

		return SUCCESS;
	}
};

