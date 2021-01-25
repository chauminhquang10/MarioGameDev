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
	{
		int id = CGame::GetInstance()->GetCurrentScene()->GetId();
		if (id != INTRO_SCENE_ID)
		{
			if (colli_time_tail != 0)
				CalcPotentialCollisions(coObjects, coEvents);
		}
		else
			CalcPotentialCollisions(coObjects, coEvents);
	}

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CQuestionBrick *>(obj))
		{
			CQuestionBrick *question_brick = dynamic_cast<CQuestionBrick *>(obj);
			if (!question_brick->GetIsAlive() && question_brick->GetType() == QUESTION_BRICK_HAVE_LEAF && !question_brick->GetIsUsed())
			{
				if (mario->GetLevel() != MARIO_LEVEL_SMALL)
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
			if (background_stage->GetType() == BACKGROUND_STAGE_TYPE_FINAL && background_stage->GetIsAppear())
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
		int id = CGame::GetInstance()->GetCurrentScene()->GetId();
		if (id != INTRO_SCENE_ID)
		{
			if (GetTickCount() - upping_start >= LEAF_UPPING_START_TIME_1)
			{
				StartColliTimeTail();
			}
		}
		else
			StartColliTimeTail();
		if (GetTickCount() - upping_start >= LEAF_UPPING_START_TIME_2)
		{
			SetState(LEAF_STATE_DOWN);
		}

	}


	if (state == LEAF_STATE_DOWN)
	{
		if (downing_start == LEAF_DOWNING_START_TIME_ORIGIN_VALUE)
		{
			StartDowning();
		}
		if (GetTickCount() - downing_start >= LEAF_DOWNING_START_TIME)
		{
			vx = -vx;
			downing_start = LEAF_DOWNING_START_TIME_ORIGIN_VALUE;
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
		//vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();

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
					mario->StartTransforming();
					mario->SetLevel(MARIO_LEVEL_BIG);
					mario->SetTransformRecog(true);
					isAppear = false;
					SetPosition(LEAF_SET_POSITION, LEAF_SET_POSITION);
				}
				else if (mario->GetLevel() == MARIO_LEVEL_BIG)
				{
					mario->StartTransforming();
					int id = CGame::GetInstance()->GetCurrentScene()->GetId();
					if (id != INTRO_SCENE_ID)
					{
						mario->SetLevel(MARIO_LEVEL_TAIL);
						mario->SetTransformRecog(true);
					}
					isAppear = false;
					SetPosition(LEAF_SET_POSITION, LEAF_SET_POSITION);
				}

				else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
				{
					mario->SetTransformRecog(true);
					if (GetTickCount() - colli_time_tail >= COLLI_TIME_TAIL)
					{
						isAppear = false;
						SetPosition(LEAF_SET_POSITION, LEAF_SET_POSITION);
						CGame::GetInstance()->ScoreUp(SCORE_VALUE_1000);
					}
				}
				else
				{
					isAppear = false;
					SetPosition(LEAF_SET_POSITION, LEAF_SET_POSITION);
					mario->SetTransformRecog(true);
				}
				int id = CGame::GetInstance()->GetCurrentScene()->GetId();
				if (id == PLAY_SCENE_1_1_ID || id ==PLAY_SCENE_1_4_ID)
				{
					vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
					mario->SetShowPointX(mario->x);
					mario->SetShowPointY(mario->y);
					this->SetIsAllowToShowScore(true);
					for (unsigned int i = 0; i < scores_panel.size(); i++)
					{
						CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
						if (!score_panel->GetIsUsed())
						{
							score_panel->SetValue(SCORE_VALUE_1000);
							score_panel->SetIsUsed(true);
							break;
						}
						
					}
					if (mario->GetLevel() != MARIO_LEVEL_TAIL)
						CGame::GetInstance()->ScoreUp(SCORE_VALUE_1000);
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
		vx = vy = LEAF_STATE_IDLE_SPEED;
		break;
	case LEAF_STATE_UP:
		vy = -LEAF_STATE_UP_SPEED;
		break;
	case LEAF_STATE_DOWN:
		vx = LEAF_STATE_DOWN_VX;
		vy = LEAF_STATE_DOWN_VY;
		break;
	}
}


