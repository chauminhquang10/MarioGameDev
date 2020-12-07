#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Scence.h"
#include "WorldMap.h"
#include "Game.h"
#include "Utils.h"
#include "WorldMapObjects.h"
#include "KeyEventHandler.h"
#include <vector>

#define WORLD_MAP_TYPE_NODE_NORMAL			55
#define WORLD_MAP_TYPE_NODE_SPECIAL			66

typedef Node* CNode;

class Node : public CGameObject
{
	int type;

	int left = -1, top = -1, right = -1, bottom = -1;

	int node_id;

public:
	Node(int ctype);
	~Node();
	virtual void Render() {}
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {}
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	int GetTop()
	{
		return top;
	}
	int GetBottom()
	{
		return bottom;
	}
	int GetRight()
	{
		return right;
	}
	int GetLeft()
	{
		return left;
	}
	CNode FindNodeTop()
	{
		vector<CNode> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();
		for (size_t i = 0; i < Nodes.size(); i++)
		{
			if (this->y < Nodes[i]->y <= this->y - 30)
			{
				return Nodes[i];
			}
		}
		return NULL;
	}
	CNode FindNodeBottom()
	{
		vector<CNode> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();
		for (size_t i = 0; i < Nodes.size(); i++)
		{
			if (this->y < Nodes[i]->y <= this->y + 30)
			{
				return Nodes[i];
			}
		}
		return NULL;
	}
	CNode FindNodeRight()
	{
		vector<CNode> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();
		for (size_t i = 0; i < Nodes.size(); i++)
		{
			if (this->x < Nodes[i]->x <= this->x + 30)
			{
				return Nodes[i];
			}
		}
		return NULL;
	}
	CNode FindNodeLeft()
	{
		vector<CNode> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();

		for (size_t i = 0; i < Nodes.size(); i++)
		{
			if (this->x < Nodes[i]->x <= this->x - 30)
			{
				return Nodes[i];
			}
		}
		return NULL;
	}
};

