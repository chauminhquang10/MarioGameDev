#pragma once

#include "GameObject.h"
#include "Mario.h"
#include"Goomba.h"
#include "PlayScence.h"
#include "Scence.h"
#include "FireBullet.h"


#define KOOPAS_WALKING_SPEED	 0.02f
#define KOOPAS_GRAVITY			 0.002f
#define KOOPAS_DIE_DEFLECT_SPEED 0.5f
#define KOOPAS_SHELL_DEFLECT_SPEED 0.4f
#define KOOPAS_BBOX_WIDTH		 16
#define KOOPAS_BBOX_HEIGHT		 26
#define KOOPAS_BBOX_HEIGHT_SHELL 16
#define KOOPAS_BBOX_HEIGHT_DIE	 16

#define KOOPAS_STATE_WALKING	100
#define KOOPAS_STATE_SHELL		200
#define KOOPAS_STATE_DIE		300
#define KOOPAS_STATE_SPINNING	400
#define KOOPAS_STATE_HOLDING	500
#define KOOPAS_STATE_WALKING_RIGHT			600
#define KOOPAS_STATE_WALKING_RIGHT_FASTER	700


#define KOOPAS_XANH_ANI_WALKING_LEFT  0
#define KOOPAS_XANH_ANI_WALKING_RIGHT 1
#define KOOPAS_XANH_ANI_FLYING_LEFT   2
#define KOOPAS_XANH_MAI_ANI_UP		  3
#define KOOPAS_XANH_MAI_ANI_NGUA	  4	
#define KOOPAS_RED_ANI_WALKING_LEFT   5
#define KOOPAS_RED_ANI_WALKING_RIGHT  6
#define KOOPAS_RED_ANI_FLYING_LEFT	  7
#define KOOPAS_RED_MAI_ANI_UP		  8
#define KOOPAS_XANH_MAI_ANI_SPINNING  9
#define KOOPAS_RED_MAI_ANI_SPINNING	  10
#define KOOPAS_XANH_ANI_REVIVING	  11	
#define KOOPAS_RED_ANI_REVIVING		  12
#define KOOPAS_RED_MAI_ANI_NGUA		  13
#define KOOPAS_XANH_ANI_REVIVING_NGUA			14
#define KOOPAS_RED_ANI_REVIVING_NGUA			15
#define KOOPAS_BLACK_UP							16
#define KOOPAS_BLACK_NGUA						17
#define KOOPAS_XANH_ANI_WALKING_RIGHT_FASTER	18
#define KOOPAS_XANH_MAI_ANI_SPINNING_NGUA		19
#define KOOPAS_RED_MAI_ANI_SPINNING_NGUA		20

#define KOOPAS_JUMP_SPEED		0.35f
#define KOOPAS_TIME_JUMPING		900


#define KOOPAS_XANH_WALK	111
#define KOOPAS_XANH_FLY		222
#define KOOPAS_RED_WALK		333
#define KOOPAS_BLACK		444

#define KOOPAS_TYPE_LINE	555
#define KOOPAS_TYPE_FASTER	666


class CKoopas : public CGameObject
{
	int type;
	bool isHolding = false;
	float CheckPosition_Y;
	bool CanPullBack = false;
	bool reviveRender = false;
	bool shellUpRender = false;
	bool isKickedRevive = false;
	int dieDirection = -1;
	bool renderRecognization = false;

	bool spinningRecognization = false;

	bool isAppear = true;
	int jump_count= 0;

	bool mario_recog = false;
	bool reset_spinning = false;

	bool spinning_shell_intro = false;

	DWORD jumpingStart = 0;
	DWORD reviveStart = 0;


	int pointPara = 1;

	bool  isAllowToUpPointPara;
	bool toEndTheScoreProgress = false;

	bool isAllowToShowScore = false;
	DWORD timing_score;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	void FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	bool GetIsAllowToUpPointPara()
	{
		return isAllowToUpPointPara;
	}
	void SetIsAllowToUpPointPara(bool isAllowToUpPointParaBool)
	{
		isAllowToUpPointPara = isAllowToUpPointParaBool;
	}
	bool GetIsHolding()
	{
		return isHolding;
	}
	void SetIsHolding(bool isHoldingBool)
	{
		this->isHolding = isHoldingBool;
	}
	bool GetShellUpRender()
	{
		return shellUpRender;
	}
	void SetShellUpRender(bool shellUpRenderBool)
	{
		this->shellUpRender = shellUpRenderBool;
	}
	bool GetReviveRender()
	{
		return reviveRender;
	}
	void SetReviveRender(bool reviveRenderBool)
	{
		this->reviveRender = reviveRenderBool;
	}
	bool GetRenderRegconization()
	{
		return renderRecognization;
	}
	void SetRenderRegconization(bool renderRegconizationBool)
	{
		this->renderRecognization = renderRegconizationBool;
	}
	bool GetSpinningRegconization()
	{
		return spinningRecognization;
	}
	void SetSpinningRegconization(bool spinningRegconizationBool)
	{
		this->spinningRecognization = spinningRegconizationBool;
	}
	bool GetIsKickedRevive()
	{
		return isKickedRevive;
	}
	void SetIsKickedRevive(bool isKickedReviveBool)
	{
		this->isKickedRevive = isKickedReviveBool;
	}
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	CKoopas(int ctype , int scene_id);
	void StartRevive()
	{
		reviveStart = GetTickCount();
	}
	DWORD GetReviveStart()
	{
		return reviveStart;
	}
	void  SetReviveStart(DWORD reviveStartTime)
	{
		reviveStart = reviveStartTime;
	}
	void SetResetSpinning(bool resetSpinningBool)
	{
		this->reset_spinning = resetSpinningBool;
	}
	virtual void SetState(int state);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	int GetPointPara()
	{
		return pointPara;
	}
	void ResetPointPara()
	{
		pointPara = 1;
	}
	void CalcDoublePointPara()
	{
		pointPara *= 2;
	}
	bool GetToEndTheScoreProgress()
	{
		return toEndTheScoreProgress;
	}
	void SetToEndTheScoreProgress(bool toEndTheScoreProgressBool)
	{
		toEndTheScoreProgress = toEndTheScoreProgressBool;
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