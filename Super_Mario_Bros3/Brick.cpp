#include "Brick.h"
#include "Game.h"
void CBrick::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}


void CBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == INTRO_SCENE_ID)
	{
		l = x;
		t = y;
		r = x + INTRO_BRICK_BBOX_WIDTH;
		b = y + INTRO_BRICK_BBOX_HEIGHT;
	}
	else
	{
		l = x;
		t = y;
		r = x + BRICK_BBOX_WIDTH;
		b = y + BRICK_BBOX_HEIGHT;
	}
}