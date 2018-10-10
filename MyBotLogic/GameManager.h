#ifndef GAME_MANAGER_H
#define GAME_MANAGER_H

#include "Utils\Singleton.h"
#include "AIHelper.h"
#include "GameObject\Map.h"
#include "GameObject\Npc.h"
#include "Algorithm/BlackBoard.h"

#include "LevelInfo.h"
#include "TurnInfo.h"

#include <map>



class GameManager : Singleton
{
private:
    GameManager() = default;

    Map map;
    AIHelper AIhelper;
    BlackBoard bb;

public:
    static GameManager& getInstance();

    void Init(const LevelInfo&);
    void Update(const TurnInfo&);
    void FillActionList(std::vector<Action*>&);

    Map& getMap() noexcept;
    AIHelper& getAIHelper() noexcept;

    ~GameManager() = default;
};

#endif // GAME_MANAGER_H