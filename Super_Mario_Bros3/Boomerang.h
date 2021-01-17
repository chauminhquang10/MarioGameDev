#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Scence.h"
#include <algorithm>

#define BOOMERANG_BBOX_WIDTH  16
#define BOOMERANG_BBOX_HEIGHT 16






#define BOOMERANG_ANI_FLY_RIGHT		0
#define BOOMERANG_ANI_FLY_LEFT		1

class CBoomerang : public CGameObject
{

	bool isInState_1 = false;
	bool isInState_2 = false;
	bool isInState_3 = false;
	bool isInState_4 = false;

	int id;

	bool isAllowToSetPosition = true;

	DWORD time_switch_state = 0;

	bool isAllowToThrowBoomerang = false;

	bool isAllowToColliWithBoomerangEnemy = false;

	int boomerangDirection = 1;

	DWORD pre_get_tick_count = 0;
	DWORD sub_time = 0;

public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);

	bool GetIsInState_1()
	{
		return isInState_1;
	}
	void SetIsInState_1(bool isInState_1_Bool)
	{
		isInState_1 = isInState_1_Bool;
	}
	bool GetIsInState_2()
	{
		return isInState_2;
	}
	void SetIsInState_2(bool isInState_2_Bool)
	{
		isInState_2 = isInState_2_Bool;
	}
	bool GetIsInState_3()
	{
		return isInState_3;
	}
	void SetIsInState_3(bool isInState_3_Bool)
	{
		isInState_3 = isInState_3_Bool;
	}
	bool GetIsInState_4()
	{
		return isInState_4;
	}
	void SetIsInState_4(bool isInState_4_Bool)
	{
		isInState_4 = isInState_4_Bool;
	}
	void StartTimeSwitchingState()
	{
		if (time_switch_state == 0)
		{
			time_switch_state = GetTickCount();
		}
	}
	DWORD GetTimeSwitchingState()
	{
		return time_switch_state;
	}
	bool GetIsAllowToThrowBoomerang()
	{
		return isAllowToThrowBoomerang;
	}
	void SetIsAllowToThrowBoomerang(bool isAllowToThrowBoomerangBool)
	{
		isAllowToThrowBoomerang = isAllowToThrowBoomerangBool;
	}
	int GetId()
	{
		return id;
	}

	bool GetIsAllowToSetPosition()
	{
		return isAllowToSetPosition;
	}
	void SetIsAllowToSetPosition(bool isAllowToSetPositionBool)
	{
		isAllowToSetPosition = isAllowToSetPositionBool;
	}
	int GetBoomerangDirection()
	{
		return boomerangDirection;
	}
	void SetBoomerangDirection(int boomerangDirectionInt)
	{
		boomerangDirection = boomerangDirectionInt;
	}
	CBoomerang(int idInt);
};
