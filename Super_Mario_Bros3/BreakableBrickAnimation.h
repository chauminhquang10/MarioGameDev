#pragma once

#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include "Define.h"



#define BREAKABLE_BRICK_ANIMATION_STATE_IDLE	0
#define BREAKABLE_BRICK_ANIMATION_STATE_MOVE	100

#define BREAKABLE_BRICK_ANIMATION_GRAVITY		0.002f

#define BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP			111
#define BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP		222
#define BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM		333
#define BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM		444




class CBreakableBrickAnimation : public CGameObject
{
	int type;
	bool isUsed = false;
	DWORD timing_start = 0;
public:

	CBreakableBrickAnimation(int ctype);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void StartTiming()
	{
		if (timing_start == 0)
			timing_start = GetTickCount();
	}
	void SetIsUsed(bool isUsedBool)
	{
		isUsed = isUsedBool;
	}
	bool GetIsUsed()
	{
		return isUsed;
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