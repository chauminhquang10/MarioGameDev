#include "Rectangle.h"

void CRectangle::Render()
{
	//animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CRectangle::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CKoopas *>(obj))
		{
			CKoopas* koopas = dynamic_cast<CKoopas *>(obj);
			if (koopas->GetIsAllowToSubRecWidth())
			{
				isAllowToSubBBoxWidth = true;
			}
			else
			{
				isAllowToSubBBoxWidth = false;
			}
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
		//x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.4f;


		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
		}




	}




	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}


void CRectangle::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (!isAllowToSubBBoxWidth)
	{
		l = x;
		t = y;
		r = x + RECTANGLE_BBOX_WIDTH;
		b = y;
	}
	else
	{
		l = x;
		t = y;
		r = x;
		b = y;
	}
}
