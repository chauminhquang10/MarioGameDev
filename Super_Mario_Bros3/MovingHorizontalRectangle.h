#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"


#define MOVING_HORIZONTAL_RECTANGLE_BBOX_WIDTH		48
#define MOVING_HORIZONTAL_RECTANGLE_BBOX_HEIGHT		16



#define  MOVING_HORIZONTAL_RECTANGLE_STATE_NORMAL		0
#define  MOVING_HORIZONTAL_RECTANGLE_STATE_DOWN			100

#define	 MOVING_HORIZONTAL_RECTANGLE_STATE_NORMAL_SPEED   -0.05f
#define  MOVING_HORIZONTAL_RECTANGLE_STATE_DOWN_SPEED	   0.04f



class CMovingHorizontalRectangle : public CGameObject
{
	bool isAppear = false;
	DWORD upping_start = 0;

	int id;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CMovingHorizontalRectangle(int moving_horizontal_rectangle_id);
	virtual void SetState(int state);
	int GetId()
	{
		return this->id;
	}
	
};