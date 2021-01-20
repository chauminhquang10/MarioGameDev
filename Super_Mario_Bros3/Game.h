#pragma once

#include <unordered_map>

#include <Windows.h>
#include <d3d9.h>
#include <d3dx9.h>
#include "Node.h"

#define DIRECTINPUT_VERSION 0x0800


#include <dinput.h>

#include "Scence.h"
#include "IntroScence.h"

using namespace std;

#define KEYBOARD_BUFFER_SIZE 1024

class CGame
{
	static CGame * __instance;

	int life = 3;

	int score = 0;

	int money = 0;
	
	bool isPassedScene1_1 = false;
	bool isPassedScene1_4 = false;

	bool isControlMarioRender = false;

	int saved_level_world_map = 1;

	int saved_Node_Id = -1;

	vector<int> items_type = { 1,1,1 };

	HWND hWnd;									// Window handle

	LPDIRECT3D9 d3d = NULL;						// Direct3D handle
	LPDIRECT3DDEVICE9 d3ddv = NULL;				// Direct3D device object

	LPDIRECT3DSURFACE9 backBuffer = NULL;
	LPD3DXSPRITE spriteHandler = NULL;			// Sprite helper library to help us draw 2D image on the screen 

	LPDIRECTINPUT8       di;		// The DirectInput object         
	LPDIRECTINPUTDEVICE8 didv;		// The keyboard device 

	BYTE  keyStates[256];			// DirectInput keyboard state buffer 
	DIDEVICEOBJECTDATA keyEvents[KEYBOARD_BUFFER_SIZE];		// Buffered keyboard data

	LPKEYEVENTHANDLER keyHandler;

	float cam_x;
	float cam_y= -50;

	int screen_width;
	int screen_height;

	unordered_map<int, LPSCENE> scenes;
	int current_scene;

	void _ParseSection_SETTINGS(string line);
	void _ParseSection_SCENES(string line);

public:

	void InitKeyboard();
	void SetKeyHandler(LPKEYEVENTHANDLER handler) { keyHandler = handler; }
	void Init(HWND hWnd);
	void Draw(float x, float y, LPDIRECT3DTEXTURE9 texture, int left, int top, int right, int bottom, int alpha = 255);

	int IsKeyDown(int KeyCode);
	void ProcessKeyboard();

	void Load(LPCWSTR gameFile);
	LPSCENE GetCurrentScene() { return scenes[current_scene]; }
	void SwitchScene(int scene_id);

	int GetScreenWidth() { return screen_width; }
	int GetScreenHeight() { return screen_height; }

	static void SweptAABB(
		float ml,			// move left 
		float mt,			// move top
		float mr,			// move right 
		float mb,			// move bottom
		float dx,			// 
		float dy,			// 
		float sl,			// static left
		float st,
		float sr,
		float sb,
		float &t,
		float &nx,
		float &ny);

	LPDIRECT3DDEVICE9 GetDirect3DDevice() { return this->d3ddv; }
	LPDIRECT3DSURFACE9 GetBackBuffer() { return backBuffer; }
	LPD3DXSPRITE GetSpriteHandler() { return this->spriteHandler; }

	void SetCamPos(float x = 0, float y = -50) { cam_x = (int)x; cam_y = (int)y; }

	static CGame * GetInstance();

	int GetLife()
	{
		return life;
	}
	void SetLifeUp()
	{
		life++;
	}
	void SetLifeDown()
	{
		if(life>=1)
		life--;
	}
	int GetMoney()
	{
		return money;
	}
	void MoneyUp()
	{
		money++;
	}
	vector<int> GetItemsTypeRender()
	{
		return items_type;
	}
	float GetCamX()
	{
		return cam_x;
	}
	float GetCamY()
	{
		return cam_y;
	}
	void SetCamX(float cam_x_FLoat)
	{
		cam_x = cam_x_FLoat;
	}
	void SetCamY(float cam_y_FLoat)
	{
		cam_y = cam_y_FLoat;
	}
	void SetItems(vector<int> vector_items)
	{
		items_type = vector_items;
	}
	int GetScore()
	{
		return score;
	}
	void ScoreUp(int scoreInt)
	{
		score += scoreInt;
	}
	bool GetIsPassedScene1_1()
	{
		return isPassedScene1_1;
	}
	void SetIsPassedScene1_1(bool isPassedScene1_1_Bool)
	{
		isPassedScene1_1 = isPassedScene1_1_Bool;
	}
	bool GetIsPassedScene1_4()
	{
		return isPassedScene1_4;
	}
	void SetIsPassedScene1_4(bool isPassedScene1_4_Bool)
	{
		isPassedScene1_4 = isPassedScene1_4_Bool;
	}
	bool GetIsControlMarioRenderWorldMap()
	{
		return isControlMarioRender;
	}
	void SetControlMarioRenderWorldMap(bool isControlMarioRenderBool)
	{
		isControlMarioRender = isControlMarioRenderBool;
	}
	int GetSavedNodeID() 
	{
		return saved_Node_Id;
	}
	void SetSavedNodeID(int saved_Node_Id_Int)
	{
		saved_Node_Id = saved_Node_Id_Int;
	}
	int GetMarioLevelWorldMap()
	{
		return saved_level_world_map;
	}
	void SetMarioLevelWorldMap(int saved_level_world_map_Int)
	{
		saved_level_world_map = saved_level_world_map_Int;
	}
	~CGame();
};


