#pragma once

#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Map.h"
#include "WorldMapObjects.h"
#include "Node.h"
#include <vector>
#include "HUD.h"
using namespace std;

class CWorldMap : public  CScene
{

protected:

	vector<LPGAMEOBJECT> objects;
	Map* map;

	Node* current_node;

	vector<Node*> Nodes;

	bool key_control = true;

	vector<CHUD*>  scores;
	vector<CHUD*>  moneys;
	vector<CHUD*>  items;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);


public:
	CWorldMap(int id, LPCWSTR filePath);
	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	vector<LPGAMEOBJECT> GetObjects()
	{
		return objects;
	}

	vector<Node*> GetNodes();
	
	Node* GetCurrentNode();
	
	void SetCurrentNode(Node* current_Node);

	bool GetKeyControl()
	{
		return key_control;
	}

	void SetKeyControl(bool keyControlBool)
	{
		key_control = keyControlBool;
	}

	vector<CHUD*> GetScores()
	{
		return scores;
	}
	vector<CHUD*> GetMoneys()
	{
		return moneys;
	}
	vector<CHUD*> GetItems()
	{
		return items;
	}


	~CWorldMap();
};

class CWorldMapKeyHandler : public CScenceKeyHandler
{

public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CWorldMapKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};
