#pragma once
#ifndef DEF_FETCH_GOAL_H
#define DEF_FETCH_GOAL_H
#include "../Node.h"
#include "../../AIHelper.h"
#include "../../GameManager.h"

class FetchGoal : public Node
{
public :
	virtual NodeState run(unsigned int identifiant) {
		if (GameManager::getInstance().getAIHelper().blackBoard.existGoal(identifiant))
			return SUCCESS;
		return FAIL;
	}
};

#endif
