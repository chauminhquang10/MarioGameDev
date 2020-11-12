#include "MushRoom.h"

CMushRoom::CMushRoom(int ctype)
{
	type = ctype;
	SetState(MUSHROOM_STATE_IDLE);
}

void CMushRoom::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (!dynamic_cast<CMario *>(coObjects->at(i)) && this->state != MUSHROOM_STATE_MOVE)
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
void CMushRoom::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isAppear)
	{
		l = x;
		t = y;
		r = x + MUSHROOM_BBOX_WIDTH;
		b = y + MUSHROOM_BBOX_HEIGHT;
	}
	else
	{
		l = t = r = b = 0;
	}
}

void CMushRoom::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	// Simple fall down
	if(state == MUSHROOM_STATE_MOVE)
	vy += MUSHROOM_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isAppear)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CQuestionBrick *>(obj))
		{
			CQuestionBrick *question_brick = dynamic_cast<CQuestionBrick *>(obj);
			if (!question_brick->GetIsAlive() && question_brick->GetType() == QUESTION_BRICK_HAVE_LEAF/* && !question_brick->GetIsUsed()*/)
			{
				if (mario->GetLevel() == MARIO_LEVEL_SMALL && type == MUSHROOM_RED)
				{
					if (!isAppear)
					{
						if ((this->x == question_brick->x) && (this->y == question_brick->y))
						{
							SetState(MUSHROOM_STATE_UP);
							isAppear = true;
							StartUpping();
							//question_brick->SetIsUsed(true);
						}

					}
				}
			}
		}
	}

	if (state == MUSHROOM_STATE_UP)
	{
		if (GetTickCount() - upping_start >= 300)
		{
			SetState(MUSHROOM_STATE_MOVE);
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

		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario *>(e->obj))
			{
				CMario *mario = dynamic_cast<CMario *>(e->obj);
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				{
					mario->SetLevel(MARIO_LEVEL_BIG);
					isAppear = false;
					SetPosition(6000, 6000);
				}
				else
				{
					isAppear = false;
					SetPosition(6000, 6000);
					//Cong diem
				}
			}
			else  // Collisions with other things  
			{
				if (nx != 0 && ny == 0)
				{
					if (!dynamic_cast<CMario *>(e->obj) && !dynamic_cast<CFireBullet *>(e->obj))
						vx = -vx;
				}
			}
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CMushRoom::Render()
{
	int ani = -1;

	if (isAppear)
	{
		if (type == MUSHROOM_RED)
			ani = MUSHROOM_ANI_RED;
		else
			ani = MUSHROOM_ANI_GREEN;
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CMushRoom::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case MUSHROOM_STATE_IDLE:
		vx = vy = 0;
		break;
	case MUSHROOM_STATE_MOVE:
		vx = 0.04f;
		break;
	case MUSHROOM_STATE_UP:
		vy = -0.08f;
		break;
	}
}


