#include "Coin.h"
#include "Mario.h"
void CCoin::Render()
{
	if (disappear)
		return;
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}




void CCoin::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (disappear)
		return;
	l = x;
	t = y;
	r = x + COIN_BBOX_WIDTH;
	b = y + COIN_BBOX_HEIGHT;
}