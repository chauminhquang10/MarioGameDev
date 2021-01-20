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
#include "FireFlower.h"
#include"Cell.h"
#include <iostream>
#include <fstream>
#include "HitEffect.h"

#define TUNNEL_CAM_Y	980

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6
#define SCENE_SECTION_MAP				7
#define SCENE_SECTION_GRID				8

#define OBJECT_TYPE_MARIO				 0
#define OBJECT_TYPE_BRICK				 1
#define OBJECT_TYPE_GOOMBA_NORMAL		 2
#define OBJECT_TYPE_KOOPAS_XANH_WALK	 3
#define OBJECT_TYPE_NO_COLLISION_OBJECTS 4
#define OBJECT_TYPE_RECTANGLE			 5
#define OBJECT_TYPE_PIPE_NORMAL			 6
#define OBJECT_TYPE_KOOPAS_XANH_BAY		 7 
#define OBJECT_TYPE_KOOPAS_RED_WALK		 8
#define OBJECT_TYPE_COIN_NORMAL			 10
#define OBJECT_TYPE_GOOMBA_RED_FLY  11 
#define OBJECT_TYPE_FIRE_BULLET		12
#define OBJECT_TYPE_FLOWER_RED		13
#define OBJECT_TYPE_FLOWER_BULLET	14
#define OBJECT_TYPE_COIN_CAN_MOVE	15
#define OBJECT_TYPE_LEAF			16
#define OBJECT_TYPE_MUSHROOM_RED		17
#define OBJECT_TYPE_QUESTION_BRICK_HAVE_LEAF	18
#define OBJECT_TYPE_MUSHROOM_GREEN		19
#define OBJECT_TYPE_QUESTION_BRICK_JUST_HAVE_MUSHROOM	20
#define OBJECT_TYPE_FLOWER_GREEN				21
#define OBJECT_TYPE_FLOWER_GREEN_CAN_SHOOT		22
#define OBJECT_TYPE_BREAKABLE_BRICK				23
#define OBJECT_TYPE_BELL						24

#define OBJECT_TYPE_HUD_PANEL			25
#define OBJECT_TYPE_MARIO_LUIGI			26
#define OBJECT_TYPE_LIFE				27
#define OBJECT_TYPE_MONEY				28
#define OBJECT_TYPE_SCORE				29
#define OBJECT_TYPE_TIME_PICKER			30
#define OBJECT_TYPE_WORLD				31
#define OBJECT_TYPE_STACK_NORMAL		32
#define OBJECT_TYPE_STACK_MAX			33
#define OBJECT_TYPE_ITEM				34

#define OBJECT_TYPE_BLACK_BLACK			35
#define OBJECT_TYPE_SPECIAL_ITEM		36

#define OBJECT_TYPE_PIPE_DOWN			37
#define OBJECT_TYPE_PIPE_UP				38
#define OBJECT_TYPE_SCORE_AND_1LV		39

#define OBJECT_TYPE_WORDS_END_SCENE_COURSE_CLEAR		40
#define OBJECT_TYPE_WORDS_END_SCENE_YOU_GOT_A_CARD		41
#define OBJECT_TYPE_WORDS_END_SCENE_ITEM				42

#define OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP				43
#define OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP			44
#define OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM			45
#define OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM			46

#define OBJECT_TYPE_NEW_MAP_CAM											47
#define OBJECT_TYPE_QUESTION_BRICK_HAVE_MULTIPLE_LIFE					48
#define OBJECT_TYPE_MOVING_HORIZONTAL_RECTANGLE							49

#define OBJECT_TYPE_BOOMERANG											50
#define OBJECT_TYPE_BOOMERANG_ENEMY										51

#define OBJECT_TYPE_KOOPAS_RED_FLY										52

#define OBJECT_TYPE_FIRE_FLOWER											53

#define OBJECT_TYPE_HIT_EFFECT_TURN_TAIL								54

#define OBJECT_TYPE_HIT_EFFECT_FIRE_BULLET								55

#define OBJECT_TYPE_PORTAL												56

#define CAM_X_BONUS	50

#define MAX_SCENE_LINE 1024



#define IN_USE_WIDTH		330
#define IN_USE_HEIGHT		300


#define GRID_SECTION_SETTINGS	1
#define GRID_SECTION_OBJECTS	2

#define MOVE_CAM_X_VX		0.1f



#define GRID_SECTION_SETTINGS	1
#define GRID_SECTION_OBJECTS	2
#define MAX_GRID_LINE 1024

class CGrid
{
	int numRow, numCol;
	int cellWidth;
	int  cellHeight;
	Cell** cells;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_OBJECTS(string line);
public:
	CGrid() {}
	CGrid(LPCWSTR filePath);
	void GetObjects(vector<LPGAMEOBJECT>& listObject, int playerX, int playerY);
	void Load(LPCWSTR filePath);
	void Unload();
};

class CPlayScene : public CScene
{
protected:
	CMario *player;					// A play scene has to have player, right? 

	vector<LPGAMEOBJECT> objects;

	CGrid* grid;

	Map* map;

	vector<CHUD*>  timers;
	vector<CHUD*>  scores;
	vector<CHUD*>  moneys;
	vector<CHUD*>  normarl_stacks;
	vector<CHUD*>  items;


	vector<LPGAMEOBJECT> scores_panel;

	/*vector<LPGAMEOBJECT> hit_effects_turn_tail;
	vector<LPGAMEOBJECT> hit_effects_fire_bullet;
*/
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
	void _ParseSection_GRID(string line);


public:
	CPlayScene(int id, LPCWSTR filePath);

	virtual void Load();
	virtual void Update(DWORD dt);
	virtual void Render();
	virtual void Unload();

	bool IsInUseArea(float x, float y);

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

	/*vector<LPGAMEOBJECT> GetHitEffectsTurnTail()
	{
		return hit_effects_turn_tail;
	}

	vector<LPGAMEOBJECT> GetHitEffectsFireBullet()
	{
		return hit_effects_fire_bullet;
	}*/

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

