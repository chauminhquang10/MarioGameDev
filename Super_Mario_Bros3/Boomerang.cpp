#include"Boomerang.h"

CBoomerang::CBoomerang(int idInt) : CGameObject()
{
	this->id = idInt;
}





void CBoomerang::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (!dynamic_cast<CMario *>(e->obj) && !dynamic_cast<CBoomerangEnemy *>(e->obj))
			continue;

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);



}

void CBoomerang::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt);


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	// turn on collision when firebullet used 


	if(isAllowToColliWithBoomerangEnemy)
	CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBoomerangEnemy *>(obj))
		{
			CBoomerangEnemy *boomerang_enemy = dynamic_cast<CBoomerangEnemy *>(obj);
			if (this->isAllowToThrowBoomerang && isAllowToSetPosition)
			{
				SetPosition(boomerang_enemy->x-8, boomerang_enemy->y-5);
				isAllowToSetPosition = false;
			}
		}
	}

	

	if (isInState_1)
	{
		if (GetTickCount() - time_switch_state >= 500)
		{
			isInState_2 = true;
			isInState_1 = false;
		}
		else
		{
			if (GetTickCount() - time_switch_state >= 50)
			{
				vx = 0.1f;
				vy += -0.00006f *dt;
			}
			else
			{
				vy = vx = 0;
			}
		}
		if (GetTickCount() - time_switch_state >= 250)
		{
			isAllowToColliWithBoomerangEnemy = true;
		}
	}
	else if (isInState_2)
	{
		if (GetTickCount() - time_switch_state >= 1200)
		{
			isInState_2 = false;
			isInState_3 = true;
		}
		else
		{
			vx = 0.1f;
			vy += 0.00015f *dt;
		}
	}
	else if (isInState_3)
	{
		if (GetTickCount() - time_switch_state >= 1600)
		{
			isInState_3 = false;
			isInState_4 = true;
		}
		else
		{
			vx = -0.1f;
			vy += 0.00006f *dt;
		}
	}
	else if (isInState_4)
	{
		if (GetTickCount() - time_switch_state >= 4000)
		{
			isInState_4 = false;
			time_switch_state = 0;
			SetPosition(33000, 33000);
			isAllowToSetPosition = true;
			isAllowToThrowBoomerang = false;
			isAllowToColliWithBoomerangEnemy = false;
		}
		else
		{
			vx = -0.1f;
			vy = 0;
		}
	}


	//DebugOut(L"gia tri thoi gian la: %d \n",GetTickCount()-time_switch_state);
	//DebugOut(L"gia tri state 3 la %d \n",isInState_3);

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


		// Collision logic with the others Koopas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CBoomerangEnemy *>(e->obj))
			{
				CBoomerangEnemy *boomerang_enemy = dynamic_cast<CBoomerangEnemy *>(e->obj);
				this->isAllowToThrowBoomerang = false;
				SetPosition(33000, 33000);
				isAllowToSetPosition = true;
				isAllowToColliWithBoomerangEnemy = false;
			}
		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CBoomerang::Render()
{
	//CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int ani = -1;


	if (nx > 0)
	{
		ani = BOOMERANG_ANI_FLY_RIGHT;
	}
	else
	{
		ani = BOOMERANG_ANI_FLY_LEFT;
	}


	animation_set->at(ani)->Render(x, y);
	//RenderBoundingBox();
}

void CBoomerang::SetState(int state)
{
	CGameObject::SetState(state);


}

void CBoomerang::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + BOOMERANG_BBOX_WIDTH;
	b = y + BOOMERANG_BBOX_HEIGHT;
}