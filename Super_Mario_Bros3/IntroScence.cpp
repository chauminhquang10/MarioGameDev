#include "IntroScence.h"

#include <iostream>
#include <fstream>

using namespace std;



CIntroScence::CIntroScence(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CIntroScenceKeyHandler(this);
	CGame::GetInstance()->SetCamPos();
}

CIntroScence::~CIntroScence()
{
}


#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO_RED			 0
#define OBJECT_TYPE_BRICK				 1
#define OBJECT_TYPE_GOOMBA_NORMAL		 2
#define OBJECT_TYPE_KOOPAS_NORMAL		 3
#define OBJECT_TYPE_NO_COLLISION_OBJECTS 4
#define OBJECT_TYPE_STAR				 5
#define OBJECT_TYPE_BACKGROUND_STAGE	 6
#define OBJECT_TYPE_LEAF				 7
#define OBJECT_TYPE_MUSHROOM_RED		 8
#define OBJECT_TYPE_MENU_GAME			 9
#define OBJECT_TYPE_MARIO_GREEN			 10

#define OBJECT_TYPE_PORTAL	50


#define MARIO_TIME_COUNT  1000

#define MAX_SCENE_LINE 1024


void CIntroScence::_ParseSection_TEXTURES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 5) return; // skip invalid lines

	int texID = atoi(tokens[0].c_str());
	wstring path = ToWSTR(tokens[1]);
	int R = atoi(tokens[2].c_str());
	int G = atoi(tokens[3].c_str());
	int B = atoi(tokens[4].c_str());

	CTextures::GetInstance()->Add(texID, path.c_str(), D3DCOLOR_XRGB(R, G, B));
}

void CIntroScence::_ParseSection_SPRITES(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 6) return; // skip invalid lines

	int ID = atoi(tokens[0].c_str());
	int l = atoi(tokens[1].c_str());
	int t = atoi(tokens[2].c_str());
	int r = atoi(tokens[3].c_str());
	int b = atoi(tokens[4].c_str());
	int texID = atoi(tokens[5].c_str());

	LPDIRECT3DTEXTURE9 tex = CTextures::GetInstance()->Get(texID);
	if (tex == NULL)
	{
		DebugOut(L"[ERROR] Texture ID %d not found!\n", texID);
		return;
	}

	CSprites::GetInstance()->Add(ID, l, t, r, b, tex);
}

void CIntroScence::_ParseSection_ANIMATIONS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 3) return; // skip invalid lines - an animation must at least has 1 frame and 1 frame time

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	LPANIMATION ani = new CAnimation();

	int ani_id = atoi(tokens[0].c_str());
	for (int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
	{
		int sprite_id = atoi(tokens[i].c_str());
		int frame_time = atoi(tokens[i + 1].c_str());
		ani->Add(sprite_id, frame_time);
	}

	CAnimations::GetInstance()->Add(ani_id, ani);
}

void CIntroScence::_ParseSection_ANIMATION_SETS(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 2) return; // skip invalid lines - an animation set must at least id and one animation id

	int ani_set_id = atoi(tokens[0].c_str());

	LPANIMATION_SET s = new CAnimationSet();

	CAnimations *animations = CAnimations::GetInstance();

	for (int i = 1; i < tokens.size(); i++)
	{
		int ani_id = atoi(tokens[i].c_str());

		LPANIMATION ani = animations->Get(ani_id);
		s->push_back(ani);
	}

	CAnimationSets::GetInstance()->Add(ani_set_id, s);
}

/*
	Parse a line in section [OBJECTS]
*/
void CIntroScence::_ParseSection_OBJECTS(string line)
{

	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = atof(tokens[1].c_str());
	float y = atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO_RED:
		obj = new CMario(1, x, y);
		player1 = (CMario*)obj;
		DebugOut(L"[INFO] Player1 object created!\n");
		break;
	case OBJECT_TYPE_MARIO_GREEN:
		obj = new CMario(2, x, y);
		player2 = (CMario*)obj;
		DebugOut(L"[INFO] Player2 object created!\n");
		break;
	/*case OBJECT_TYPE_GOOMBA_NORMAL: obj = new CGoomba(888); break;*/
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	//case OBJECT_TYPE_KOOPAS_NORMAL: obj = new CKoopas(111); break;
	case OBJECT_TYPE_NO_COLLISION_OBJECTS:obj = new CNoCollisionObjects(); break;
	/*case OBJECT_TYPE_LEAF:	           obj = new CLeaf(); break;*/
	/*case OBJECT_TYPE_MUSHROOM_RED:	   obj = new CMushRoom(567); break;*/
		/*case OBJECT_TYPE_STAR:				obj = new CStar(); break;
		case OBJECT_TYPE_BACKGROUND_STAGE:  obj = new CBackgroundStage(); break;
		case OBJECT_TYPE_MENU_GAME:	           obj = new CMenuGame(); break; */
	//case OBJECT_TYPE_PORTAL:
	//{
	//	float r = atof(tokens[4].c_str());
	//	float b = atof(tokens[5].c_str());
	//	int scene_id = atoi(tokens[6].c_str());
	//	obj = new CPortal(x, y, r, b, scene_id);
	//}
	//break;
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);

}

void CIntroScence::Load()
{
	DebugOut(L"[INFO] Start loading scene resources from : %s \n", sceneFilePath);

	ifstream f;
	f.open(sceneFilePath);

	// current resource section flag
	int section = SCENE_SECTION_UNKNOWN;

	char str[MAX_SCENE_LINE];
	while (f.getline(str, MAX_SCENE_LINE))
	{
		string line(str);

		if (line[0] == '#') continue;	// skip comment lines	

		if (line == "[TEXTURES]") { section = SCENE_SECTION_TEXTURES; continue; }
		if (line == "[SPRITES]") {
			section = SCENE_SECTION_SPRITES; continue;
		}
		if (line == "[ANIMATIONS]") {
			section = SCENE_SECTION_ANIMATIONS; continue;
		}
		if (line == "[ANIMATION_SETS]") {
			section = SCENE_SECTION_ANIMATION_SETS; continue;
		}
		if (line == "[OBJECTS]") {
			section = SCENE_SECTION_OBJECTS; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CIntroScence::Update(DWORD dt)
{
	StartTimeCount();
	
	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<CNoCollisionObjects *>(objects[i]))
			coObjects.push_back(objects[i]);
	}

	


	player1->nx = -1;

	player2->Update(dt, &coObjects);

	player1->Update(dt, &coObjects);

	
	if (GetTickCount() - time_count >= 300 && jump_count<2)
	{
		if (player2->GetIsJumping() == false)
		{
			if(jump_count == 0)
			player2->SetState(MARIO_STATE_JUMP);
			else
			player2->SetState(MARIO_STATE_JUMP_HIGH);
			player2->SetIsJumping(true);
			jump_count++;
		}
	}
	else
	{
		player2->SetState(MARIO_STATE_WALKING_RIGHT);
	}


	if (player1->GetState() != MARIO_STATE_SITDOWN && !idle_recog)
	{
		player1->SetState(MARIO_STATE_WALKING_LEFT);
	}
	else
	{
		StartSitDownCount();
	}
	

	if (player1->GetState() == MARIO_STATE_SITDOWN && GetTickCount() - sit_down_count >= 200)
	{
		player1->SetState(MARIO_STATE_IDLE);
		idle_recog = true;
	}



	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player1 == NULL) return;

	if (player2 == NULL) return;
	// Update camera to follow mario	
	
	
	
}

void CIntroScence::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CIntroScence::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player1 = NULL;
	player2 = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CIntroScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	switch (KeyCode)
	{
		case DIK_3:
			CGame::GetInstance()->SwitchScene(3);
			break;
		case DIK_DOWN:
			// xu li chon che do 1 hay 2 nguoi choi
			break;
		case DIK_UP:
			// xu li chon che do 1 hay 2 nguoi choi
			break;
	}
}
void CIntroScenceKeyHandler::OnKeyUp(int KeyCode)
{

}
void CIntroScenceKeyHandler::KeyState(BYTE *states)
{

}