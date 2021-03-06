
#include <iostream>
#include <fstream>
#include "PlayScence.h"



using namespace std;



CPlayScene::CPlayScene(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CPlayScenceKeyHandler(this);

	cam_state = CAM_STATE_ORIGIN;
	
}

/*
	Load scene resources from scene file (textures, sprites, animations and objects)
	See scene1.txt, scene2.txt for detail format specification
*/



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
	for (unsigned int i = 1; i < tokens.size(); i += 2)	// why i+=2 ?  sprite_id | frame_time  
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
void CPlayScene::_ParseSection_OBJECTS(string line)
{

	vector<string> tokens = split(line);

	//DebugOut(L"--> %s\n",ToWSTR(line).c_str());

	if (tokens.size() < 3) return; // skip invalid lines - an object set must have at least id, x, y

	int object_type = atoi(tokens[0].c_str());
	float x = (float)atof(tokens[1].c_str());
	float y = (float)atof(tokens[2].c_str());

	int ani_set_id = atoi(tokens[3].c_str());

	int renderLayer = atoi(tokens[4].c_str());


	CAnimationSets * animation_sets = CAnimationSets::GetInstance();

	CGameObject *obj = NULL;

	CHUD *HUD_items = NULL;


	CNewMapCam* new_map_cam = NULL;

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
	case OBJECT_TYPE_GOOMBA_NORMAL: obj = new CGoomba(GOOMBA_NORMAL, 3); break;
	case OBJECT_TYPE_GOOMBA_RED_FLY: obj = new CGoomba(GOOMBA_RED_FLY, 3); break;
	case OBJECT_TYPE_BRICK: obj = new CBrick(); break;
	case OBJECT_TYPE_KOOPAS_XANH_WALK: obj = new CKoopas(KOOPAS_XANH_WALK, 3); break;
	case OBJECT_TYPE_KOOPAS_RED_FLY: obj = new CKoopas(KOOPAS_RED_FLY, 4); break;
	case OBJECT_TYPE_RECTANGLE: obj = new CRectangle(); break;
	case OBJECT_TYPE_COIN_NORMAL: obj = new CCoin(COIN_NORMAL); break;
	case OBJECT_TYPE_COIN_CAN_MOVE: obj = new CCoin(COIN_CAN_MOVE); break;
	case OBJECT_TYPE_PIPE_NORMAL:
	{
		//	int pipe_id = atof(tokens[4].c_str());
		obj = new CPipe(PIPE_TYPE_NORMAL);
	}
	break;
	case OBJECT_TYPE_PIPE_DOWN:
	{
		//int pipe_id = atof(tokens[4].c_str());
		obj = new CPipe(PIPE_TYPE_DOWN);
	}
	break;
	case OBJECT_TYPE_PIPE_UP:
	{
		//int pipe_id = atof(tokens[4].c_str());
		obj = new CPipe(PIPE_TYPE_UP);
	}
	break;
	case OBJECT_TYPE_NO_COLLISION_OBJECTS:obj = new CNoCollisionObjects(3, NoCollisionObjects_TYPE_NORMAL); break;
	case OBJECT_TYPE_KOOPAS_XANH_BAY: obj = new CKoopas(KOOPAS_XANH_FLY, 3); break;
	case OBJECT_TYPE_KOOPAS_RED_WALK: obj = new CKoopas(KOOPAS_RED_WALK, 3); break;
	case OBJECT_TYPE_FIRE_BULLET:     obj = new CFireBullet(); break;
	case OBJECT_TYPE_FLOWER_RED:	  obj = new CFlower(FLOWER_RED); break;
	case OBJECT_TYPE_FLOWER_GREEN:	  obj = new CFlower(FLOWER_GREEN); break;
	case OBJECT_TYPE_FLOWER_GREEN_CAN_SHOOT:   obj = new CFlower(FLOWER_GREEN_CAN_SHOOT); break;
	case OBJECT_TYPE_FLOWER_BULLET:	   obj = new CFlowerBullet(); break;
	case OBJECT_TYPE_QUESTION_BRICK_HAVE_LEAF: obj = new CQuestionBrick(QUESTION_BRICK_HAVE_LEAF); break;
	case OBJECT_TYPE_QUESTION_BRICK_JUST_HAVE_MUSHROOM: obj = new CQuestionBrick(QUESTION_BRICK_JUST_HAVE_MUSHROOM); break;
	case OBJECT_TYPE_LEAF:	           obj = new CLeaf(); break;
	case OBJECT_TYPE_FIRE_FLOWER:	   obj = new CFireFlower(); break;
	case OBJECT_TYPE_MUSHROOM_RED:	   obj = new CMushRoom(MUSHROOM_RED); break;
	case OBJECT_TYPE_MUSHROOM_GREEN:   obj = new CMushRoom(MUSHROOM_GREEN); break;
	case OBJECT_TYPE_BREAKABLE_BRICK: obj = new CBreakableBrick(); break;
	case OBJECT_TYPE_BELL: obj = new CBell(); break;
	case OBJECT_TYPE_BLACK_BLACK: obj = new CHUD(HUD_TYPE_BLACK_BLACK); break;
	case OBJECT_TYPE_SPECIAL_ITEM: obj = new CSpecial_Item(); break;
	case OBJECT_TYPE_WORDS_END_SCENE_COURSE_CLEAR: obj = new CWordsEndScene(WORDS_END_SCENE_TYPE_COURSE_CLEAR); break;
	case OBJECT_TYPE_WORDS_END_SCENE_YOU_GOT_A_CARD: obj = new CWordsEndScene(WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD); break;
	case OBJECT_TYPE_WORDS_END_SCENE_ITEM: obj = new CWordsEndScene(WORDS_END_SCENE_TYPE_ITEM); break;
	case OBJECT_TYPE_HUD_PANEL:
		obj = new CHUD(HUD_TYPE_PANEL);
		break;
	case OBJECT_TYPE_WORLD:
		obj = new CHUD(HUD_TYPE_WORLD);
		break;
	case OBJECT_TYPE_MARIO_LUIGI:
		obj = new CHUD(HUD_TYPE_MARIO_LUIGI);
		break;
	case OBJECT_TYPE_LIFE:
		obj = new CHUD(HUD_TYPE_LIFE);
		break;
	case OBJECT_TYPE_TIME_PICKER:
		HUD_items = new CHUD(HUD_TYPE_TIME_PICKER);
		timers.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_SCORE:
		HUD_items = new CHUD(HUD_TYPE_SCORE);
		scores.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_MONEY:
		HUD_items = new CHUD(HUD_TYPE_MONEY);
		moneys.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_STACK_NORMAL:
		HUD_items = new CHUD(HUD_TYPE_STACK_NORMAL);
		normarl_stacks.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_STACK_MAX:
		HUD_items = new CHUD(HUD_TYPE_STACK_MAX);
		max_stack = (CHUD*)HUD_items;
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_ITEM:
		HUD_items = new CHUD(HUD_TYPE_ITEM);
		items.push_back(HUD_items);
		HUD_items->SetPosition(x, y);
		break;
	case OBJECT_TYPE_SCORE_AND_1LV:
		obj = new CScore();
		scores_panel.push_back(obj);
		break;
	case OBJECT_TYPE_HIT_EFFECT_TURN_TAIL:
		obj = new CHitEffect(HIT_EFFECT_TURN_TAIL);
		hit_effects_turn_tail.push_back(obj);
		break;
	case OBJECT_TYPE_HIT_EFFECT_FIRE_BULLET:
		obj = new CHitEffect(HIT_EFFECT_FIRE_BULLET);
		hit_effects_fire_bullet.push_back(obj);
		break;
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
	case OBJECT_TYPE_QUESTION_BRICK_HAVE_MULTIPLE_LIFE:
		obj = new CQuestionBrick(QUESTION_BRICK_HAVE_COIN_MULTIPLE_LIFE);
		break;
	case OBJECT_TYPE_MOVING_HORIZONTAL_RECTANGLE:
	{
		int moving_horizontal_rectangle_id =(int) atof(tokens[7].c_str());
		obj = new CMovingHorizontalRectangle(moving_horizontal_rectangle_id);
	}
	break;
	case OBJECT_TYPE_NEW_MAP_CAM:
	{
		float y_limit = (float)atof(tokens[4].c_str());
		float y_start = (float)atof(tokens[5].c_str());
		new_map_cam = new CNewMapCam(ani_set_id, x, y, y_limit, y_start);
		new_map_cams.push_back(new_map_cam);
	}
	break;
	case OBJECT_TYPE_BOOMERANG_ENEMY:
		obj = new CBoomerangEnemy();
		break;
	case OBJECT_TYPE_BOOMERANG:
	{
		int boomerang_id = (int)atof(tokens[7].c_str());
		obj = new CBoomerang(boomerang_id);
	}
	break;
	case OBJECT_TYPE_PORTAL:
	{
		int portal_id= (int)atof(tokens[4].c_str());
		float arrive_position_x= (float)atof(tokens[5].c_str());
		float arrive_position_y = (float)atof(tokens[6].c_str());
		obj = new CPortal(portal_id,arrive_position_x,arrive_position_y);
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
		if (!dynamic_cast<CNewMapCam*>(obj))
		{
			obj->SetPosition(x, y);
			obj->SetAnimationSet(ani_set);
			obj->SetOrigin(x, y, obj->GetState());
			obj->SetRenderLayer(renderLayer);
			obj->SetisOriginObj(true);
		}
		if (!dynamic_cast<CScore*>(obj))
			objects.push_back(obj);
	}

	if (HUD_items != NULL)
		HUD_items->SetAnimationSet(ani_set);


}


void CPlayScene::_ParseSection_MAP(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 7) return; // skip invalid lines

	int idTileSet = atoi(tokens[0].c_str());
	int totalRowsTileSet = atoi(tokens[1].c_str());
	int totalColumnsTileSet = atoi(tokens[2].c_str());
	int totalRowsMap = atoi(tokens[3].c_str());
	int totalColumnsMap = atoi(tokens[4].c_str());
	int totalTiles = atoi(tokens[5].c_str());
	wstring file_path = ToWSTR(tokens[6]);

	map = new Map(idTileSet, totalRowsTileSet, totalColumnsTileSet, totalRowsMap, totalColumnsMap, totalTiles);
	map->LoadMap(file_path.c_str());
	map->ExtractTileFromTileSet();
}


void CPlayScene::_ParseSection_GRID(string line)
{
	vector<string> tokens = split(line);

	if (tokens.size() < 1) return;

	wstring file_path = ToWSTR(tokens[0]);

	if (grid == NULL)
		grid = new CGrid(file_path.c_str());
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
		if (line == "[MAP]") { section = SCENE_SECTION_MAP; continue; }
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
		if (line == "[GRID]") {
			section = SCENE_SECTION_GRID; continue;
		}
		if (line[0] == '[') { section = SCENE_SECTION_UNKNOWN; continue; }

		//
		// data section
		//
		switch (section)
		{
		case SCENE_SECTION_TEXTURES: _ParseSection_TEXTURES(line); break;
		case SCENE_SECTION_MAP: _ParseSection_MAP(line); break;
		case SCENE_SECTION_SPRITES: _ParseSection_SPRITES(line); break;
		case SCENE_SECTION_ANIMATIONS: _ParseSection_ANIMATIONS(line); break;
		case SCENE_SECTION_ANIMATION_SETS: _ParseSection_ANIMATION_SETS(line); break;
		case SCENE_SECTION_OBJECTS: _ParseSection_OBJECTS(line); break;
		case SCENE_SECTION_GRID: _ParseSection_GRID(line); break;
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

bool CPlayScene::CheckCamY()
{
	float px, py;
	player->GetPosition(px, py);
	if (py > new_map_cams[cam_state - 1]->GetCamYLimit())
		camYMove = false;
	if (py < new_map_cams[cam_state - 1]->GetCamYLimit() || player->GetIsFlying())
		camYMove = true;
	return camYMove;
}



void CPlayScene::Update(DWORD dt)
{
	// We know that Mario is the first object in the list hence we won't add him into the colliable object list
	// TO-DO: This is a "dirty" way, need a more organized way 
	float cx, cy;
	player->GetPosition(cx, cy);


	CGame *game = CGame::GetInstance();

	if (game->GetCamX() == 0 && game->GetCamY() == 0)
		CGame::GetInstance()->SetCamPos((int)new_map_cams[cam_state - 1]->GetStartCamX(),(int) new_map_cams[cam_state - 1]->GetYStart());

	StartTimeCounter();

	cam_x_diff = game->GetCamX();
	cam_y_diff = game->GetCamY();

	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	
	if (id == PLAY_SCENE_1_1_ID)
	{
		cx -= game->GetScreenWidth() / 2;
		if (player->x >= (game->GetScreenWidth() / 2 + new_map_cams[cam_state - 1]->GetStartCamX()))
		{
			if (CheckCamY() || player->GetIsAtTheTunnel())
			{
				cy -= game->GetScreenHeight() / 2;
				CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
			}
			else
			{
				CGame::GetInstance()->SetCamPos((int)cx,(int) new_map_cams[cam_state - 1]->GetYStart());
			}
		}
		else
		{
			if (CheckCamY())
			{
				cy -= game->GetScreenHeight() / 2;
				CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
			}
			else
			{
				CGame::GetInstance()->SetCamPos((int)new_map_cams[cam_state - 1]->GetStartCamX(), (int)new_map_cams[cam_state - 1]->GetYStart());
			}

			
		}
	}
	else if (id == PLAY_SCENE_1_4_ID)
	{
		if (cam_state == CAM_STATE_ORIGIN)
		{
			StartTimeCamMove();
			if (time_cam_move != 0)
			{
				if (GetTickCount() - time_cam_move >= 10)
				{
					time_cam_move = 0;
					int cam_x_update = UpdateCamMoveX(dt);
					CGame::GetInstance()->SetCamPos((int)cam_x_update, (int)new_map_cams[cam_state - 1]->GetYStart());
				}
			}
		}
		else
		{
			if (cam_state == CAM_STATE_2)
			{
				cx -= game->GetScreenWidth() / 2;
				if (player->x >= (game->GetScreenWidth() / 2 + new_map_cams[cam_state - 1]->GetStartCamX()))
				{
					if (CheckCamY())
					{
						cy -= game->GetScreenHeight() / 2;
						CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
					}
					else
					{
						CGame::GetInstance()->SetCamPos((int)cx, (int)new_map_cams[cam_state - 1]->GetYStart());
					}
				}
				else
				{
					if (CheckCamY())
					{
						cy -= game->GetScreenHeight() / 2;
						CGame::GetInstance()->SetCamPos((int)cx, (int)cy);
					}
					else
					{
						CGame::GetInstance()->SetCamPos((int)new_map_cams[cam_state - 1]->GetStartCamX(),(int) new_map_cams[cam_state - 1]->GetYStart());
					}
				}
			}
		}
	}

	if (game->GetCamX() >= new_map_cams[cam_state - 1]->GetEndCamX())
		game->SetCamX((float)new_map_cams[cam_state - 1]->GetEndCamX());


	cx = (float)game->GetCamX();

	cy = (float)game->GetCamY();

	for (size_t i = 0; i < objects.size(); i++)
	{
		float Ox, Oy;
		objects[i]->GetPosition(Ox, Oy);
		if (!IsInUseArea(Ox, Oy) && !objects[i]->GetisOriginObj())
		{
			objects[i]->SetActive(false);
			objects.erase(objects.begin() + i);
		}
	}

	grid->GetObjects(objects, (int)cx, (int)cy);

	player->GetPosition(cx, cy);


	//DebugOut(L"So Luong CooBJ %d \n", objects.size());


	//vector<LPGAMEOBJECT> temp_objects;

	//

	//for (size_t i = 0; i < objects.size(); i++)
	//{
	//	if (objects[i]->vx!=0 || objects[i]->vy != 0 || objects[i]->GetisOriginObj())
	//		temp_objects.push_back(objects[i]);
	//}


	for (size_t i = 0; i < objects.size(); i++)
	{
	
		if (!player->GetIsTransforming())
		{
			if (!dynamic_cast<CNoCollisionObjects*>(objects[i]))
				objects[i]->Update(dt, &objects);
		}
		else
		{
			if (dynamic_cast<CMario*>(objects[i]) || dynamic_cast<CHUD*>(objects[i]) || dynamic_cast<CScore*>(objects[i]) || dynamic_cast<CCoin*>(objects[i]) || dynamic_cast<CHitEffect*>(objects[i]))
				objects[i]->Update(dt, &objects);
		}
	}

	if (GetTickCount() - time_counter >= 1000 && time_picker > 0 && !player->GetLoseControl())
	{
		time_picker--;
		time_counter = 0;
	}


	for (size_t i = 0; i < timers.size(); i++)
	{
		timers[i]->Update(dt, NULL);
	}

	for (size_t i = 0; i < scores.size(); i++)
	{
		scores[i]->Update(dt, NULL);
	}

	for (size_t i = 0; i < moneys.size(); i++)
	{
		moneys[i]->Update(dt, NULL);
	}

	for (size_t i = 0; i < normarl_stacks.size(); i++)
	{
		normarl_stacks[i]->Update(dt, NULL);
	}

	for (size_t i = 0; i < items.size(); i++)
	{
		items[i]->Update(dt, NULL);
	}
	for (size_t i = 0; i < scores_panel.size(); i++)
	{
		scores_panel[i]->Update(dt, &objects);
	}


	max_stack->Update(dt, NULL);



	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)
	if (player == NULL) return;

	// Update camera to follow mario	



}

void CPlayScene::Render()
{
	
	CGame* game = CGame::GetInstance();

	float cx, cy;

	player->GetPosition(cx, cy);


	vector<LPGAMEOBJECT> render[MAX_RENDER_LAYER];

	for (size_t i = 0; i < objects.size(); i++)
	{
		render[objects[i]->GetRenderLayer() - 1].push_back(objects[i]);
	}

	
	if (map)
	{
		this->map->Render((int)game->GetCamX(), (int)game->GetCamY());
	}

	for (int i = 0; i < MAX_RENDER_LAYER; i++)
	{
		for (size_t j = 0; j < render[i].size(); j++)
			render[i][j]->Render();
	}



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
	for (size_t i = 0; i < scores_panel.size(); i++)
	{
		scores_panel[i]->Render();
	}

}

/*
	Unload current scene
*/
void CPlayScene::Unload()
{
	for (unsigned int i = 0; i < objects.size(); i++)
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

	for (size_t i = 0; i < scores_panel.size(); i++)
	{
		delete scores_panel[i];
	}

	//for (size_t i = 0; i < hit_effects_fire_bullet.size(); i++)
	//{
	//	delete hit_effects_fire_bullet[i];
	//}

	//for (size_t i = 0; i < hit_effects_turn_tail.size(); i++)
	//{
	//	delete hit_effects_turn_tail[i];
	//}

	cam_state = CAM_STATE_ORIGIN;

	objects.clear();
	items.clear();
	moneys.clear();
	scores.clear();
	objects.clear();
	normarl_stacks.clear();
	timers.clear();
	scores_panel.clear();
	//hit_effects_fire_bullet.clear();
	//hit_effects_turn_tail.clear();

	grid->Unload();

	grid = nullptr;

	delete grid;


	player = NULL;
	delete map;
	map = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

bool CPlayScene::IsInUseArea(float Ox, float Oy)
{
	int CamX, CamY;

	CamX = CGame::GetInstance()->GetCamX();

	CamY = CGame::GetInstance()->GetCamY();

	if (((CamX < Ox + 50) && (Ox < CamX + IN_USE_WIDTH)) && ((CamY < Oy) && (Oy < CamY + IN_USE_HEIGHT)))
		return true;
	return false;
}

int CPlayScene::UpdateCamMoveX(DWORD dt)
{

	int cam_x_end_temp =(int) new_map_cams.at(1)->GetEndCamX();


	int cam_x_game = CGame::GetInstance()->GetCamX();
	

	if (cam_x_game < cam_x_end_temp)
	{
		cam_x_game += (int)(MOVE_CAM_X_VX * dt);
		return cam_x_game;
	}
	else
	{
		return cam_x_end_temp;
	}


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
			mario->SetIsOnMovingHorizontalRectangle(false);
			mario->SetMarioMovingHorizotalRecID(-1);
			mario->SetControlMarioColliWithMovingRec(false);
			mario->StartTimingJumpingLonger();
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
		//Holding the koopas shell
		mario->SetIsHolding(true);
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
	case DIK_A:
		if (mario->GetState() != MARIO_STATE_PIPE_DOWNING)
		{
			mario->SetIsHolding(false);
			mario->SetCanHold(false);
		}
		mario->SetIsFiring(false);
		mario->SetIsFired(false);
		break;
	case DIK_S:
		mario->SetCanFly(false);
		mario->SetIsFlying(false);
		mario->SetIsFalling(false);
		mario->SetTimingjumingLonger(0);
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


	if (game->IsKeyDown(DIK_S))
	{
		if (mario->GetMarioTime() >= MARIO_MAX_STACK)
		{
			mario->SetCanFly(true);
			if (!mario->GetIsKeepingMaxStack())
				mario->SetIsKeepingMaxStack(true);
			if (mario->GetMarioTempVx() == 0)
				mario->SetMarioTempVx(mario->vx);
		}

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
			if (mario->GetLevel() == MARIO_LEVEL_TAIL && mario->GetIsJumping() && mario->vy >= 0)
			{
				mario->SetState(MARIO_STATE_FALLING_DOWN);
				mario->SetIsFalling(true);
			}
			if (!mario->GetIsKeepingMaxStack())
				mario->CalcTheMarioTimeDown();
		}

	}




	if (game->IsKeyDown(DIK_RIGHT))
	{
		/*if (!mario->GetIsPipeLockedRight())
		{*/
		if (game->IsKeyDown(DIK_A))//Running right
		{
			if (!game->IsKeyDown(DIK_S))
			{
				if (mario->GetRunningStart() == 0)
				{
					mario->StartRunning();
				}
				mario->SetState(MARIO_STATE_RUNNING_RIGHT);
				mario->CalcTheMarioTimeUp();
			}
			//DebugOut(L"[INFO] Stack Tang la: %d \n", mario->GetMarioTime());
		}
		else
		{
			if (!game->IsKeyDown(DIK_S))
			{
				if (!mario->GetIsKeepingMaxStack())
					mario->CalcTheMarioTimeDown();
			}
			mario->SetState(MARIO_STATE_WALKING_RIGHT); // Just walking right
		}

		/*	if(!mario->GetIsColliWithPipe())
			mario->SetIsPipeLockedLeft(false);*/
			//}
	}



	else if (game->IsKeyDown(DIK_LEFT))
	{/*
		if (!mario->GetIsPipeLockedLeft())
		{*/
		//DebugOut(L"Khoa phim trai %d \n", mario->GetIsPipeLockedLeft());
		if (game->IsKeyDown(DIK_A)) //Running Left
		{
			if (!game->IsKeyDown(DIK_S))
			{
				if (mario->GetRunningStart() == 0)
				{
					mario->StartRunning();
				}
				mario->SetState(MARIO_STATE_RUNNING_LEFT);
				mario->CalcTheMarioTimeUp();
			}
		}
		else
		{
			if (!game->IsKeyDown(DIK_S))
			{
				if (!mario->GetIsKeepingMaxStack())
					mario->CalcTheMarioTimeDown();
			}
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
		if (!mario->GetIsKeepingMaxStack())
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
		if (!mario->GetIsKeepingMaxStack())
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
			if (!mario->GetIsKeepingMaxStack())
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
