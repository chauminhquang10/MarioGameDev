//#pragma once
//#include "GameObject.h"
//#include "Mario.h"
//#include "PlayScence.h"
//#include "Scence.h"
//#include <algorithm>
//
//#define TAIL_BBOX_WIDTH  8
//#define TAIL_BBOX_HEIGHT 8
//
//#define TAIL_FLYING_SPEED	0.1f 
//
//#define TAIL_STATE_FLYING		100
//#define TAIL_STATE_IDLE		200
//
//
//
//class CTail : public CGameObject
//{
//
//	
//	bool isAllowToShowScore = false;
//	DWORD timing_score;
//public:
//	virtual void Render();
//	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
//	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
//	virtual void GetBoundingBox(float &l, float &t, float &r, float &b);
//	void SetState(int state);
//	bool GetIsAllowToShowScore()
//	{
//		if (this != NULL)
//			return isAllowToShowScore;
//		else
//		{
//			return false;
//		}
//	}
//	void SetIsAllowToShowScore(bool isAllowToShowScoreBool)
//	{
//		if (this != NULL)
//			isAllowToShowScore = isAllowToShowScoreBool;
//	}
//	void StartTimingScore()
//	{
//		timing_score = GetTickCount();
//	}
//	CTail();
//};
