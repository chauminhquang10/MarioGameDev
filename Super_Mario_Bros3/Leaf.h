#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"

#define LEAF_BBOX_WIDTH		16
#define LEAF_BBOX_HEIGHT	14

#define LEAF_STATE_IDLE		0
#define LEAF_STATE_UP		100
#define LEAF_STATE_DOWN		200



#define LEAF_ANI_LEFT		0
#define LEAF_ANI_RIGHT		1


class CLeaf : public CGameObject
{
	bool isAppear = false;


	bool isAllowToShowScore = false;
	DWORD timing_score;
	
	DWORD upping_start;
	DWORD downing_start=0;
	DWORD colli_time_tail = 0;
	
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CLeaf();
	virtual void SetState(int state);
	void StartUpping()
	{
		upping_start = GetTickCount();
	}
	void StartDowning()
	{
		downing_start = GetTickCount();
	}
	void StartColliTimeTail()
	{
		if (colli_time_tail == 0)
			colli_time_tail = GetTickCount();
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