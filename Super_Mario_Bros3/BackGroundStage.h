#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include "Define.h"




class CBackGroundStage : public CGameObject
{
	DWORD time_showing = 0;
	DWORD time_down = 0;
	DWORD time_shake = 0;

	int type;
	int shakeDirection = 1;
	bool isAppear = true;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	CBackGroundStage(int ctype);
	virtual void SetState(int state);
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	void StartShowing()
	{
		if (time_showing == 0)
			time_showing = GetTickCount();
	}
	void StartDown()
	{
		if (time_down == 0)
			time_down = GetTickCount();
	}
	void StartShake()
	{
		if (time_shake == 0)
			time_shake = GetTickCount();
	}
	bool GetIsAppear()
	{
		return isAppear;
	}
	void SetIsAppear(bool IsAppearBool)
	{
		this->isAppear = IsAppearBool;
	}
};
