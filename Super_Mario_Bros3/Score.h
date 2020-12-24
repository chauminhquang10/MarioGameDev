#pragma once

#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"



#define SCORE_STATE_IDLE	0
#define SCORE_STATE_UP		100


#define SCORE_TYPE_100			100
#define SCORE_TYPE_200			200	
#define SCORE_TYPE_400			400
#define SCORE_TYPE_800			800
#define SCORE_TYPE_1000			1000
#define SCORE_TYPE_2000			2000
#define SCORE_TYPE_4000			4000
#define SCORE_TYPE_8000			8000
#define SCORE_TYPE_1LIFE		900




#define SCORE_100_ANI			0
#define SCORE_200_ANI			1	
#define SCORE_400_ANI			2
#define SCORE_800_ANI			3
#define SCORE_1000_ANI			4
#define SCORE_2000_ANI			5
#define SCORE_4000_ANI			6
#define SCORE_8000_ANI			7
#define SCORE_1LIFE_ANI			8


class CScore : public CGameObject
{
	bool isUsed = false;
	int value=0;
	bool isAppear = false;

	DWORD timing_start = 0;
	bool score_recog = false;
public:

	CScore();
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	virtual void SetState(int state);
	void StartTiming()
	{
		if (timing_start == 0)
			timing_start = GetTickCount();
	}
	void SetIsUsed(bool isUsedBool)
	{
		isUsed = isUsedBool;
	}
	bool GetIsUsed()
	{
		return isUsed;
	}
	void SetValue(int valueInt)
	{
		value = valueInt;
	}
	void SetIsAppear(bool isAppearBool)
	{
		isAppear = isAppearBool;
	}
	bool GetIsAppear()
	{
		return isAppear;
	}
	

};