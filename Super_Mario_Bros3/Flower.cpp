#include "Flower.h"
#include "Pipe.h"
#include"Brick.h"
#include"Mario.h"
#include"PlayScence.h"

CFlower::CFlower()
{
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
	l = x;
	t = y;
	r = x + FLOWER_BBOX_WIDTH;
	b = y + FLOWER_BBOX_HEIGHT;
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

	if (isUp)
	{
		if (time_showing == 0)
			StartShowing();
		if (GetTickCount() - time_showing <= TIME_SHOWING_LIMIT)
		{
			vy = -0.02f;
			if (this->y <= FLOWER_TOP_LIMIT)
				vy = 0;
		}
		else
		{
			isUp = false;
			time_showing = 0;
		}

	}
	else
	{
		if (time_showing == 0)
			StartShowing();
		if (GetTickCount() - time_showing <= TIME_SHOWING_LIMIT)
		{
			vy = 0.02f;
			if (this->y >= FLOWER_BOT_LIMIT)
				vy = 0;
		}
		else
		{
			isUp = true;
			time_showing = 0;
		}
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
		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;


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
	if (mario->x <= this->x)
	{
		if (vy != 0)
		{
			ani = FLOWER_ANI_LEFT;
		}
		else
		{
			ani = FLOWER_ANI_LEFT_IDLE;
		}
	}
	else
	{
		if (vy != 0)
		{
			ani = FLOWER_ANI_RIGHT;
		}
		else
		{
			ani = FLOWER_ANI_RIGHT_IDLE;
		}
	}
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CFlower::SetState(int state)
{
	/*CGameObject::SetState(state);*/
}


