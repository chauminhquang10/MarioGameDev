#include "Portal.h"


CPortal::CPortal(int portal_id, float arrive_position_x, float arrive_position_y)
{
	this->portal_id = portal_id;
	this->arrive_position_x = arrive_position_x;
	this->arrive_position_y = arrive_position_y;

}

void CPortal::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	float leftRecMario, rightRecMario, topRecMario, bottomRecMario;
	float leftRecPortal, rightRecPortal, topRecPortal, bottomRecPortal;
	mario->GetBoundingBox(leftRecMario,topRecMario,rightRecMario,bottomRecMario);
	this->GetBoundingBox(leftRecPortal,topRecPortal,rightRecPortal,bottomRecPortal);
	if (((leftRecMario <= rightRecPortal && leftRecMario >= leftRecPortal) || (rightRecMario <= rightRecPortal && rightRecMario >= leftRecPortal) || ((leftRecMario <= leftRecPortal) && (rightRecMario >= rightRecPortal))) && ((topRecMario <= bottomRecPortal && topRecMario >= topRecPortal) || (bottomRecMario <= bottomRecPortal && bottomRecMario >= topRecPortal)))
	{
		if (mario->GetState() == MARIO_STATE_PIPE_DOWNING || mario->GetState() == MARIO_STATE_PIPE_UPPING)
		{
			mario->SetPosition(this->arrive_position_x, arrive_position_y);
		}
	}
}


void CPortal::Render()
{
	//RenderBoundingBox();
}

void CPortal::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + PORTAL_WIDTH;
	b = y + PORTAL_HEIGHT;
}