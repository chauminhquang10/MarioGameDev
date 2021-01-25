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
					if (breakable_brick->GetBreakPiecesCount() < BREAKABLE_BRICK_ANIMATION_MAX_PIECES)
					{
						if (!isUsed)
						{
							if (breakable_brick->GetBreakableBrickAnimationX() != BREAKABLE_BRICK_ANIMATION_X && breakable_brick->GetBreakableBrickAnimationY() != BREAKABLE_BRICK_ANIMATION_Y)
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
				if (GetTickCount() - timing_start >= BREAKABLE_BRICK_ANIMATION_TIME_UP)
				{
					vy += BREAKABLE_BRICK_ANIMATION_GRAVITY * dt;
				}
				if (GetTickCount() - timing_start >= BREAKABLE_BRICK_ANIMATION_TIME_DOWN)
				{
					isUsed = false;
					SetPosition(BREAKABLE_BRICK_ANIMATION_ORIGIN_POSITION_X, BREAKABLE_BRICK_ANIMATION_ORIGIN_POSITION_Y);
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
		vx = vy = BREAKABLE_BRICK_ANIMATION_STATE_IDLE_SPEED;
		break;
	case  BREAKABLE_BRICK_ANIMATION_STATE_MOVE:
		switch (type)
		{
		case BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP:
			vx = -BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP_SPEED_VX;
			vy = -BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_TOP_SPEED_VY;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM:
			vx = -BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM_SPEED_VX;
			vy = -BREAKABLE_BRICK_ANIMATION_TYPE_LEFT_BOTTOM_SPEED_VY;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP:
			vx = BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP_SPEED_VX;
			vy = -BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_TOP_SPEED_VY;
			break;
		case BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM:
			vx = BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM_SPEED_VX;
			vy = -BREAKABLE_BRICK_ANIMATION_TYPE_RIGHT_BOTTOM_SPEED_VY;
			break;
		}
		break;
	}
}

