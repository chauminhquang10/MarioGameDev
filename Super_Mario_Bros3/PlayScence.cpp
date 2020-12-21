
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

	CHUD *HUD_items = NULL;




	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		if (player != NULL)
		{
			DebugOut(L"[ERROR] MARIO object was created before!\n");
			return;
		}
		obj = new CMario(1, x, y);
		player = (CMario*)obj;
		DebugOut(L"[INFO] Player object created!\n");
		break;
	case OBJECT_TYPE_GOOMBA_NORMAL: obj = new CGoomba(888, 3); break;
	case OBJECT_TYPE_GOOMBA_RED_FLY: obj = new CGoomba(999, 3); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS_XANH_WALK: obj = new CKoopas(111, 3); break;
	case OBJECT_TYPE_RECTANGLE: obj = new CRectangle(); break;
	case OBJECT_TYPE_COIN_NORMAL: obj = new CCoin(222); break;
	case OBJECT_TYPE_COIN_CAN_MOVE: obj = new CCoin(333); break;
	case OBJECT_TYPE_PIPE_NORMAL: obj = new CPipe(100); break;
	case OBJECT_TYPE_PIPE_DOWN: obj = new CPipe(200); break;
	case OBJECT_TYPE_PIPE_UP: obj = new CPipe(300); break;
	case OBJECT_TYPE_NO_COLLISION_OBJECTS:obj = new CNoCollisionObjects(3, 1); break;
	case OBJECT_TYPE_KOOPAS_XANH_BAY: obj = new CKoopas(222, 3); break;
	case OBJECT_TYPE_KOOPAS_RED_WALK: obj = new CKoopas(333, 3); break;
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
	case OBJECT_TYPE_BLACK_BLACK: obj = new CHUD(1000); break;
	case OBJECT_TYPE_SPECIAL_ITEM: obj = new CSpecial_Item(); break;
	case OBJECT_TYPE_WORDS_END_SCENE_COURSE_CLEAR: obj = new CWordsEndScene(111); break;
	case OBJECT_TYPE_WORDS_END_SCENE_YOU_GOT_A_CARD: obj = new CWordsEndScene(222); break;
	case OBJECT_TYPE_WORDS_END_SCENE_ITEM: obj = new CWordsEndScene(333); break;
	case OBJECT_TYPE_HUD_PANEL:
		obj = new CHUD(11);
		break;
	case OBJECT_TYPE_WORLD:
		obj = new CHUD(22);
		break;
	case OBJECT_TYPE_MARIO_LUIGI:
		obj = new CHUD(77);
		break;
	case OBJECT_TYPE_LIFE:
		obj = new CHUD(33);
		break;
	case OBJECT_TYPE_TIME_PICKER:
		HUD_items = new CHUD(44);
		timers.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_SCORE:
		HUD_items = new CHUD(55);
		scores.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_MONEY:
		HUD_items = new CHUD(66);
		moneys.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_STACK_NORMAL:
		HUD_items = new CHUD(88);
		normarl_stacks.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_STACK_MAX:
		HUD_items = new CHUD(99);
		max_stack = (CHUD*)HUD_items;
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_ITEM:
		HUD_items = new CHUD(100);
		items.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	/*case OBJECT_TYPE_SCORE_AND_1LV:
		obj = new CScore();
		scores_panel.push_back(obj);
		break;*/
	case OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP:
		obj = new CBreakableBrickAnimation(BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP); 
		break;
	case  OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP:
		obj = new CBreakableBrickAnimation(BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP);
		break;
	case  OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM:
		obj = new CBreakableBrickAnimation(BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM);
		break;
	case OBJECT_TYPE_BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM:
		obj = new CBreakableBrickAnimation(BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM); 
		break;
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
	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);
	if (obj != NULL)
	{
		obj->SetPosition(x, y);
		obj->SetAnimationSet(ani_set);
		objects.push_back(obj);
	}

	if (HUD_items != NULL)
		HUD_items->SetAnimationSet(ani_set);


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


	StartTimeCounter();

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		if (!dynamic_cast<CNoCollisionObjects *>(objects[i]))
			coObjects.push_back(objects[i]);
	}




	float cx, cy;
	player->GetPosition(cx, cy);

	CGame *game = CGame::GetInstance();

	cam_x_diff = game->GetCamX();
	cam_y_diff = game->GetCamY();

	if (player->x >= (game->GetScreenWidth() / 2))
	{
		cx -= game->GetScreenWidth() / 2;
		CGame::GetInstance()->SetCamPos((int)cx);

		if (player->y <= (game->GetScreenHeight() / 3))
		{
			cy -= game->GetScreenHeight() / 2;
			CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
		}
		if (player->GetLoseControl())
		{
			CGame::GetInstance()->SetCamPos(2500, -62);
		}
	}
	else
	{
		CGame::GetInstance()->SetCamPos(0);
	}

	if (player->GetIsAtTheTunnel())
	{
		CGame::GetInstance()->SetCamPos(1300, 980);
	}

	player->GetPosition(cx, cy);

	for (size_t i = 0; i < objects.size(); i++)
	{
		float xx, xy;
		objects[i]->GetPosition(xx, xy);
		if ((((xx < cx + game->GetScreenWidth() / 2 && xx > cx - game->GetScreenWidth() / 2 - 16) && abs(xy - cy) <= 500) || dynamic_cast<CBreakableBrickAnimation*>(objects[i]) || dynamic_cast<CWordsEndScene*>(objects[i]) || dynamic_cast<CFireBullet*>(objects[i]) /*|| dynamic_cast<CScore*>(objects[i])*/ || dynamic_cast<CFlowerBullet*>(objects[i]) || dynamic_cast<CHUD*>(objects[i])))
		{
			if (!player->GetIsTransforming())
			{
				if (!dynamic_cast<CNoCollisionObjects*>(objects[i]))
					objects[i]->Update(dt, &coObjects);
			}
			else
			{
				if (dynamic_cast<CMario*>(objects[i]) || dynamic_cast<CHUD*>(objects[i]) /*|| dynamic_cast<CScore*>(objects[i])*/)
					objects[i]->Update(dt, &coObjects);
			}
		}
	}

	if (GetTickCount() - time_counter >= 1000 && time_picker > 0 && !player->GetLoseControl())
	{
		time_picker--;
		time_counter = 0;
	}


	for (size_t i = 0; i < timers.size(); i++)
	{
		timers[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < scores.size(); i++)
	{
		scores[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < moneys.size(); i++)
	{
		moneys[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < normarl_stacks.size(); i++)
	{
		normarl_stacks[i]->Update(dt, &coObjects);
	}

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Update(dt, &coObjects);
	}



	max_stack->Update(dt, &coObjects);



	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario	



}

void CPlayScene::Render()
{

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();

	for (size_t i = 0; i < timers.size(); i++)
	{
		timers[i]->Render(i);
	}
	for (size_t i = 0; i < scores.size(); i++)
	{
		scores[i]->Render(i);
	}
	for (size_t i = 0; i < moneys.size(); i++)
	{
		moneys[i]->Render(i);
	}
	for (size_t i = 0; i < normarl_stacks.size(); i++)
	{
		normarl_stacks[i]->Render(i);
	}
	max_stack->Render();

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Render(i);
	}


}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (int i = 0; i < objects.size(); i++)
	{
		delete objects[i];
	}
	for (size_t i = 0; i < scores.size(); i++)
	{
		delete scores[i];
	}
	for (size_t i = 0; i < moneys.size(); i++)
	{
		delete moneys[i];
	}
	for (size_t i = 0; i < items.size(); i++)
	{
		delete items[i];
	}
	for (size_t i = 0; i < timers.size(); i++)
	{
		delete timers[i];
	}
	for (size_t i = 0; i < normarl_stacks.size(); i++)
	{
		delete normarl_stacks[i];
	}

	/*for (size_t i = 0; i < scores_panel.size(); i++)
	{
		delete scores_panel[i];
	}*/

	
	objects.clear();
	items.clear();
	moneys.clear();
	scores.clear();
	objects.clear();
	normarl_stacks.clear();
	timers.clear();
	//scores_panel.clear();

	player = NULL;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CPlayScenceKeyHandler::OnKeyDown(int KeyCode)
{
	//DebugOut(L"[INFO] KeyDown: %d\n", KeyCode);

	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE || mario->GetState() == MARIO_STATE_PIPE_DOWNING || mario->GetState() == MARIO_STATE_PIPE_UPPING || mario->GetIsTransforming()) return;
	if (mario->GetLoseControl()) return;
	switch (KeyCode)
	{
	case DIK_S:
		if (mario->GetIsJumping() == false)
		{
			mario->SetState(MARIO_STATE_JUMP);
			mario->SetIsJumping(true);
		}
		break;
	case DIK_P:
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
	case DIK_A:
		if (mario->GetIsTurning() == false && mario->GetLevel() == MARIO_LEVEL_TAIL)
		{
			mario->StartTurning();
			mario->SetState(MARIO_STATE_TURNING_TAIL);
			mario->SetIsTurning(true);
		}
		else if (mario->GetLevel() == MARIO_LEVEL_FIRE && !mario->GetFireRecog())
		{
			mario->StartFireRecog();
			mario->SetIsFiring(true);
		}
		break;

	}
}
void CPlayScenceKeyHandler::OnKeyUp(int KeyCode)
{
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();
	if (mario->GetState() == MARIO_STATE_DIE || mario->GetState() == MARIO_STATE_PIPE_DOWNING || mario->GetState() == MARIO_STATE_PIPE_UPPING || mario->GetIsTransforming()) return;
	if (mario->GetLoseControl()) return;
	switch (KeyCode)
	{
	case DIK_Q:
		mario->SetIsHolding(false);
		mario->SetCanHold(false);
		break;
	case DIK_A:
		mario->SetIsFiring(false);
		mario->SetIsFired(false);
		break;
	case DIK_S:
		mario->SetCanFly(false);
		mario->SetIsFlying(false);
		mario->SetIsFalling(false);
		break;
	}
}
void CPlayScenceKeyHandler::KeyState(BYTE *states)
{
	CGame *game = CGame::GetInstance();
	CMario *mario = ((CPlayScene*)scence)->GetPlayer();

	// disable control key when Mario die 
	if (mario->GetState() == MARIO_STATE_DIE || mario->GetState() == MARIO_STATE_PIPE_DOWNING || mario->GetState() == MARIO_STATE_PIPE_UPPING || mario->GetIsTransforming()) return;
	if (mario->GetLoseControl())
	{
		mario->SetState(MARIO_STATE_WALKING_RIGHT);
		return;
	}

	if (game->IsKeyDown(DIK_Q))    //Holding the koopas shell
	{
		mario->SetIsHolding(true);
		mario->CalcTheMarioTimeDown();
	}

	else if (game->IsKeyDown(DIK_S))
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
			mario->CalcTheMarioTimeUp();
			//DebugOut(L"[INFO] Stack Tang la: %d \n", mario->GetMarioTime());
		}
		else
		{
			if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->GetIsJumping() && mario->vy >= 0)
			{
				mario->SetState(MARIO_STATE_FALLING_DOWN);
				mario->SetIsFalling(true);
			}
			mario->CalcTheMarioTimeDown();
		}

	}


	if (game->IsKeyDown(DIK_RIGHT))
	{
		/*if (!mario->GetIsPipeLockedRight())
		{*/
			if (game->IsKeyDown(DIK_A))//Running right
			{
				if (mario->GetRunningStart() == 0)
				{
					mario->StartRunning();
				}
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);
				mario->CalcTheMarioTimeUp();

				//DebugOut(L"[INFO] Stack Tang la: %d \n", mario->GetMarioTime());

			}
			else
			{
				if (!game->IsKeyDown(DIK_S))
					mario->CalcTheMarioTimeDown();
				mario->SetState(MARIO_STATE_WALKING_RIGHT); // Just walking right
			}
		/*	if(!mario->GetIsColliWithPipe())
			mario->SetIsPipeLockedLeft(false);*/
		//}
	}
	else if (game->IsKeyDown(DIK_LEFT))
	{
		/*if (!mario->GetIsPipeLockedLeft())
		{*/
			//DebugOut(L"Khoa phim trai %d \n", mario->GetIsPipeLockedLeft());
			if (game->IsKeyDown(DIK_A)) //Running Left
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
				if (!game->IsKeyDown(DIK_S))
					mario->CalcTheMarioTimeDown();
				mario->SetState(MARIO_STATE_WALKING_LEFT); // Just Walking left
			}
		/*	if (!mario->GetIsColliWithPipe())
			mario->SetIsPipeLockedRight(false);*/
		//}
	}
	else if (game->IsKeyDown(DIK_DOWN))    //Sit down
	{
		if (mario->GetLevel() != MARIO_LEVEL_SMALL)
			mario->SetState(MARIO_STATE_SITDOWN);
		else
		{
			mario->SetState(MARIO_STATE_IDLE);
		}
		mario->CalcTheMarioTimeDown();
		if (mario->GetCanPipeDowning())
		{
			mario->SetState(MARIO_STATE_PIPE_DOWNING);
			mario->StartPipeDowning();
			return;
		}
	}
	else if (game->IsKeyDown(DIK_UP))
	{
		mario->CalcTheMarioTimeDown();
		if (mario->GetCanPipeUpping())
		{
			mario->SetState(MARIO_STATE_PIPE_UPPING);
			mario->StartPipeUpping();
			return;
		}
	}
	else
	{
		if (!game->IsKeyDown(DIK_S))
		{
			mario->CalcTheMarioTimeDown();
		}
		//DebugOut(L"[INFO] Stack Giam la: %d \n", mario->GetMarioTime());

		if ((mario->nx > 0 && mario->vx <= 0) || (mario->nx < 0 && mario->vx >= 0))
		{
			mario->SetState(MARIO_STATE_IDLE);
		}
		if (mario->vx != 0)
		{
			mario->SetState(MARIO_STATE_SPEED_DOWN);
		}

	}

}