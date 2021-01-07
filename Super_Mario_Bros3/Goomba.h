#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Koopas.h"

#define GOOMBA_WALKING_SPEED		 0.05f
#define GOOMBA_GRAVITY				 0.002f
#define GOOMBA_DIE_DEFLECT_SPEED	 0.5f
#define GOOMBA_JUMP_SPEED			 0.35f



#define GOOMBA_NORMAL_BBOX_WIDTH			16
#define GOOMBA_NORMAL_BBOX_HEIGHT			15
#define GOOMBA_RED_FLY_BBOX_WIDTH			20
#define GOOMBA_RED_FLY_BBOX_HEIGHT			24
#define GOOMBA_RED_LOSE_WINGS_BBOX_WIDTH	20
#define GOOMBA_RED_LOSE_WINGS_BBOX_HEIGHT	19


#define GOOMBA_STATE_WALKING		100
#define GOOMBA_STATE_DIE			200
#define GOOMBA_STATE_DISAPPEAR		300
#define GOOMBA_STATE_DIE_BY_KICK	400
#define GOOMBA_STATE_RED_LOSE_WINGS 500
#define GOOMBA_STATE_IDLE			600

#define GOOMBA_NORMAL_ANI_WALKING		0
#define GOOMBA_NORMAL_ANI_DIE			1
#define GOOMBA_RED_FLY_ANI_WALKING		2
#define GOOMBA_RED_FLY_ANI_DIE			3	
#define GOOMBA_RED_FLY_ANI_LOSE_WINGS	4
#define GOOMBA_NORMAL_ANI_IDLE			5	

#define GOOMBA_NORMAL	888
#define GOOMBA_RED_FLY	999

#define GOOMBA_TIME_JUMPING		900

class CGoomba : public CGameObject
{
	int type;
	bool isAppear = true;
	DWORD  jumpingStart = 0;
	DWORD  dyingStart = 0;


	DWORD  runningStart = 0;

	int pointPara = 1;

	bool isAllowToShowScore = false;
	DWORD timing_score;
public:
	CGoomba(int ctype, int scene_id);
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	void FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy);
	int GetType()
	{
		return type;
	}
	virtual void SetState(int state);
	void StartDying()
	{
		dyingStart = GetTickCount();
	}
	void StartRunning()
	{
		runningStart = GetTickCount();
	}
	int GetPointPara()
	{
		return pointPara;
	}
	void CalcDoublePointPara()
	{
		pointPara *= 2;
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