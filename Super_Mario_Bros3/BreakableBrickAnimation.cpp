#include "BreakableBrickAnimation.h"



CBreakableBrickAnimation::CBreakableBrickAnimation(int ctype)
{
	type = ctype;
	SetState(BREAKABLE_BRICK_ANIMATION_STATE_IDLE);
	SetPosition(12000, 12000);
}




void CBreakableBrickAnimation::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = t = r = b = 0;
}

void CBreakableBrickAnimation::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);


	if(isUsed && state==BREAKABLE_BRICK_ANIMATION_STATE_IDLE)
	vy += BREAKABLE_BRICK_ANIMATION_GRAVITY * dt;


	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();


	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBreakableBrick *>(obj))
		{
			CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *>(obj);
			if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_BREAK && mario->GetIsAllowToShowBreakableBrickAnimation())
			{
				if (!isUsed && mario->GetBreakableBrickAnimationCount()<=4)
				{
					if (mario->GetBreakableBrickAnimationX() != 0 && mario->GetBreakableBrickAnimationY() != 0)
					{
						SetPosition(mario->GetBreakableBrickAnimationX(), mario->GetBreakableBrickAnimationY());
						SetState(BREAKABLE_BRICK_ANIMATION_STATE_MOVE);
						StartTiming();
						isUsed = true;
						mario->SetBreakableBrickAnimationCountUp();
					}
				}
			}
		}
	}

	if (state == BREAKABLE_BRICK_ANIMATION_STATE_MOVE)
	{
		if (GetTickCount() - timing_start >= 700)
		{
			SetState(BREAKABLE_BRICK_ANIMATION_STATE_IDLE);
		}
		if (GetTickCount() - timing_start >= 1200)
		{
			isUsed = false;
			SetPosition(12000, 12000);
			timing_start = 0;
			mario->SetIsAllowToShowBreakableBrickAnimation(false);
			if(mario->GetBreakableBrickAnimationCount()>4)
			mario->ResetBreakableBrickAnimationCount();
		}
	}

	x += dx;
	y += dy;

}

void CBreakableBrickAnimation::Render()
{
	if (isUsed)
	{
		animation_set->at(0)->Render(x, y);
	}
	else return;


	//RenderBoundingBox();
}

void CBreakableBrickAnimation::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  BREAKABLE_BRICK_ANIMATION_STATE_IDLE:
		vx = 0;
		break;
	case  BREAKABLE_BRICK_ANIMATION_STATE_MOVE:
		switch (type)
		{
		case BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP:
		case BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM:
			vx = -0.1f;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP:
		case BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM:
			vx = 0.1f;
			break;
		}
		vy = -0.1f;
		break;
	}
}

