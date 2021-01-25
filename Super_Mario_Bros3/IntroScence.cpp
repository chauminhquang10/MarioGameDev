#include "IntroScence.h"

#include <iostream>
#include <fstream>

using namespace std;



CIntroScence::CIntroScence(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CIntroScenceKeyHandler(this);
	menu_game_key_handler = false;
	
}

CIntroScence::~CIntroScence()
{
}





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
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
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

	for (unsigned int i = 1; i < tokens.size(); i++)
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
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	CNewMapCam* new_map_cam = NULL;

	switch (object_type)
	{
	case INTRO_SCENE_OBJECT_TYPE_MARIO_RED:
		obj = new CMario(1, x, y);
		player1 = (CMario*)obj;
		DebugOut(L"[INFO] Player1 object created!\n");
		break;
	case INTRO_SCENE_OBJECT_TYPE_MARIO_GREEN:
		obj = new CMario(2, x, y);
		player2 = (CMario*)obj;
		DebugOut(L"[INFO] Player2 object created!\n");
		break;
	case INTRO_SCENE_OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case INTRO_SCENE_OBJECT_TYPE_BACKGROUND_STAGE_BLACK:  obj = new CBackGroundStage(BACKGROUND_STAGE_TYPE_BLACK); break;
	case INTRO_SCENE_OBJECT_TYPE_BACKGROUND_STAGE_COLOR:  obj = new CBackGroundStage(BACKGROUND_STAGE_TYPE_COLOR); break;
	case INTRO_SCENE_OBJECT_TYPE_BACKGROUND_STAGE_FINAL:  obj = new CBackGroundStage(BACKGROUND_STAGE_TYPE_FINAL); break;
	case INTRO_SCENE_OBJECT_TYPE_SCROLLING_STAGE: obj = new CScrollingStage(); break;
	case INTRO_SCENE_OBJECT_TYPE_NO_COLLISION_OBJECTS_NUMBER_THREE:	obj = new CNoCollisionObjects(1, NoCollisionObjects_TYPE_NUMBER_THREE); break;
	case INTRO_SCENE_OBJECT_TYPE_GOOMBA_NORMAL: obj = new CGoomba(GOOMBA_NORMAL, 1); break;
	case INTRO_SCENE_OBJECT_TYPE_LEAF:	           obj = new CLeaf(); break;
	case INTRO_SCENE_OBJECT_TYPE_MUSHROOM_RED:	   obj = new CMushRoom(MUSHROOM_RED); break;
	case INTRO_SCENE_OBJECT_TYPE_STAR:				obj = new CStar(); break;
	case INTRO_SCENE_OBJECT_TYPE_KOOPAS_BLACK: obj = new CKoopas(KOOPAS_BLACK, 1); break;
	case INTRO_SCENE_OBJECT_TYPE_KOOPAS_XANH: obj = new CKoopas(KOOPAS_XANH_WALK, 1); break;
	case INTRO_SCENE_OBJECT_TYPE_NO_COLLISION_OBJECTS_BUSH: obj = new CNoCollisionObjects(1, NoCollisionObjects_TYPE_BUSH_INTRO_SCENE); break;
	case INTRO_SCENE_OBJECT_TYPE_MENU_GAME:	           obj = new CMenuGame(); break;
	case INTRO_SCENE_OBJECT_TYPE_KOOPAS_LINE: obj = new CKoopas(KOOPAS_TYPE_LINE, 1); break;
	case INTRO_SCENE_OBJECT_TYPE_KOOPAS_FASTER: obj = new CKoopas(KOOPAS_TYPE_FASTER, 1); break;
	case OBJECT_TYPE_NEW_MAP_CAM:
	{
		float y_limit = (float)atof(tokens[4].c_str());
		float y_start = (float)atof(tokens[5].c_str());
		new_map_cam = new CNewMapCam(ani_set_id, x, y, y_limit, y_start);
		new_map_cams.push_back(new_map_cam);
	}
	break;
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
	if (obj != NULL)
	{
		obj->SetPosition(x, y);

		LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}


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
		coObjects.push_back(objects[i]);
	}


	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!player1->GetIsTransforming())
		{
			objects[i]->Update(dt, &coObjects);
		}
		else
		{
			if (dynamic_cast<CMario*>(objects[i]))
				objects[i]->Update(dt, &coObjects);

		}
	}


	player1->SetIsAppear(false);
	player2->SetIsAppear(false);



	player1->nx = -1;

	if (player1->GetState() == MARIO_STATE_SITDOWN)
	{
		isAllowToWalkRed = false;
		StartSitDownCount();
		if (GetTickCount() - sit_down_count >= SIT_DOWN_COUNT_TIME)
			player1->SetState(MARIO_STATE_IDLE);
	}

	if (GetTickCount() - time_count >= TIME_COUNT_TIME_1)
	{
		player2->SetIsAppear(true);
		player1->SetIsAppear(true);
	}

	if (GetTickCount() - time_count >= TIME_COUNT_TIME_2)
	{
		if (player1->GetLevel() == MARIO_LEVEL_SMALL)
		{
			if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME)
			{
				player1->SetIsAppear(false);

			}
		}
		if (isAllowToWalkGreen)
		{
			player2->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		if (isAllowToWalkRed)
		{
			player1->SetState(MARIO_STATE_WALKING_LEFT);
		}
	}


	if (GetTickCount() - time_count >= TIME_COUNT_TIME_3)
	{
		isAllowToWalkGreen = false;
		if (green_jump_count < 2)
		{
			if (player2->GetIsJumping() == false)
			{
				if (green_jump_count == 0)
					player2->SetState(MARIO_STATE_JUMP);
				else
					player2->SetState(MARIO_STATE_JUMP_HIGH);
				player2->SetIsJumping(true);
				green_jump_count++;
			}
		}
		else
		{
			if (GetTickCount() - time_count <= TIME_COUNT_TIME_4)
				isAllowToWalkGreen = true;
		}
	}


	if (player1->GetState() == MARIO_STATE_HITTED)
	{
		if (GetTickCount() - player1->GetHittedStart() >= HITTED_START_TIME)
		{
			player1->SetState(MARIO_STATE_LOOK_UP);
			player1->StartHitted();
		}
	}


	if (player1->GetState() == MARIO_STATE_LOOK_UP)
	{
		if (GetTickCount() - player1->GetHittedStart() >= MARIO_STATE_LOOK_UP_TIME)
		{
			if (!player1->GetIsJumping())
			{
				player1->SetState(MARIO_STATE_JUMP);
				player1->SetIsJumping(true);
			}
		}
	}

	if (player1->GetIsJumping())
	{
		if (red_jump_time_count == 0)
		{
			player1->SetCanFall(true);
		}
	}
	else
	{
		player1->SetCanFall(false);

	}

	if (player1->GetLevel() == MARIO_LEVEL_TAIL)
	{
		if (player1->GetCanFall())
		{
			player1->SetState(MARIO_STATE_FALLING_DOWN);
			player1->SetIsFalling(true);
			if (red_jump_time_count == 0)
			{
				red_jump_time_count++;
			}
		}
		else
		{
			if (!isAllowToWalkRed)
			{
				if (red_jump_time_count == 1)
				{
					player1->SetState(MARIO_STATE_WALKING_RIGHT);
				}

			}
		}

	}


	if (player1->GetState() == MARIO_STATE_WALKING_RIGHT)
	{
		StartRedIdleCount();
		if (GetTickCount() - red_idle_count >= MARIO_STATE_WALKING_RIGHT_TIME)
		{
			player1->SetState(MARIO_STATE_IDLE);
		}
	}

	if (player2->x >= PLAYER2_LIMIT_X && !player2->GetIsHolding())
	{
		player2->nx = -PLAYER2_NX;
		player2->SetState(MARIO_STATE_IDLE);
		isAllowToWalkGreen = false;
	}



	if (player2->GetIsHolding())
	{
		player2->SetState(MARIO_STATE_WALKING_LEFT);
		StartGreenIdleCount();
		if (GetTickCount() - green_idle_count > GREEN_IDLE_COUNT_1)
		{
			isAllowToWalkRed = true;
			StartRedJumpCount();
		}
		if (GetTickCount() - green_idle_count > GREEN_IDLE_COUNT_2)
		{
			player2->SetIsHolding(false);
			player2->SetCanHold(false);
			player2->SetState(MARIO_STATE_IDLE);
		}
	}

	if (red_jump_count != 0)
	{
		if (GetTickCount() - red_jump_count >= RED_JUMP_COUNT_1)
		{
			if (red_jump_time_count == 1)
			{
				if (!player1->GetIsJumping())
				{
					player1->SetState(MARIO_STATE_JUMP);
					red_jump_time_count++;
					player1->SetIsJumping(true);
				}
				isAllowToWalkRed = false;
			}

		}
		if (GetTickCount() - red_jump_count >= RED_JUMP_COUNT_2)
		{
			player1->nx = 1;
		}
		if (GetTickCount() - red_jump_count >= RED_JUMP_COUNT_3)
		{
			player1->vx = MARIO_WALKING_SPEED / 2;

		}
		if (GetTickCount() - red_jump_count >= RED_JUMP_COUNT_4)
		{
			player1->SetIsHolding(true);
		}

		if (GetTickCount() - red_jump_count >= RED_JUMP_COUNT_5)
		{
			player1->SetIsHolding(false);
			player1->SetCanHold(false);
			StartGreenRunAwayCount();
		}
		if (GetTickCount() - red_jump_count >= RED_JUMP_COUNT_6)
		{
			player1->SetState(MARIO_STATE_IDLE);
		}
	}

	if (green_run_away_count != 0)
	{
		if (GetTickCount() - green_run_away_count >= GREEN_RUN_AWAY_COUNT_TIME)
		{
			player2->SetState(MARIO_STATE_WALKING_RIGHT);
		}

	} 

	if (player1->GetLevel() == MARIO_LEVEL_SMALL)
	{
		StartRedSmallCount();
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_2)
		{
			player1->nx = -1;

		}
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_3)
		{
			player1->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_4)
		{
			player1->SetState(MARIO_STATE_WALKING_LEFT);
		}
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_5)
		{
			player1->SetState(MARIO_STATE_IDLE);
		}
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_6)
		{
			player1->SetState(MARIO_STATE_WALKING_LEFT);
		}
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_7)
		{
			player1->SetState(MARIO_STATE_IDLE);
			player1->SetIsAllowToShowBush(true);
		}
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_8)
		{
			player1->SetState(MARIO_STATE_WALKING_RIGHT);
		}
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_9)
		{
			player1->SetIsAllowToShowMenuGame(true);
		}
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_10)
		{
			player1->SetIsAllowToShowKoopasLine(true);
		}
		if (GetTickCount() - red_small_count >= RED_SMALL_COUNT_TIME_11)
		{
			player1->SetIsAllowToShowKoopasFaster(true);
		}
	}

	CGame* game = CGame::GetInstance();

	if (game->GetCamX() == 0 && game->GetCamY() == 0)
		CGame::GetInstance()->SetCamPos((int)new_map_cams[0]->GetStartCamX(),(int)new_map_cams[0]->GetYStart());

	
	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player1 == NULL) return;

	if (player2 == NULL) return;
	// Update camera to follow mario	



}

void CIntroScence::Render()
{
	for (unsigned int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CIntroScence::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}

	for (size_t i = 0; i < new_map_cams.size(); i++)
	{
		delete new_map_cams[i];
	}

	new_map_cams.clear();

	objects.clear();
	player1 = NULL;
	player2 = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CIntroScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);
	CIntroScence* intro_scene = dynamic_cast<CIntroScence *>(CGame::GetInstance()->GetCurrentScene());
	CMario* player1 = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer1();
	if (player1->GetIsAllowToShowMenuGame())
	{
		switch (KeyCode)
		{
		case DIK_W:
			CGame::GetInstance()->SwitchScene(2);
			break;
		case DIK_DOWN:
			intro_scene->menu_game_key_handler = true;
			break;
		case DIK_UP:
			intro_scene->menu_game_key_handler = false;
			break;
		}
	}
}
void CIntroScenceKeyHandler::OnKeyUp(int KeyCode)
{

}

void CIntroScenceKeyHandler::KeyState(BYTE *states)
{

}