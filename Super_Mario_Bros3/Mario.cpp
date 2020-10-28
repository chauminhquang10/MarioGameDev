#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "Rectangle.h"
#include "Goomba.h"
#include "Portal.h"
#include "PlayScence.h"
CMario::CMario(float x, float y) : CGameObject()
{
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;

}
void CMario::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{

		CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (dynamic_cast<CRectangle *>(coObjects->at(i)) && player->vy < 0)
		{
			continue;
		}
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}
void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	// Simple fall down
	vy += MARIO_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when die 
	if (state != MARIO_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (GetTickCount() - turning_start > MARIO_TURNING_TIME)
	{
		isTurning = false;
	}


	if (GetTickCount() - kicking_start > MARIO_KICKING_TIME)
	{
		isKicking = false;
	}
	CalcTheMarioTimeUp();

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

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!
		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;

		if (ny != 0) vy = 0;

		if (ny < 0)    // Handle Jumping
			isJumping = false;
		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						goomba->SetState(GOOMBA_STATE_DIE);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
					else if (level == MARIO_LEVEL_TAIL && isTurning)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE_2)
							goomba->SetState(GOOMBA_STATE_DIE_2);
					}
				}
			}
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}
			else if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Koopas 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (e->ny < 0)
				{
					if (koopas->GetState() != KOOPAS_STATE_SHELL)
					{
						koopas->SetState(KOOPAS_STATE_SHELL);
						vy = -MARIO_JUMP_DEFLECT_SPEED;
					}
				}
				else if (nx != 0)
				{

					/*if (untouchable == 0)
					{
						if (koopas->GetState() != KOOPAS_STATE_SHELL)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								level = MARIO_LEVEL_SMALL;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
					else*/ if (koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						StartKicking();
						isKicking = true;
						koopas->nx = nx;
						koopas->SetState(KOOPAS_STATE_SPINNING);
						
					}

				}
			
			}




		}
	}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
		// simple screen edge collision!!!
		if (vx < 0 && x < 0) x = 0;


	}

	void CMario::Render()
	{
		int ani = -1;
		if (state == MARIO_STATE_DIE)
			ani = MARIO_ANI_DIE;
		else if (isJumping == true)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_JUMPING_RIGHT;
				else ani = MARIO_ANI_BIG_JUMPING_LEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
				else ani = MARIO_ANI_SMALL_JUMPING_LEFT;
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_JUMPING_RIGHT;
				else ani = MARIO_ANI_TAIL_JUMPING_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_JUMPING_RIGHT;
				else ani = MARIO_ANI_FIRE_JUMPING_LEFT;
			}
		}
		else if (state == MARIO_STATE_SITDOWN)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
				else ani = MARIO_ANI_BIG_SITDOWN_LEFT;
			}
			if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
				else ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
				else ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
			}
		}
		else if (state == MARIO_STATE_RUNNING_RIGHT)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_RUNNING_RIGHT;

			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_RUNNING_RIGHT;

			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				ani = MARIO_ANI_TAIL_RUNNING_RIGHT;

			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_RUNNING_RIGHT;

			}
		}

		else if (state == MARIO_STATE_RUNNING_LEFT)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_RUNNING_LEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_RUNNING_LEFT;
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				ani = MARIO_ANI_TAIL_RUNNING_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_RUNNING_LEFT;
			}
		}
		/*
		else if (state == MARIO_STATE_FLYING)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_FLYING_RIGHT;
				else ani = MARIO_ANI_BIG_FLYING_LEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_FLYING_RIGHT;
				else ani = MARIO_ANI_SMALL_FLYING_LEFT;
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_FLYING_RIGHT;
				else ani = MARIO_ANI_TAIL_JUMPING_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_FLYING_RIGHT;
				else ani = MARIO_ANI_FIRE_FLYING_LEFT;
			}
		}

		else if (state == MARIO_STATE_BRAKING)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_BRAKING_RIGHT;
				else ani = MARIO_ANI_BIG_BRAKING_LEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_BRAKING_RIGHT;
				else ani = MARIO_ANI_SMALL_BRAKING_LEFT;
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_BRAKING_RIGHT;
				else ani = MARIO_ANI_TAIL_BRAKING_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_BRAKING_RIGHT;
				else ani = MARIO_ANI_FIRE_BRAKING_LEFT;
			}
		}

		else if (state == MARIO_STATE_HOLDING)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_HOLDING_RIGHT;
				else ani = MARIO_ANI_BIG_HOLDING_LEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_HOLDING_RIGHT;
				else ani = MARIO_ANI_SMALL_HOLDING_LEFT;
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_HOLDING_RIGHT;
				else ani = MARIO_ANI_TAIL_HOLDING_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_HOLDING_RIGHT;
				else ani = MARIO_ANI_FIRE_HOLDING_LEFT;
			}
		}
		*/

		else if (isKicking)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_KICKING_RIGHT;
				else ani = MARIO_ANI_BIG_KICKING_LEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_KICKING_RIGHT;
				else ani = MARIO_ANI_SMALL_KICKING_LEFT;
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_KICKING_RIGHT;
				else ani = MARIO_ANI_TAIL_KICKING_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_KICKING_RIGHT;
				else ani = MARIO_ANI_FIRE_KICKING_LEFT;
			}
		}

		else if (isTurning)
		{
			if (nx > 0) ani = MARIO_ANI_TAIL_KICKING_RIGHT;
			else ani = MARIO_ANI_TAIL_KICKING_LEFT;
		}

		else if (state == MARIO_STATE_IDLE)
		{
			if (level == MARIO_LEVEL_BIG)
			{
				if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_ANI_BIG_IDLE_LEFT;
			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
				else ani = MARIO_ANI_SMALL_IDLE_LEFT;
			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_RIGHT;
				else ani = MARIO_ANI_TAIL_IDLE_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
				else ani = MARIO_ANI_FIRE_IDLE_LEFT;
			}
		}

		else if (vx > 0) // walking right
		{
			if (level == MARIO_LEVEL_BIG)
			{

				ani = MARIO_ANI_BIG_WALKING_RIGHT;

			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_WALKING_RIGHT;

			}
			else if (level == MARIO_LEVEL_TAIL)
			{
				ani = MARIO_ANI_TAIL_WALKING_RIGHT;

			}
			else if (level == MARIO_LEVEL_FIRE)
			{
				ani = MARIO_ANI_FIRE_WALKING_RIGHT;

			}

		}

		else if (vx < 0) // walking left
		{
			if (level == MARIO_LEVEL_BIG)
			{
				ani = MARIO_ANI_BIG_WALKING_LEFT;

			}
			else if (level == MARIO_LEVEL_SMALL)
			{
				ani = MARIO_ANI_SMALL_WALKING_LEFT;

			}
			else if (level == MARIO_LEVEL_TAIL)
			{

				ani = MARIO_ANI_TAIL_WALKING_LEFT;
			}
			else if (level == MARIO_LEVEL_FIRE)
			{

				ani = MARIO_ANI_FIRE_WALKING_LEFT;
			}
		}


		int alpha = 255;
		if (untouchable) alpha = 128;

		animation_set->at(ani)->Render(x, y, alpha);

		RenderBoundingBox();
	}

void CMario::SetState(int state)
{
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_WALKING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		if (time_mario > MARIO_MAX_STACK)
		{
			vx = MARIO_RUNNING_SPEED;
		}
		else
		{
			vx += MARIO_ACCELERATION * time_mario;
		}
		nx = 1;
		break;
	case MARIO_STATE_RUNNING_LEFT:
		if (time_mario > MARIO_MAX_STACK)
		{
			vx = -MARIO_RUNNING_SPEED;
		}
		else
		{
			vx -= MARIO_ACCELERATION * time_mario;
		}
		nx = -1;
		break;

		//case MARIO_STATE_BRAKING_RIGHT:
		//	vx -= (MARIO_WALKING_GIATOC * 4) * dt;
		//	if (vx <= 0)
		//		vx = 0;
		//	break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_SITDOWN:
		vx = 0;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_SPEED_DOWN:
		if (vx > 0)
		{
			vx -= MARIO_SPEED_DOWN;
		}
		else if ( vx < 0)
		{
			vx += MARIO_SPEED_DOWN;
		}
		break;
	//case MARIO_STATE_BRAKING_RIGHT:
	//	vx = MARIO_WALKING_SPEED / 2;
	//	break;
	case MARIO_STATE_TURNING_TAIL:
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}
}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{

	left = x;
	top = y;

	if (state == MARIO_STATE_SITDOWN && level != MARIO_LEVEL_SMALL) // Set the bounding box again when mario sits down
	{
		top += 9;

	}

	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;
		
	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TAIL)
	{
		right = x + MARIO_TAIL_BBOX_WIDTH;
		bottom = y + MARIO_TAIL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		right = x + MARIO_FIRE_BBOX_WIDTH;
		bottom = y + MARIO_FIRE_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::SetLevel(int l)
{
	int oldlevel = this->level;
	this->level = l;
	if (level == MARIO_LEVEL_SMALL)
	{
		y += MARIO_DIFFERENCE_HEIGHT;
	}
	else if (oldlevel == MARIO_LEVEL_SMALL)
	{
		y -= MARIO_DIFFERENCE_HEIGHT;
	}
}
