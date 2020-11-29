#pragma once
#include "Game.h"
#include "Textures.h"
#include "Scence.h"
#include "GameObject.h"
#include "Mario.h"
#include "PlayScence.h"
#include "ScrollingStage.h"
class CIntroScence : public  CScene
{
protected:

	CMario* player1;	// MARIO RED
	CMario* player2;	// MARIO GREEN
	vector<LPGAMEOBJECT> objects;



	DWORD time_count =  0;
	DWORD sit_down_count = 0;



	int jump_count = 0;

	bool isAllowToWalkGreen = true;
	bool isAllowToWalkRed = true;


	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);

public:
	CIntroScence(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	void StartTimeCount()
	{
		if (time_count == 0)
			time_count = GetTickCount();
	}


	void StartSitDownCount()
	{
		if (sit_down_count == 0)
			sit_down_count = GetTickCount();
	}

	CMario * GetPlayer1() { return player1; }

	CMario * GetPlayer2() { return player2; }

	~CIntroScence();
};

class CIntroScenceKeyHandler : public CScenceKeyHandler
{

public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CIntroScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};
};

