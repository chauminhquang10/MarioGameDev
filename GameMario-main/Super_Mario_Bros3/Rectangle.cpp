#include "Rectangle.h"

void CRectangle::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CRectangle::GetBoundingBox(float &l, float &t, float &r, float &b)
{



	l = x;
	t = y;
	r = x + RECTANGLE_BBOX_WIDTH;
	b = y;


}
