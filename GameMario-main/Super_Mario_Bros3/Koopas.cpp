#include "Koopas.h"

CKoopas::CKoopas(int ctype)
{
	type = ctype;
	SetState(KOOPAS_STATE_WALKING);
}

void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;

	if (state == KOOPAS_STATE_DIE)
		bottom = y + KOOPAS_BBOX_HEIGHT_DIE;
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	//CGameObject::Update(dt, coObjects);

	////
	//// TO-DO: make sure Koopas can interact with the world and to each of them too!
	//// 

	////x += dx;
	////y += dy;

	//if (vx < 0 && x < 0) {
	//	x = 0; vx = -vx;
	//}

	//if (vx > 0 && x > 290) {
	//	x = 290; vx = -vx;
	//}
}

void CKoopas::Render()
{
	int ani = -1;
	switch (type)
	{

	case KOOPAS_XANH_WALK:
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_XANH_MAI_ANI_NGUA;
		}
		else if (vx > 0) ani = KOOPAS_XANH_ANI_WALKING_RIGHT;
		else if (vx <= 0) ani = KOOPAS_XANH_ANI_WALKING_LEFT;
		break;

	case KOOPAS_XANH_FLY:
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_XANH_MAI_ANI_NGUA;
		}
		else if (vx > 0) ani = KOOPAS_XANH_ANI_FLYING_LEFT;
		else if (vx <= 0) ani = KOOPAS_XANH_ANI_WALKING_LEFT;
		break;

	case KOOPAS_RED_WALK:
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_RED_MAI_ANI_UP;
		}
		else if (vx > 0) ani = KOOPAS_RED_ANI_WALKING_LEFT;
		else if (vx <= 0) ani = KOOPAS_RED_ANI_WALKING_LEFT;
		break;

	case KOOPAS_RED_FLY:
		if (state == KOOPAS_STATE_DIE) {
			ani = KOOPAS_RED_MAI_ANI_UP;
		}
		else if (vx > 0) ani = KOOPAS_RED_ANI_FLYING_LEFT;
		else if (vx <= 0) ani = KOOPAS_RED_ANI_FLYING_LEFT;
		break;
	}

	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		y += KOOPAS_BBOX_HEIGHT - KOOPAS_BBOX_HEIGHT_DIE + 1;
		vx = 0;
		vy = 0;
		break;
	case KOOPAS_STATE_WALKING:
		vx = KOOPAS_WALKING_SPEED;
		break;
	}

}