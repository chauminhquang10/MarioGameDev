#include "BreakableBrick.h"


CBreakableBrick::CBreakableBrick()
{
	SetState(BREAKABLE_BRICK_STATE_NORMAL);
}

void CBreakableBrick::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}


void CBreakableBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (state ==  BREAKABLE_BRICK_STATE_NORMAL)
	{
		l = x;
		t = y;
		r = x + BREAKABLE_BRICK_BBOX_WIDTH;
		b = y + BREAKABLE_BRICK_BBOX_HEIGHT;
	}
	else if (state == BREAKABLE_BRICK_STATE_COIN)
	{
		l = x;
		t = y;
		r = x + COIN_BBOX_WIDTH;
		b = y + COIN_BBOX_HEIGHT;
	}
	else
	{
		l = t = r = b = 0;
	}
}

void CBreakableBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (state != BREAKABLE_BRICK_STATE_BREAK)
	CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();


	if (isRevive && reviveTime == 0)
		StartRevive();

	if (state == BREAKABLE_BRICK_STATE_COIN)
	{
		if (GetTickCount() - reviveTime >= 5000)
		{
			SetState(BREAKABLE_BRICK_STATE_NORMAL);
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
		//x += min_tx * dx + nx * 0.5f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.5f;

		/*if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;*/

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CBreakableBrick::Render()
{
	int ani = -1;

	if (state == BREAKABLE_BRICK_STATE_NORMAL)
		ani = BREAKABLE_BRICK_ANI_NORMAL;
	else if (state == BREAKABLE_BRICK_STATE_COIN)
		ani = BREAKABLE_BRICK_ANI_COIN;
	else return;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CBreakableBrick::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case BREAKABLE_BRICK_STATE_NORMAL:
	case BREAKABLE_BRICK_STATE_BREAK:	
	case BREAKABLE_BRICK_STATE_COIN:
		vx = vy = 0;
		break;
	}
}


