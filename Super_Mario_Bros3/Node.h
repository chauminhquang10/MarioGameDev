#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Scence.h"
#include "Utils.h"
#include "WorldMapObjects.h"
#include "KeyEventHandler.h"
#include <vector>
#include "Define.h"


class Node : public CGameObject
{
	int type;

	int left, top, right, bottom;

	int node_id;

public:
	Node(int id, int l,int r, int t ,int b, int ctype);
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
	void SetTop(int topInt)
	{
		this->top = topInt;
	}
	void SetBottom(int bottomInt)
	{
		this->bottom = bottomInt;
	}
	void SetRight(int rightInt)
	{
		this->right = rightInt;
	}
	void SetLeft(int leftInt)
	{
		this->left = leftInt;
	}
	Node* FindNode(int direction);
	
	

	
};
