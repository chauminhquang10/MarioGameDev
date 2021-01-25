#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include "Special_Item.h"
#include "Define.h"


#define WORDS_END_SCENE_TYPE_COURSE_CLEAR_ANI		0
#define WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD_ANI		1
#define WORDS_END_SCENE_TYPE_ITEM_ANI_MUSHROOM		2
#define WORDS_END_SCENE_TYPE_ITEM_ANI_FLOWER		3
#define WORDS_END_SCENE_TYPE_ITEM_ANI_STAR			4


#define WORDS_END_SCENE_TYPE_COURSE_CLEAR		111
#define WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD		222
#define WORDS_END_SCENE_TYPE_ITEM				333


class CWordsEndScene : public CGameObject
{
	
	int type;
	bool isAppear;
	DWORD timing_recog_start;

public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	CWordsEndScene(int ctype);
	virtual void SetState(int state);
	int GetType()
	{
		return type;
	}
	void SetType(int typeInt)
	{
		type = typeInt;
	}
	bool GetIsAppear()
	{
		return isAppear;
	}
	void SetIsAppear(bool IsAppearBool)
	{
		this->isAppear = IsAppearBool;
	}
	void StartTimeRecog()
	{
		if(timing_recog_start==0)
		timing_recog_start = GetTickCount();
	}
};
