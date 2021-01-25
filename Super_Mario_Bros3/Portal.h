#pragma once

#include "GameObject.h"
#include "Playscence.h"
#include "Define.h"

#define PORTAL_WIDTH	32
#define PORTAL_HEIGHT	10
/*
	Object that triggers scene switching
*/
class CPortal : public CGameObject
{
	int portal_id;	// target scene to switch to 

	float	arrive_position_x;
	float   arrive_position_y;

public:
	CPortal(int portal_id, float arrive_position_x, float arrive_position_y);
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *colliable_objects = NULL);
	int GetPortalId() { return portal_id; }
};