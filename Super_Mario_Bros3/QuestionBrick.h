#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include "Leaf.h"


#define QUESTION_BRICK_BBOX_WIDTH  16
#define QUESTION_BRICK_BBOX_HEIGHT 16



#define QUESTION_BRICK_ANI_ALIVE	0
#define QUESTION_BRICK_ANI_DEAD		1
#define QUESTION_BRICK_ANI_NEW_TYPE	2


#define QUESTION_BRICK_HAVE_LEAF			777
#define QUESTION_BRICK_JUST_HAVE_MUSHROOM	888


class CQuestionBrick : public CGameObject
{
	int type;
	bool isAlive = true;
	bool isUsed = false;

	bool isUp = false;
	bool Calc_Y_Colli = false;

	DWORD upDownTime = 0;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CQuestionBrick(int ctype);
	virtual void SetState(int state);
	bool GetIsAlive()
	{
		return isAlive;
	}
	void SetIsAlive(bool isAliveBool)
	{
		isAlive = isAliveBool;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int Ctype)
	{
		type = Ctype;
	}
	bool GetIsUsed()
	{
		return isUsed;
	}
	void SetIsUsed(bool isUsedBool)
	{
		isUsed = isUsedBool;
	}
	bool GetIsUp()
	{
		return isUp;
	}
	void SetIsUp(bool isUpBool)
	{
		isUp = isUpBool;
	}
	bool GetCalcYColli()
	{
		return Calc_Y_Colli;
	}
	void SetCalcYColli(bool Calc_Y_Colli_Bool)
	{
		Calc_Y_Colli = Calc_Y_Colli_Bool;
	}
	void StartTime()
	{
		upDownTime = GetTickCount();
	}

};