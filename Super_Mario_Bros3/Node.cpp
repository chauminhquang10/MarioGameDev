#include "Node.h"
#include "Game.h"
#include "WorldMap.h"

Node::Node(int ctype ,int id)
{
	type = ctype;
	node_id = id;
}


Node::~Node()
{

}

Node* Node::FindNodeTop()
{
	vector<Node*> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();
	for (size_t i = 0; i < Nodes.size(); i++)
	{
		if (this->y < Nodes[i]->y <= this->y - 30)
		{
			return Nodes[i];
		}
	}
	return NULL;
}

Node* Node::FindNodeBottom()
{
	vector<Node*> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();
	for (size_t i = 0; i < Nodes.size(); i++)
	{
		if (this->y < Nodes[i]->y <= this->y + 30)
		{
			return Nodes[i];
		}
	}
	return NULL;
}

Node* Node::FindNodeRight()
{
	vector<Node*> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();
	for (size_t i = 0; i < Nodes.size(); i++)
	{
		if (this->x < Nodes[i]->x <= this->x + 50)
		{
			return Nodes[i];
		}
	}
	return NULL;
}

Node* Node::FindNodeLeft()
{

	vector<Node*> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();

	for (size_t i = 0; i < Nodes.size(); i++)
	{
		if (this->x < Nodes[i]->x <= this->x - 30)
		{
			return Nodes[i];
		}
	}
	return NULL;
}

void Node::Render()
{
	animation_set->at(0)->Render(x, y);
}