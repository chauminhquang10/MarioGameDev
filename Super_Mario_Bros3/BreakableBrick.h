#pragma once
#include "GameObject.h"
#include "algorithm"
#include "Mario.h"
#include "PlayScence.h"
#include "Coin.h"
#include"Define.h"



class CBreakableBrick : public CGameObject
{
	DWORD reviveTime=0;
	bool isRevive = false;
	bool isAllowToPullBreakPieces = false;
	int breakPiecesCount = 0;

	float breakable_brick_animation_X = 0;
	float breakable_brick_animation_Y = 0;

	bool isAllowToShowBreakableBrickAnimation = false;

	bool isUp = false;
	int time_Y_Up = 0;
	bool isAllowQuestionBrickSlide = false;
public:
	virtual void GetBoundingBox(float &left, float &top, float &right, float &bottom);
	virtual void Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects);
	virtual void Render();
	void CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents);
	CBreakableBrick();
	void StartRevive()
	{
		reviveTime = GetTickCount();
	}
	virtual void SetState(int state);
	bool GetIsRevive()
	{
		return isRevive;
	}
	void SetIsRevive(bool IsReviveBool)
	{
		isRevive = IsReviveBool;
	}
	bool GetIsAllowToPullBreakPiece()
	{
		return isAllowToPullBreakPieces;
	}
	void SetIsAllowToPullBreakPiece(bool isAllowToPullBreakPieceBool)
	{
		isAllowToPullBreakPieces = isAllowToPullBreakPieceBool;
	}
	void CalcUpBreakPieces()
	{
		this->breakPiecesCount++;
	}
	int GetBreakPiecesCount()
	{
		return breakPiecesCount;
	}
	float GetBreakableBrickAnimationX()
	{
		return breakable_brick_animation_X;
	}
	float GetBreakableBrickAnimationY()
	{
		return breakable_brick_animation_Y;
	}
	void SetBreakableBrickAnimationX(float position_X_Float)
	{
		this->breakable_brick_animation_X = position_X_Float;
	}
	void SetBreakableBrickAnimationY(float position_Y_Float)
	{
		this->breakable_brick_animation_Y = position_Y_Float;
	}
	bool GetIsAllowToShowBreakableBrickAnimation()
	{
		return isAllowToShowBreakableBrickAnimation;
	}
	void SetIsAllowToShowBreakableBrickAnimation(bool isAllowToShowBreakableBrickAnimationBool)
	{
		isAllowToShowBreakableBrickAnimation = isAllowToShowBreakableBrickAnimationBool;
	}
	bool GetIsUp()
	{
		return isUp;
	}
	void SetIsUp(bool isUpBool)
	{
		isUp = isUpBool;
	}
	bool GetIsAllowQuestionBrickSlide()
	{
		return isAllowQuestionBrickSlide;
	}
	void SetIsAllowQuestionBrickSlide(bool isAllowQuestionBrickSlideBool)
	{
		isAllowQuestionBrickSlide = isAllowQuestionBrickSlideBool;
	}
};