#pragma once
#include "GameObject.h"
#include "algorithm"

#define FLOWER_BBOX_WIDTH  20
#define FLOWER_BBOX_HEIGHT 20



class CFlower : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);

public:
	CFlower(){}
	virtual void SetState(int state);
};