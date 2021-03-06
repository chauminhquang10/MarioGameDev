#include "Coin.h"
#include "Mario.h"

CCoin::CCoin(int ctype)
{
	type = ctype;
	if (type == COIN_NORMAL)
	{
		isAppear = true;
	}
	else
		isAppear = false;
	SetState(COIN_STATE_IDLE);
}




void CCoin::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
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


void CCoin::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isAppear && type != COIN_CAN_MOVE)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();



	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CQuestionBrick *>(obj))
		{
			CQuestionBrick *question_brick = dynamic_cast<CQuestionBrick *>(obj);
			int id = CGame::GetInstance()->GetCurrentScene()->GetId();
			if (id == PLAY_SCENE_1_1_ID)
			{
				if (!question_brick->GetIsAlive() && question_brick->GetType() == QUESTION_BRICK_HAVE_LEAF && !question_brick->GetIsUsed())
				{
					if (!isAppear && type == COIN_CAN_MOVE)
					{
						if ((this->x == question_brick->x) && (this->y == question_brick->y))
						{
							SetState(COIN_STATE_UP);
							StartTiming();
							isAppear = true;
							CGame::GetInstance()->MoneyUp();
							question_brick->SetIsUsed(true);
						}
					}

				}
			}
			else if (id == PLAY_SCENE_1_4_ID)
			{
				if (question_brick->GetIsAlive() && question_brick->GetType() == QUESTION_BRICK_HAVE_COIN_MULTIPLE_LIFE && !question_brick->GetControlMultipleCoin() && question_brick->GetIsAllowToShowMultipleCoin())
				{
					if (!isAppear && type == COIN_CAN_MOVE)
					{
						if ((this->x == question_brick->x) && (this->y == question_brick->y))
						{
							SetState(COIN_STATE_UP);
							StartTiming();
							isAppear = true;
							CGame::GetInstance()->MoneyUp();
							question_brick->SetControlMultipleCoin(true);
						}
					}
				}
			}
		}
	}





	if (state == COIN_STATE_UP)
	{
		if (GetTickCount() - timing_start >= COIN_STATE_UP_TIME_UP)
		{
			SetState(COIN_STATE_DOWN);
			StartTiming();
		}
	}

	if (state == COIN_STATE_DOWN)
	{
		if (GetTickCount() - timing_start >= COIN_STATE_DOWN_TIME_DOWN)
		{
			isAppear = false;
			CGame::GetInstance()->ScoreUp(SCORE_VALUE_100);
			SetState(COIN_STATE_IDLE);
			vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
			mario->SetShowPointX(this->x);
			mario->SetShowPointY(this->y - COIN_POSITION_SHOW_SCORE_Y);
			for (unsigned int i = 0; i < scores_panel.size(); i++)
			{
				CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
				if (!score_panel->GetIsUsed())
				{
					score_panel->SetValue(SCORE_VALUE_100);
					score_panel->SetIsUsed(true);
					break;
				}
				
			}
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

void CCoin::Render()
{
	if (!isAppear)
		return;

	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}



void CCoin::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (!isAppear || type == COIN_CAN_MOVE)
		return;
	else
	{
		l = x;
		t = y;
		r = x + COIN_BBOX_WIDTH;
		b = y + COIN_BBOX_HEIGHT;
	}
}

void CCoin::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case COIN_STATE_IDLE:
		vx = vy = COIN_STATE_IDLE_SPEED;
		break;
	case COIN_STATE_UP:
		vy = -COIN_STATE_UP_SPEED;
		break;
	case COIN_STATE_DOWN:
		vy = COIN_STATE_DOWN_SPEED;
		break;
	}
}