#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Map.h"
#include "WorldMapObjects.h"


class CWorldMap : public  CScene
{

protected:

	vector<LPGAMEOBJECT> objects; 
	Map* map;

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

