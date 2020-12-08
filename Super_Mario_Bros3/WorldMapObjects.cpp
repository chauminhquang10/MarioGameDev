
#include "WorldMapObjects.h"

CWorldMapObjects::CWorldMapObjects(int ctype)
{
	type = ctype;
	if (type == WORLD_MAP_TYPE_GOLD_DIGGER)
		SetState(GOLD_DIGGER_STATE_WALKING_RIGHT);
	if (type == WORLD_MAP_TYPE_MARIO)
	{
		SetState(MARIO_STATE_CANT_MOVE);
	}
}



void CWorldMapObjects::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	x += dx;
	y += dy;

	
	switch (type)
	{
	case WORLD_MAP_TYPE_GOLD_DIGGER:
		if (state == GOLD_DIGGER_STATE_WALKING_RIGHT)
		{
			GoldDiggerStartWalking();
			if (GetTickCount() - gold_digger_walk_start >= 1500)
			{
				SetState(GOLD_DIGGER_STATE_WALKING_LEFT);
				gold_digger_walk_start = 0;
			}
		}
		else
		{
			GoldDiggerStartWalking();
			if (GetTickCount() - gold_digger_walk_start >= 1500)
			{
				SetState(GOLD_DIGGER_STATE_WALKING_RIGHT);
				gold_digger_walk_start = 0;
			}
		}
		break;
	case WORLD_MAP_TYPE_HELP:
		HelpAppearStart();
		if (GetTickCount() - help_appear_start >= 1000)
		{
			isHelpAppear = -isHelpAppear;
			help_appear_start = 0;
		}
		break;
	case WORLD_MAP_TYPE_MARIO:
		if (state != MARIO_STATE_CANT_MOVE)
		{
			MarioMoveStart();
			if (GetTickCount() - mario_move_start >= 200)
			{
				SetState(MARIO_STATE_CANT_MOVE);
				mario_move_start = 0;
				mario_move_control = true;
			}
			else
			{
				mario_move_control = false;
			}
		}
	}
	
}

void CWorldMapObjects::Render()
{
	int ani = -1;
	switch (type)
	{
	case WORLD_MAP_TYPE_BUSH:
		ani = WORLD_MAP_TYPE_ANI_BUSH;
		break;
	case WORLD_MAP_TYPE_GOLD_DIGGER:
		if (state == GOLD_DIGGER_STATE_WALKING_RIGHT)
			ani = WORLD_MAP_TYPE_ANI_GOLD_DIGGER_WALK_RIGHT;
		else
			ani = WORLD_MAP_TYPE_ANI_GOLD_DIGGER_WALK_LEFT;
		break;
	case WORLD_MAP_TYPE_HELP:
		if (isHelpAppear == 1)
			ani = WORLD_MAP_TYPE_ANI_HELP;
		else return;
		break;
	case WORLD_MAP_TYPE_MARIO:
		ani = WORLD_MAP_TYPE_ANI_MARIO;
		break;
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}





void CWorldMapObjects::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case MARIO_STATE_CANT_MOVE:
		vx = vy = 0;
		break;
	case MARIO_STATE_MOVE_RIGHT:
		vx = 0.22f;
		break;
	case MARIO_STATE_MOVE_LEFT:
		vx = -0.22f;
		break;
	case MARIO_STATE_MOVE_UP:
		vy = -0.22f;
		break;
	case MARIO_STATE_MOVE_DOWN:
		vy = 0.22f;
		break;
	case GOLD_DIGGER_STATE_WALKING_RIGHT:
		vx = 0.02f;
		break;
	case GOLD_DIGGER_STATE_WALKING_LEFT:
		vx = -0.02f;
		break;
	}
}