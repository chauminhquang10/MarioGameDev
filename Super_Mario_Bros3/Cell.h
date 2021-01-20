#pragma once
#include "GameObject.h"

class Cell
{
	vector<LPGAMEOBJECT> listObjects;
public:
	void Add(CGameObject* obj);
	vector<LPGAMEOBJECT> GetListObjects();
	void Unload();
};

typedef Cell* LPCELL;