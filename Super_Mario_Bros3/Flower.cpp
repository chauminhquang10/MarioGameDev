#include "Flower.h"
#include "Pipe.h"
#include"Brick.h"
#include"Mario.h"
#include"PlayScence.h"

CFlower::CFlower(int ctype)
{
	type = ctype;
	vx = 0;
}




void CFlower::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CPipe*>(coObjects->at(i)))
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
void CFlower::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isAlive)
	{
		switch (type)
		{
		case FLOWER_RED:
			l = x;
			t = y;
			r = x + FLOWER_RED_BBOX_WIDTH;
			b = y + FLOWER_RED_BBOX_HEIGHT;
			break;
		case FLOWER_GREEN:
			l = x;
			t = y;
			r = x + FLOWER_GREEN_BBOX_WIDTH;
			b = y + FLOWER_GREEN_BBOX_HEIGHT;
			break;
		case FLOWER_GREEN_CAN_SHOOT:
			l = x;
			t = y;
			r = x + FLOWER_GREEN_CAN_SHOOT_BBOX_WIDTH;
			b = y + FLOWER_GREEN_CAN_SHOOT_BBOX_HEIGHT;
			break;
		}
	}
	else
	{
		l = t = r = b = 0;
	}

}

void CFlower::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();



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

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	//if (abs((this->x - mario->x) <= 30) && (abs(this->y - mario->y) <= 50))
	//{
	//	isAllowFlowerToUpdate = false;
	//}
	//else
	//{
	//	isAllowFlowerToUpdate = true;
	//}


	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id != 1)
	{

		if (mario->GetIsTurning())
		{
			float leftRec = mario->GetLeftRecMarioTail();
			float topRec = mario->GetTopRecMarioTail();
			float rightRec = mario->GetRightRecMarioTail();
			float bottomRec = mario->GetBottomRecMarioTail();



			if (bottomRec != 0 && topRec != 0 && leftRec != 0 && rightRec != 0)
			{
				float leftRecGoomba, rightRecGoomba, topRecGoomba, bottomRecGoomba;
				this->GetBoundingBox(leftRecGoomba, topRecGoomba, rightRecGoomba, bottomRecGoomba);
				if (((leftRec <= rightRecGoomba && leftRec >= leftRecGoomba) || (rightRec <= rightRecGoomba && rightRec >= leftRecGoomba) || ((leftRec <= leftRecGoomba) && (rightRec >= rightRecGoomba))) && ((topRec <= bottomRecGoomba && topRec >= topRecGoomba) || (bottomRec <= bottomRecGoomba && bottomRec >= topRecGoomba)))
				{

					if (isAlive)
					{
						isAlive = false;
						int id = CGame::GetInstance()->GetCurrentScene()->GetId();
						if (id == 3 )
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
									score_panel->SetValue(100);
									score_panel->SetIsUsed(true);
									break;
								}
							}
							CGame::GetInstance()->ScoreUp(100);


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




	switch (type)
	{
	case FLOWER_RED:
		if (isAlive)
		{
			if (isUp)
			{
				if (time_showing == 0)
					StartShowing();
				if (GetTickCount() - time_showing <= TIME_SHOWING_LIMIT + sub_time)
				{
					vy = -0.02f;
					if (this->y <= FLOWER_RED_TOP_LIMIT)
					{
						vy = 0;
					}
					if (GetTickCount() - time_showing >= 2900 +sub_time)
						isFiring = true;
				}
				else
				{
					isUp = false;
					isFiring = false;
					time_showing = 0;
					sub_time = 0;
					isFired = false;
				}
			}
			else
			{
				if (time_showing == 0)
					StartShowing();
				if (GetTickCount() - time_showing <= TIME_SHOWING_LIMIT + sub_time)
				{
					vy = 0.02f;
					if (this->y >= FLOWER_RED_BOT_LIMIT)
					{
						vy = 0;
					}
				}
				else
				{
					if (this->GetIsAllowFlowerToUpdate())
					{
						isUp = true;
						time_showing = 0;
						sub_time = 0;
					}
				}
			}
		}
		break;
	case FLOWER_GREEN:
		if (isAlive)
		{
			if (isUp)
			{
				if (time_showing == 0)
					StartShowing();
				if (GetTickCount() - time_showing <= GREEN_TIME_SHOWING_LIMIT + sub_time)
				{
					vy = -0.02f;
					if (this->y <= FLOWER_GREEN_TOP_LIMIT)
					{
						vy = 0;

					}
				}
				else
				{
					isUp = false;
					time_showing = 0;
					sub_time = 0;
				}

			}
			else
			{
				if (time_showing == 0)
					StartShowing();
				if (GetTickCount() - time_showing <= GREEN_TIME_SHOWING_LIMIT + sub_time)
				{
					vy = 0.02f;
					if (this->y >= FLOWER_GREEN_BOT_LIMIT)
					{
						vy = 0;
					}
				}
				else
				{
					if (this->GetIsAllowFlowerToUpdate())
					{
						isUp = true;
						time_showing = 0;
						sub_time = 0;
					}
				}
			}
		}
		break;
	case FLOWER_GREEN_CAN_SHOOT:
		if (isAlive)
		{
			if (isUp)
			{
				if (time_showing == 0)
					StartShowing();
				if (GetTickCount() - time_showing <= TIME_SHOWING_LIMIT + sub_time)
				{
					vy = -0.02f;
					if (this->y <= FLOWER_GREEN_CAN_SHOOT_TOP_LIMIT)
					{
						vy = 0;
					}
					if (GetTickCount() - time_showing >= 2900 + sub_time)
						isFiring = true;
				}
				else
				{
					isUp = false;
					isFiring = false;
					isFired = false;
					time_showing = 0;
					sub_time = 0;
				}

			}
			else
			{
				if (time_showing == 0)
					StartShowing();
				if (GetTickCount() - time_showing <= TIME_SHOWING_LIMIT + sub_time)
				{
					vy = 0.02f;
					if (this->y >= FLOWER_GREEN_CAN_SHOOT_BOT_LIMIT)
					{
						vy = 0;
					}
				}
				else
				{
					if (this->GetIsAllowFlowerToUpdate())
					{
						isUp = true;
						time_showing = 0;
						sub_time = 0;
					}
				}
			}
		}
		break;
	}



	CalcPotentialCollisions(coObjects, coEvents);

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
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


		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}




	}




	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CFlower::Render()
{
	int ani = -1;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (type)
	{
	case FLOWER_RED:
		if (isAlive)
		{
			if (mario->x <= this->x)
			{
				if (mario->y >= this->y)
				{
					if (vy == 0)
					{
						ani = FLOWER_RED_ANI_LEFT_IDLE;
					}
					else
					{
						ani = FLOWER_RED_ANI_LEFT;
					}
				}
				else
				{
					if (vy == 0)
					{
						ani = FLOWER_RED_ANI_LEFT_IDLE_UP;
					}
					else
					{
						ani = FLOWER_RED_ANI_LEFT_UP;
					}
				}

			}
			else
			{
				if (mario->y >= this->y)
				{
					if (vy == 0)
					{
						ani = FLOWER_RED_ANI_RIGHT_IDLE;
					}
					else
					{
						ani = FLOWER_RED_ANI_RIGHT;
					}

				}
				else
				{
					if (vy == 0)
					{
						ani = FLOWER_RED_ANI_RIGHT_IDLE_UP;
					}
					else
					{
						ani = FLOWER_RED_ANI_RIGHT_UP;
					}
				}
			}
		}
		else return;
		break;
	case FLOWER_GREEN:
		if (isAlive)
		{
			ani = FLOWER_GREEN_ANI;
		}
		else return;
		break;
	case FLOWER_GREEN_CAN_SHOOT:
		if (isAlive)
		{
			if (mario->x <= this->x)
			{
				if (mario->y >= this->y)
				{
					if (vy == 0)
					{
						ani = FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE;
					}
					else
					{
						ani = FLOWER_GREEN_CAN_SHOOT_ANI_LEFT;
					}

				}
				else
				{
					if (vy == 0)
					{
						ani = FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_IDLE_UP;
					}
					else
					{
						ani = FLOWER_GREEN_CAN_SHOOT_ANI_LEFT_UP;
					}
				}

			}
			else
			{
				if (mario->y >= this->y)
				{
					if (vy == 0)
					{
						ani = FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE;
					}
					else
					{
						ani = FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT;
					}

				}
				else
				{
					if (vy == 0)
					{
						ani = FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_IDLE_UP;
					}
					else
					{
						ani = FLOWER_GREEN_CAN_SHOOT_ANI_RIGHT_UP;
					}
				}
			}
		}
		else return;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CFlower::SetState(int state)
{
	/*CGameObject::SetState(state);*/
}


