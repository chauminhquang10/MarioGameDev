#include "NoCollisionObjects.h"




void CNoCollisionObjects::Render()
{
	animation_set->at(0)->Render(x, y);
	//RenderBoundingBox();
}

void CNoCollisionObjects::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	/*l = x;
	t = y;
	r = x + NoCollisionObjects_BBOX_WIDTH;
	b = y + NoCollisionObjects_BBOX_HEIGHT;*/

}