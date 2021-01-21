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


	if (pre_get_tick_count == 0)
		pre_get_tick_count = GetTickCount();
	else
	{
		if (GetTickCount() - pre_get_tick_count <= 50)
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
	if (id != 1)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetIsTurning())
		{
			float leftRec = mario->GetLeftRecMarioTail();
			float topRec = mario->GetTopRecMarioTail();
			float rightRec = mario->GetRightRecMarioTail();
			float bottomRec = mario->GetBottomRecMarioTail();


			if (bottomRec != 0 && topRec != 0 && leftRec != 0 && rightRec != 0)
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
						if (id == 4)
						{
							vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
							mario->SetShowPointX(this->x);
							mario->SetShowPointY(this->y);
							this->SetIsAllowToShowScore(true);
							for (int i = 0; i < scores_panel.size(); i++)
							{
								CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
								if (!score_panel->GetIsUsed())
								{
									score_panel->SetValue(1000);
									score_panel->SetIsUsed(true);
									break;
								}
							}
							CGame::GetInstance()->ScoreUp(1000);


							vector<LPGAMEOBJECT> hit_effects_turn_tail = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetHitEffectsTurnTail();
							mario->SetShowTurnTailEffectX(this->x);
							mario->SetShowTurnTailEffectY(this->y);
							mario->SetIsAllowToShowHitEffectTurnTail(true);
							for (int i = 0; i < hit_effects_turn_tail.size(); i++)
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


	if (mario->x - this->x >= 18)
	{
		if (state != BOOMERANG_ENEMY_STATE_DIE)
		{
			nx = 1;
		}
	}
	else if ((mario->x - this->x <= -1))
	{
		if (state != BOOMERANG_ENEMY_STATE_DIE)
		{
			nx = -1;
		}
	}


	if (isAlive)
	{

		if (GetTickCount() - time_rendering_throw_ani >= 300)
		{
			isAllowToRenderThrowAni = false;
			time_rendering_throw_ani = 0;
		}


		StartTimeSwitchingState();

		for (UINT i = 0; i < coObjects->size(); i++)
		{
			LPGAMEOBJECT obj = coObjects->at(i);
			if (dynamic_cast<CBoomerang *>(obj))
			{
				CBoomerang *boomerang = dynamic_cast<CBoomerang *>(obj);
				int boomerang_id = boomerang->GetId();
				if (boomerang_id == 1)
				{
					if (!boomerang->GetIsAllowToThrowBoomerang())
					{
						if (GetTickCount() - time_switch_state >= 900 + sub_time)
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
						if (GetTickCount() - time_switch_state >= 3300 + sub_time)
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


		if (GetTickCount() - time_switch_state >= 700 + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_MOVE_FORWARD);
		}

		if (GetTickCount() - time_switch_state >= 2000 + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_IDLE);
		}
		if (GetTickCount() - time_switch_state >= 2700 + sub_time)
		{
			SetState(BOOMERANG_ENEMY_STATE_MOVE_BACKWARD);
		}

		if (GetTickCount() - time_switch_state >= 4000 + sub_time)
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
		vx = 0.03f;
		vy = 0;
		break;
	case  BOOMERANG_ENEMY_STATE_MOVE_BACKWARD:
		vx = -0.03f;
		vy = 0;
		break;
	case BOOMERANG_ENEMY_STATE_DIE:
		vx = 0;
	}
}


