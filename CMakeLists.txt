cmake_minimum_required( VERSION 3.0 )
project( AIBot C CXX )

#add_definitions(-DDEBUG_FRAMEWORK)

include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
)

# Filters
include( Filters.cmake )


SET( AIBOT_BUILDFILES 

	ChangeLog.txt
	CheckInstall.bat
	CleanAll.bat
	CMakeLists.txt
	Filters.cmake
	GenAIBootCampViewerFiles.bat
	GenAIBootCampViewerFiles.py
	INSTALL.txt
	PlayLocalMatch.bat
	rebuild_vis_sln_x64.bat
	SubmitAIBot.bat
	SubmitAIBot.py
	TeamInfo.team
)

SET( AIBOT_HEADERS 

	#Framework
	Bot.h	
	BotLogicIF.h
	Globals.h
	LevelInfo.h
	Logger.h
	NPCInfo.h
	ObjectInfo.h
	TileInfo.h
	TurnInfo.h

	#MY_BOT
	MyBotLogic/MyBotLogic.h
	MyBotLogic/GameManager.h
	MyBotLogic/AIHelper.h
	
	MyBotLogic/Algorithm/AStar.h
	MyBotLogic/Algorithm/BlackBoard.h
	MyBotLogic/Algorithm/Record.h
	MyBotLogic/Algorithm/StateMachine.h
	
	MyBotLogic/GameObject/Map.h
	MyBotLogic/GameObject/Npc.h
	
	MyBotLogic/MapStructure/CoordAxial.h
	MyBotLogic/MapStructure/Edge.h
	MyBotLogic/MapStructure/Hex.h
	
	MyBotLogic/Utils/DebugHelper.h
	MyBotLogic/Utils/PathHelper.h
	MyBotLogic/Utils/Singleton.h

)

SET( AIBOT_SRC 

	#Framework
	Bot.cpp
	LevelInfo.cpp
	Logger.cpp
	TurnInfo.cpp

	#MY_BOT
	MyBotLogic/MyBotLogic.cpp
	MyBotLogic/GameManager.cpp
	MyBotLogic/AIHelper.cpp
	
	MyBotLogic/Algorithm/AStar.cpp
	MyBotLogic/Algorithm/BlackBoard.cpp
	MyBotLogic/Algorithm/StateMachine.cpp
	
	MyBotLogic/GameObject/Map.cpp
	
	MyBotLogic/Utils/DebugHelper.cpp
	MyBotLogic/Utils/PathHelper.cpp
	
	#Main
	Main.cpp
)

SET(jobfiles "${AIBOT_HEADERS};${AIBOT_SRC};${AIBOT_BUILDFILES}")
file(WRITE jobfiles.txt "${jobfiles}")

add_executable(
	aibot

	${AIBOT_BUILDFILES}
	${AIBOT_HEADERS}
	${AIBOT_SRC}
)

CREATE_VISUAL_STUDIO_FILTERS(AIBOT)

