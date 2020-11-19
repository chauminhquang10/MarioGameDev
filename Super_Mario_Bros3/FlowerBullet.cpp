#include "FlowerBullet.h"

void CFlowerBullet::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (!dynamic_cast<CMario *>(coObjects->at(i)))
		{
			continue;
		}

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



	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	//CalcPotentialCollisions(coObjects, coEvents);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CFlower *>(obj))
		{
			CFlower *flower = dynamic_cast<CFlower *>(obj);
			if (abs(mario->x - flower->x) <= 160)
			{
				if (flower->GetIsFiring() && !isUsed && flower->GetType() != FLOWER_GREEN)
				{
					if (!flower->GetIsFired())
					{
						y = flower->y + 5;
						if (mario->y <= flower->y)
						{
							flower->SetIsShootingUp(-1);
						}
						else
						{
							flower->SetIsShootingUp(1);
						}
						if (mario->x <= flower->x)
						{
							x = flower->x - 1;
							flower->nx = -1;
						}
						else
						{
							x = flower->x + FLOWER_RED_BBOX_WIDTH + 2;
							flower->nx = 1;
						}

						if (abs(mario->x - this->x) <= FLOWER_BULLET_X_LIMIT)
						{
							vx = FLOWER_BULLET_FLYING_SPEED * flower->nx;
							vy = 0.05f * flower->GetIsShootingUp();
						}
						else
						{
							vx = FLOWER_BULLET_FLYING_SPEED * flower->nx;
							vy = 0.02f * flower->GetIsShootingUp();
						}
					}
					SetState(FLOWER_BULLET_STATE_FLYING);
					flower->SetIsFired(true);
				}
			}
		}
	}




if (this->y >= 170 || this->y <= -100 )
{
	isUsed = false;
	SetState(FIRE_BULLET_STATE_HIDDEN);

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
{
	int ani = -1;

	if (isUsed)
	{
		ani = FLOWER_BULLET_ANI;
	}
	else return;

	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CFlowerBullet::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case FIRE_BULLET_STATE_FLYING:
		isUsed = true;
		break;
	case FIRE_BULLET_STATE_HIDDEN:
		vx = 0;
		vy = 0;
		SetPosition(2000, 2000);
		break;
	}


}

void CFlowerBullet::GetBoundingBox(float &l, float &t, float &r, float &b)
{

	l = x;
	t = y;
	r = x + FLOWER_BULLET_BBOX_WIDTH;
	b = y + FLOWER_BULLET_BBOX_HEIGHT;
}