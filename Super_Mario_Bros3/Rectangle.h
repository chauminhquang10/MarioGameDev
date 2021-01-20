#pragma once
#include "GameObject.h"
#include "PlayScence.h"

#define RECTANGLE_BBOX_WIDTH  16
#define RECTANGLE_BBOX_HEIGHT 16

class CRectangle : public CGameObject
{
	bool isAllowToSubBBoxWidth = false;

public:

	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	bool GetIsAllowToSubBBoxWidth()
	{
		return isAllowToSubBBoxWidth;
	}
	void SetIsAllowToSubBBoxWidth(bool isAllowToSubBBoxWidthBool)
	{
		isAllowToSubBBoxWidth = isAllowToSubBBoxWidthBool;
	}
};
