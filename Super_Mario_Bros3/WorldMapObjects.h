#pragma once

#include "GameObject.h"
#include "algorithm"
#include "Node.h"


#define MARIO_STATE_MOVE_RIGHT		100
#define MARIO_STATE_MOVE_LEFT		200
#define MARIO_STATE_MOVE_UP			300
#define MARIO_STATE_MOVE_DOWN		400
#define MARIO_STATE_CANT_MOVE		500

#define GOLD_DIGGER_STATE_WALKING_RIGHT	600
#define GOLD_DIGGER_STATE_WALKING_LEFT	700

#define WORLD_MAP_TYPE_ANI_BUSH			0
#define WORLD_MAP_TYPE_ANI_HELP			1
#define WORLD_MAP_TYPE_ANI_GOLD_DIGGER_WALK_RIGHT	2
#define WORLD_MAP_TYPE_ANI_MARIO		3
#define WORLD_MAP_TYPE_ANI_GOLD_DIGGER_WALK_LEFT	4
#define WORLD_MAP_TYPE_STAGE_CLEAR_ANI_IN_PROGRESS			5	
#define WORLD_MAP_TYPE_STAGE_CLEAR_ANI_FINISH		6

#define WORLD_MAP_TYPE_MARIO		11
#define WORLD_MAP_TYPE_HELP			22
#define WORLD_MAP_TYPE_GOLD_DIGGER	33
#define WORLD_MAP_TYPE_BUSH			44
#define WORLD_MAP_TYPE_STAGE		55



class CWorldMapObjects : public CGameObject
{
	int type;

	DWORD gold_digger_walk_start = 0;
	DWORD help_appear_start = 0;
	int isHelpAppear = 1;

	DWORD mario_move_start = 0;

	bool isAllowToRenderClearStage = false;

	DWORD timing_clear = 0;
	bool isRenderStageInProgress = false;
	
	bool mario_move_control = true;
public:
	CWorldMapObjects(int ctype);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {}
	virtual void SetState(int state);
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	void GoldDiggerStartWalking()
	{
		if (gold_digger_walk_start == 0)
			gold_digger_walk_start = GetTickCount();
	}
	void HelpAppearStart()
	{
		if (help_appear_start == 0)
			help_appear_start = GetTickCount();
	}
	void MarioMoveStart()
	{
		if (mario_move_start == 0)
			mario_move_start = GetTickCount();
	}
	bool GetMarioMoveControl()
	{
		return mario_move_control;
	}
	
	void StartClearingStage()
	{
		if (timing_clear == 0)
		{
			timing_clear = GetTickCount();
		}
	}
};