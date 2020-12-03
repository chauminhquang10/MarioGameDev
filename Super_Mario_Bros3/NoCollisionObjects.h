#pragma once
#include "GameObject.h"



#define NoCollisionObjects_TYPE_NORMAL	1
#define NoCollisionObjects_TYPE_NUMBER_THREE	2
#define NoCollisionObjects_TYPE_BUSH_INTRO_SCENE	3
class CNoCollisionObjects : public CGameObject
{
	bool isRender ;
	int type;
public:
	CNoCollisionObjects(int scene_id,int ctype);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
