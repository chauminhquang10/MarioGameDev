#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"


#define FIRE_FLOWER_BBOX_WIDTH		16
#define FIRE_FLOWER_BBOX_HEIGHT		16

#define FIRE_FLOWER_STATE_IDLE		0
#define FIRE_FLOWER_STATE_UP		100




class CFireFlower : public CGameObject
{
	bool isAppear = false;

	DWORD upping_start = 0;


	bool isAllowToShowScore = false;
	DWORD timing_score;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CFireFlower();
	virtual void SetState(int state);

	void StartUpping()
	{
		upping_start = GetTickCount();
	}
	bool GetIsAllowToShowScore()
	{
		if (this != NULL)
			return isAllowToShowScore;
		else
		{
			return false;
		}
	}
	void SetIsAllowToShowScore(bool isAllowToShowScoreBool)
	{
		if (this != NULL)
			isAllowToShowScore = isAllowToShowScoreBool;
	}
	void StartTimingScore()
	{
		timing_score = GetTickCount();
	}

};