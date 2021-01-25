#include "BoomerangEnemy.h"

CBoomerangEnemy::CBoomerangEnemy()
{
	SetState(BOOMERANG_ENEMY_STATE_IDLE);
	nx = 1;
}

void CBoomerangEnemy::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CBoomerangEnemy::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isAllowToHaveBBox)
	{
		l = x;
		t = y;
		r = x + BOOMERANG_ENEMY_BBOX_WIDTH;
		b = y + BOOMERANG_ENEMY_BBOX_HEIGHT;
	}
	else return;


}

void CBoomerangEnemy::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);


	if (pre_get_tick_count == PRE_GET_TICK_COUNT_ORIGIN_VALUE)
		pre_get_tick_count = GetTickCount();
	else
	{
		if (GetTickCount() - pre_get_tick_count <= GET_TICK_COUNT_NORMAL_VALUE)
		{
			pre_get_tick_count = GetTickCount();
		}
		else
		{
			sub_time = GetTickCount() - pre_get_tick_count;
			pre_get_tick_count = GetTickCount();
		}
	}

	// Simple fall down

	vy += BOOMERANG_ENEMY_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isAlive)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();



	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id != INTRO_SCENE_ID)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetIsTurning())
		{
			float leftRec =(float) mario->GetLeftRecMarioTail();
			float topRec = (float)mario->GetTopRecMarioTail();
			float rightRec = (float)mario->GetRightRecMarioTail();
			float bottomRec = (float)mario->GetBottomRecMarioTail();


			if (bottomRec != MARIO_REC_BOTTOM_ORIGIN_VALUE && topRec != MARIO_REC_TOP_ORIGIN_VALUE && leftRec != MARIO_REC_LEFT_ORIGIN_VALUE && rightRec != MARIO_REC_RIGHT_ORIGIN_VALUE)
			{
				float leftRecBoomerangEnemy, rightRecBoomerangEnemy, topRecBoomerangEnemy, bottomRecBoomerangEnemy;
				this->GetBoundingBox(leftRecBoomerangEnemy, topRecBoomerangEnemy, rightRecBoomerangEnemy, bottomRecBoomerangEnemy);
				if (((leftRec <= rightRecBoomerangEnemy && leftRec >= leftRecBoomerangEnemy) || (rightRec <= rightRecBoomerangEnemy && rightRec >= leftRecBoomerangEnemy) || ((leftRec <= leftRecBoomerangEnemy) && (rightRec >= rightRecBoomerangEnemy))) && ((topRec <= bottomRecBoomerangEnemy && topRec >= topRecBoomerangEnemy) || (bottomRec <= bottomRecBoomerangEnemy && bottomRec >= topRecBoomerangEnemy)))
				{
					if (!mario->GetIsAllowToShowHitEffectTurnTail())
					{
						this->SetIsAlive(false);
						this->SetState(BOOMERANG_ENEMY_STATE_DIE);
						this->SetIsAllowToHaveBBox(false);
						this->vy = -KOOPAS_SHELL_DEFLECT_SPEED;
						int id = CGame::GetInstance()->GetCurrentScene()->GetId();
						if (id == PLAY_SCENE_1_4_ID)
						{
							vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
							mario->SetShowPointX(this->x);
							mario->SetShowPointY(this->y);
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
							CGame::GetInstance()->ScoreUp(SCORE_VALUE_1000);


							vector<LPGAMEOBJECT> hit_effects_turn_tail = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetHitEffectsTurnTail();
							mario->SetShowTurnTailEffectX(this->x);
							mario->SetShowTurnTailEffectY(this->y);
							mario->SetIsAllowToShowHitEffectTurnTail(true);
							for (unsigned int i = 0; i < hit_effects_turn_tail.size(); i++)
							{
								CHitEffect* hit_effects_turn_tail_object = dynamic_cast<CHitEffect*> (hit_effects_turn_tail[i]);
								if (!hit_effects_turn_tail_object->GetIsUsed())
								{
									hit_effects_turn_tail_object->SetIsUsed(true);
									hit_effects_turn_tail_object->SetIsAllowToShowHitEffectTurnTail(true);
									break;
								}

							}
						}

							
						
					}
				}
			}
		}
	}


	if (mario->x - this->x >= BOOMERANG_ENEMY_LOOK_RIGTH_LIMIT)
	{
		if (state != BOOMERANG_ENEMY_STATE_DIE)
		{
			nx = 1;
		}
	}
	else if (mario->x - this->x < BOOMERANG_ENEMY_LOOK_LEFT_LIMIT)
	{
		if (state != BOOMERANG_ENEMY_STATE_DIE)
		{
			nx = -1;
		}
	}


	if (isAlive)
	{

		if ((DWORD)GetTickCount64() - time_rendering_throw_ani >= RENDER_THROW_ANI_TIME_LIMIT)
		{
			isAllowToRenderThrowAni = false;
			time_rendering_throw_ani = 0;
		}


		StartTimeSwitchingState();

		for (UINT i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT obj = coObjects->at(i);
			if (dynamic_cast<CBoomerang*>(obj))
			{
				CBoomerang* boomerang = dynamic_cast<CBoomerang*>(obj);
				int boomerang_id = boomerang->GetId();
				if (boomerang_id == 1)
				{
					if (!boomerang->GetIsAllowToThrowBoomerang())
					{
						if ((DWORD)GetTickCount64() - time_switch_state >= RENDER_THROW_TIME_LIMIT + sub_time)
						{
							if (this->nx > 0)
								boomerang->SetBoomerangDirection(1);
							else
								boomerang->SetBoomerangDirection(-1);
							boomerang->SetIsAllowToThrowBoomerang(true);
							boomerang->SetIsInState_1(true);
							isAllowToRenderThrowAni = true;
							StartTimeRenderingThrowAni();
							boomerang->StartTimeSwitchingState();
						}
					}
				}
				else
				{
					if (!boomerang->GetIsAllowToThrowBoomerang())
					{
						if ((DWORD)GetTickCount64() - time_switch_state >= RENDER_SECOND_THROW_TIME_LIMIT + sub_time)
						{
							if (this->nx > 0)
								boomerang->SetBoomerangDirection(1);
							else
								boomerang->SetBoomerangDirection(-1);
							boomerang->SetIsAllowToThrowBoomerang(true);
							boomerang->SetIsInState_1(true);
							isAllowToRenderThrowAni = true;
							StartTimeRenderingThrowAni();
							boomerang->StartTimeSwitchingState();
						}
					}
					//DebugOut(L"da vao day \n");
				}

			}
		}


		if ((DWORD)GetTickCount64() - time_switch_state >= BOOMETANG_ENEMY_STATE_MOVE_FORWARD_LIMIT + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_MOVE_FORWARD);
		}

		if ((DWORD)GetTickCount64() - time_switch_state >= BOOMETANG_ENEMY_STATE_IDLE_LIMIT + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_IDLE);
		}

		if ((DWORD)GetTickCount64() - time_switch_state >= BOOMETANG_ENEMY_STATE_MOVE_BACKWARD_LIMIT + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_MOVE_BACKWARD);
		}

		if ((DWORD)GetTickCount64() - time_switch_state >= BOOMETANG_ENEMY_STATE_IDLE_2_LIMIT + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_IDLE);
			time_switch_state = 0;
			sub_time = 0;
		}

	}
	else
	{
		SetState(BOOMERANG_ENEMY_STATE_DIE);
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



		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CBoomerangEnemy::Render()
{
	int ani = -1;

	if (isAlive)
	{
		if (isAllowToRenderThrowAni)
		{
			if (nx > 0)
				ani = BOOMERANG_ENEMY_ANI_THROW_BOOMERANG_RIGHT;
			else
			{
				ani = BOOMERANG_ENEMY_ANI_THROW_BOOMERANG_LEFT;
			}
		}
		else
		{
			if (nx > 0)
				ani = BOOMERANG_ENEMY_ANI_NORMAL_RIGHT;
			else
			{
				ani = BOOMERANG_ENEMY_ANI_NORMAL_LEFT;
			}
		}
	}
	else
	{
		if (nx > 0)
			ani = BOOMERANG_ENEMY_ANI_DIE_RIGHT;
		else
			ani = BOOMERANG_ENEMY_ANI_DIE_LEFT;
	}


	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBoomerangEnemy::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case  BOOMERANG_ENEMY_STATE_IDLE:
		vx = vy = 0;
		break;
	case  BOOMERANG_ENEMY_STATE_MOVE_FORWARD:
		vx = BOOMETANG_ENEMY_VX;
		vy = BOOMETANG_ENEMY_VY;
		break;
	case  BOOMERANG_ENEMY_STATE_MOVE_BACKWARD:
		vx = -BOOMETANG_ENEMY_VX;
		vy = BOOMETANG_ENEMY_VY;
		break;
	case BOOMERANG_ENEMY_STATE_DIE:
		vx = BOOMETANG_ENEMY_VX;
	}
}



