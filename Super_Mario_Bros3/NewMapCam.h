#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"



class CNewMapCam : public CGameObject
{
	int id;
	float start_cam_x;
	float end_cam_x;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom) {};
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects) {};
	virtual void Render() {};
	CNewMapCam(int id, float start_cam_x , float end_cam_x) ;
	virtual void SetState(int state) {};
	float GetStartCamX()
	{
		return start_cam_x;
	}
	float GetEndCamX()
	{
		return start_cam_x;
	}

};