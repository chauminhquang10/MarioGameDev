#include "Goomba.h"
#include "FireBullet.h"
CGoomba::CGoomba(int ctype, int scene_id)
{
	nx = -1;
	if (scene_id == 1)
	{
		isAppear = false;
		SetState(GOOMBA_STATE_IDLE);
	}
	else
	{
		isAppear = true;
		SetState(GOOMBA_STATE_WALKING);
	}
	type = ctype;


}
void CGoomba::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
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



void CGoomba::FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}

		if (dynamic_cast<CMario *>(c->obj))
		{
			CMario* mario = dynamic_cast<CMario *>(c->obj);
			ny = -0.001f;
			if(!mario->GetIsTransforming() && mario->GetUntouchable()==1)
				nx = 0;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}



void CGoomba::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;

	if (state == GOOMBA_STATE_DIE)
	{
		top = y + 7;
		right = x + GOOMBA_NORMAL_BBOX_WIDTH;
		bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
	}
	else if (state == GOOMBA_STATE_WALKING || state == GOOMBA_STATE_IDLE || state==GOOMBA_STATE_FLYING || state== GOOMBA_STATE_GEARING)
	{
		if (type == GOOMBA_NORMAL)
		{
			right = x + GOOMBA_NORMAL_BBOX_WIDTH;
			bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
		}
		else
		{
			right = x + GOOMBA_RED_FLY_BBOX_WIDTH;
			if (state == GOOMBA_STATE_WALKING)
			{
				bottom = y + 19;
			}
			else
			{
				bottom = y + GOOMBA_RED_FLY_BBOX_HEIGHT;
			}
		}
	}
	else if (state == GOOMBA_STATE_RED_LOSE_WINGS)
	{
		top = y + 7;
		right = x + GOOMBA_RED_LOSE_WINGS_BBOX_WIDTH;
		bottom = y + GOOMBA_NORMAL_BBOX_HEIGHT;
	}
	else
	{
		left = top = right = bottom = 0;
	}

}

void CGoomba::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	// 
	// Simple fall down
	if (isAppear)
		vy += GOOMBA_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();



	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetIsTurning())
	{
		if (GetTickCount() - mario->GetTurningStart() <= 200)
		{
			if (mario->nx > 0)
			{
				leftRec = mario->x;
				topRec = mario->y + 5;
				rightRec = leftRec + 24;
				bottomRec = topRec + 8;
			}
			else
			{
				leftRec = mario->x - 25;
				topRec = mario->y + 5;
				rightRec = mario->x;
				bottomRec = topRec + 8;
			}
		}
		else
		{
			if (mario->nx > 0)
			{
				leftRec = mario->x - 25;
				topRec = mario->y + 5;
				rightRec = mario->x;
				bottomRec = topRec + 8;
			}
			else
			{
				leftRec = mario->x;
				topRec = mario->y + 5;
				rightRec = leftRec + 24;
				bottomRec = topRec + 8;
			}
		}
	}
	else
	{
		leftRec = topRec = rightRec = bottomRec = 0;
	}

	if (bottomRec != 0 && topRec != 0 && leftRec != 0 && rightRec != 0)
	{
		if ((this->x >= leftRec && this->x <= rightRec) && (this->y >= topRec && this->y <= bottomRec))
		{
			if (state != GOOMBA_STATE_DIE_BY_KICK)
			{
				SetState(GOOMBA_STATE_DIE_BY_KICK);
				int id = CGame::GetInstance()->GetCurrentScene()->GetId();
				if (id == 3 || id == 4)
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
				}
			}
		}
	}
	//DebugOut(L"gia tri topRec la: %d \n",topRec);
	//DebugOut(L"gia tri bottomRec la: %d \n",bottomRec);
	//DebugOut(L"gia tri y goomba la: %f \n",this->y);

	/*DebugOut(L"gia tri leftRec la: %d \n", leftRec);
	DebugOut(L"gia tri rightRec la: %d \n", rightRec);
	DebugOut(L"gia tri x goomba la: %f \n", this->x);
*/
	if (type == GOOMBA_RED_FLY)
	{
		StartTimeSwitchingState();
		if (state == GOOMBA_STATE_WALKING)
		{
			if (GetTickCount() - time_switch_state >= 1000)
			{
				SetState(GOOMBA_STATE_GEARING);
				y -= 5;
				time_switch_state = 0;
			}
		}
		else if (state == GOOMBA_STATE_GEARING)
		{
			if (GetTickCount() - time_switch_state >= 1000)
			{
				SetState(GOOMBA_STATE_FLYING);
				time_switch_state = 0;
			}
		}
		else if (state == GOOMBA_STATE_FLYING)
		{
			if (GetTickCount() - time_switch_state >= 1000)
			{
				SetState(GOOMBA_STATE_WALKING);
				time_switch_state = 0;
				control_flying = false;
			}
		}
	}

	if (state == GOOMBA_STATE_GEARING)
	{
		StartJumping();
		if (GetTickCount() - jumpingStart >= 400)
		{
			jumpingStart = 0;
			if (control_jump_time == 0)
				control_jump_time++;
		}
		if (control_jump_time == 1)
		{
			vy = -0.2f;
			control_jump_time = 0;
		}
	}

	if (state == GOOMBA_STATE_FLYING)
	{
		if (!control_flying)
		{
			vy = -0.4f;
			control_flying = true;
		}
	}

	// turn off collision when goomba kicked 
	if (state != GOOMBA_STATE_DIE_BY_KICK && state != GOOMBA_STATE_DISAPPEAR)
		CalcPotentialCollisions(coObjects, coEvents);



	CMario* player1 = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer1();
	if (state != GOOMBA_STATE_DIE && state != GOOMBA_STATE_DISAPPEAR)
	{
		if (player1->GetLevel() == MARIO_LEVEL_TAIL)
		{
			if (runningStart == 0)
				StartRunning();
			if (GetTickCount() - runningStart >= 200)
			{
				SetState(GOOMBA_STATE_WALKING);
			}
		}
	}
	if (state == GOOMBA_STATE_DIE)
	{
		if (dyingStart == 0)
			StartDying();
		if (GetTickCount() - dyingStart >= 500)
		{
			SetState(GOOMBA_STATE_DISAPPEAR);
			dyingStart = 0;
		}
	}


	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBackGroundStage *>(obj))
		{
			CBackGroundStage *background_stage = dynamic_cast<CBackGroundStage *>(obj);
			if (background_stage->GetType() == BACKGROUND_STAGE_TYPE_FINAL && background_stage->GetIsAppear())
			{
				isAppear = true;

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
		if (state != GOOMBA_STATE_DIE)
		{
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
			y += min_ty * dy + ny * 0.4f;
		}


		if (ny != 0) vy = 0;

		if (ny < 0)
		{
			if (type == GOOMBA_RED_FLY)
			{
				if (state == GOOMBA_STATE_FLYING && (GetTickCount() - time_switch_state >= 500))
				{
					SetState(GOOMBA_STATE_WALKING);
					time_switch_state = 0;
					control_flying = false;
				}
			}
		}


		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
				if (e->nx != 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->vx = -goomba->vx;
						this->vx = -this->vx;
						this->nx = -this->nx;
					}

				}
			}
			else // Collisions with other things  
			{
				if (e->nx != 0 && ny == 0 && !dynamic_cast<CKoopas *>(e->obj))
				{
					vx = -vx;
					this->nx = -this->nx;
				}

			}
		}
	}

	if (vx < 0 && x < 0)
	{
		x = 0;
		vx = -vx;
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CGoomba::Render()
{
	int ani = -1;
	if (isAppear)
	{
		switch (type)
		{
		case GOOMBA_NORMAL:
			ani = GOOMBA_NORMAL_ANI_WALKING;
			if (state == GOOMBA_STATE_DISAPPEAR)
				return;
			else if (state == GOOMBA_STATE_DIE) {
				ani = GOOMBA_NORMAL_ANI_DIE;
			}
			else if (state == GOOMBA_STATE_DIE_BY_KICK)
			{
				ani = GOOMBA_NORMAL_ANI_DIE_BY_KICK;
			}
			else if (state == GOOMBA_STATE_IDLE)
			{
				ani = GOOMBA_NORMAL_ANI_IDLE;
			}
			break;
		case GOOMBA_RED_FLY:
			if (state == GOOMBA_STATE_DISAPPEAR)
				return;
			else if (state == GOOMBA_STATE_WALKING)
			{
				ani = GOOMBA_RED_FLY_ANI_WALKING;
			}
			else if (state == GOOMBA_STATE_GEARING)
			{
				ani = GOOMBA_RED_ANI_GEARING;
			}
			else if (state == GOOMBA_STATE_FLYING)
			{
				ani = GOOMBA_RED_ANI_FLYING;
			}
			else if (state == GOOMBA_STATE_RED_LOSE_WINGS)
			{
				ani = GOOMBA_RED_FLY_ANI_LOSE_WINGS;
			}
			else if (state == GOOMBA_STATE_DIE_BY_KICK)
			{
				ani = GOOMBA_RED_FLY_ANI_DIE_BY_KICK;
			}
			else if (state == GOOMBA_STATE_DIE)
			{
				ani = GOOMBA_RED_FLY_ANI_DIE;
			}
			break;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CGoomba::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case GOOMBA_STATE_IDLE:
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_DIE:
		vx = 0;
		vy = 0;
		break;
	case GOOMBA_STATE_DIE_BY_KICK:
		vy = -GOOMBA_DIE_DEFLECT_SPEED;
		vx = -vx;
		break;
	case GOOMBA_STATE_RED_LOSE_WINGS:
		vy = 0;
		break;
	case GOOMBA_STATE_WALKING:
		vx = GOOMBA_WALKING_SPEED*nx;
		break;
	}
}