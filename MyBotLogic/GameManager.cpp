#include "GameManager.h"

#include "Algorithm\AStar.h"
#include "Utils\DebugHelper.h"

#include <set>
#include <algorithm>
#include <sstream>
#include <assert.h>

class NpcWithoutObjective {};

GameManager& GameManager::getInstance() {
    static GameManager instance;
    return instance;
}

void GameManager::Init(const LevelInfo& levelInfo)
{
    TurnInfo turnInfo;
    turnInfo.npcs = levelInfo.npcs;
    turnInfo.objects = levelInfo.objects;
    turnInfo.tiles = levelInfo.tiles;
    turnInfo.turnNb = 0;

    map.InitLayout(
        static_cast<int>(levelInfo.rowCount),
        static_cast<int>(levelInfo.colCount));
    map.Update(turnInfo);
    bb.Init(map.getLayout().size());
    AIhelper = { levelInfo };
	bt = BehaviourTree();


}

void GameManager::Update(const TurnInfo& turnInfo)
{
    std::stringstream ss;
    ss << std::endl << "Turn:" << turnInfo.turnNb << std::endl;
    DebugHelper::getInstance().Log(ss.str());

    map.Update(turnInfo);
    AIhelper.Update(turnInfo);
    bb.Update(map, turnInfo);

    auto a = 9;
}

void GameManager::FillActionList(std::vector<Action*>& actionList)
{
    AIhelper.FillActionList(actionList);
}

Map& GameManager::getMap() noexcept {
    return map;
}

AIHelper& GameManager::getAIHelper() noexcept {
    return AIhelper;
}

std::map<unsigned int, Movement>& GameManager::getTurnMove() noexcept
{
	return turnMove;
}
