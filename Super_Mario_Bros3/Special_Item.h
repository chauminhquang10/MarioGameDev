#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include"Define.h"

#define SPECIAL_ITEM_BBOX_WIDTH		16
#define SPECIAL_ITEM_BBOX_HEIGHT	16

#define SPECIAL_ITEM_STATE_FLOWER_IDLE			100
#define SPECIAL_ITEM_STATE_MUSHROOM_IDLE		200
#define SPECIAL_ITEM_STATE_STAR_IDLE			300

#define SPECIAL_ITEM_STATE_FLOWER_UP			400
#define SPECIAL_ITEM_STATE_MUSHROOM_UP			500
#define SPECIAL_ITEM_STATE_STAR_UP				600

#define SPECIAL_ITEM_STATE_FLOWER_IDLE_ANI			0
#define SPECIAL_ITEM_STATE_MUSHROOM_IDLE_ANI		1
#define SPECIAL_ITEM_STATE_STAR_IDLE_ANI			2

#define SPECIAL_ITEM_STATE_FLOWER_UP_ANI			3
#define SPECIAL_ITEM_STATE_MUSHROOM_UP_ANI			4
#define SPECIAL_ITEM_STATE_STAR_UP_ANI				5



class CSpecial_Item : public CGameObject
{
	bool isAppear=true;
	DWORD upping_start = 0;
	DWORD switching_state_start= 0;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CSpecial_Item();
	virtual void SetState(int state);

	void StartUpping()
	{
		if(upping_start==0)
		upping_start = GetTickCount();
	}
	void StartSwitchState()
	{
		if (switching_state_start == 0)
			switching_state_start = GetTickCount();
	}

};