#include "MyBotLogic.h"

#include "TurnInfo.h"
#include "NPCInfo.h"
#include "LevelInfo.h"
#include "Utils/DebugHelper.h"

#include "windows.h"

#include <sstream>
#include <iostream>

MyBotLogic::MyBotLogic()
{
	//Write Code Here
}

/*virtual*/ MyBotLogic::~MyBotLogic()
{
	//Write Code Here
}

/*virtual*/ void MyBotLogic::Configure(int argc, char *argv[], const std::string& _logpath)
{
#ifdef BOT_LOGIC_DEBUG
	mLogger.Init(_logpath, "MyBotLogic.log");
#endif

	BOT_LOGIC_LOG(mLogger, "Configure", true);

    DebugHelper::getInstance().InitLogger(_logpath);
}

/*virtual*/ void MyBotLogic::Start()
{
}

/*virtual*/ void MyBotLogic::Init(LevelInfo& _levelInfo)
{
    GameManager::getInstance().Init(_levelInfo);
}

/*virtual*/ void MyBotLogic::OnGameStarted()
{
	//Write Code Here
}

/*virtual*/ void MyBotLogic::FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList)
{
    //GameManager::getInstance().Update(_turnInfo);
	//GameManager::getInstance().FillActionList(_actionList);

	GameManager* gm = &GameManager::getInstance();
	for (auto npc : _turnInfo.npcs) {
		_actionList.push_back(new Move(npc.second.npcID,
										gm->getTurnMove()[npc.second.npcID].direction));
	}
}

/*virtual*/ void MyBotLogic::Exit()
{
	//Write Code Here
}