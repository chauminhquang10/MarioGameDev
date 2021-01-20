//#include "HitEffect.h"
//
//
//
//CHitEffect::CHitEffect(int ctype)
//{
//	this->type = ctype;
//	isUsed = false;
//	SetState(HIT_EFFECT_STATE_IDLE);
//}
//
//
//
//
//void CHitEffect::GetBoundingBox(float &l, float &t, float &r, float &b)
//{
//	l = t = r = b = 0;
//}
//
//void CHitEffect::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
//{
//	CGameObject::Update(dt);
//
//	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
//
//
//	for (UINT i = 0; i < coObjects->size(); i++)
//	{
//		LPGAMEOBJECT obj = coObjects->at(i);
//		if (this->type == HIT_EFFECT_TURN_TAIL)
//		{
//			if (dynamic_cast<CGoomba *>(obj) || dynamic_cast<CKoopas *>(obj) || dynamic_cast<CFlower *>(obj) || dynamic_cast<CBoomerangEnemy *>(obj) || dynamic_cast<CFireBullet *>(obj))
//			{
//				CGoomba *goomba = dynamic_cast<CGoomba *>(obj);
//				CKoopas *koopas = dynamic_cast<CKoopas *>(obj);
//				CFlower *flower = dynamic_cast<CFlower *>(obj);
//				CFireBullet* fire_bullet = dynamic_cast<CFireBullet *>(obj);
//				CBoomerangEnemy* boomerang_enemy = dynamic_cast<CBoomerangEnemy *>(obj);
//				if (state == HIT_EFFECT_STATE_IDLE && (goomba->GetIsAllowToShowHitEffectTurnTail() || koopas->GetIsAllowToShowHitEffectTurnTail() || flower->GetIsAllowToShowHitEffectTurnTail() || fire_bullet->GetIsAllowToShowHitEffectTurnTail() || boomerang_enemy->GetIsAllowToShowHitEffectTurnTail()) && isUsed)
//				{
//					StartTiming();
//					this->x = mario->GetShowPointX() + 5;
//					this->y = mario->GetShowPointY() - 10;
//					if (GetTickCount() - timing_start >= 600)
//					{
//						isUsed = false;
//						SetPosition(6000, 6000);
//						SetState(HIT_EFFECT_STATE_IDLE);
//						goomba->SetIsAllowToShowScore(false);
//						koopas->SetIsAllowToShowScore(false);
//						flower->SetIsAllowToShowScore(false);
//						boomerang_enemy->SetIsAllowToShowScore(false);
//						timing_start = 0;
//
//					}
//				}
//			}
//		}
//		else
//		{
//			if (dynamic_cast<CGoomba *>(obj) || dynamic_cast<CKoopas *>(obj) || dynamic_cast<CFlower *>(obj) || dynamic_cast<CBoomerangEnemy *>(obj) || dynamic_cast<CFireBullet *>(obj))
//			{
//				CGoomba *goomba = dynamic_cast<CGoomba *>(obj);
//				CKoopas *koopas = dynamic_cast<CKoopas *>(obj);
//				CFlower *flower = dynamic_cast<CFlower *>(obj);
//				CFireBullet* fire_bullet = dynamic_cast<CFireBullet *>(obj);
//				CBoomerangEnemy* boomerang_enemy = dynamic_cast<CBoomerangEnemy *>(obj);
//				if (state == HIT_EFFECT_STATE_IDLE && (goomba->GetIsAllowToShowHitEffectFireBullet() || koopas->GetIsAllowToShowHitEffectFireBullet() || flower->GetIsAllowToShowHitEffectFireBullet() || fire_bullet->GetIsAllowToShowHitEffectFireBullet() || boomerang_enemy->GetIsAllowToShowHitEffectFireBullet()) && isUsed)
//				{
//					StartTiming();
//					this->x = mario->GetShowPointX() + 5;
//					this->y = mario->GetShowPointY() - 10;
//					if (GetTickCount() - timing_start >= 600)
//					{
//						isUsed = false;
//						SetPosition(6000, 6000);
//						SetState(HIT_EFFECT_STATE_IDLE);
//						goomba->SetIsAllowToShowScore(false);
//						koopas->SetIsAllowToShowScore(false);
//						flower->SetIsAllowToShowScore(false);
//						boomerang_enemy->SetIsAllowToShowScore(false);
//						timing_start = 0;
//
//					}
//				}
//
//			}
//		}
//	}
//
//
//
//
//
//	x += dx;
//	y += dy;
//
//
//
//}
//
//void CHitEffect::Render()
//{
//	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
//
//	int ani = -1;
//	if (isUsed)
//	{
//		switch (type)
//		{
//		case HIT_EFFECT_TURN_TAIL:
//			if (isAllowToShowHitEffectTurnTail)
//			{
//				if (mario->GetIsTurning())
//				{
//					if (nx > 0)
//						ani = HIT_EFFECT_TURN_TAIL_ANI_RIGHT;
//					else
//						ani = HIT_EFFECT_TURN_TAIL_ANI_LEFT;
//				}
//			}
//			else return;
//			break;
//		case HIT_EFFECT_FIRE_BULLET:
//			if (isAllowToShowHitEffectFireBullet)
//			{
//				ani = HIT_EFFECT_ANI_FIRE_BULLET;
//			}
//			else return;
//			break;
//		}
//	}
//	else return;
//	animation_set->at(ani)->Render(x, y);
//
//	//RenderBoundingBox();
//}
//
//void CHitEffect::SetState(int state)
//{
//	CGameObject::SetState(state);
//	switch (state)
//	{
//	case HIT_EFFECT_STATE_IDLE:
//		vx = vy = 0;
//		break;
//	}
//}
//
