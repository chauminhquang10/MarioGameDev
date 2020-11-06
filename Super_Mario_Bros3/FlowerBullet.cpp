#include "FlowerBullet.h"

void CFlowerBullet::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);



}

void CFlowerBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt);



	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn on collision when firebullet used 
	//if (isUsed)

		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	/*if (mario->GetIsFiring() && !isUsed)
	{
		if (!mario->GetIsFired())
		{
			y = mario->y;
			if (mario->nx > 0)
			{
				x = mario->x + MARIO_FIRE_BBOX_WIDTH + 1;
				vx = FIRE_BULLET_FLYING_SPEED / 2;

			}
			else
			{
				x = mario->x - MARIO_FIRE_BBOX_WIDTH - 1;
				vx = -FIRE_BULLET_FLYING_SPEED / 2;

			}
			SetState(FIRE_BULLET_STATE_FLYING);
			vy = 0.1f;
			Height_Limit = mario->y;
			mario->SetIsFired(true);
		}
	}
	if (!isUsed)
	{
		SetPosition(1.0, 1.0);
	}
	if (this->y <= Height_Limit)
		vy = 0.1f;

*/
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

	/*	if (ny > 0)
			Height_Limit = this->y;

		if (ny != 0) vy = -vy;*/

		// Collision logic with the others Koopas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CFlowerBullet::Render()
{  /*
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int ani = -1;
	if (isUsed)
	{
		if (mario->nx > 0)
		{
			ani = FIRE_BULLET_ANI_RIGHT;
		}
		else
			ani = FIRE_BULLET_ANI_LEFT;
	}
	else return;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();*/
}

void CFlowerBullet::SetState(int state)
{
	CGameObject::SetState(state);
	/*CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case FIRE_BULLET_STATE_FLYING:
		isUsed = true;
		break;
	case FIRE_BULLET_STATE_HIDDEN:
		vx = 0;
		vy = 0;
		SetPosition(1, 1);
		break;
	}*/


}

void CFlowerBullet::GetBoundingBox(float &l, float &t, float &r, float &b)
{

	l = x;
	t = y;
	r = x + FLOWER_BULLET_BBOX_WIDTH;
	b = y + FLOWER_BULLET_BBOX_HEIGHT;
}