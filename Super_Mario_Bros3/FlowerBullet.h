#pragma once
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "Scence.h"
#include "Flower.h"
#include <algorithm>
#include "Define.h"

#define FLOWER_BULLET_BBOX_WIDTH  8
#define FLOWER_BULLET_BBOX_HEIGHT 9

#define FLOWER_BULLET_X_LIMIT		72


#define FLOWER_BULLET_FLYING_SPEED		0.05f 

#define FLOWER_BULLET_STATE_FLYING		100
#define FLOWER_BULLET_STATE_HIDDEN		200

#define FLOWER_BULLET_ANI			0

class CFlowerBullet : public CGameObject
{
	bool isUsed;
	
public:
	virtual void Render();
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	void SetState(int state);

	CFlowerBullet() 
	{ 
		isUsed = false;
		SetState(FLOWER_BULLET_STATE_HIDDEN);
	}
};
