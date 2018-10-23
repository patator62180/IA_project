#ifndef AI_HELPER_H
#define AI_HELPER_H

#include "LevelInfo.h"
#include "TurnInfo.h"
#include "Algorithm/StateMachine.h"
#include "Algorithm/BlackBoard.h"
#include "Algorithm/BehaviourTree.h"

#include <set>

class AIHelper
{
private:
    StateMachine stateMachine;
    std::set<unsigned int> npcsCurrentHexID;

public:
    BlackBoard blackBoard;
	static std::map<unsigned int, NpcStateInfo> npcsInfo;
	BehaviourTree bt;

    AIHelper() = default;
    void Init(const LevelInfo&);

    void Update(const TurnInfo&);
    void FillActionList(std::vector<Action*>&);

    bool isHexIDOccupied(const unsigned int);
    bool TryAddNpcCurrentHexID(NpcStateInfo&);

    const std::set<unsigned int>& getNpcsCurrentHexID() noexcept;

    ~AIHelper() = default;
};

#endif // AI_HELPER_H