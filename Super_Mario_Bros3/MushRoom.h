#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"


#define MUSHROOM_BBOX_WIDTH		16
#define MUSHROOM_BBOX_HEIGHT	16

#define MUSHROOM_STATE_IDLE		0
#define MUSHROOM_STATE_UP		100
#define MUSHROOM_STATE_MOVE		200
#define MUSHROOM_STATE_MOVE_LEFT 300

#define MUSHROOM_GRAVITY		0.002f
#define MUSHROOM_RED			567
#define MUSHROOM_GREEN			678

#define MUSHROOM_ANI_RED		0
#define MUSHROOM_ANI_GREEN		1


class CMushRoom : public CGameObject
{
	bool isAppear = false;
	bool haveGravity = false;
	int type;
	DWORD upping_start = 0;
	int moveDirection = 1;

	bool isAllowToShowScore = false;
	DWORD timing_score;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CMushRoom(int ctype);
	virtual void SetState(int state);
	
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
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