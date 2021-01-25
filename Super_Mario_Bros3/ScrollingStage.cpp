#include "ScrollingStage.h"

CScrollingStage::CScrollingStage()
{
	SetState(SCROLLING_STAGE_STATE_IDLE);
}


void CScrollingStage::GetBoundingBox(float &l, float &t, float &r, float &b)
{
		l = t = r = b = 0;	
}

void CScrollingStage::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (time_showing == 0)
		time_showing = GetTickCount();

	if (GetTickCount() - time_showing >= SCROLLING_STAGE_STATE_UP_TIME_SHOWING)
		SetState(SCROLLING_STAGE_STATE_UP);
	

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

void CScrollingStage::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CScrollingStage::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case SCROLLING_STAGE_STATE_IDLE:
		vx = vy = SCROLLING_STAGE_STATE_IDLE_SPEED;
		break;
	case SCROLLING_STAGE_STATE_UP:
		vy = -SCROLLING_STAGE_STATE_UP_SPEED;
		break;
	}
}



