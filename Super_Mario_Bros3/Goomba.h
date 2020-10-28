#pragma once
#include "GameObject.h"
#include "algorithm"

#define GOOMBA_WALKING_SPEED 0.05f
#define GOOMBA_GRAVITY 0.002f
#define GOOMBA_DIE_DEFLECT_SPEED	0.5f
#define GOOMBA_BBOX_WIDTH 16
#define GOOMBA_BBOX_HEIGHT 15
#define GOOMBA_BBOX_HEIGHT_DIE 9

#define GOOMBA_STATE_WALKING	100
#define GOOMBA_STATE_DIE		200
#define GOOMBA_STATE_DISAPPEAR  300
#define GOOMBA_STATE_DIE_2		400

#define GOOMBA_NORMAL_ANI_WALKING   0
#define GOOMBA_NORMAL_ANI_DIE		1
#define GOOMBA_RED_FLY_ANI_WALKING	2
#define GOOMBA_RED_FLY_ANI_DIE		3	

#define GOOMBA_NORMAL	888
#define GOOMBA_RED_FLY	999


class CGoomba : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
protected:
	int type;
public:
	CGoomba(int ctype);
	virtual void SetState(int state);
};