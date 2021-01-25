#include "Node.h"
#include "Game.h"
#include "WorldMap.h"

Node::Node(int id, int l , int r,int t, int b, int ctype)
{
	node_id = id;
	left = l;
	right = r;
	top = t;
	bottom = b;
	type = ctype;
}


Node::~Node()
{

}

Node* Node::FindNode(int direction)
{
	vector<Node*> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();
	switch (direction)
	{
	case 1:
		for (size_t i = 0; i < Nodes.size(); i++)
		{
			if ( Nodes[i]->y == this->y - NODE_FIND_DISTANCE && Nodes[i]->x == this->x)
			{
				return Nodes[i];
			}
		}
		break;
	case 2:
		for (size_t i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->y == this->y + NODE_FIND_DISTANCE && Nodes[i]->x == this->x)
			{
				return Nodes[i];
			}
		}
		break;
	case 3:
		for (size_t i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->x == this->x + NODE_FIND_DISTANCE && Nodes[i]->y == this->y)
			{
				return Nodes[i];
			}
		}
		break;
	case 4:
		for (size_t i = 0; i < Nodes.size(); i++)
		{
			if (Nodes[i]->x == this->x - NODE_FIND_DISTANCE && Nodes[i]->y == this->y)
			{
				return Nodes[i];
			}
		}
		break;
	}
	return NULL;
}



void Node::Render()
{

}