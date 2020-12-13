//#pragma once
//#include "GameObject.h"
//#include "algorithm"
//#include "Mario.h"
//#include "PlayScence.h"
//
//
//#define SPECIAL_ITEM_BBOX_WIDTH		16
//#define SPECIAL_ITEM_BBOX_HEIGHT	16
//
//
//
//#define SPECIAL_ITEM_STATE_IDLE		0
//#define SPECIAL_ITEM_STATE_UP		100
//
//
//
//#define BELL_ANI_ALIVE		0
//#define BELL_ANI_DIE		1
//
//
//class CBell : public CGameObject
//{
//	bool isAppear = false;
//	DWORD upping_start = 0;
//
//public:
//	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
//	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
//	virtual void Render();
//	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
//	CBell();
//	virtual void SetState(int state);
//
//	void StartUpping()
//	{
//		upping_start = GetTickCount();
//	}
//
//};