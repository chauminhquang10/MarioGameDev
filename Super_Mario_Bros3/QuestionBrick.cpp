#include "QuestionBrick.h"

CQuestionBrick::CQuestionBrick(int ctype)
{
	type = ctype;
	
}

void CQuestionBrick::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
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
void CQuestionBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + QUESTION_BRICK_BBOX_WIDTH;
	b = y + QUESTION_BRICK_BBOX_HEIGHT;
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
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
		//x += min_tx * dx + nx * 0.5f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.5f;

		if (nx != 0) vx = 0;
		if (ny != 0) vy = 0;

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			

		}




	}




	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CQuestionBrick::Render()
{
	int ani = -1;

	if (isAlive)
	{
		ani = QUESTION_BRICK_ANI_ALIVE;
	}
	else
		ani = QUESTION_BRICK_ANI_DEAD;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CQuestionBrick::SetState(int state)
{
	/*CGameObject::SetState(state);*/
}


