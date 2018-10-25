#pragma once
#ifndef DEF_ON_GOAL_H
#define DEF_ON_GOAL_H
#include "../Node.h"
#include "../../AIHelper.h"
#include "../../GameManager.h"

class OnGoal : public Node
{
public:
	virtual NodeState run(unsigned int identifiant) {
		for (Goal goal : GameManager::getInstance().getAIHelper().blackBoard.goals) {
			if(GameManager::getInstance().getAIHelper().npcsInfo[identifiant].npc.hexID == goal.hexID)
				return SUCCESS ;
		}
		return FAIL;
	}
};

#endif
