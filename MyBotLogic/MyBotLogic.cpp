#include "MyBotLogic.h"

#include "TurnInfo.h"
#include "NPCInfo.h"
#include "LevelInfo.h"
#include "Utils/DebugHelper.h"
#include "BT/Node.h"
#include "BT/Selector.h"
#include "BT/Sequencer.h"
#include "BT/Leaf/Deplacement.h"
#include "BT/Leaf/NbTiles.h"

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
	NbTiles nbTiles = NbTiles(9);
	Deplacement dep1 = Deplacement(Tile::NW);
	Deplacement dep2 = Deplacement(Tile::SW);
	Sequencer seq;
	seq.children.push_back(&nbTiles);
	seq.children.push_back(&dep1);
	Selector root;
	root.children.push_back(&seq);
	root.children.push_back(&dep2);
}

/*virtual*/ void MyBotLogic::OnGameStarted()
{
	//Write Code Here
}

/*virtual*/ void MyBotLogic::FillActionList(TurnInfo& _turnInfo, std::vector<Action*>& _actionList)
{
    //GameManager::getInstance().Update(_turnInfo);
    //GameManager::getInstance().FillActionList(_actionList);
	root.run();

}

/*virtual*/ void MyBotLogic::Exit()
{
	//Write Code Here
}