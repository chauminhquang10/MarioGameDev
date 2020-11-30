#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"


#define STAR_BBOX_WIDTH		16
#define STAR_BBOX_HEIGHT	16


#define STAR_STATE_IDLE		0



class CStar : public CGameObject
{
	bool isAppear = false;
	

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CStar();
	virtual void SetState(int state);


};