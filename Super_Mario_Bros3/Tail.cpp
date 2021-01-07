//#include "Tail.h"
//
//CTail::CTail() : CGameObject()
//{
//	SetState(TAIL_STATE_IDLE);
//
//}
//
//
//
//
//
//void CTail::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
//{
//
//	for (UINT i = 0; i < coObjects->size(); i++)
//	{
//		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
//
//		if (e->t > 0 && e->t <= 1.0f)
//			coEvents.push_back(e);
//		else
//			delete e;
//	}
//
//	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
//
//
//
//}
//
//void CTail::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
//{
//
//	CGameObject::Update(dt);
//
//
//
//	vector<LPCOLLISIONEVENT> coEvents;
//	vector<LPCOLLISIONEVENT> coEventsResult;
//
//	coEvents.clear();
//
//	// turn on collision when firebullet used 
//	
//	CalcPotentialCollisions(coObjects, coEvents);
//
//	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
//
//	if (mario->GetIsFiring())
//	{
//		if (!mario->GetIsFired())
//		{
//			y = mario->y;
//			if (mario->nx > 0)
//			{
//				x = mario->x + MARIO_FIRE_BBOX_WIDTH + 1;
//				vx = FIRE_BULLET_FLYING_SPEED / 2;
//
//			}
//			else
//			{
//				x = mario->x - MARIO_FIRE_BBOX_WIDTH - 1;
//				vx = -FIRE_BULLET_FLYING_SPEED / 2;
//
//			}
//			SetState(FIRE_BULLET_STATE_FLYING);
//			vy = 0.1f;
//			
//			mario->SetIsFired(true);
//		}
//	}
//	if (!isUsed)
//	{
//		SetState(FIRE_BULLET_STATE_HIDDEN);
//	}
//	
//
//	// No collision occured, proceed normally
//	if (coEvents.size() == 0)
//	{
//		x += dx;
//		y += dy;
//	}
//	else
//	{
//		//vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
//		float min_tx, min_ty, nx = 0, ny;
//		float rdx = 0;
//		float rdy = 0;
//
//		// TODO: This is a very ugly designed function!!!!
//		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);
//
//		// block 
//
//		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
//		y += min_ty * dy + ny * 0.4f;
//
//	
//
//		if (ny != 0) vy = -vy;
//
//		// Collision logic with the others Koopas
//		for (UINT i = 0; i < coEventsResult.size(); i++)
//		{
//			LPCOLLISIONEVENT e = coEventsResult[i];
//			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
//			{
//				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
//
//				if (e->nx != 0 || e->ny < 0)
//				{
//					if (goomba->GetState() != GOOMBA_STATE_DIE)
//					{
//						if (goomba->GetType() != GOOMBA_RED_FLY)
//						{
//							goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
//							isUsed = false;
//						}
//						else
//						{
//							if (goomba->GetState() != GOOMBA_STATE_RED_LOSE_WINGS)
//							{
//								goomba->SetState(GOOMBA_STATE_RED_LOSE_WINGS);
//								isUsed = false;
//							}
//
//							else
//							{
//								goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
//								isUsed = false;
//							}
//
//
//						}
//					}
//				}
//				vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
//				mario->SetShowPointX(this->x);
//				mario->SetShowPointY(this->y);
//				this->SetIsAllowToShowScore(true);
//				for (int i = 0; i < scores_panel.size(); i++)
//				{
//					CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
//					if (!score_panel->GetIsUsed())
//					{
//						score_panel->SetValue(100);
//						score_panel->SetIsUsed(true);
//						break;
//					}
//
//				}
//				CGame::GetInstance()->ScoreUp(100);
//			}
//			else if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Koopas 
//			{
//				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
//				if (e->nx != 0 || e->ny < 0)
//				{
//					if (koopas->GetState() != KOOPAS_STATE_DIE && koopas->GetState() != KOOPAS_STATE_SHELL)
//					{
//						koopas->SetState(KOOPAS_STATE_SHELL);
//						isUsed = false;
//					}
//					else if (koopas->GetState() == KOOPAS_STATE_SHELL)
//					{
//						koopas->SetState(KOOPAS_STATE_DIE);
//						isUsed = false;
//					}
//				}
//				vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
//				mario->SetShowPointX(this->x);
//				mario->SetShowPointY(this->y);
//				this->SetIsAllowToShowScore(true);
//				for (int i = 0; i < scores_panel.size(); i++)
//				{
//					CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
//					if (!score_panel->GetIsUsed())
//					{
//						score_panel->SetValue(100);
//						score_panel->SetIsUsed(true);
//						break;
//					}
//				}
//				CGame::GetInstance()->ScoreUp(100);
//			}
//			else if (dynamic_cast<CMario *>(e->obj))
//			{
//				isUsed = false;
//			}
//			else if (dynamic_cast<CFlower *>(e->obj))
//			{
//				CFlower *flower = dynamic_cast<CFlower *>(e->obj);
//				flower->SetIsAlive(false);
//				isUsed = false;
//				vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
//				mario->SetShowPointX(this->x);
//				mario->SetShowPointY(this->y);
//				this->SetIsAllowToShowScore(true);
//				for (int i = 0; i < scores_panel.size(); i++)
//				{
//					CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
//					if (!score_panel->GetIsUsed())
//					{
//						score_panel->SetValue(100);
//						score_panel->SetIsUsed(true);
//						break;
//					}
//				}
//				CGame::GetInstance()->ScoreUp(100);
//			}
//
//			else if (dynamic_cast<CRectangle *>(e->obj))
//			{
//				if (e->ny > 0)
//				{
//					this->vy = -this->vy;
//				}
//			}
//			else if (dynamic_cast<CBrick *>(e->obj))
//			{
//				if (e->ny > 0)
//				{
//					this->vy = -this->vy;
//				}
//				else if (nx != 0 && ny == 0)
//					isUsed = false;
//			}
//			/*else if (dynamic_cast<CPipe *>(e->obj))
//			{
//				if (e->ny > 0)
//				{
//					this->vy = -this->vy;
//				}
//				else if (nx != 0 && ny == 0)
//					isUsed = false;
//			}*/
//
//			else // Collisions with other things  
//			{
//				if (nx != 0 && ny == 0)
//					isUsed = false;
//			}
//
//
//		}
//	}
//
//	// clean up collision events
//	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
//}
//
//
//void CTail::Render()
//{
//	
//}
//
//void CTail::SetState(int state)
//{
//	CGameObject::SetState(state);
//	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
//	switch (state)
//	{
//	case TAIL_STATE_FLYING:
//		vx = TAIL_FLYING_SPEED;
//		break;
//	case TAIL_STATE_IDLE:
//		vx = 0;
//		vy = 0;
//		SetPosition(4000, 4000);
//		break;
//	}
//
//
//}
//
//void CTail::GetBoundingBox(float &l, float &t, float &r, float &b)
//{
//	l = x;
//	t = y;
//	r = x + TAIL_BBOX_WIDTH;
//	b = y + TAIL_BBOX_HEIGHT;
//}