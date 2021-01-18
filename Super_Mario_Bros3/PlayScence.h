#pragma once
#include "Game.h"
#include "GameObject.h"
#include "Textures.h"
#include "Scence.h"
#include "HUD.h"
#include "Brick.h"
#include "Mario.h"
#include "Goomba.h"
#include "Koopas.h"
#include "Coin.h"
#include "Pipe.h"
#include "Rectangle.h"
#include "NoCollisionObjects.h"
#include "FlowerBullet.h"
#include "QuestionBrick.h"
#include "Utils.h"
#include "Textures.h"
#include "Sprites.h"
#include "Portal.h"
#include "FireBullet.h"
#include "Flower.h"
#include "FlowerBullet.h"
#include "Leaf.h"
#include "MushRoom.h"
#include "BreakableBrick.h"
#include "Bell.h"
#include "Special_Item.h"
#include "Score.h"
#include "WordsEndScene.h"
#include "BreakableBrickAnimation.h"
#include "Tail.h"
#include "Map.h"
#include "NewMapCam.h"
#include "MovingHorizontalRectangle.h"
#include"Boomerang.h"
#include "BoomerangEnemy.h"
#include "Grid.h"

#define IN_USE_WIDTH		330
#define IN_USE_HEIGHT		300


#define GRID_SECTION_SETTINGS	1
#define GRID_SECTION_OBJECTS	2

#define MOVE_CAM_X_VX		0.1f

class CPlayScene : public CScene
{
protected:
	CMario *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;

	//CGrid* grid;


	Map* map;

	vector<CHUD*>  timers;
	vector<CHUD*>  scores;
	vector<CHUD*>  moneys;
	vector<CHUD*>  normarl_stacks;
	vector<CHUD*>  items;


	vector<LPGAMEOBJECT> scores_panel;

	CHUD* max_stack;

	int time_picker = 300;

	int cam_state;

	vector<CNewMapCam*> new_map_cams;

	DWORD time_counter = 0;

	DWORD time_cam_move = 0;



	float cam_x_diff = 0;
	float cam_y_diff = 0;

	void _ParseSection_TEXTURES(string line);
	void _ParseSection_SPRITES(string line);
	void _ParseSection_ANIMATIONS(string line);
	void _ParseSection_ANIMATION_SETS(string line);
	void _ParseSection_OBJECTS(string line);
	void _ParseSection_MAP(string line);
	//void _ParseSection_GRID(string line);


public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	//bool IsInUseArea(float x, float y);

	CMario * GetPlayer() { return player; }

	int GetTimePicker()
	{
		return time_picker;
	}

	void StartTimeCounter()
	{
		if (time_counter == 0)
			time_counter = GetTickCount();
	}

	void StartTimeCamMove()
	{
		if (time_cam_move == 0)
			time_cam_move = GetTickCount();
	}

	

	vector<LPGAMEOBJECT> GetScoresPanel()
	{
		return scores_panel;
	}

	vector<CHUD*> GetTimers()
	{
		return timers;
	}
	vector<CHUD*> GetScores()
	{
		return scores;
	}
	vector<CHUD*> GetMoneys()
	{
		return moneys;
	}
	vector<CHUD*> GetNormalStacks()
	{
		return normarl_stacks;
	}
	CHUD* GetMaxStack()
	{
		return max_stack;
	}
	vector<CHUD*> GetItems()
	{
		return items;
	}
	float GetCamXDiff()
	{
		return cam_x_diff;
	}
	float GetCamYDiff()
	{
		return cam_y_diff;
	}
	void SetTimePicker(int timePickerInt)
	{
		this->time_picker = timePickerInt;
	}
	void SetCamState(int camStateInt)
	{
		cam_state = camStateInt;
	}
	int GetCamState()
	{
		return cam_state;
	}
	void SetTimeDown()
	{
		if (time_picker >= 50)
		{
			this->time_picker -= 50;
		}
		else
		{
			this->time_picker -= (this->time_picker % 50);
		}
	}
	float UpdateCamMoveX(DWORD dt);
	
	//friend class CPlayScenceKeyHandler;
};

class CPlayScenceKeyHandler : public CScenceKeyHandler
{

public:
	virtual void KeyState(BYTE *states);
	virtual void OnKeyDown(int KeyCode);
	virtual void OnKeyUp(int KeyCode);
	CPlayScenceKeyHandler(CScene *s) :CScenceKeyHandler(s) {};

};

