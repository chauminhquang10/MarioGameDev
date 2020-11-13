#pragma once
#include "GameObject.h"

#define COIN_BBOX_WIDTH  10
#define COIN_BBOX_HEIGHT 16

class CCoin : public CGameObject
{
	bool disappear=false;
public:
	void SetDisappear(bool disappearBool)
	{
		disappear = disappearBool;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	

};