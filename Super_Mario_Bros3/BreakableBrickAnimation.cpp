#include "BreakableBrickAnimation.h"



CBreakableBrickAnimation::CBreakableBrickAnimation(int ctype)
{
	type = ctype;
	SetState(BREAKABLE_BRICK_ANIMATION_STATE_IDLE);
}




void CBreakableBrickAnimation::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = t = r = b = 0;
}

void CBreakableBrickAnimation::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt);


	x += dx;
	y += dy;



	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBreakableBrick *>(obj))
		{
			CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *>(obj);
			if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_BREAK && breakable_brick->GetIsAllowToShowBreakableBrickAnimation())
			{
				if (breakable_brick->GetIsAllowToPullBreakPiece())
				{
					if (breakable_brick->GetBreakPiecesCount() < 4)
					{
						if (!isUsed)
						{
							if (breakable_brick->GetBreakableBrickAnimationX() != 0 && breakable_brick->GetBreakableBrickAnimationY() != 0)
							{
								SetPosition(breakable_brick->GetBreakableBrickAnimationX(), breakable_brick->GetBreakableBrickAnimationY());
								SetState(BREAKABLE_BRICK_ANIMATION_STATE_MOVE);
								StartTiming();
								isUsed = true;
								breakable_brick->CalcUpBreakPieces();
							}
						}
					}
					else
					{
						breakable_brick->SetIsAllowToPullBreakPiece(false);
					}
				}
			}
			if (state == BREAKABLE_BRICK_ANIMATION_STATE_MOVE)
			{
				if (GetTickCount() - timing_start >= 600)
				{
					vy += BREAKABLE_BRICK_ANIMATION_GRAVITY * dt;
				}
				if (GetTickCount() - timing_start >= 1000)
				{
					isUsed = false;
					SetPosition(12000, 12000);
					timing_start = 0;
					breakable_brick->SetIsAllowToShowBreakableBrickAnimation(false);
				}
			}
		}
	}





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
		vx = vy = 0;
		break;
	case  BREAKABLE_BRICK_ANIMATION_STATE_MOVE:
		switch (type)
		{
		case BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP:
			vx = -0.04f;
			vy = -0.08f;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM:
			vx = -0.04f;
			vy = -0.04f;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP:
			vx = 0.04f;
			vy = -0.08f;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM:
			vx = 0.04f;
			vy = -0.04f;
			break;
		}
		break;
	}
}

