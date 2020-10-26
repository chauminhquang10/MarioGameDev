#pragma once
#include "GameObject.h"

#define RECTANGLE_BBOX_WIDTH  16
#define RECTANGLE_BBOX_HEIGHT 16

class CRectangle : public CGameObject
{

public:

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
