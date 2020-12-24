#include "Koopas.h"

CKoopas::CKoopas(int ctype, int scene_id)
{
	type = ctype;
	nx = -1;
	if (type == KOOPAS_BLACK || scene_id == 1)
	{
		SetState(KOOPAS_STATE_SHELL);
		isAppear = false;
	}
	else
	{
		SetState(KOOPAS_STATE_WALKING);
		if (type == KOOPAS_XANH_FLY)
		{
			isAllowToUpPointPara = true;
		}
		else
		{
			isAllowToUpPointPara = false;
		}
	}
}

void CKoopas::FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy)
{
	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	CMario* mario = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer1();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
		if (dynamic_cast<CMario *>(c->obj))
		{
			ny = -0.01f;
			if(mario->GetIsAllowToThroughMario())
			{
				nx = 0;
			}
		}

	}
	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void CKoopas::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		//CMario* player = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (dynamic_cast<CRectangle *>(coObjects->at(i)) && vy < 0)
		{
			continue;
		}
		if (dynamic_cast<CFlowerBullet *>(coObjects->at(i)) || dynamic_cast<CLeaf *>(coObjects->at(i)))
		{
			continue;
		}

		if (dynamic_cast<CBreakableBrick *>(coObjects->at(i)))
		{
			CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *>(coObjects->at(i));
			if (breakable_brick->GetState() != BREAKABLE_BRICK_STATE_NORMAL)
			{
				continue;
			}
		}
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}


void CKoopas::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{
	if (state == KOOPAS_STATE_DIE || !isAppear)
		return;
	left = x;
	top = y;
	right = x + KOOPAS_BBOX_WIDTH;
	if (state == KOOPAS_STATE_SHELL || state == KOOPAS_STATE_SPINNING)
	{
		bottom = y + KOOPAS_BBOX_HEIGHT_SHELL;
	}
	else
		bottom = y + KOOPAS_BBOX_HEIGHT;
}

void CKoopas::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt, coObjects);

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CBackGroundStage *>(obj))
		{
			CBackGroundStage *background_stage = dynamic_cast<CBackGroundStage *>(obj);
			if (background_stage->GetType() == BACKGROUND_STAGE_TYPE_FINAL && background_stage->GetIsAppear())
			{
				if (type != KOOPAS_TYPE_LINE && type != KOOPAS_TYPE_FASTER)
					isAppear = true;
			}
		}
	}


	

	CMario* player1 = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer1();
	if (player1->GetIsAllowToShowKoopasLine())
	{
		if (type == KOOPAS_TYPE_LINE)
		{
			isAppear = true;
			SetState(KOOPAS_STATE_WALKING_RIGHT);
		}
	}

	if (player1->GetIsAllowToShowKoopasFaster())
	{
		if (type == KOOPAS_TYPE_FASTER)
		{
			isAppear = true;
			SetState(KOOPAS_STATE_WALKING_RIGHT_FASTER);
		}
	}



	// Simple fall down
	if (!isHolding && isAppear)
	{
		vy += KOOPAS_GRAVITY * dt;
	}
	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();



	// turn off collision when goomba kicked 
	if (state != KOOPAS_STATE_DIE)
		CalcPotentialCollisions(coObjects, coEvents);

	if (GetTickCount() - jumpingStart >= KOOPAS_TIME_JUMPING && type == KOOPAS_XANH_FLY) // KOOPAS XANH FLY JUMP
	{
		vy = -KOOPAS_JUMP_SPEED;
		jumpingStart = GetTickCount();
	}

	if (type != KOOPAS_BLACK)
	{
		int id = CGame::GetInstance()->GetCurrentScene()->GetId();
		if (id != 1)
		{
			CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
			if (state == KOOPAS_STATE_SHELL)
			{
				if (reviveStart == 0 || isKickedRevive)
				{
					StartRevive();
					isKickedRevive = false;
				}
			}
			if (state == KOOPAS_STATE_SHELL)
			{
				if (GetTickCount() - reviveStart >= 5000)
				{
					y -= 10;
					x += 5 * mario->nx;
					SetState(KOOPAS_STATE_WALKING);
					if (mario->x >= this->x)
						vx = -vx;
					isHolding = false;
					mario->SetCanHold(false);
					reviveStart = 0;
					reviveRender = false;
					renderRecognization = false;
				}
				else
				{
					if (GetTickCount() - reviveStart >= 3000)
					{
						reviveRender = true;
						shellUpRender = false;
					}
				}
			}

			if (renderRecognization)
			{
				spinningRecognization = true;
			}
			else
			{
				spinningRecognization = false;
			}


			if (state != KOOPAS_STATE_WALKING)
				CanPullBack = false;

			if (isHolding)
			{
				if (!mario->GetIsHolding())
				{
					isHolding = false;
					mario->StartKicking();
					mario->SetIsKicking(true);
					nx = mario->nx;
					SetState(KOOPAS_STATE_SPINNING);
				}
			}
			//shell is being held
			if (isHolding)
			{
				y = mario->y + 8;
				if (mario->nx > 0)
				{
					if (mario->GetLevel() == MARIO_LEVEL_BIG)
					{
						x = mario->x + MARIO_BIG_BBOX_WIDTH;
					}
					else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					{
						x = mario->x + MARIO_SMALL_BBOX_WIDTH;
						y = y - 10;
					}
					else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
					{
						x = mario->x + MARIO_TAIL_BBOX_WIDTH;
					}
					else
					{
						x = mario->x + MARIO_FIRE_BBOX_WIDTH;
					}
				}
				else
				{
					if (mario->GetLevel() == MARIO_LEVEL_BIG)
					{
						x = mario->x - MARIO_BIG_BBOX_WIDTH;
					}
					else if (mario->GetLevel() == MARIO_LEVEL_SMALL)
					{
						x = mario->x - MARIO_SMALL_BBOX_WIDTH;
						y = y - 10;
					}
					else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
					{
						x = mario->x - 15;
					}
					else
					{
						x = mario->x - MARIO_FIRE_BBOX_WIDTH;
					}
				}
				mario->SetCanHold(true);
			}

		}
		else
		{

			CMario* mario = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer1();

			if (!mario->GetIsHolding() && !mario_recog)
			{
				mario = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer2();
			}
			else
			{
				mario_recog = true;
			}


			if (isHolding)
			{
				if (!mario->GetIsHolding())
				{
					isHolding = false;
					mario->StartKicking();
					mario->SetIsKicking(true);
					nx = mario->nx;
					SetState(KOOPAS_STATE_SPINNING);

				}
			}

			if (isHolding)
			{
				y = mario->y + 8;
				if (mario->nx > 0)
				{
					if (mario->GetLevel() == MARIO_LEVEL_BIG)
					{
						x = mario->x + MARIO_BIG_BBOX_WIDTH;
					}
					else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
					{
						x = mario->x + MARIO_TAIL_BBOX_WIDTH;
					}

				}
				else
				{
					if (mario->GetLevel() == MARIO_LEVEL_BIG)
					{
						x = mario->x - MARIO_BIG_BBOX_WIDTH;
					}
					else if (mario->GetLevel() == MARIO_LEVEL_TAIL)
					{
						x = mario->x - 15;
					}
				}
				mario->SetCanHold(true);
			}

		}
	}


	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

		if (CanPullBack && type == KOOPAS_RED_WALK)
		{
			if (y - CheckPosition_Y >= 1.0f)
			{

				y -= 5;
				if (vx < 0)
					x += 12;
				else
					x -= 12;
				vx = -vx;
			}
		}

	}

	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);


		// block 
		if (!isHolding)
			x += min_tx * dx + nx * 0.4f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		if (type != KOOPAS_TYPE_FASTER && type != KOOPAS_TYPE_LINE)
			y += min_ty * dy + ny * 0.4f;

		if (ny != 0)   vy = 0;

		if (ny < 0 && state == KOOPAS_STATE_SHELL)
		{
			vx = 0;
			spinning_shell_intro = false;
			if (jump_count < 2 && type == KOOPAS_BLACK)
			{
				if (jump_count == 0)
				{
					vy = -0.4f;
				}
				else
				{
					vy = -0.15f;
				}
				jump_count++;
			}

		}

		// Collision logic with the others Koopas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CFireBullet *>(e->obj) && nx != 0)
			{
				if (nx > 0)
					dieDirection = 1;
			}

			if (!dynamic_cast<CMario *>(e->obj) && nx == 0)
			{
				CheckPosition_Y = y;
				CanPullBack = true;
			}

			int id = CGame::GetInstance()->GetCurrentScene()->GetId();
			if (id == 1)
			{
				if (dynamic_cast<CMario *>(e->obj))
				{
					CMario* player1 = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer1();
					if (e->ny < 0 && nx == 0)
					{
						player1->SetState(MARIO_STATE_HITTED);
						player1->StartHitted();
						vx = -0.2f;
						vy = -0.1f;
						if(type==KOOPAS_XANH_WALK)
						this->spinning_shell_intro = true;
					}
				}
			}


			if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Koopas 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (e->nx != 0 && this->GetState() == KOOPAS_STATE_SPINNING)
				{
					if (koopas->GetState() != KOOPAS_STATE_SPINNING)
					{
						if (nx < 0)
							koopas->dieDirection = 1;
						if (koopas->GetType() == KOOPAS_XANH_FLY)
						{
							koopas->SetType(KOOPAS_XANH_WALK);
						}
						koopas->SetState(KOOPAS_STATE_DIE);
						int id = CGame::GetInstance()->GetCurrentScene()->GetId();
						if (id == 3)
						{
							vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
							CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
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

					}
					else
					{
						this->vx = -this->vx;
						koopas->vx = -koopas->vx;
					}

				}

			}

			else if (dynamic_cast<CQuestionBrick *>(e->obj))
			{
				if (e->nx != 0 && ny == 0)
				{
					CQuestionBrick *question_brick = dynamic_cast<CQuestionBrick *>(e->obj);
					if (state == KOOPAS_STATE_SPINNING && question_brick->GetIsAlive())
					{
						question_brick->SetIsAlive(false);
						question_brick->SetIsUp(true);
					}
					vx = -vx;
				}
			}

			else if (dynamic_cast<CFlower *>(e->obj))
			{
				if (this->GetState() == KOOPAS_STATE_SPINNING)
				{
					CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
					vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();
					CFlower *flower = dynamic_cast<CFlower *>(e->obj);
					flower->SetIsAlive(false);
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
						}
						break;
					}
					CGame::GetInstance()->ScoreUp(100);
					vx = -vx;
				}
			}

			else if (dynamic_cast<CBreakableBrick *>(e->obj))
			{
				CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *>(e->obj);
				if (nx != 0 && ny == 0)
				{
					if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_NORMAL)
					{
						if (state == KOOPAS_STATE_SPINNING)
						{
							breakable_brick->SetBreakableBrickAnimationX(breakable_brick->x + (BREAKABLE_BRICK_BBOX_WIDTH / 2));
							breakable_brick->SetBreakableBrickAnimationY(breakable_brick->y + (BREAKABLE_BRICK_BBOX_HEIGHT / 2));
							breakable_brick->SetIsAllowToShowBreakableBrickAnimation(true);
							breakable_brick->SetIsAllowToPullBreakPiece(true);
							breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
							CGame::GetInstance()->ScoreUp(10);
						}
						vx = -vx;
					}
				}
			}
			else if (!dynamic_cast<CMario *>(e->obj) && !dynamic_cast<CFireBullet *>(e->obj))
			{
				if (nx != 0 && ny == 0)
				{
					if (dynamic_cast<CGoomba *>(e->obj))
					{
						CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);
						if (goomba->GetState() != GOOMBA_STATE_DIE && (this->GetState() == KOOPAS_STATE_SPINNING || isHolding))
						{
							goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
						}
					}
					else
					{
						vx = -vx;
					}
				}
			}
		}
	}



	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];


	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == 1)
	{
		if (this->x >= 315)
		{
			if (state == KOOPAS_STATE_SPINNING && reset_spinning)
			{
				SetPosition(-30, 117);
				reset_spinning = false;
			}
		}
	}

	

}

void CKoopas::Render()
{
	int ani = -1;
	if (isAppear)
	{
		switch (type)
		{
		case KOOPAS_XANH_WALK:
			if (state == KOOPAS_STATE_DIE)
			{
				ani = KOOPAS_XANH_MAI_ANI_NGUA;
			}
			else if (state == KOOPAS_STATE_SHELL)
			{
				if (shellUpRender)
				{
					ani = KOOPAS_XANH_MAI_ANI_NGUA;
				}
				else if (reviveRender)
				{
					if (renderRecognization)
						ani = KOOPAS_XANH_ANI_REVIVING_NGUA;
					else
						ani = KOOPAS_XANH_ANI_REVIVING;
				}
				else
					ani = KOOPAS_XANH_MAI_ANI_UP;

				if (spinning_shell_intro)
				{
					ani = KOOPAS_XANH_MAI_ANI_SPINNING;
				}
			}
			else if (state == KOOPAS_STATE_SPINNING)
			{
				if (spinningRecognization)
				{
					ani = KOOPAS_XANH_MAI_ANI_SPINNING_NGUA;
				}
				else
				{
					ani = KOOPAS_XANH_MAI_ANI_SPINNING;
				}
			}
			else if (vx > 0) ani = KOOPAS_XANH_ANI_WALKING_RIGHT;
			else  ani = KOOPAS_XANH_ANI_WALKING_LEFT;
			break;

		case KOOPAS_XANH_FLY:
			ani = KOOPAS_XANH_ANI_FLYING_LEFT;
			break;

		case KOOPAS_RED_WALK:
			if (state == KOOPAS_STATE_DIE)
			{
				ani = KOOPAS_RED_MAI_ANI_NGUA;
			}
			else if (state == KOOPAS_STATE_SHELL)
			{
				if (shellUpRender)
				{
					ani = KOOPAS_RED_MAI_ANI_NGUA;
				}
				else if (reviveRender)
				{
					if (renderRecognization)
						ani = KOOPAS_RED_ANI_REVIVING_NGUA;
					else
						ani = KOOPAS_RED_ANI_REVIVING;
				}
				else
				{
					ani = KOOPAS_RED_MAI_ANI_UP;
				}
			}
			else if (state == KOOPAS_STATE_SPINNING)
			{
				if (spinningRecognization)
				{
					ani = KOOPAS_RED_MAI_ANI_SPINNING_NGUA;

				}
				else
				{
					ani = KOOPAS_RED_MAI_ANI_SPINNING;
				}
			}
			else if (vx < 0) ani = KOOPAS_RED_ANI_WALKING_LEFT;
			else  ani = KOOPAS_RED_ANI_WALKING_RIGHT;
			break;

		case KOOPAS_BLACK:
			if (state == KOOPAS_STATE_DIE)
			{
				ani = KOOPAS_BLACK_NGUA;
			}
			else if (state == KOOPAS_STATE_SHELL)
			{
				ani = KOOPAS_BLACK_UP;
			}
			break;

		case KOOPAS_TYPE_LINE:
			ani = KOOPAS_XANH_ANI_WALKING_RIGHT;
			break;

		case KOOPAS_TYPE_FASTER:
			ani = KOOPAS_XANH_ANI_WALKING_RIGHT_FASTER;
			break;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	RenderBoundingBox();
}

void CKoopas::SetState(int state)
{
	CGameObject::SetState(state);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	switch (state)
	{
	case KOOPAS_STATE_DIE:
		vy = -KOOPAS_DIE_DEFLECT_SPEED;
		vx = 0.1f * dieDirection;
		break;
	case KOOPAS_STATE_WALKING:
		vx = -KOOPAS_WALKING_SPEED;
		break;
	case KOOPAS_STATE_WALKING_RIGHT:
		vx = KOOPAS_WALKING_SPEED * 2;
		break;
	case KOOPAS_STATE_WALKING_RIGHT_FASTER:
		vx = KOOPAS_WALKING_SPEED * 4;
		break;
	case KOOPAS_STATE_SPINNING:
		if (nx > 0)
			vx = KOOPAS_WALKING_SPEED * 7;
		else
			vx = -KOOPAS_WALKING_SPEED * 7;
		break;
	case KOOPAS_STATE_SHELL:
		vx = 0;
		break;

	}

}