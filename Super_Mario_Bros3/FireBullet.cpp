#include "FireBullet.h"

CFireBullet::CFireBullet() : CGameObject()
{
	SetState(FIRE_BULLET_STATE_HIDDEN);
	isUsed = false;
	Height_Limit = 0;

}





void CFireBullet::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));
		if (dynamic_cast<CFireBullet *>(e->obj))
			continue;

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);



}

void CFireBullet::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt);

	

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	if (isAllowToShowScore)
	{
		if (GetTickCount() - timing_score >= 1000)
			isAllowToShowScore = false;
	}

	// turn on collision when firebullet used 
	if (isUsed)
		CalcPotentialCollisions(coObjects, coEvents);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	
	if (mario->GetIsFiring() && !isUsed)
	{
		if (!mario->GetIsFired())
		{
			y = mario->y;
			if (mario->nx > 0)
			{
				x = mario->x + MARIO_FIRE_BBOX_WIDTH + 1;
				vx = FIRE_BULLET_FLYING_SPEED / 2;

			}
			else
			{
				x = mario->x - MARIO_FIRE_BBOX_WIDTH - 1;
				vx = -FIRE_BULLET_FLYING_SPEED / 2;

			}
			SetState(FIRE_BULLET_STATE_FLYING);
			vy = 0.1f;
			Height_Limit = mario->y;
			mario->SetIsFired(true);
		}
	}
	if (!isUsed)
	{
		SetState(FIRE_BULLET_STATE_HIDDEN);
	}
	if (this->y <= Height_Limit)
		vy = 0.1f;


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;
	}
	else
	{
		//vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 

		x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		y += min_ty * dy + ny * 0.4f;

		if (ny > 0)
			Height_Limit = this->y;

		if ( ny != 0) vy = -vy;

		// Collision logic with the others Koopas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];
			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				if (e->nx != 0 || e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						if (goomba->GetType() != GOOMBA_RED_FLY)
						{
							goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
							isUsed = false;
						}
						else
						{
							if (goomba->GetState() != GOOMBA_STATE_RED_LOSE_WINGS)
							{
								goomba->SetState(GOOMBA_STATE_RED_LOSE_WINGS);
								isUsed = false;
							}

							else
							{
								goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
								isUsed = false;
							}


						}
					}
				}
				vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
				mario->SetShowPointX(this->x);
				mario->SetShowPointY(this->y);
				this->SetIsAllowToShowScore(true);
				for (int i = 0; i < scores_panel.size(); i++)
				{
					CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
					if (!score_panel->GetIsUsed())
					{
						score_panel->SetValue(100);
						score_panel->SetIsUsed(true);
						break;
					}
				
				}
				CGame::GetInstance()->ScoreUp(100);
			}
			else if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Koopas 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (e->nx != 0 || e->ny < 0)
				{
					if (koopas->GetState() != KOOPAS_STATE_DIE && koopas->GetState() != KOOPAS_STATE_SHELL)
					{
						koopas->SetState(KOOPAS_STATE_SHELL);
						isUsed = false;
					}
					else if (koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						koopas->SetState(KOOPAS_STATE_DIE);
						isUsed = false;
					}
				}
				vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
				mario->SetShowPointX(this->x);
				mario->SetShowPointY(this->y);
				this->SetIsAllowToShowScore(true);
				for (int i = 0; i < scores_panel.size(); i++)
				{
					CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
					if (!score_panel->GetIsUsed())
					{
						score_panel->SetValue(100);
						score_panel->SetIsUsed(true);
						break;
					}
				}
				CGame::GetInstance()->ScoreUp(100);
			}
			else if (dynamic_cast<CMario *>(e->obj))
			{
				isUsed = false;
			}
			else if (dynamic_cast<CFlower *>(e->obj))
			{
				CFlower *flower = dynamic_cast<CFlower *>(e->obj);
				flower->SetIsAlive(false);
				isUsed = false;
				vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
				mario->SetShowPointX(this->x);
				mario->SetShowPointY(this->y);
				this->SetIsAllowToShowScore(true);
				for (int i = 0; i < scores_panel.size(); i++)
				{
					CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
					if (!score_panel->GetIsUsed())
					{
						score_panel->SetValue(100);
						score_panel->SetIsUsed(true);
						break;
					}	
				}
				CGame::GetInstance()->ScoreUp(100);
			}

			else if (dynamic_cast<CRectangle *>(e->obj))
			{
				if (e->ny > 0)
				{
					this->vy = -this->vy;
				}
			}
			else if (dynamic_cast<CBrick *>(e->obj))
			{
				if ( e->ny > 0)
				{
					this->vy = -this->vy;
				}
				else if (nx != 0 && ny == 0)
					isUsed = false;
			}
			/*else if (dynamic_cast<CPipe *>(e->obj))
			{
				if (e->ny > 0)
				{
					this->vy = -this->vy;
				}
				else if (nx != 0 && ny == 0)
					isUsed = false;
			}*/

			else // Collisions with other things  
			{
				if (nx != 0 && ny == 0)
					isUsed = false;
			}


		}
	}

	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];
}


void CFireBullet::Render()
{
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	int ani = -1;
	if (isUsed)
	{
		if (mario->nx > 0)
		{
			ani = FIRE_BULLET_ANI_RIGHT;
		}
		else
			ani = FIRE_BULLET_ANI_LEFT;
	}

	else return;

	animation_set->at(ani)->Render(x, y);
	RenderBoundingBox();
}

void CFireBullet::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case FIRE_BULLET_STATE_FLYING:
		isUsed = true;
		break;
	case FIRE_BULLET_STATE_HIDDEN:
		vx = 0;
		vy = 0;
		SetPosition(1000, 1000);
		break;
	}


}

void CFireBullet::GetBoundingBox(float &l, float &t, float &r, float &b)
{
		l = x;
		t = y;
		r = x + FIRE_BULLET_BBOX_WIDTH;
		b = y + FIRE_BULLET_BBOX_HEIGHT;	
}