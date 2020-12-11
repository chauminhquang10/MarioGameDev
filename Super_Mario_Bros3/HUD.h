#pragma once

#include "GameObject.h"
#include "algorithm"



#define HUD_TYPE_PANEL			11
#define	HUD_TYPE_WORLD			22
#define HUD_TYPE_LIFE			33
#define HUD_TYPE_TIME_PICKER	44
#define HUD_TYPE_SCORE			55
#define HUD_TYPE_MONEY			66
#define HUD_TYPE_MARIO_LUIGI	77
#define HUD_TYPE_STACK_NORMAL	88
#define HUD_TYPE_STACK_MAX		99
#define HUD_TYPE_ITEM			100

#define	HUD_TYPE_WORLD_ANI			1
#define	HUD_TYPE_MARIO_LUIGI_ANI	0
#define HUD_TYPE_PANEL_ANI			0	

#define HUD_TYPE_STACK_NORMAL_ANI_EMPTY		2
#define HUD_TYPE_STACK_NORMAL_ANI_FILLED	0
#define HUD_TYPE_STACK_MAX_ANI_EMPTY		3
#define HUD_TYPE_STACK_MAX_ANI_FILLED		1
	

class CHUD : public CGameObject
{
	int type;
	
	bool stack_alive = false;

	bool max_stack_alive = false;

	bool render_recog_money = false;
	
public:
	CHUD(int ctype);
	CHUD(){}
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void Render(int timer_id);
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
	
};