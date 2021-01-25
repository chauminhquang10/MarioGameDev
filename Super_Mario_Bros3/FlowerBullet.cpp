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
			if (abs(mario->x - flower->x) <= FLOWER_BULLET_ALLOW_TO_SHOW_DISTANCE)
			{
				if (flower->GetIsFiring() && !isUsed && flower->GetType() != FLOWER_GREEN)
				{
					if (!flower->GetIsFired())
					{
						y = flower->y + FLOWER_BULLER_ADDTION_Y;
						if (mario->y <= flower->y)
						{
							flower->SetIsShootingUp(-FLOWER_IS_SHOOTING_UP_DIRECTION);
						}
						else
						{
							flower->SetIsShootingUp(FLOWER_IS_SHOOTING_UP_DIRECTION);
						}
						if (mario->x <= flower->x)
						{
							x = flower->x - FLOWER_BULLET_MINUS_X;
							flower->nx = -FLOWER_BULLET_CONTROL_FLOWER_NX;
						}
						else
						{
							x = flower->x + FLOWER_RED_BBOX_WIDTH + FLOWER_RED_BBOX_WIDTH_ADDITION_X;
							flower->nx = FLOWER_BULLET_CONTROL_FLOWER_NX;
						}

						if (abs(mario->x - this->x) <= FLOWER_BULLET_X_LIMIT)
						{
							vx = FLOWER_BULLET_FLYING_SPEED * flower->nx;
							vy = FLOWER_BULLET_SPEED_VY_1 * flower->GetIsShootingUp();
						}
						else
						{
							vx = FLOWER_BULLET_FLYING_SPEED * flower->nx;
							vy = FLOWER_BULLET_SPEED_VY_2 * flower->GetIsShootingUp();
						}
					}
					SetState(FLOWER_BULLET_STATE_FLYING);
					flower->SetIsFired(true);
				}
			}
		}
	}




if (this->y >= FLOWER_BULLET_LIMIT_Y_1 || this->y <= -FLOWER_BULLET_LIMIT_Y_2)
{
	isUsed = false;
	SetState(FLOWER_BULLET_STATE_HIDDEN);

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
	case FLOWER_BULLET_STATE_FLYING:
		isUsed = true;
		break;
	case FLOWER_BULLET_STATE_HIDDEN:
		vx = FLOWER_BULLET_ORIGIN_SPEED;
		vy = FLOWER_BULLET_ORIGIN_SPEED;
		SetPosition(FLOWER_BULLET_ORIGIN_POSITION, FLOWER_BULLET_ORIGIN_POSITION);
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