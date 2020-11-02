#include "FireBullet.h"

CFireBullet::CFireBullet() : CGameObject()
{
	
}





void CFireBullet::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
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

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt);

	// Simple fall down
	vy += FIRE_BULLET_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn on collision when firebullet used 
	if (isUsed)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();



	if (mario->GetIsFiring() && !isUsed )
	{
		y = mario->y ;
		if (this->nx > 0)
		{
			if (mario->GetLevel() == MARIO_LEVEL_BIG)
			{
				x = mario->x + MARIO_BIG_BBOX_WIDTH;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				x = mario->x + MARIO_SMALL_BBOX_WIDTH;
				y = y - 10;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			{
				x = mario->x + MARIO_TAIL_BBOX_WIDTH;
			}
			else
			{
				x = mario->x + MARIO_FIRE_BBOX_WIDTH;
			}
		}
		else
		{
			if (mario->GetLevel() == MARIO_LEVEL_BIG)
			{
				x = mario->x - MARIO_BIG_BBOX_WIDTH;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				x = mario->x - MARIO_SMALL_BBOX_WIDTH;
				y = y - 10;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			{
				x = mario->x - MARIO_TAIL_BBOX_WIDTH;
			}
			else
			{
				x = mario->x - MARIO_FIRE_BBOX_WIDTH;
			}
		}
		isUsed = true;
		
	}

	if (isUsed)
	{
		if (nx > 0)
			state = FIRE_BULLET_STATE_FLYING_RIGHT;
		else
			state = FIRE_BULLET_STATE_FLYING_LEFT;
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


		if (ny != 0) vy = 0;

		// Collision logic with the Goomba
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				if (e->nx != 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE_BY_KICK)
					{
						goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
					}
				}

			}
			else
				vy = -vy;




		}
	}
}

	

void CFireBullet::Render()
{
	int ani = -1;
	if (isUsed)
	{
		if (nx > 0)
		{
			ani = FIRE_BULLET_ANI_RIGHT;
		}
		else
			ani = FIRE_BULLET_ANI_LEFT;
	}
	else return;
	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CFireBullet::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case FIRE_BULLET_STATE_FLYING_RIGHT:
		vx = FIRE_BULLET_FLYING_SPEED;
		vy = FIRE_BULLET_FLYING_SPEED;
		break;
	case FIRE_BULLET_STATE_FLYING_LEFT:
		vx = -FIRE_BULLET_FLYING_SPEED;
		vy = -FIRE_BULLET_FLYING_SPEED;
		break;
	}


}



void CFireBullet::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + FIRE_BULLET_BBOX_WIDTH;
	b = y + FIRE_BULLET_BBOX_HEIGHT;
}