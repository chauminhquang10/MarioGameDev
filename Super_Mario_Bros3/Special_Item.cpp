
#include "Special_Item.h"

CSpecial_Item::CSpecial_Item()
{
	SetState(SPECIAL_ITEM_STATE_MUSHROOM_IDLE);
}

void CSpecial_Item::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (!dynamic_cast<CMario *>(coObjects->at(i)))
		{
			continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CSpecial_Item::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (vy==0)
	{
		l = x;
		t = y;
		r = x + SPECIAL_ITEM_BBOX_WIDTH;
		b = y + SPECIAL_ITEM_BBOX_HEIGHT;

	}
	else
	{
		l = t = r = b = 0;
	}
}

void CSpecial_Item::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isAppear)
		CalcPotentialCollisions(coObjects, coEvents);

	


	if (vy != 0)
	{
		StartUpping();
		if (GetTickCount() - upping_start >= 1400)
		{
			isAppear = false;
		}
	}
	else
	{
		StartSwitchState();
		if (GetTickCount() - switching_state_start >= 250)
		{
			switch (state)
			{
			case SPECIAL_ITEM_STATE_FLOWER_IDLE:
				SetState(SPECIAL_ITEM_STATE_STAR_IDLE);
				break;
			case SPECIAL_ITEM_STATE_MUSHROOM_IDLE:
				SetState(SPECIAL_ITEM_STATE_FLOWER_IDLE);
				break;
			case SPECIAL_ITEM_STATE_STAR_IDLE:
				SetState(SPECIAL_ITEM_STATE_MUSHROOM_IDLE);
				break;
			}
			switching_state_start = 0;
		}
	}


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CSpecial_Item::Render()
{
	int ani = -1;

	if (isAppear)
	{
		switch (state)
		{
		case SPECIAL_ITEM_STATE_FLOWER_IDLE:
			ani = SPECIAL_ITEM_STATE_FLOWER_IDLE_ANI;
			break;
		case SPECIAL_ITEM_STATE_MUSHROOM_IDLE:
			ani = SPECIAL_ITEM_STATE_MUSHROOM_IDLE_ANI;
			break;
		case SPECIAL_ITEM_STATE_STAR_IDLE:
			ani = SPECIAL_ITEM_STATE_STAR_IDLE_ANI;
			break;
		case SPECIAL_ITEM_STATE_FLOWER_UP:
			ani = SPECIAL_ITEM_STATE_FLOWER_UP_ANI;
			break;
		case SPECIAL_ITEM_STATE_MUSHROOM_UP:
			ani = SPECIAL_ITEM_STATE_MUSHROOM_UP_ANI;
			break;
		case SPECIAL_ITEM_STATE_STAR_UP:
			ani = SPECIAL_ITEM_STATE_STAR_UP_ANI;
			break;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CSpecial_Item::SetState(int state)
{
	CGameObject::SetState(state);
	//CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case SPECIAL_ITEM_STATE_FLOWER_IDLE:
	case SPECIAL_ITEM_STATE_MUSHROOM_IDLE:
	case SPECIAL_ITEM_STATE_STAR_IDLE:
		vx = vy = 0;
		break;
	case SPECIAL_ITEM_STATE_FLOWER_UP:
	case SPECIAL_ITEM_STATE_MUSHROOM_UP:
	case SPECIAL_ITEM_STATE_STAR_UP:
		vy = -0.09f;
		break;
	}
}



