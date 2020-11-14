#include "Koopas.h"

CKoopas::CKoopas(int ctype)
{
	type = ctype;
	SetState(KOOPAS_STATE_WALKING);
	nx = -1;
}




void CKoopas::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{

		//CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (dynamic_cast<CRectangle *>(coObjects->at(i)) && vy < 0)
		{
			continue;
		}
		if (dynamic_cast<CFlowerBullet *>(coObjects->at(i)))
		{
			continue;
		}
		if (dynamic_cast<CBreakableBrick *>(coObjects->at(i)))
		{
			CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *>(coObjects->at(i));
			if(breakable_brick->GetState() != BREAKABLE_BRICK_STATE_NORMAL)
			{
				continue;
			}
		}
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}


void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == KOOPAS_STATE_DIE)
		return;
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SPINNING)
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	// Simple fall down
	if (!isHolding)
		vy += KOOPAS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when goomba kicked 
	if (state != KOOPAS_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (GetTickCount() - jumpingStart >= KOOPAS_TIME_JUMPING && type == KOOPAS_XANH_FLY) // KOOPAS XANH FLY JUMP
	{
		vy = -GOOMBA_JUMP_SPEED;
		jumpingStart = GetTickCount();

	}


	//shell is being held
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (isHolding == true)
	{
		if (!mario->GetIsHolding())
		{
			isHolding = false;
			mario->StartKicking();
			mario->SetIsKicking(true);
			nx = mario->nx;
			SetState(KOOPAS_STATE_SPINNING);
		}
	}
	if (isHolding)
	{
		y = mario->y + 8;
		if (mario->nx > 0)
		{
			if (mario->GetLevel() == MARIO_LEVEL_BIG)
			{
				x = mario->x + MARIO_BIG_BBOX_WIDTH;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				x = mario->x + MARIO_SMALL_BBOX_WIDTH;
				y = y - 10;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			{
				x = mario->x + MARIO_TAIL_BBOX_WIDTH;
			}
			else
			{
				x = mario->x + MARIO_FIRE_BBOX_WIDTH;
			}
		}
		else
		{
			if (mario->GetLevel() == MARIO_LEVEL_BIG)
			{
				x = mario->x - MARIO_BIG_BBOX_WIDTH;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
			{
				x = mario->x - MARIO_SMALL_BBOX_WIDTH;
				y = y - 10;
			}
			else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
			{
				x = mario->x - 15;
			}
			else
			{
				x = mario->x - MARIO_FIRE_BBOX_WIDTH;
			}
		}
		mario->SetCanHold(true);
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
		if (!isHolding)
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		/*  y += min_ty * dy + ny * 0.4f;*/


		if (ny != 0) vy = 0;



		// Collision logic with the others Koopas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Koopas 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (e->nx != 0 && this->GetState() == KOOPAS_STATE_SPINNING)
				{
					if (koopas->GetState() != KOOPAS_STATE_SHELL)
					{
						koopas->SetState(KOOPAS_STATE_DIE);

					}
					else if (koopas->GetState() == KOOPAS_STATE_SPINNING || koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						this->vx = -this->vx;
						koopas->vx = -koopas->vx;
					}

				}

			}

			else if (dynamic_cast<CQuestionBrick *>(e->obj))
			{
				if (e->nx != 0 && ny == 0)
				{
					CQuestionBrick *question_brick = dynamic_cast<CQuestionBrick *>(e->obj);
					if (state == KOOPAS_STATE_SPINNING)
					{
						question_brick->SetIsAlive(false);
						vx = -vx;
					}

				}

			}
			else if (dynamic_cast<CBreakableBrick *>(e->obj))
			{
				CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *>(e->obj);
				if (nx != 0 && ny == 0)
				{
					if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_NORMAL)
					{
						if (state == KOOPAS_STATE_SPINNING)
						{
							breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
						}
							    vx = -vx;
					}
				}
			}

			else  // Collisions with other things  
			{
				if (nx != 0 && ny == 0)
				{
					if (dynamic_cast<CGoomba *>(e->obj))
					{
						CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
						if (goomba->GetState() != GOOMBA_STATE_DIE && (this->GetState() == KOOPAS_STATE_SPINNING || isHolding))
						{
							goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
						}
					}
					else if (!dynamic_cast<CMario *>(e->obj) && !dynamic_cast<CFireBullet *>(e->obj))
						vx = -vx;
				}
			}
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CKoopas::Render()
{
	int ani = -1;
	switch (type)
	{
	case KOOPAS_XANH_WALK:
		if (state == KOOPAS_STATE_DIE) {
			if (nx > 0)
				ani = KOOPAS_XANH_ANI_WALKING_RIGHT;
			else
				ani = KOOPAS_XANH_ANI_WALKING_LEFT;
		}
		else if (state == KOOPAS_STATE_SHELL)
		{
			ani = KOOPAS_XANH_MAI_ANI_UP;
		}
		else if (state == KOOPAS_STATE_SPINNING)
		{
			ani = KOOPAS_XANH_MAI_ANI_SPINNING;
		}
		else if (vx > 0) ani = KOOPAS_XANH_ANI_WALKING_RIGHT;
		else  ani = KOOPAS_XANH_ANI_WALKING_LEFT;
		break;

	case KOOPAS_XANH_FLY:
		if (state == KOOPAS_STATE_DIE)
		{
			ani = KOOPAS_XANH_ANI_FLYING_LEFT;
		}
		else if (state == KOOPAS_STATE_SHELL)
		{
			ani = KOOPAS_XANH_MAI_ANI_UP;
		}
		else if (state == KOOPAS_STATE_SPINNING)
		{
			ani = KOOPAS_XANH_MAI_ANI_SPINNING;
		}

		else  ani = KOOPAS_XANH_ANI_FLYING_LEFT;
		break;


	case KOOPAS_RED_WALK:
		if (state == KOOPAS_STATE_DIE) {
			if (nx < 0)
				ani = KOOPAS_RED_ANI_WALKING_LEFT;
			else
				ani = KOOPAS_RED_ANI_WALKING_RIGHT;
		}
		else if (state == KOOPAS_STATE_SHELL)
		{
			ani = KOOPAS_RED_MAI_ANI_UP;
		}
		else if (state == KOOPAS_STATE_SPINNING)
		{
			ani = KOOPAS_RED_MAI_ANI_SPINNING;
		}
		else if (vx < 0) ani = KOOPAS_RED_ANI_WALKING_LEFT;
		else  ani = KOOPAS_RED_ANI_WALKING_RIGHT;
		break;


	case KOOPAS_RED_FLY:
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_RED_MAI_ANI_UP;
		}
		else if (vx > 0) ani = KOOPAS_RED_ANI_FLYING_LEFT;
		else  ani = KOOPAS_RED_ANI_FLYING_LEFT;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vy = -KOOPAS_DIE_DEFLECT_SPEED;
		vx = -vx;
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_SPINNING:
		if (nx > 0)
			vx = KOOPAS_WALKING_SPEED * 5;
		else
			vx = -KOOPAS_WALKING_SPEED * 5;
		break;
	case KOOPAS_STATE_SHELL:
		vx = 0;
		break;

	}

}