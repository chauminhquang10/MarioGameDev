#include <algorithm>
#include <assert.h>
#include "Utils.h"

#include "Mario.h"
#include "Game.h"
#include "Rectangle.h"
#include "Goomba.h"
#include "Portal.h"
#include "PlayScence.h"
#include "KeyEventHandler.h"
#include "Koopas.h"
#include "Flower.h"
#include "Special_Item.h"

CMario::CMario(int ctype, float x, float y) : CGameObject()
{
	type = ctype;
	level = MARIO_LEVEL_BIG;
	untouchable = 0;
	SetState(MARIO_STATE_IDLE);
	start_x = x;
	start_y = y;
	this->x = x;
	this->y = y;
}
void CMario::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		if (dynamic_cast<CRectangle *>(coObjects->at(i)) && vy < 0)
		{
			continue;
		}

		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (e->t > 0 && e->t <= 1.0f)
			coEvents.push_back(e);
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}


void CMario::FilterCollision(vector<LPCOLLISIONEVENT> &coEvents, vector<LPCOLLISIONEVENT> &coEventsResult, float &min_tx, float &min_ty, float &nx, float &ny, float &rdx, float &rdy)
{

	min_tx = 1.0f;
	min_ty = 1.0f;
	int min_ix = -1;
	int min_iy = -1;

	nx = 0.0f;
	ny = 0.0f;

	coEventsResult.clear();

	for (UINT i = 0; i < coEvents.size(); i++)
	{
		LPCOLLISIONEVENT c = coEvents[i];

		if (c->t < min_tx && c->nx != 0) {
			min_tx = c->t; nx = c->nx; min_ix = i; rdx = c->dx;
		}

		if (c->t < min_ty  && c->ny != 0) {
			min_ty = c->t; ny = c->ny; min_iy = i; rdy = c->dy;
		}
		if (dynamic_cast<CCoin *>(c->obj))
		{
			if (isJumping)
			{
				nx = 0;
				ny = 0;
			}
			else
			{
				ny = -0.001f;
			}
		}
		if (dynamic_cast<CMushRoom *>(c->obj) || dynamic_cast<CFlowerBullet *>(c->obj) || dynamic_cast<CKoopas *>(c->obj) || dynamic_cast<CGoomba *>(c->obj) || dynamic_cast<CMario *>(c->obj))
		{
			ny = -0.0001f;
		}
		if (dynamic_cast<CLeaf *>(c->obj))
		{
			if (!isJumping)
			{
				ny = -0.0001f;
			}
			else
			{
				ny = 0;
				nx = 0;
			}
		}
		if (dynamic_cast<CBreakableBrick *>(c->obj))
		{
			CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *> (c->obj);
			if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_COIN)
			{
				if (isJumping)
				{
					nx = 0;
					ny = 0;
				}

			}
		}
		if (dynamic_cast<CRectangle *>(c->obj))
		{
			nx = 0;
		}
	}

	if (min_ix >= 0) coEventsResult.push_back(coEvents[min_ix]);
	if (min_iy >= 0) coEventsResult.push_back(coEvents[min_iy]);
}


void CMario::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	// Calculate dx, dy 
	CGameObject::Update(dt);

	int time_temp = 0;

	// Simple fall down
	if (state != MARIO_STATE_PIPE_DOWNING && state != MARIO_STATE_PIPE_UPPING && !isTransforming)
		vy += MARIO_GRAVITY * dt;


	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();


	// turn off collision when die 

	if (state != MARIO_STATE_DIE && state != MARIO_STATE_PIPE_DOWNING && state != MARIO_STATE_PIPE_UPPING)
		CalcPotentialCollisions(coObjects, coEvents);



	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == 3)
	{

		if (this->y >= 750 && !lose_control && !isAtTheTunnel)
		{
			SetState(MARIO_STATE_DIE);
		}


		int time_picker = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetTimePicker();

		if (time_picker == 0 && state != MARIO_STATE_DIE)
		{
			SetState(MARIO_STATE_DIE);
		}



		if (state == MARIO_STATE_DIE && isAllowToSetLifeDown)
		{
			if (!lose_control)
				CGame::GetInstance()->SetLifeDown();
			isAllowToSetLifeDown = false;
			//StartSwitchScene();
		}

		if (state == MARIO_STATE_DIE)
		{
			if (GetTickCount() - switch_scene_start >= 2000)
			{
				//CGame::GetInstance()->SwitchScene(2);
			}
		}

		if (lose_control && switch_scene_start != 0)
		{
			/*if (GetTickCount() - switch_scene_start >= 1000)
			{*/
				this->isAllowToShowWordsEndScene = true;
			
			if (GetTickCount() - switch_scene_start >= 3000)
			{
				StartCountDownTimePicker();
				if (GetTickCount() - count_down_time_start >= 50)
				{
					if (time_picker > 0)
					{
						((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->SetTimeDown();
						count_down_time_start = 0;
					}
				}
			
			}
			if (GetTickCount() - switch_scene_start >= 6000)
			{
				if (time_picker == 0 && time_temp != 0)
				{
					CGame::GetInstance()->ScoreUp(50 * time_temp);
					time_temp = 0;
				}
			}
			/*	if (GetTickCount() - switch_scene_start >= 10000)
				{
					CGame::GetInstance()->SwitchScene(2);
				}*/
		}

	}






	// reset untouchable timer if untouchable time has passed
	if (GetTickCount() - untouchable_start > MARIO_UNTOUCHABLE_TIME)
	{
		untouchable_start = 0;
		untouchable = 0;
	}

	if (GetTickCount() - turning_start > MARIO_TURNING_TIME)
	{
		isTurning = false;
	}

	if (GetTickCount() - fire_recog_start > MARIO_FIRING_RECOG_TIME)
	{
		isFiring = false;
	}

	if (GetTickCount() - kicking_start > MARIO_KICKING_TIME)
	{
		isKicking = false;
	}


	if (time_mario == 0)
	{
		canFly = false;
		isFlying = false;
		flying_start = 0;
	}

	if (isTransforming)
	{
		vx = 0;
	}

	if (abs((y - CheckPosition)) >= 1)
	{
		isJumping = true;
	}


	if (y - CheckPosition <= 1)
	{
		if (on_the_air_start == 0)
			StartOnTheAir();
	}

	if (state == MARIO_STATE_PIPE_DOWNING)
	{
		if (GetTickCount() - pipe_downing_start >= 3000)
		{
			this->SetPosition(1330, 1050);
			canPipeDowning = false;
			isAtTheTunnel = true;
			SetState(MARIO_STATE_IDLE);
			pipe_downing_start = 0;
		}
		CalcTheMarioTimeDown();
	}

	if (state == MARIO_STATE_PIPE_UPPING)
	{
		if (GetTickCount() - pipe_upping_start >= 3100)
		{
			isAtTheTunnel = false;
			if (!setPositionOutOfTunnel)
			{
				this->SetPosition(2330, 122);
				setPositionOutOfTunnel = true;
			}
		}

		if (GetTickCount() - pipe_upping_start >= 6350)
		{
			canPipeUpping = false;
			SetState(MARIO_STATE_IDLE);
			pipe_upping_start = 0;
			setPositionOutOfTunnel = false;
		}
		CalcTheMarioTimeDown();
	}

	if (transforming_start != 0)
	{
		if (GetTickCount() - transforming_start >= 2000)
		{
			isTransforming = false;
			transforming_start = 0;
			int id = CGame::GetInstance()->GetCurrentScene()->GetId();
			if (id == 1)
			{
				if (level == MARIO_LEVEL_TAIL)
				{
					SetLevel(MARIO_LEVEL_SMALL);
				}
				else if (level == MARIO_LEVEL_BIG)
				{
					SetLevel(MARIO_LEVEL_TAIL);
				}
			}
			else
			{
				if (!transformRecog)
				{
					if (level == MARIO_LEVEL_TAIL)
					{
						SetLevel(MARIO_LEVEL_BIG);
					}
					else
					{
						SetLevel(MARIO_LEVEL_SMALL);
					}
				}
			}

		}
		else
		{
			isTransforming = true;
			int id = CGame::GetInstance()->GetCurrentScene()->GetId();
			if (id == 1)
			{
				if (level == MARIO_LEVEL_BIG && type == MARIO_TYPE_RED)
				{
					SetState(MARIO_STATE_IDLE);
					this->vy = 0;
				}
			}
			else
			{
				SetState(MARIO_STATE_IDLE);
				this->vy = 0;
			}
		}
	}



	//DebugOut(L"[ERROR] bien jumping mario luc nay la %d!\n", isJumping);
	//DebugOut(L"[ERROR] state mario luc nay la %d!\n", state);


	CMario* player1 = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer1();

	vector<LPGAMEOBJECT> scores_panel = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetScoresPanel();

	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		x += dx;
		y += dy;

	}
	else
	{
		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// how to push back Mario if collides with a moving objects, what if Mario is pushed this way into another object?
		//if (rdx != 0 && rdx!=dx)
		//	x += nx*abs(rdx); 

		// block every object first!

		x += min_tx * dx + nx * 0.4f;
		y += min_ty * dy + ny * 0.4f;


		if (ny != 0) vy = 0;


		CheckPosition = y;


		if (ny < 0)    // Handle Jumping
		{
			isJumping = false;
			isFalling = false;
			canFly = true;
			canFall = false;
			on_the_air_start = 0;
		}

		if (ny < 0 && this->time_mario < MARIO_MAX_STACK)
		{
			canFly = false;
		}


		//
		// Collision logic with other objects
		//
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

			if (dynamic_cast<CPipe *>(e->obj))
			{
				CPipe* pipe = dynamic_cast<CPipe *>(e->obj);
				if (pipe->GetType() == PIPE_TYPE_DOWN)
				{
					if ((pipe->x + 2) <= this->x && this->x <= (pipe->x + 16))
					{
						this->canPipeDowning = true;
					}
					else
					{
						this->canPipeDowning = false;
					}
				}
				else if (pipe->GetType() == PIPE_TYPE_UP)
				{
					if ((pipe->x + 2) <= this->x && this->x <= (pipe->x + 16))
					{
						this->canPipeUpping = true;
					}
					else
					{
						this->canPipeUpping = false;
					}
				}
			}

			if (dynamic_cast<CGoomba *>(e->obj)) // if e->obj is Goomba 
			{
				CGoomba *goomba = dynamic_cast<CGoomba *>(e->obj);

				// jump on top >> kill Goomba and deflect a bit 
				if (e->ny < 0)
				{
					if (goomba->GetState() != GOOMBA_STATE_DIE)
					{
						if (goomba->GetType() != GOOMBA_RED_FLY)
						{
							goomba->SetState(GOOMBA_STATE_DIE);
							vy = -MARIO_JUMP_DEFLECT_SPEED;
						}
						else
						{
							if (goomba->GetState() == GOOMBA_STATE_WALKING)
							{
								goomba->SetState(GOOMBA_STATE_RED_LOSE_WINGS);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
							}
							else
							{
								goomba->SetState(GOOMBA_STATE_DIE);
								vy = -MARIO_JUMP_DEFLECT_SPEED;
								pointPara *= 2;
							}
						}
					}
					this->SetShowPointX(this->x);
					this->SetShowPointY(this->y);
					this->SetIsAllowToShowScore(true);
					for (int i = 0; i < scores_panel.size(); i++)
					{
						CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
						if (!score_panel->GetIsUsed())
						{
							score_panel->SetValue(100 * this->pointPara);
							score_panel->SetIsUsed(true);
						}
						break;
					}
					int id = CGame::GetInstance()->GetCurrentScene()->GetId();
					if (id != 1)
						CGame::GetInstance()->ScoreUp(100 * this->pointPara);
					SetPointPara(1);
				}
				else if (e->nx != 0)
				{
					if (untouchable == 0 && isTurning == false)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								transformRecog = false;
								StartTransforming();
								isFiring = false;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
					else if (level == MARIO_LEVEL_TAIL && isTurning)
					{
						if (goomba->GetState() != GOOMBA_STATE_DIE_BY_KICK)
							goomba->SetState(GOOMBA_STATE_DIE_BY_KICK);
						this->SetShowPointX(this->x);
						this->SetShowPointY(this->y);
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

					}
				}
			}
			else if (dynamic_cast<CPortal *>(e->obj))
			{
				CPortal *p = dynamic_cast<CPortal *>(e->obj);
				CGame::GetInstance()->SwitchScene(p->GetSceneId());
			}

			else if (dynamic_cast<CKoopas *>(e->obj)) // if e->obj is Koopas 
			{
				CKoopas *koopas = dynamic_cast<CKoopas *>(e->obj);
				if (e->ny < 0)
				{
					if (koopas->GetType() == KOOPAS_XANH_FLY)
					{
						koopas->SetType(KOOPAS_XANH_WALK);
						vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
						koopas->SetIsAllowToUpPointPara(true);
					}
					else
					{
						if (koopas->GetState() != KOOPAS_STATE_SHELL)
						{
							koopas->SetState(KOOPAS_STATE_SHELL);
							if (koopas->GetSpinningRegconization())
								koopas->SetShellUpRender(true);
							koopas->SetReviveRender(false);
							koopas->SetReviveStart(0);
							vy = -1.5f * MARIO_JUMP_DEFLECT_SPEED;
							if (koopas->GetIsAllowToUpPointPara() && !this->toEndTheScoreProgress)
							{
								pointPara *= 2;
							}
						}
						else if (koopas->GetState() == KOOPAS_STATE_SHELL)
						{
							vy = -1.5f *MARIO_JUMP_DEFLECT_SPEED;
							koopas->SetState(KOOPAS_STATE_SPINNING);
							pointPara *= 2;
							koopas->SetIsAllowToUpPointPara(true);
						}
					}
					this->SetShowPointX(this->x);
					this->SetShowPointY(this->y);
					this->SetIsAllowToShowScore(true);
					for (int i = 0; i < scores_panel.size(); i++)
					{
						CScore* score_panel = dynamic_cast<CScore*> (scores_panel[i]);
						if (!score_panel->GetIsUsed())
						{
							score_panel->SetValue(100 * this->pointPara);
							score_panel->SetIsUsed(true);
						}
						break;
					}
					int id = CGame::GetInstance()->GetCurrentScene()->GetId();
					if (id != 1)
						CGame::GetInstance()->ScoreUp(100 * this->pointPara);
					if (pointPara == 8)
					{
						SetPointPara(1);
						this->toEndTheScoreProgress = true;
					}
					else
					{
						this->toEndTheScoreProgress = false;
					}
				}
				else if (nx != 0)
				{
					if (koopas->GetState() == KOOPAS_STATE_SPINNING && this->type == MARIO_TYPE_GREEN)
					{
						koopas->SetState(KOOPAS_STATE_SHELL);
						koopas->SetResetSpinning(true);
						this->isHolding = true;
					}

					if (level == MARIO_LEVEL_TAIL && isTurning)
					{
						if (koopas->GetType() == KOOPAS_XANH_FLY)
							koopas->SetType(KOOPAS_XANH_WALK);
						koopas->SetState(KOOPAS_STATE_SHELL);
						koopas->SetShellUpRender(true);
						koopas->SetRenderRegconization(true);
						koopas->SetIsKickedRevive(true);
						koopas->vy = -KOOPAS_SHELL_DEFLECT_SPEED;
						koopas->vx = 0.1f * (-nx);
					}
					else if (koopas->GetState() == KOOPAS_STATE_SHELL)
					{
						if (isHolding)
						{
							koopas->SetIsHolding(true);
						}
						else
						{
							StartKicking();
							isKicking = true;
							koopas->nx = this->nx;
							koopas->SetReviveStart(0);
							koopas->SetReviveRender(false);
							koopas->SetState(KOOPAS_STATE_SPINNING);

						}
					}
					else if (untouchable == 0 && isKicking == false)
					{
						if (koopas->GetState() != KOOPAS_STATE_SHELL)
						{
							if (level > MARIO_LEVEL_SMALL)
							{
								StartTransforming();
								transformRecog = false;
								int id = CGame::GetInstance()->GetCurrentScene()->GetId();
								if (id == 1)
								{
									isAllowToThroughMario = true;
								}
								isFiring = false;
								StartUntouchable();
							}
							else
								SetState(MARIO_STATE_DIE);
						}
					}
				}
			}
			else if (dynamic_cast<CCoin *>(e->obj)) // if e->obj is Coin
			{
				CCoin *coin = dynamic_cast<CCoin *>(e->obj);
				if (coin->GetType() == COIN_NORMAL)
				{
					coin->SetIsAppear(false);
					CGame::GetInstance()->MoneyUp();
					CGame::GetInstance()->ScoreUp(50);
				}

			}


			else if (dynamic_cast<CMario *>(e->obj))
			{
				CMario *mario_green = dynamic_cast<CMario *>(e->obj);
				if (this->type == MARIO_TYPE_RED && mario_green->type == MARIO_TYPE_GREEN)
				{

					this->SetState(MARIO_STATE_SITDOWN);

				}

			}


			else if (dynamic_cast<CFlower *>(e->obj))
			{
				if (level == MARIO_LEVEL_TAIL && isTurning)
				{
					CFlower *flower = dynamic_cast<CFlower *>(e->obj);
					flower->SetIsAlive(false);
					this->SetShowPointX(this->x);
					this->SetShowPointY(this->y);
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
				}
				else if (untouchable == 0 && !isTurning)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						StartTransforming();
						transformRecog = false;
						isFiring = false;
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);
				}
			}

			else if (dynamic_cast<CFlowerBullet *>(e->obj))
			{
				if (untouchable == 0)
				{
					if (level > MARIO_LEVEL_SMALL)
					{
						StartTransforming();
						transformRecog = false;
						isFiring = false;
						StartUntouchable();
					}
					else
						SetState(MARIO_STATE_DIE);

				}
			}



			else if (dynamic_cast<CSpecial_Item *>(e->obj))
			{
				int time_picker = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetTimePicker();
				CSpecial_Item *special_item = dynamic_cast<CSpecial_Item *>(e->obj);
				int special_item_state = special_item->GetState();
				this->words_end_scene_item_id = special_item_state;
				switch (special_item_state)
				{
				case 100:
					special_item->SetState(400);
					break;
				case 200:
					special_item->SetState(500);
					break;
				case 300:
					special_item->SetState(600);
					break;
				}

				vector<int> items_render = CGame::GetInstance()->GetItemsTypeRender();
				for (int i = 0; i < items_render.size(); i++)
				{
					if (items_render[i] == 1)
					{
						switch (special_item_state)
						{
						case 100:
							items_render[i] = 3;
							break;
						case 200:
							items_render[i] = 2;
							break;
						case 300:
							items_render[i] = 4;
							break;
						}
						break;
					}
				}
				CGame::GetInstance()->SetItems(items_render);
				this->lose_control = true;
				StartSwitchScene();
				time_temp = time_picker;
			
			}

			else if (dynamic_cast<CQuestionBrick *>(e->obj))
			{
				CQuestionBrick *question_brick = dynamic_cast<CQuestionBrick *>(e->obj);
				if (e->ny > 0)
				{
					if (question_brick->GetIsAlive())
					{
						question_brick->SetIsUp(true);
						question_brick->SetIsAlive(false);
						MushroomCheckPosition = this->x;
					}
				}
				else if (nx != 0)
				{
					if (this->level == MARIO_LEVEL_TAIL && isTurning)
					{
						if (question_brick->GetIsAlive())
						{
							question_brick->SetIsUp(true);
							question_brick->SetIsAlive(false);
						}
					}
				}

			}
			else if (dynamic_cast<CBell *>(e->obj))
			{
				if (e->ny < 0)
				{
					for (UINT i = 0; i < coObjects->size(); i++)
					{
						LPGAMEOBJECT obj = coObjects->at(i);
						if (dynamic_cast<CBreakableBrick *>(obj))
						{
							CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *>(obj);
							if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_NORMAL && !breakable_brick->GetIsRevive())
							{
								breakable_brick->SetState(BREAKABLE_BRICK_STATE_COIN);
								breakable_brick->SetIsRevive(true);
							}

						}
					}
					CBell *bell = dynamic_cast<CBell *>(e->obj);
					bell->SetState(BELL_STATE_DIE);
				}

			}
			else if (dynamic_cast<CBreakableBrick *>(e->obj))
			{
				CBreakableBrick *breakable_brick = dynamic_cast<CBreakableBrick *>(e->obj);
				if (nx != 0 && breakable_brick->GetState() == BREAKABLE_BRICK_STATE_NORMAL)
				{
					if (isTurning && breakable_brick->y >= this->y + MARIO_TURNING_BONUS_HEIGHT)
					{
						breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
						CGame::GetInstance()->ScoreUp(10);
					}
				}
				else if (breakable_brick->GetState() == BREAKABLE_BRICK_STATE_COIN)
				{
					breakable_brick->SetState(BREAKABLE_BRICK_STATE_BREAK);
					CGame::GetInstance()->MoneyUp();
				}
			}

		}

		// clean up collision events
		for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

		// simple screen edge collision!!!
		if (vx < 0 && x < 0) x = 0;


	}
}

void CMario::Render()
{
	int ani = -1;

	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (isAppear)
	{
		switch (type)
		{
		case MARIO_TYPE_RED:


			if (state == MARIO_STATE_DIE)
				ani = MARIO_ANI_DIE;


			else if (isFalling)
			{
				if (nx > 0) ani = MARIO_ANI_FALLING_RIGHT;
				else ani = MARIO_ANI_FALLING_LEFT;
			}

			else if (isFlying)
			{
				if (nx > 0) ani = MARIO_ANI_TAIL_FLYING_RIGHT;
				else ani = MARIO_ANI_TAIL_FLYING_LEFT;
			}




			else if (isTurning)
			{
				if (nx < 0) ani = MARIO_ANI_TAIL_TURNING_RIGHT;
				else ani = MARIO_ANI_TAIL_TURNING_LEFT;
			}



			else if (canHold)
			{
				if (vx == 0)
				{
					if (level == MARIO_LEVEL_BIG)
					{
						if (nx > 0) ani = MARIO_ANI_BIG_IDLE_HOLDING_RIGHT;
						else ani = MARIO_ANI_BIG_IDLE_HOLDING_LEFT;
					}
					else if (level == MARIO_LEVEL_SMALL)
					{
						if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_HOLDING_RIGHT;
						else ani = MARIO_ANI_SMALL_IDLE_HOLDING_LEFT;
					}
					else if (level == MARIO_LEVEL_TAIL)
					{
						if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_HOLDING_RIGHT;
						else ani = MARIO_ANI_TAIL_IDLE_HOLDING_LEFT;
					}
					else if (level == MARIO_LEVEL_FIRE)
					{
						if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_HOLDING_RIGHT;
						else ani = MARIO_ANI_FIRE_IDLE_HOLDING_LEFT;
					}
				}
				else
				{
					if (level == MARIO_LEVEL_BIG)
					{
						if (nx > 0) ani = MARIO_ANI_BIG_HOLDING_RIGHT;
						else ani = MARIO_ANI_BIG_HOLDING_LEFT;
					}
					else if (level == MARIO_LEVEL_SMALL)
					{
						if (nx > 0) ani = MARIO_ANI_SMALL_HOLDING_RIGHT;
						else ani = MARIO_ANI_SMALL_HOLDING_LEFT;
					}
					else if (level == MARIO_LEVEL_TAIL)
					{
						if (nx > 0) ani = MARIO_ANI_TAIL_HOLDING_RIGHT;
						else ani = MARIO_ANI_TAIL_HOLDING_LEFT;
					}
					else if (level == MARIO_LEVEL_FIRE)
					{
						if (nx > 0) ani = MARIO_ANI_FIRE_HOLDING_RIGHT;
						else ani = MARIO_ANI_FIRE_HOLDING_LEFT;
					}
				}
			}

			else if (isFiring == true)
			{
				if (nx > 0) ani = MARIO_ANI_FIRE_SHOOTING_RIGHT;
				else ani = MARIO_ANI_FIRE_SHOOTING_LEFT;

			}


			else if (isJumping == true)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					if (vy < 0)
					{
						if (nx > 0) ani = MARIO_ANI_BIG_JUMPING_RIGHT;
						else ani = MARIO_ANI_BIG_JUMPING_LEFT;
					}
					else
					{
						if (nx > 0) ani = MARIO_ANI_BIG_JUMP_FALL_RIGHT;
						else ani = MARIO_ANI_BIG_JUMP_FALL_LEFT;
					}
				}

				else if (level == MARIO_LEVEL_SMALL)
				{
					if (nx > 0) ani = MARIO_ANI_SMALL_JUMPING_RIGHT;
					else ani = MARIO_ANI_SMALL_JUMPING_LEFT;
				}

				else if (level == MARIO_LEVEL_TAIL)
				{
					if (vy < 0)
					{
						if (nx > 0) ani = MARIO_ANI_TAIL_JUMPING_RIGHT;
						else ani = MARIO_ANI_TAIL_JUMPING_LEFT;
					}
					else
					{
						if (nx > 0) ani = MARIO_ANI_TAIL_JUMP_FALL_RIGHT;
						else ani = MARIO_ANI_TAIL_JUMP_FALL_LEFT;
					}
				}
				else if (level == MARIO_LEVEL_FIRE)
				{
					if (vy < 0)
					{
						if (nx > 0) ani = MARIO_ANI_FIRE_JUMPING_RIGHT;
						else ani = MARIO_ANI_FIRE_JUMPING_LEFT;
					}
					else
					{
						if (nx > 0) ani = MARIO_ANI_FIRE_JUMP_FALL_RIGHT;
						else ani = MARIO_ANI_FIRE_JUMP_FALL_LEFT;
					}
				}
			}



			else if (state == MARIO_STATE_IDLE)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					if (nx > 0) ani = MARIO_ANI_BIG_IDLE_RIGHT;
					else ani = MARIO_ANI_BIG_IDLE_LEFT;
				}
				else if (level == MARIO_LEVEL_SMALL)
				{
					if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					else ani = MARIO_ANI_SMALL_IDLE_LEFT;
				}
				else if (level == MARIO_LEVEL_TAIL)
				{
					if (nx > 0) ani = MARIO_ANI_TAIL_IDLE_RIGHT;
					else ani = MARIO_ANI_TAIL_IDLE_LEFT;
				}
				else if (level == MARIO_LEVEL_FIRE)
				{
					if (nx > 0) ani = MARIO_ANI_FIRE_IDLE_RIGHT;
					else ani = MARIO_ANI_FIRE_IDLE_LEFT;
				}
			}


			else if (canBrake)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					if (nx > 0) ani = MARIO_ANI_BIG_BRAKING_RIGHT;
					else ani = MARIO_ANI_BIG_BRAKING_LEFT;
				}
				else if (level == MARIO_LEVEL_SMALL)
				{
					if (nx > 0) ani = MARIO_ANI_SMALL_BRAKING_RIGHT;
					else ani = MARIO_ANI_SMALL_BRAKING_LEFT;
				}
				else if (level == MARIO_LEVEL_TAIL)
				{
					if (nx > 0) ani = MARIO_ANI_TAIL_BRAKING_RIGHT;
					else ani = MARIO_ANI_TAIL_BRAKING_LEFT;
				}
				else if (level == MARIO_LEVEL_FIRE)
				{
					if (nx > 0) ani = MARIO_ANI_FIRE_BRAKING_RIGHT;
					else ani = MARIO_ANI_FIRE_BRAKING_LEFT;
				}
			}




			else if (state == MARIO_STATE_SITDOWN)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					if (nx > 0) ani = MARIO_ANI_BIG_SITDOWN_RIGHT;
					else ani = MARIO_ANI_BIG_SITDOWN_LEFT;
				}
				if (level == MARIO_LEVEL_SMALL)
				{
					if (nx > 0) ani = MARIO_ANI_SMALL_IDLE_RIGHT;
					else ani = MARIO_ANI_SMALL_IDLE_LEFT;
				}
				else if (level == MARIO_LEVEL_TAIL)
				{
					if (nx > 0) ani = MARIO_ANI_TAIL_SITDOWN_RIGHT;
					else ani = MARIO_ANI_TAIL_SITDOWN_LEFT;
				}
				else if (level == MARIO_LEVEL_FIRE)
				{
					if (nx > 0) ani = MARIO_ANI_FIRE_SITDOWN_RIGHT;
					else ani = MARIO_ANI_FIRE_SITDOWN_LEFT;
				}
			}
			else if (state == MARIO_STATE_RUNNING_RIGHT)
			{
				if (time_mario == MARIO_MAX_STACK)
				{

					if (level == MARIO_LEVEL_BIG)
					{
						ani = MARIO_ANI_BIG_MAX_SPEED_RIGHT;

					}
					else if (level == MARIO_LEVEL_SMALL)
					{
						ani = MARIO_ANI_SMALL_MAX_SPEED_RIGHT;

					}
					else if (level == MARIO_LEVEL_TAIL)
					{
						ani = MARIO_ANI_TAIL_MAX_SPEED_RIGHT;

					}
					else if (level == MARIO_LEVEL_FIRE)
					{
						ani = MARIO_ANI_FIRE_MAX_SPEED_RIGHT;

					}
				}
				else
				{
					if (level == MARIO_LEVEL_BIG)
					{
						ani = MARIO_ANI_BIG_RUNNING_RIGHT;

					}
					else if (level == MARIO_LEVEL_SMALL)
					{
						ani = MARIO_ANI_SMALL_RUNNING_RIGHT;

					}
					else if (level == MARIO_LEVEL_TAIL)
					{
						ani = MARIO_ANI_TAIL_RUNNING_RIGHT;

					}
					else if (level == MARIO_LEVEL_FIRE)
					{
						ani = MARIO_ANI_FIRE_RUNNING_RIGHT;

					}
				}
			}

			else if (state == MARIO_STATE_RUNNING_LEFT)
			{
				if (time_mario == MARIO_MAX_STACK)
				{
					if (level == MARIO_LEVEL_BIG)
					{
						ani = MARIO_ANI_BIG_MAX_SPEED_LEFT;
					}
					else if (level == MARIO_LEVEL_SMALL)
					{
						ani = MARIO_ANI_SMALL_MAX_SPEED_LEFT;
					}
					else if (level == MARIO_LEVEL_TAIL)
					{
						ani = MARIO_ANI_TAIL_MAX_SPEED_LEFT;
					}
					else if (level == MARIO_LEVEL_FIRE)
					{
						ani = MARIO_ANI_FIRE_MAX_SPEED_LEFT;
					}
				}
				else
				{
					if (level == MARIO_LEVEL_BIG)
					{
						ani = MARIO_ANI_BIG_RUNNING_LEFT;
					}
					else if (level == MARIO_LEVEL_SMALL)
					{
						ani = MARIO_ANI_SMALL_RUNNING_LEFT;
					}
					else if (level == MARIO_LEVEL_TAIL)
					{
						ani = MARIO_ANI_TAIL_RUNNING_LEFT;
					}
					else if (level == MARIO_LEVEL_FIRE)
					{
						ani = MARIO_ANI_FIRE_RUNNING_LEFT;
					}
				}
			}


			else if (isKicking)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					if (nx > 0) ani = MARIO_ANI_BIG_KICKING_RIGHT;
					else ani = MARIO_ANI_BIG_KICKING_LEFT;
				}
				else if (level == MARIO_LEVEL_SMALL)
				{
					if (nx > 0) ani = MARIO_ANI_SMALL_KICKING_RIGHT;
					else ani = MARIO_ANI_SMALL_KICKING_LEFT;
				}
				else if (level == MARIO_LEVEL_TAIL)
				{
					if (nx > 0) ani = MARIO_ANI_TAIL_KICKING_RIGHT;
					else ani = MARIO_ANI_TAIL_KICKING_LEFT;
				}
				else if (level == MARIO_LEVEL_FIRE)
				{
					if (nx > 0) ani = MARIO_ANI_FIRE_KICKING_RIGHT;
					else ani = MARIO_ANI_FIRE_KICKING_LEFT;
				}
			}

			else if (state == MARIO_STATE_HITTED)
			{
				ani = MARIO_RED_ANI_HITTED;
			}

			else if (state == MARIO_STATE_LOOK_UP)
			{
				ani = MARIO_RED_LOOKING_UP;
			}

			else if (nx > 0) // walking right
			{
				if (level == MARIO_LEVEL_BIG)
				{

					ani = MARIO_ANI_BIG_WALKING_RIGHT;

				}
				else if (level == MARIO_LEVEL_SMALL)
				{
					ani = MARIO_ANI_SMALL_WALKING_RIGHT;

				}
				else if (level == MARIO_LEVEL_TAIL)
				{
					ani = MARIO_ANI_TAIL_WALKING_RIGHT;

				}
				else if (level == MARIO_LEVEL_FIRE)
				{
					ani = MARIO_ANI_FIRE_WALKING_RIGHT;

				}

			}

			else if (nx < 0) // walking left
			{
				if (level == MARIO_LEVEL_BIG)
				{
					ani = MARIO_ANI_BIG_WALKING_LEFT;

				}
				else if (level == MARIO_LEVEL_SMALL)
				{
					ani = MARIO_ANI_SMALL_WALKING_LEFT;

				}
				else if (level == MARIO_LEVEL_TAIL)
				{

					ani = MARIO_ANI_TAIL_WALKING_LEFT;
				}
				else if (level == MARIO_LEVEL_FIRE)
				{

					ani = MARIO_ANI_FIRE_WALKING_LEFT;
				}
			}

			if (state == MARIO_STATE_PIPE_DOWNING || state == MARIO_STATE_PIPE_UPPING)
			{
				if (level == MARIO_LEVEL_BIG)
				{
					ani = MARIO_ANI_BIG_PIPE;
				}
				else if (level == MARIO_LEVEL_SMALL)
				{
					ani = MARIO_ANI_SMALL_PIPE;
				}
				else if (level == MARIO_LEVEL_TAIL)
				{
					ani = MARIO_ANI_TAIL_PIPE;
				}
				else
				{
					ani = MARIO_ANI_FIRE_PIPE;
				}
			}



			if (id != 1)
			{
				if (level == MARIO_LEVEL_BIG || level == MARIO_LEVEL_SMALL)
				{
					if (isTransforming)
					{
						if (nx > 0) ani = MARIO_TRANSFORM_RIGHT;
						else ani = MARIO_TRANSFORM_LEFT;
					}

				}
				else if (level == MARIO_LEVEL_TAIL)
				{
					if (isTransforming)
					{
						if (nx > 0)	ani = MARIO_SMOKE_TRANSFORM_RIGHT;
						else ani = MARIO_SMOKE_TRANSFORM_LEFT;
					}
				}
			}
			else
			{
				if (level == MARIO_LEVEL_BIG)
				{
					if (isTransforming)
					{
						ani = MARIO_SMOKE_TRANSFORM_LEFT;
					}
				}
				else if (level == MARIO_LEVEL_TAIL)
				{
					if (isTransforming)
					{
						ani = MARIO_TRANSFORM_RIGHT;
					}
				}
			}

			break;

		case MARIO_TYPE_GREEN:

			if (isKicking)
			{
				if (nx > 0) ani = MARIO_GREEN_ANI_BIG_KICKING_RIGHT;
				else ani = MARIO_GREEN_ANI_BIG_KICKING_LEFT;

			}

			else if (canHold)
			{
				if (vx == 0)
				{
					if (nx > 0) ani = MARIO_GREEN_ANI_BIG_HOLDING_IDLE_RIGHT;
					else ani = MARIO_GREEN_ANI_BIG_HOLDING_IDLE_LEFT;

				}
				else
				{

					if (nx > 0) ani = MARIO_GREEN_ANI_BIG_HOLDING_RIGHT;
					else ani = MARIO_GREEN_ANI_BIG_HOLDING_LEFT;

				}
			}

			else if (state == MARIO_STATE_IDLE)
			{
				if (nx > 0) ani = MARIO_GREEN_ANI_BIG_IDLE_RIGHT;
				else ani = MARIO_GREEN_ANI_BIG_IDLE_LEFT;

			}



			else if (isJumping == true)
			{

				if (vy < 0)
				{
					if (nx > 0) ani = MARIO_GREEN_ANI_BIG_JUMPING_RIGHT;
					else ani = MARIO_GREEN_ANI_BIG_JUMPING_LEFT;
				}
				else
				{
					if (nx > 0) ani = MARIO_GREEN_ANI_JUMP_FALL_RIGHT;
					else ani = MARIO_GREEN_ANI_JUMP_FALL_LEFT;
				}

			}


			else if (state == MARIO_STATE_RUNNING_RIGHT)
			{
				ani = MARIO_GREEN_ANI_BIG_RUNNING_RIGHT;
			}

			else if (state == MARIO_STATE_RUNNING_LEFT)
			{
				ani = MARIO_GREEN_ANI_BIG_RUNNING_LEFT;

			}



			else if (nx > 0) // walking right
			{
				//if (state == MARIO_STATE_WALKING_RIGHT)
				ani = MARIO_GREEN_ANI_BIG_WALKING_RIGHT;

			}

			else if (nx < 0) // walking left
			{

				ani = MARIO_GREEN_ANI_BIG_WALKING_LEFT;
			}
			break;

		}
	}
	else return;


	int alpha = 255;
	if (untouchable) alpha = 128;

	animation_set->at(ani)->Render(x, y, alpha);

	//RenderBoundingBox();
}

void CMario::SetState(int state)
{
	CMario* player1 = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer1();
	CGameObject::SetState(state);

	switch (state)
	{
	case MARIO_STATE_WALKING_RIGHT:
		nx = 1;
		if (BrakingCalculation() == false)
		{
			vx = MARIO_WALKING_SPEED / 2;
		}
		break;
	case MARIO_STATE_WALKING_LEFT:
		nx = -1;
		if (BrakingCalculation() == false)
			vx = -MARIO_WALKING_SPEED / 2;
		break;
	case MARIO_STATE_RUNNING_RIGHT:
		nx = 1;
		if (BrakingCalculation() == false)
		{
			if (time_mario == MARIO_MAX_STACK)
			{
				vx = MARIO_RUNNING_SPEED * 5;
			}
			else
			{
				vx = MARIO_ACCELERATION * 5 * time_mario;
			}
		}
		break;
	case MARIO_STATE_RUNNING_LEFT:
		nx = -1;
		if (BrakingCalculation() == false)
		{
			if (time_mario == MARIO_MAX_STACK)
			{
				vx = -MARIO_RUNNING_SPEED * 5;
			}
			else
			{
				vx = -(MARIO_ACCELERATION * 5 * time_mario);
			}
		}
		break;
	case MARIO_STATE_JUMP:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -MARIO_JUMP_SPEED_Y;
		break;
	case MARIO_STATE_JUMP_HIGH:
		// TODO: need to check if Mario is *current* on a platform before allowing to jump again
		vy = -0.77f;
		break;
	case MARIO_STATE_SITDOWN:
		vx = 0;
		break;
	case MARIO_STATE_IDLE:
		vx = 0;
		break;
	case MARIO_STATE_SPEED_DOWN:
		if (vx > 0)
		{
			vx -= MARIO_SPEED_DOWN;
		}
		else if (vx < 0)
		{
			vx += MARIO_SPEED_DOWN;
		}
		break;
	case MARIO_STATE_TURNING_TAIL:
		vx = 0;
		break;
	case MARIO_STATE_FLYING_RIGHT:
		vx = MARIO_WALKING_SPEED;
		vy = -MARIO_WALKING_SPEED;
		nx = 1;
		break;
	case MARIO_STATE_FLYING_LEFT:
		vx = -MARIO_WALKING_SPEED;
		vy = -MARIO_WALKING_SPEED;
		nx = -1;
		break;
	case MARIO_STATE_FALLING_DOWN:
		vy = 0.08f;
		player1->vx = -0.16f;
		break;
	case MARIO_STATE_PIPE_DOWNING:
		vy = 0.01f;
		break;
	case MARIO_STATE_PIPE_UPPING:
		vy = -0.01f;
		vx = 0;
		break;
	case MARIO_STATE_DIE:
		vy = -MARIO_DIE_DEFLECT_SPEED;
		break;
	}

}

void CMario::GetBoundingBox(float &left, float &top, float &right, float &bottom)
{

	left = x;
	top = y;

	if (state == MARIO_STATE_SITDOWN && level != MARIO_LEVEL_SMALL) // Set the bounding box again when mario sits down
	{
		top += 9;

	}

	if (level == MARIO_LEVEL_BIG)
	{
		right = x + MARIO_BIG_BBOX_WIDTH;
		bottom = y + MARIO_BIG_BBOX_HEIGHT;

	}
	else if (level == MARIO_LEVEL_SMALL)
	{
		right = x + MARIO_SMALL_BBOX_WIDTH;
		bottom = y + MARIO_SMALL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_TAIL)
	{
		right = x + MARIO_TAIL_BBOX_WIDTH;
		bottom = y + MARIO_TAIL_BBOX_HEIGHT;
	}
	else if (level == MARIO_LEVEL_FIRE)
	{
		right = x + MARIO_FIRE_BBOX_WIDTH;
		bottom = y + MARIO_FIRE_BBOX_HEIGHT;
	}
}

/*
	Reset Mario status to the beginning state of a scene
*/
void CMario::Reset()
{
	SetState(MARIO_STATE_IDLE);
	SetLevel(MARIO_LEVEL_BIG);
	SetPosition(start_x, start_y);
	SetSpeed(0, 0);
}

void CMario::SetLevel(int l)
{
	int oldlevel = this->level;
	this->level = l;
	if (level == MARIO_LEVEL_SMALL)
	{
		y += MARIO_DIFFERENCE_HEIGHT;
	}
	else if (oldlevel == MARIO_LEVEL_SMALL)
	{
		y -= MARIO_DIFFERENCE_HEIGHT;
	}
}