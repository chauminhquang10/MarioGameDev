#include "Pipe.h"

CPipe::CPipe(int ctype )
{
	type = ctype;
	
}


void CPipe::Render()
{
	animation_set->at(0)->Render(x, y);

	//RenderBoundingBox();
}

void CPipe::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	if (type == PIPE_TYPE_NORMAL)
	{
		l = x;
		t = y;
		r = x + PIPE_BBOX_WIDTH_NORMAL;
		b = y + PIPE_BBOX_HEIGHT;
	}
	else
	{
		l = x+1;
		t = y;
		r = x + PIPE_BBOX_WIDTH;
		b = y + PIPE_BBOX_HEIGHT;
	}
}