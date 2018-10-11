#pragma once

namespace Tile
{
	enum ETilePosition { NE, E, SE, SW, W, NW, CENTER };
	enum ETileType
	{
		TileAttribute_Default,
		TileAttribute_Goal,
		TileAttribute_Forbidden,
		TileAttribute_Omniscient,
	};
}

namespace Object
{
	enum EObjectType
	{
		ObjectType_Wall,
		ObjectType_Window,
		ObjectType_Door,
		ObjectType_PressurePlate,
        //MY CODE START
        ObjectType_None
        //MY CODE END
	};

	enum EObjectState
	{
		ObjectState_Opened,
		ObjectState_Closed
	};
}

//MY CODE START
using HexDirection = Tile::ETilePosition;
using HexType = Tile::ETileType;
using EdgeType = Object::EObjectType;
//MY CODE END

struct Action
{
public:
	
	enum EActionType { ActionType_None, ActionType_Move, ActionType_Interact };

	Action(unsigned int _unitID, EActionType _actionType)
		: unitID(_unitID)
		, actionType(_actionType)
	{

	}

	virtual Action* Clone() = 0;

	unsigned int unitID;
	EActionType actionType;

private:
};

struct Move : Action
{
	Move(unsigned int _unitID, Tile::ETilePosition _direction)
		: Action (_unitID, EActionType::ActionType_Move)
		, direction(_direction)
	{

	}

	Move(const Move& _other)
		: Action (_other.unitID, _other.actionType)
		, direction(_other.direction)
	{

	}

	virtual Action* Clone()
	{
		return new Move(*this);
	}
	
	Tile::ETilePosition direction;
};

struct Interact : Action
{
	enum EInteraction
	{
		Interaction_None,
		Interaction_OpenDoor,
		Interaction_CloseDoor,
		Interaction_SearchHiddenDoor,
	};
	
	Interact(unsigned int _unitID, unsigned int _objectID, EInteraction _interaction)
		: Action(_unitID, EActionType::ActionType_Interact)
		, objectID(_objectID)
		, interaction(_interaction)
	{

	}

	Interact(const Interact& _other)
		: Action(_other.unitID, _other.actionType)
		, objectID(_other.objectID)
		, interaction(_other.interaction)
	{

	}

	virtual Action* Clone()
	{
		return new Interact(*this);
	}

	unsigned int objectID;
	EInteraction interaction;
};
