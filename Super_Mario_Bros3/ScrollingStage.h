#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"


#define SCROLLING_STAGE_STATE_IDLE		0
#define SCROLLING_STAGE_STATE_UP		100


class CScrollingStage : public CGameObject
{
	DWORD time_showing = 0;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	CScrollingStage();
	virtual void SetState(int state);

};