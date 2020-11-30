#include "Star.h"

CStar::CStar()
{
	SetState(STAR_STATE_IDLE);
}

void CStar::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
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
void CStar::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (isAppear)
	{

		l = x;
		t = y;
		r = x + STAR_BBOX_WIDTH;
		b = y + STAR_BBOX_HEIGHT;

	}
	else
	{
		l = t = r = b = 0;
	}
}

void CStar::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	if (isAppear)
		vy += MUSHROOM_GRAVITY * dt;

	
	CalcPotentialCollisions(coObjects, coEvents);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBackGroundStage *>(obj))
		{
			CBackGroundStage *background_stage = dynamic_cast<CBackGroundStage *>(obj);
			if (background_stage->GetType() == BACKGROUND_STAGE_TYPE_FINAL && background_stage->GetIsAppear())
			{
				isAppear = true;
				/*DebugOut(L"[INFO] Hien hinh goomba \n");*/

			}
		}
	}


	if (this->x >= 300)
		SetPosition(1000, 1000);


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
		y += min_ty * dy +  ny * 0.4f;

		//if (nx != 0) vx = 0;
		if (ny != 0)
		{
			vx = 0.2f;
			vy = -0.4f;
		}

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}
	}


	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CStar::Render()
{

	if (isAppear)
	{
		animation_set->at(0)->Render(x, y);
	}
	else return;

	//RenderBoundingBox();
}

void CStar::SetState(int state)
{
	CGameObject::SetState(state);
	//CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case STAR_STATE_IDLE:
		vx = vy = 0;
		break;
	}
}



