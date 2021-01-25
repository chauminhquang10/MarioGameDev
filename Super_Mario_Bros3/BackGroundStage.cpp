#include "BackGroundStage.h"



CBackGroundStage::CBackGroundStage(int ctype)
{
	type = ctype;
	if (type != BACKGROUND_STAGE_TYPE_BLACK)
		isAppear = false;
	SetState(BACKGROUND_STAGE_STATE_IDLE);
}




void CBackGroundStage::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = t = r = b = 0;
}

void CBackGroundStage::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	StartShowing();

	if (GetTickCount() - time_showing >= BACKGROUND_STAGE_TYPE_COLOR_TIME_SHOWING)
	{
		if (type == BACKGROUND_STAGE_TYPE_COLOR)
		{
			isAppear = true;
			StartDown();
		}
	}

	if (GetTickCount() - time_showing >= BACKGROUND_STAGE_TYPE_FINAL_TIME_SHOWING)
	{
		if (type == BACKGROUND_STAGE_TYPE_FINAL)
		{
			isAppear = true;
		}
	}


	if (type == BACKGROUND_STAGE_TYPE_COLOR && isAppear)
	{
		if (GetTickCount() - time_down >= BACKGROUND_STAGE_TYPE_COLOR_TIME_IDLE)
		{
			SetState(BACKGROUND_STAGE_STATE_IDLE);
		}
		else
		{
			SetState(BACKGROUND_STAGE_STATE_DOWN);
		}
		if (GetTickCount() - time_down >= BACKGROUND_STAGE_STATE_SHAKE_TIME_SHAKE)
		{
			SetState(BACKGROUND_STAGE_STATE_SHAKE);
		}
		if (GetTickCount() - time_down >= BACKGROUND_STAGE_STATE_IDLE_TIME_IDLE)
		{
			SetState(BACKGROUND_STAGE_STATE_IDLE);
		}

	}
	
	if (state == BACKGROUND_STAGE_STATE_SHAKE)
	{
		StartShake();
		if (GetTickCount() - time_shake >= BACKGROUND_STAGE_STATE_SHAKE_TIME_SWITCHING_UP_DOWN)
		{
			this->vy = BACKGROUND_STAGE_STATE_SHAKE_SPEED *shakeDirection;
			shakeDirection = -shakeDirection;
			time_shake = 0;
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
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy +  ny * 0.4f;

		//if (nx != 0) vx = 0;
		//if (ny != 0) vy = 0;

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CBackGroundStage::Render()
{
	int ani = -1;
	if (isAppear)
	{
		switch (type)
		{
		case BACKGROUND_STAGE_TYPE_BLACK:
			ani = BACKGROUND_STAGE_TYPE_BLACK_ANI;
			break;
		case BACKGROUND_STAGE_TYPE_COLOR:
			ani = BACKGROUND_STAGE_TYPE_COLOR_ANI;
			break;
		case BACKGROUND_STAGE_TYPE_FINAL:
			ani = BACKGROUND_STAGE_TYPE_FINAL_ANI;
			break;
		}
	}
	else
		return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CBackGroundStage::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  BACKGROUND_STAGE_STATE_IDLE:
		vx = vy = BACKGROUND_STAGE_STATE_IDLE_SPEED_IDLE;
		break;
	case  BACKGROUND_STAGE_STATE_DOWN:
		vy = BACKGROUND_STAGE_STATE_DOWN_SPEED;
		break;
	}
}

