#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"

#define QUESTION_BRICK_BBOX_WIDTH  16
#define QUESTION_BRICK_BBOX_HEIGHT 16


#define QUESTION_BRICK_ANI_ALIVE	0
#define QUESTION_BRICK_ANI_DEAD		1



class CQuestionBrick : public CGameObject
{
	
	bool isAlive = true;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CQuestionBrick();
	virtual void SetState(int state);

};