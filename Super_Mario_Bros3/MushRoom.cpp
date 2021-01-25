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
		if (dynamic_cast<CFlowerBullet *>(coObjects->at(i)) || dynamic_cast<CFireBullet *>(coObjects->at(i)))
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
	if(haveGravity)
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
			if (!question_brick->GetIsAlive() && !question_brick->GetIsUsed())
			{
				if (type == MUSHROOM_RED )
				{
					if (mario->GetLevel() == MARIO_LEVEL_SMALL && question_brick->GetType() == QUESTION_BRICK_HAVE_LEAF)
					{
						if (!isAppear)
						{
							if ((this->x == question_brick->x) && (this->y == question_brick->y))
							{
								SetState(MUSHROOM_STATE_UP);
								isAppear = true;
								StartUpping();
								question_brick->SetIsUsed(true);
								if (mario->GetMushroomCheckPosition() >= (question_brick->x + 8))
								{
									moveDirection = -MUSHROOM_MOVE_DIRECTION;
								}
							}
						}
					}
				}
				else
				{
					if (!isAppear  && question_brick->GetType() == QUESTION_BRICK_JUST_HAVE_MUSHROOM)
					{
						if ((this->x == question_brick->x) && (this->y == question_brick->y))
						{
							SetState(MUSHROOM_STATE_UP);
							isAppear = true;
							StartUpping();
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
				isAppear = true;
				haveGravity = true;
			}
		}
	}

	if (state == MUSHROOM_STATE_UP)
	{
		if (GetTickCount() - upping_start >= MUSHROOM_UPPING_START)
		{
			SetState(MUSHROOM_STATE_MOVE);
			haveGravity = true;
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
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		//if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		int id = CGame::GetInstance()->GetCurrentScene()->GetId();
		if (id == INTRO_SCENE_ID)
		{
			SetState(MUSHROOM_STATE_MOVE_LEFT);		
		}

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CMario *>(e->obj))
			{
				CMario *mario = dynamic_cast<CMario *>(e->obj);
				int id = CGame::GetInstance()->GetCurrentScene()->GetId();
				if (id == PLAY_SCENE_1_1_ID || id == PLAY_SCENE_1_4_ID)
				{
					mario->SetShowPointX(mario->x);
					mario->SetShowPointY(mario->y);
					this->SetIsAllowToShowScore(true);
				}
				if (type == MUSHROOM_RED)
				{
					if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					{
						mario->StartTransforming();
						mario->SetLevel(MARIO_LEVEL_BIG);	
						mario->SetTransformRecog(true);
						isAppear = false;
						SetPosition(MUSHROOM_SET_POSITION, MUSHROOM_SET_POSITION);
					}
					else
					{
						mario->SetTransformRecog(true);
						isAppear = false;
						SetPosition(MUSHROOM_SET_POSITION, MUSHROOM_SET_POSITION);
					}
					int id = CGame::GetInstance()->GetCurrentScene()->GetId();
					if (id == PLAY_SCENE_1_1_ID || id == PLAY_SCENE_1_4_ID)
					{
						vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
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
						CGame::GetInstance()->ScoreUp(SCORE_VALUE_1000);
					}
				}
				else
				{
					isAppear = false;
					SetPosition(MUSHROOM_SET_POSITION, MUSHROOM_SET_POSITION);
					CGame::GetInstance()->SetLifeUp();
					int id = CGame::GetInstance()->GetCurrentScene()->GetId();
					if (id == PLAY_SCENE_1_1_ID || id == PLAY_SCENE_1_4_ID)
					{
						vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
						for (unsigned int i = 0; i < scores_panel.size(); i++)
						{
							CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
							if (!score_panel->GetIsUsed())
							{
								score_panel->SetValue(SCORE_VALUE_1VP);
								score_panel->SetIsUsed(true);
								break;
							}
							
						}
					}
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
		vx = vy = MUSHROOM_STATE_IDLE_SPEED;
		break;
	case MUSHROOM_STATE_MOVE:
		vx = MUSHROOM_STATE_MOVE_SPEED *moveDirection;
		break;
	case MUSHROOM_STATE_MOVE_LEFT:
		vx = -MUSHROOM_STATE_MOVE_LEFT_SPEED;
		break;
	case MUSHROOM_STATE_UP:
		vy = -MUSHROOM_STATE_UP_SPEED;
		break;
	}
}


