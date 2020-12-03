#pragma once
#include "GameObject.h"

#define MENU_GAME_ANI_1_PLAYER	0
#define MENU_GAME_ANI_2_PLAYER	1

class CMenuGame : public CGameObject
{
	bool isRender=false;
public:
	CMenuGame();
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};
