#pragma once
#include "GameObject.h"

#define PIPE_BBOX_WIDTH  32
#define PIPE_BBOX_HEIGHT 16

#define PIPE_BBOX_WIDTH_NORMAL	16


#define PIPE_TYPE_NORMAL	100
#define PIPE_TYPE_DOWN		200
#define PIPE_TYPE_UP		300

class CPipe : public CGameObject
{
	int type;
public:
	CPipe(int ctype);
	int GetType()
	{
		return type;
	}
	void SetType(int ctype)
	{
		type = ctype;
	}
	virtual void Render();
	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
};