#pragma once

#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"

#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 16


#define COIN_STATE_IDLE		0
#define COIN_STATE_UP		100
#define COIN_STATE_DOWN		200

#define COIN_NORMAL			222
#define COIN_CAN_MOVE		333


class CCoin : public CGameObject
{
	bool isAppear;
	int type;
	DWORD timing_start ;
public:
	
	CCoin(int ctype);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void SetState(int state);
	void StartTiming()
	{
		timing_start = GetTickCount();
	}
	void SetIsAppear(bool isAppearBool)
	{
		isAppear = isAppearBool;
	}
	bool GetIsAppear()
	{
		return isAppear;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}

};