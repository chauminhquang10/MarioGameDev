#include "WorldMap.h"
#include <iostream>
#include <fstream>
#include "Game.h"
#include"Utils.h"
using namespace std;



CWorldMap::CWorldMap(int id, LPCWSTR filePath) :
	CScene(id, filePath)
{
	key_handler = new CWorldMapKeyHandler(this);
}

CWorldMap::~CWorldMap()
{
}


#define SCENE_SECTION_UNKNOWN			-1
#define SCENE_SECTION_TEXTURES			2
#define SCENE_SECTION_SPRITES			3
#define SCENE_SECTION_ANIMATIONS		4
#define SCENE_SECTION_ANIMATION_SETS	5
#define SCENE_SECTION_OBJECTS			6
#define SCENE_SECTION_MAP				7

#define OBJECT_TYPE_MARIO				0
#define OBJECT_TYPE_HELP				1
#define OBJECT_TYPE_GOLD_DIGGER			2
#define OBJECT_TYPE_BUSH				3
#define OBJECT_TYPE_NODE_NORMAL			4
#define OBJECT_TYPE_NODE_SPECIAL		5

#define MAX_SCENE_LINE 1024


void CWorldMap::_ParseSection_TEXTURES(string line)
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

void CWorldMap::_ParseSection_SPRITES(string line)
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

void CWorldMap::_ParseSection_ANIMATIONS(string line)
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

void CWorldMap::_ParseSection_ANIMATION_SETS(string line)
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
void CWorldMap::_ParseSection_OBJECTS(string line)
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
	Node* node = NULL;

	switch (object_type)
	{
	case OBJECT_TYPE_MARIO:
		obj = new CWorldMapObjects(11);
		break;
	case OBJECT_TYPE_HELP:
		obj = new CWorldMapObjects(22);
		break;
	case OBJECT_TYPE_GOLD_DIGGER:
		obj = new CWorldMapObjects(33);
		break;
	case OBJECT_TYPE_BUSH:
		obj = new CWorldMapObjects(44);
		break;
	case  OBJECT_TYPE_NODE_NORMAL:
	{
		int node_id = atof(tokens[4].c_str());
		obj = new Node(55, node_id);
		node = (Node*)obj;
		break;
	}
	/*case  OBJECT_TYPE_NODE_SPECIAL:
		obj = new Node(66);
		node = (Node*)obj;
		break;*/
	default:
		DebugOut(L"[ERR] Invalid object type: %d\n", object_type);
		return;
	}

	// General object setup
	obj->SetPosition(x, y);

	LPANIMATION_SET ani_set = animation_sets->Get(ani_set_id);

	obj->SetAnimationSet(ani_set);
	objects.push_back(obj);

	Nodes.push_back(node);

}


void CWorldMap::_ParseSection_MAP(string line)
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


void CWorldMap::Load()
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
		}
	}

	f.close();

	CTextures::GetInstance()->Add(ID_TEX_BBOX, L"textures\\bbox.png", D3DCOLOR_XRGB(255, 255, 255));

	DebugOut(L"[INFO] Done loading scene resources %s\n", sceneFilePath);
}

void CWorldMap::Update(DWORD dt)
{

	vector<LPGAMEOBJECT> coObjects;
	for (size_t i = 0; i < objects.size(); i++)
	{
		coObjects.push_back(objects[i]);
	}


	for (size_t i = 0; i < objects.size(); i++)
	{
		objects[i]->Update(dt, &coObjects);
	}




	// skip the rest if scene was already unloaded (Mario::Update might trigger PlayScene::Unload)

	CGame::GetInstance()->SetCamPos(0, 0);

}

void CWorldMap::Render()
{
	if (map)
	{
		this->map->Render();
	}

	for (int i = 0; i < objects.size(); i++)
		objects[i]->Render();
}

/*
	Unload current scene
*/
void CWorldMap::Unload()
{
	for (int i = 0; i < objects.size(); i++)
		delete objects[i];

	objects.clear();
	delete map;
	map = nullptr;

	DebugOut(L"[INFO] Scene %s unloaded! \n", sceneFilePath);
}

void CWorldMapKeyHandler::OnKeyDown(int KeyCode)
{
	
	vector<LPGAMEOBJECT>  objects = ((CWorldMap*)scence)->GetObjects();
	CWorldMapObjects* mario = (CWorldMapObjects*)objects.at(0);

	Node* current_node = ((CWorldMap*)scence)->GetCurrentNode();

	vector<Node*> Nodes = ((CWorldMap*)CGame::GetInstance()->GetCurrentScene())->GetNodes();
	//Node* node_top_result = current_node->FindNodeTop();
	//Node* node_bottom_result = current_node->FindNodeBottom();

	//Node* node_left_result = current_node->FindNodeLeft();

	switch (KeyCode)
	{
	case DIK_DOWN:
		if (mario->GetMarioMoveControl() /*&& node_bottom_result != NULL*/)
		{
			/*current_node = node_bottom_result;*/
			mario->SetState(MARIO_STATE_MOVE_DOWN);
		}
		break;
	case DIK_UP:
		if (mario->GetMarioMoveControl() /*&& node_top_result != NULL*/)
		{
			/*current_node = node_top_result;*/
			mario->SetState(MARIO_STATE_MOVE_UP);
		}
		break;
	case DIK_LEFT:
		if (mario->GetMarioMoveControl() /*&& node_left_result != NULL*/)
		{
			/*current_node = node_left_result;*/
			mario->SetState(MARIO_STATE_MOVE_LEFT);
		}
		break;
	case DIK_RIGHT:
		
		if (mario->GetMarioMoveControl() /*&& node_right_result != NULL*/)
		{
			//current_node = node_right_result;
			mario->SetState(MARIO_STATE_MOVE_RIGHT);
		}
		break;
	case DIK_G:
		CGame::GetInstance()->SwitchScene(3);
		break;
	}

}
void CWorldMapKeyHandler::OnKeyUp(int KeyCode)
{

}

void CWorldMapKeyHandler::KeyState(BYTE *states)
{

}

vector<Node*> CWorldMap::GetNodes()
{
	return Nodes;
}

Node* CWorldMap::GetCurrentNode()
{
	return current_node;
}