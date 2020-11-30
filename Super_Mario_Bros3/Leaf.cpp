#include "Leaf.h"


CLeaf::CLeaf()
{
	SetState(LEAF_STATE_IDLE);
}

void CLeaf::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
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
void CLeaf::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isAppear)
	{
		l = x;
		t = y;
		r = x + LEAF_BBOX_WIDTH;
		b = y + LEAF_BBOX_HEIGHT;
	}
	else
	{
		l = t = r = b = 0;
	}
}

void CLeaf::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
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
			if (!question_brick->GetIsAlive() && question_brick->GetType() == QUESTION_BRICK_HAVE_LEAF && !question_brick->GetIsUsed())
			{
				if (mario->GetLevel() == MARIO_LEVEL_BIG)
				{
					if (!isAppear)
					{
						if ((this->x == question_brick->x) && (this->y == question_brick->y))
						{
							SetState(LEAF_STATE_UP);
							StartUpping();
							isAppear = true;
							question_brick->SetIsUsed(true);

						}

					}
				}
			}
		}
		if (dynamic_cast<CBackGroundStage *>(obj))
		{
			CBackGroundStage *background_stage = dynamic_cast<CBackGroundStage *>(obj);
			if ( background_stage->GetType() == BACKGROUND_STAGE_TYPE_FINAL && background_stage->GetIsAppear())
			{
				if (!isAppear)
				{
					isAppear = true;
					SetState(LEAF_STATE_DOWN);
				}
				
			}
		}
	}



	if (state == LEAF_STATE_UP)
	{
		if (GetTickCount() - upping_start >= 1500)
		{
			SetState(LEAF_STATE_DOWN);
		}
	}


	if (state == LEAF_STATE_DOWN)
	{
		if (downing_start == 0)
		{
			StartDowning();
		}
		if (GetTickCount() - downing_start >= 500)
		{
			vx = -vx;
			downing_start = 0;
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
			if (dynamic_cast<CMario *>(e->obj))
			{
				CMario *mario = dynamic_cast<CMario *>(e->obj);
				if (mario->GetLevel() == MARIO_LEVEL_SMALL)
				{
					mario->SetLevel(MARIO_LEVEL_BIG);
					isAppear = false;
					SetPosition(5000, 5000);
				}
				else if (mario->GetLevel() == MARIO_LEVEL_BIG)
				{
					mario->SetLevel(MARIO_LEVEL_TAIL);
					isAppear = false;
					SetPosition(5000, 5000);
				}
				else
				{
					isAppear = false;
					SetPosition(5000, 5000);
					//Cong diem
				}
			}

		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CLeaf::Render()
{
	int ani = -1;

	if (isAppear)
	{
		if (vx < 0)
			ani = LEAF_ANI_LEFT;
		else
			ani = LEAF_ANI_RIGHT;
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CLeaf::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case LEAF_STATE_IDLE:
		vx = vy = 0;
		break;
	case LEAF_STATE_UP:
		vy = -0.04f;
		break;
	case LEAF_STATE_DOWN:
		vx = 0.04f;
		vy = 0.03f;
		break;
	}
}


