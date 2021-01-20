#include "Cell.h"
void Cell::Add(CGameObject* obj)
{
	listObjects.push_back(obj);
}

vector<LPGAMEOBJECT> Cell::GetListObjects()
{
	return listObjects;
}


void Cell::Unload()
{
	listObjects.clear();
}