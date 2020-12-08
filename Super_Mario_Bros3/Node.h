#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Scence.h"
#include "Utils.h"
#include "WorldMapObjects.h"
#include "KeyEventHandler.h"
#include <vector>

#define WORLD_MAP_TYPE_NODE_NORMAL			55
#define WORLD_MAP_TYPE_NODE_SPECIAL			66


class Node : public CGameObject
{
	int type;

	int node_id;

public:
	Node(int ctype,int id);
	~Node();
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {}
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	int GetNodeId()
	{
		return node_id;
	}

	Node* FindNodeTop();
	
	Node* FindNodeBottom();
	
	Node* FindNodeRight();
	
	Node* FindNodeLeft();
	
};
