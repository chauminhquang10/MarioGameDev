#pragma once
#include "GameObject.h"
#include "algorithm"

#define FLOWER_RED_BBOX_WIDTH  16
#define FLOWER_RED_BBOX_HEIGHT 32

#define FLOWER_GREEN_BBOX_WIDTH  16
#define FLOWER_GREEN_BBOX_HEIGHT 24

#define FLOWER_GREEN_CAN_SHOOT_BBOX_WIDTH  16
#define FLOWER_GREEN_CAN_SHOOT_BBOX_HEIGHT 24

#define FLOWER_RED_ANI_LEFT			0
#define FLOWER_RED_ANI_RIGHT		1
#define FLOWER_RED_ANI_LEFT_IDLE	2
#define FLOWER_RED_ANI_RIGHT_IDLE	3
#define FLOWER_GREEN_ANI			4
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT			5
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT		6
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE	7
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE	8
#define FLOWER_RED_ANI_LEFT_UP						9
#define FLOWER_RED_ANI_RIGHT_UP						10
#define FLOWER_RED_ANI_LEFT_IDLE_UP					11
#define FLOWER_RED_ANI_RIGHT_IDLE_UP				12
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_UP			13
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_UP			14
#define FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE_UP		15
#define FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE_UP	16

#define FLOWER_RED				100
#define FLOWER_GREEN			200
#define FLOWER_GREEN_CAN_SHOOT  300 


#define FLOWER_RED_TOP_LIMIT			336
#define FLOWER_RED_BOT_LIMIT			384
#define TIME_SHOWING_LIMIT				3000

#define FLOWER_GREEN_TOP_LIMIT			361
#define FLOWER_GREEN_BOT_LIMIT			391
#define GREEN_TIME_SHOWING_LIMIT		2000


#define FLOWER_GREEN_CAN_SHOOT_TOP_LIMIT		344
#define FLOWER_GREEN_CAN_SHOOT_BOT_LIMIT		391


class CFlower : public CGameObject
{
	int type;
	int isShootingUp = 1;
	DWORD time_showing=0;
	bool isUp=true;
	bool isFiring = false;
	bool isFired = false;
	bool isAlive = true;

	bool isAllowFlowerToUpdate = true;


	bool isAllowToShowScore = false;
	DWORD timing_score;

	DWORD pre_get_tick_count = 0;
	DWORD sub_time = 0;
	
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CFlower(int ctype);
	virtual void SetState(int state);
	void StartShowing()
	{
		time_showing = GetTickCount();
	}
	int GetIsShootingUp()
	{
		return isShootingUp;
	}
	void SetIsShootingUp(int isShootingUpInt)
	{
		isShootingUp = isShootingUpInt;
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
	void SetType(int typeInt)
	{
		type = typeInt;
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
	bool GetIsAllowFlowerToUpdate()
	{
		return isAllowFlowerToUpdate;
	}
	void SetIsAllowFlowerToUpdate(bool isAllowFlowerToUpdateBool)
	{
		isAllowFlowerToUpdate = isAllowFlowerToUpdateBool;
	}
};