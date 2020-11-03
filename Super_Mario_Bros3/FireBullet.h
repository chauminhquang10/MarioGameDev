#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Scence.h"
#include <algorithm>

#define FIRE_BULLET_BBOX_WIDTH  9
#define FIRE_BULLET_BBOX_HEIGHT 9

#define FIRE_BULLET_FLYING_SPEED	0.1f 
#define FIRE_BULLET_GRAVITY         0.0009f

#define FIRE_BULLET_STATE_FLYING		100
#define FIRE_BULLET_STATE_HIDDEN		200

#define FIRE_BULLET_ANI_RIGHT		0
#define FIRE_BULLET_ANI_LEFT		1

class CFireBullet : public CGameObject
{
	bool isUsed;
	float Height_Limit;

public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);

	CFireBullet();
};
