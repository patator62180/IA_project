#include "GameManager.h"

#include "Utils\DebugHelper.h"

#include <sstream>

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

    AIhelper.Init(levelInfo);
 //   AIhelper.Update(turnInfo);
}

void GameManager::Update(const TurnInfo& turnInfo)
{
    std::stringstream ss;
    ss << std::endl << std::endl << std::endl
        << "--------------------------------------------------------" << std::endl
        << "Turn:" << turnInfo.turnNb << std::endl
        << "--------------------------------------------------------" << std::endl;
    DebugHelper::getInstance().Log(ss.str());

    map.Update(turnInfo);
    AIhelper.Update(turnInfo);
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