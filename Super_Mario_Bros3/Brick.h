#pragma once
#include "GameObject.h"

#define BRICK_BBOX_WIDTH  16
#define BRICK_BBOX_HEIGHT 16


#define INTRO_BRICK_BBOX_WIDTH  256
#define INTRO_BRICK_BBOX_HEIGHT 38

class CBrick : public CGameObject
{
	
public:

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};