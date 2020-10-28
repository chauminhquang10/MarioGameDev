#include "Koopas.h"

CKoopas::CKoopas(int ctype)
{
	type = ctype;
	SetState(KOOPAS_STATE_WALKING);
	nx = -1;
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
	vy += KOOPAS_GRAVITY * dt;

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn off collision when goomba kicked 
	if (state != KOOPAS_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	

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
				if (e->nx != 0 && this->GetState()== KOOPAS_STATE_SPINNING)
				{
					if (koopas->GetState() != KOOPAS_STATE_SHELL )
					{
						koopas->SetState(KOOPAS_STATE_DIE);
						
					}
					else if (koopas->GetState() == KOOPAS_STATE_SPINNING)
					{
						this->vx = - this->vx;
						koopas->vx = -koopas->vx;
					}

				}
				
			}
			else  // Collisions with other things  
			{
				if (e->nx != 0 && ny == 0)
				{	
					if (!dynamic_cast<CMario *>(e->obj))
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
			if(nx>0)
			ani = KOOPAS_XANH_ANI_WALKING_RIGHT;
			else
				ani = KOOPAS_XANH_ANI_WALKING_LEFT;
		}
		else if(state == KOOPAS_STATE_SHELL)
		{
			ani = KOOPAS_XANH_MAI_ANI_UP;
		}
		else if (state == KOOPAS_STATE_SPINNING)
		{
			ani = KOOPAS_XANH_MAI_ANI_NGUA;
		}
		else if (vx > 0) ani = KOOPAS_XANH_ANI_WALKING_RIGHT;
		else  ani = KOOPAS_XANH_ANI_WALKING_LEFT;
		break;

	case KOOPAS_XANH_FLY:
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_XANH_MAI_ANI_NGUA;
		}
		else if (vx > 0) ani = KOOPAS_XANH_ANI_FLYING_LEFT;
		else  ani = KOOPAS_XANH_ANI_WALKING_LEFT;
		break;

	case KOOPAS_RED_WALK:
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_RED_MAI_ANI_UP;
		}
		else if (vx > 0) ani = KOOPAS_RED_ANI_WALKING_LEFT;
		else  ani = KOOPAS_RED_ANI_WALKING_LEFT;
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
			vx = KOOPAS_WALKING_SPEED * 4;
		else
			vx = -KOOPAS_WALKING_SPEED * 4;
		break;
	case KOOPAS_STATE_SHELL:
		vx = 0;
		break;
	}

}