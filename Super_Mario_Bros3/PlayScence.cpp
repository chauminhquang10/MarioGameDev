#include <iostream>
#include <fstream>
#include "PlayScence.h"

using namespace std;



CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/

#define SCENE_SECTION_UNKNOWN -1
#define SCENE_SECTION_TEXTURES 2
#define SCENE_SECTION_SPRITES 3
#define SCENE_SECTION_ANIMATIONS 4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS	6

#define OBJECT_TYPE_MARIO				 0
#define OBJECT_TYPE_BRICK				 1
#define OBJECT_TYPE_GOOMBA_NORMAL		 2
#define OBJECT_TYPE_KOOPAS_XANH_WALK	 3
#define OBJECT_TYPE_NO_COLLISION_OBJECTS 4
#define OBJECT_TYPE_RECTANGLE			 5
#define OBJECT_TYPE_PIPE				 6
#define OBJECT_TYPE_KOOPAS_XANH_BAY	7 
#define OBJECT_TYPE_KOOPAS_RED_WALK	8
#define OBJECT_TYPE_KOOPAS_RED_FLY	9
#define OBJECT_TYPE_COIN_NORMAL			10
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
#define OBJECT_TYPE_PORTAL	50

#define MAX_SCENE_LINE 1024


void CPlayScene::_ParseSection_TEXTURES(string line)
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

void CPlayScene::_ParseSection_SPRITES(string line)
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

void CPlayScene::_ParseSection_ANIMATIONS(string line)
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

void CPlayScene::_ParseSection_ANIMATION_SETS(string line)
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
void CPlayScene::_ParseSection_OBJECTS(string line)
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
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(x, y);
		player = (CMario*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA_NORMAL: obj = new CGoomba(888); break;
	case OBJECT_TYPE_GOOMBA_RED_FLY: obj = new CGoomba(999); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS_XANH_WALK: obj = new CKoopas(111); break;
	case OBJECT_TYPE_RECTANGLE: obj = new CRectangle(); break;
	case OBJECT_TYPE_COIN_NORMAL: obj = new CCoin(222); break;
	case OBJECT_TYPE_COIN_CAN_MOVE: obj = new CCoin(333); break;
	case OBJECT_TYPE_PIPE: obj = new CPipe(); break;
	case OBJECT_TYPE_NO_COLLISION_OBJECTS:obj = new CNoCollisionObjects(); break;
	case OBJECT_TYPE_KOOPAS_XANH_BAY: obj = new CKoopas(222); break;
	case OBJECT_TYPE_KOOPAS_RED_WALK: obj = new CKoopas(333); break;
	case OBJECT_TYPE_KOOPAS_RED_FLY:  obj = new CKoopas(444); break;
	case OBJECT_TYPE_FIRE_BULLET:     obj = new CFireBullet(); break;
	case OBJECT_TYPE_FLOWER_RED:	  obj = new CFlower(100); break;
	case OBJECT_TYPE_FLOWER_GREEN:	  obj = new CFlower(200); break;
	case OBJECT_TYPE_FLOWER_GREEN_CAN_SHOOT:   obj = new CFlower(300); break;
	case OBJECT_TYPE_FLOWER_BULLET:	   obj = new CFlowerBullet(); break;
	case OBJECT_TYPE_QUESTION_BRICK_HAVE_LEAF: obj = new CQuestionBrick(777); break;
	case OBJECT_TYPE_QUESTION_BRICK_JUST_HAVE_MUSHROOM: obj = new CQuestionBrick(888); break;
	case OBJECT_TYPE_LEAF:	           obj = new CLeaf(); break;
	case OBJECT_TYPE_MUSHROOM_RED:	   obj = new CMushRoom(567); break;
	case OBJECT_TYPE_MUSHROOM_GREEN:   obj = new CMushRoom(678); break;
	case OBJECT_TYPE_BREAKABLE_BRICK: obj = new CBreakableBrick(); break;
	case OBJECT_TYPE_BELL: obj = new CBell(); break;
	case OBJECT_TYPE_PORTAL:
	{
		float r = atof(tokens[4].c_str());
		float b = atof(tokens[5].c_str());
		int scene_id = atoi(tokens[6].c_str());
		obj = new CPortal(x, y, r, b, scene_id);
	}
	break;
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

void CPlayScene::Load()
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

void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<CNoCollisionObjects *>(objects[i]))
		coObjects.push_back(objects[i]);
	}

	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}

	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario	
	float cx, cy;
	player->GetPosition(cx, cy);
	CGame *game = CGame::GetInstance();
	
	if (player->x > (game->GetScreenWidth() / 2))
	{
		cx -= game->GetScreenWidth() / 2;
		CGame::GetInstance()->SetCamPos((int)cx);

		if ((player->y < (game->GetScreenHeight() / 2)))
		{
			cy -= game->GetScreenHeight() / 2;
			CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
		}
	}
	else
	{
		CGame::GetInstance()->SetCamPos();
	}
}

void CPlayScene::Render()
{
	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_SPACE:
		if (mario->GetIsJumping() == false)
		{
			mario->SetState(MARIO_STATE_JUMP);
			mario->SetIsJumping(true);
		}
		break;
	case DIK_A:
		mario->Reset();
		break;
	case DIK_R:
		if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			mario->SetLevel(MARIO_LEVEL_SMALL);
		break;
	case DIK_E:
		mario->SetLevel(MARIO_LEVEL_FIRE);
		break;
	case DIK_T:
		mario->SetLevel(MARIO_LEVEL_TAIL);
		break;
	case DIK_Z:
		if (mario->GetIsTurning() == false && mario->GetLevel() == MARIO_LEVEL_TAIL)
		{
			mario->StartTurning();
			mario->SetState(MARIO_STATE_TURNING_TAIL);
			mario->SetIsTurning(true);
		}
		break;
	case DIK_V:
		if (mario->GetLevel() == MARIO_LEVEL_FIRE)
		{
			mario->SetIsFiring(true);
		}
		break;
	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE) return;
	switch (KeyCode)
	{
	case DIK_Q:
		mario->SetIsHolding(false);
		mario->SetCanHold(false);
		break;
	case DIK_V:
		if (mario->GetLevel() == MARIO_LEVEL_FIRE)
		{
			mario->SetIsFiring(false);
			mario->SetIsFired(false);
		}
		break;
	case DIK_X:
		mario->SetCanFly(false);
		mario->SetIsFlying(false);
		mario->SetIsFalling(false);
		mario->SetIsJumping(true);
		break;
	}
}
void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE) return;

	if (game->IsKeyDown(DIK_Q))    //Holding the koopas shell
	{
		mario->SetIsHolding(true);
	}

	else if (game->IsKeyDown(DIK_X))
	{
		if (mario->GetMarioTime() >= MARIO_MAX_STACK)
			mario->SetCanFly(true);
		if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->GetCanFly())
		{	
				if (mario->nx > 0)
				{
					mario->SetState(MARIO_STATE_FLYING_RIGHT);

				}
				else
				{
					mario->SetState(MARIO_STATE_FLYING_LEFT);

				}
				if (mario->GetFlyingStart() == 0)
				{
					mario->StartFlying();
				}
				mario->SetIsFlying(true);
		}
		else
		{
			if (mario->GetCanFall() == true)
			{
				mario->SetState(MARIO_STATE_FALLING_DOWN);
				mario->SetIsFalling(true);
			}

		}

	}

	if (game->IsKeyDown(DIK_RIGHT))
	{
		if (game->IsKeyDown(DIK_LSHIFT) )//Running right
		{
			if (mario->GetRunningStart() == 0)
			{
				mario->StartRunning();
			}
			mario->SetState(MARIO_STATE_RUNNING_RIGHT);
			mario->CalcTheMarioTimeUp();
		}

		else
		{
			mario->SetMarioTime(0);
			mario->SetState(MARIO_STATE_WALKING_RIGHT); // Just walking right
		}

	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		if (game->IsKeyDown(DIK_LSHIFT)) //Running Left
		{
			if (mario->GetRunningStart() == 0)
			{
				mario->StartRunning();
			}
			mario->SetState(MARIO_STATE_RUNNING_LEFT);
			mario->CalcTheMarioTimeUp();
		}
		else
		{
			mario->SetMarioTime(0);
			mario->SetState(MARIO_STATE_WALKING_LEFT); // Just Walking left
		}

	}
	else if (game->IsKeyDown(DIK_DOWN))    //Sit down
	{
		if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_STATE_SITDOWN);
		
	}
	else
	{
		mario->SetMarioTime(0);
		if ((mario->nx > 0 && mario->vx <= 0) || (mario->nx < 0 && mario->vx >= 0))
		{
			mario->SetState(MARIO_STATE_IDLE);
		}
		if (mario->vx != 0)
		{
			mario->SetState(MARIO_STATE_SPEED_DOWN);
		}
		//mario->SetState(MARIO_STATE_IDLE);
	}

}
