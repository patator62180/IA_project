#ifndef AI_HELPER_H
#define AI_HELPER_H

#include "Globals.h"
#include "LevelInfo.h"
#include "TurnInfo.h"
#include "GameObject/Npc.h"
#include "Algorithm/StateMachine.h"
#include "Algorithm/BlackBoard.h"

#include <vector>
#include <set>
#include <map>

class AIHelper
{
private:
    std::map<unsigned int, Npc> npcs;

    StateMachine exploreState; //remove -> behaviour tree
    std::set<unsigned int> npcsCurrentHexID;

public:
    BlackBoard bb;

    AIHelper() = default;
    void Init(const LevelInfo&);

    void Update(const TurnInfo&);
    void FillActionList(std::vector<Action*>&);

    bool TryAddNpcCurrentHexID(const unsigned int&, const unsigned int);

    const std::set<unsigned int>& getNpcsCurrentHexID() noexcept;

    ~AIHelper() = default;
};

#endif // AI_HELPER_H