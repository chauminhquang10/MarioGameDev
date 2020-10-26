#pragma once
#include "GameObject.h"

#define NoCollisionObjects_BBOX_WIDTH  16
#define NoCollisionObjects_BBOX_HEIGHT 16

class CNoCollisionObjects : public CGameObject
{
public:
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
