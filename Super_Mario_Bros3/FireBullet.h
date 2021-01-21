#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Scence.h"
#include <algorithm>

#define FIRE_BULLET_BBOX_WIDTH  9
#define FIRE_BULLET_BBOX_HEIGHT 9

#define FIRE_BULLET_FLYING_SPEED	0.1f 

#define FIRE_BULLET_STATE_FLYING		100
#define FIRE_BULLET_STATE_HIDDEN		200

#define FIRE_BULLET_ANI_RIGHT		0
#define FIRE_BULLET_ANI_LEFT		1

class CFireBullet : public CGameObject
{
	DWORD remaining_time = 0;
	bool isExist = true;

	bool isUsed;
	float Height_Limit;
	bool isAllowToShowScore = false;
	DWORD timing_score;

	bool isAllowToShowHitEffectTurnTail = false;
	bool isAllowToShowHitEffectFireBullet = false;
public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);
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
	void StartRemainingTime()
	{
		if (remaining_time == 0)
		{
			remaining_time = GetTickCount();
		}
	}
	bool GetIsExist()
	{
		return isExist;
	}
	void SetIsExist(bool isExistBool)
	{
		isExist = isExistBool;
	}
	CFireBullet();
	
	bool GetIsAllowToShowHitEffectFireBullet()
	{
		return isAllowToShowHitEffectFireBullet;
	}
	void SetIsAllowToShowHitEffectFireBullet(bool isAllowToShowHitEffectFireBulletBool)
	{
		isAllowToShowHitEffectFireBullet = isAllowToShowHitEffectFireBulletBool;
	}
};
