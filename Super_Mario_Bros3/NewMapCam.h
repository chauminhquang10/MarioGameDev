 #pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"



class CNewMapCam : public CGameObject
{
	int id;
	float y_limit;
	float start_cam_x;
	float end_cam_x;
	float y_start;

public:
	virtual void GetBoundingBox(float& left, float& top, float& right, float& bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT>* coObjects) {};
	virtual void Render() {};
	CNewMapCam(int id, float start_cam_x, float y_limit, float end_cam_x, float y_start);
	virtual void SetState(int state) {};
	float GetStartCamX()
	{
		return start_cam_x;
	}
	float GetEndCamX()
	{
		return end_cam_x;
	}
	float GetCamYLimit()
	{
		return y_limit;
	}
	float GetYStart()
	{
		return y_start;
	}
	int GetId()
	{
		return id;
	}
};