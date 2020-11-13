#include "Bell.h"

CBell::CBell()
{
	SetState(BELL_STATE_IDLE);
}

void CBell::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
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
void CBell::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isAppear)
	{
		l = x;
		t = y;
		r = x + BELL_BBOX_WIDTH;
		b = y + BELL_BBOX_HEIGHT;
	}
	else
	{
		l = t = r = b = 0;
	}
}

void CBell::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	

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
			if (!question_brick->GetIsAlive() && !question_brick->GetIsUsed())
			{
				if (question_brick->GetType() == QUESTION_BRICK_JUST_HAVE_MUSHROOM)
				{
					if (!isAppear)
					{
						if ((this->x == question_brick->x) && (this->y == question_brick->y))
						{
							SetState(BELL_STATE_UP);
							isAppear = true;
							StartUpping();
							question_brick->SetIsUsed(true);

						}
					}
				}
			}
		}
		
	}

	if (state == BELL_STATE_UP)
	{
		if (GetTickCount() - upping_start >= 100)
		{
			SetState(BELL_STATE_IDLE);
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
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
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

void CBell::Render()
{
	int ani = -1;

	if (isAppear)
	{
		ani = BELL_ANI;
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBell::SetState(int state)
{
	CGameObject::SetState(state);
	//CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case BELL_STATE_IDLE:
		vx = vy = 0;
		break;
	case BELL_STATE_UP:
		vy = -0.1f;
		break;
	}
}



