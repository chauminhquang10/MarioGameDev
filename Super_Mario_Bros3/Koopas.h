#pragma once

#include "GameObject.h"
#include "Mario.h"

#define KOOPAS_WALKING_SPEED 0.03f
#define KOOPAS_GRAVITY 0.002f
#define KOOPAS_DIE_DEFLECT_SPEED 0.5f
#define KOOPAS_BBOX_WIDTH 16
#define KOOPAS_BBOX_HEIGHT 26
#define KOOPAS_BBOX_HEIGHT_SHELL 16
#define KOOPAS_BBOX_HEIGHT_DIE 16

#define KOOPAS_STATE_WALKING	100
#define KOOPAS_STATE_SHELL		200
#define KOOPAS_STATE_DIE		300
#define KOOPAS_STATE_SPINNING	400

#define KOOPAS_XANH_ANI_WALKING_LEFT  0
#define KOOPAS_XANH_ANI_WALKING_RIGHT 1
#define KOOPAS_XANH_ANI_FLYING_LEFT   2

#define KOOPAS_XANH_MAI_ANI_UP		  3
#define KOOPAS_XANH_MAI_ANI_NGUA	  4	
#define KOOPAS_RED_ANI_WALKING_LEFT   5
#define KOOPAS_RED_ANI_FLYING_LEFT	  6
#define KOOPAS_RED_MAI_ANI_UP		  7
#define KOOPAS_XANH_MAI_ANI_SPINNING  8
#define KOOPAS_RED_MAI_ANI_SPINNING	  9

#define KOOPAS_XANH_WALK	111
#define KOOPAS_XANH_FLY		222
#define KOOPAS_RED_WALK		333
#define KOOPAS_RED_FLY		444



class CKoopas : public CGameObject
{
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	int type;
	
	

public:
	
	
	CKoopas(int ctype);
	virtual void SetState(int state);
};