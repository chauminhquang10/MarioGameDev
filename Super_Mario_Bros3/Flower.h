#pragma once
#include "GameObject.h"
#include "algorithm"

#define FLOWER_BBOX_WIDTH  16
#define FLOWER_BBOX_HEIGHT 32


#define FLOWER_ANI_LEFT			0
#define FLOWER_ANI_RIGHT		1
#define FLOWER_ANI_LEFT_IDLE	2
#define FLOWER_ANI_RIGHT_IDLE	3

#define FLOWER_TOP_LIMIT			71
#define FLOWER_BOT_LIMIT			117
#define TIME_SHOWING_LIMIT			3000

class CFlower : public CGameObject
{
	DWORD time_showing=0;
	bool isUp=true;
	bool isFiring = false;
	bool isFired = false;
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CFlower();
	virtual void SetState(int state);
	void StartShowing()
	{
		time_showing = GetTickCount();
	}
	bool GetIsFiring()
	{
		return isFiring;
	}
	void SetIsFiring(bool isFiringBool)
	{
		isFiring = isFiringBool;
	}
	bool GetIsFired()
	{
		return isFired;
	}
	void SetIsFired(bool isFiredBool)
	{
		isFired = isFiredBool;
	}
};