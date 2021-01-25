#include "QuestionBrick.h"

CQuestionBrick::CQuestionBrick(int ctype)
{
	type = ctype;
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == PLAY_SCENE_1_4_ID)
	{
		if (type == QUESTION_BRICK_HAVE_COIN_MULTIPLE_LIFE)
			life = QUESTION_BRICK_HAVE_COIN_MULTIPLE_LIFE_NUMBER_OF_LIFES;
		else
			life = QUESTION_BRICK_NORMAL_LIFE;
	}
	else
	{
		life = QUESTION_BRICK_NORMAL_LIFE;
	}
}

void CQuestionBrick::CalcPotentialCollisions(vector<LPGAMEOBJECT> *coObjects, vector<LPCOLLISIONEVENT> &coEvents)
{
	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPCOLLISIONEVENT e = SweptAABBEx(coObjects->at(i));

		if (dynamic_cast<CQuestionBrick *>(coObjects->at(i)))
		{
			continue;
		}

		if (e->t > 0 && e->t <= 1.0f)
		{
			coEvents.push_back(e);
		}
		else
			delete e;
	}

	std::sort(coEvents.begin(), coEvents.end(), CCollisionEvent::compare);
}




void CQuestionBrick::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = x;
	t = y;
	r = x + QUESTION_BRICK_BBOX_WIDTH;
	b = y + QUESTION_BRICK_BBOX_HEIGHT;
}

void CQuestionBrick::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{

	CGameObject::Update(dt);
	//
	// TO-DO: make sure Goomba can interact with the world and to each of them too!
	//

	vector<LPCOLLISIONEVENT> coEvents;
	vector<LPCOLLISIONEVENT> coEventsResult;

	coEvents.clear();

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();

	CalcPotentialCollisions(coObjects, coEvents);


	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id != INTRO_SCENE_ID)
	{
		CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
		if (mario->GetIsTurning())
		{
			float leftRec = (float)mario->GetLeftRecMarioTail();
			float topRec = (float)mario->GetTopRecMarioTail();
			float rightRec = (float)mario->GetRightRecMarioTail();
			float bottomRec = (float)mario->GetBottomRecMarioTail();


			if (bottomRec != MARIO_REC_BOTTOM_ORIGIN_VALUE && topRec != MARIO_REC_TOP_ORIGIN_VALUE && leftRec != MARIO_REC_LEFT_ORIGIN_VALUE && rightRec != MARIO_REC_RIGHT_ORIGIN_VALUE)
			{
				float leftRecQuestionBrick, rightRecQuestionBrick, topRecQuestionBrick, bottomQuestionBrick;
				this->GetBoundingBox(leftRecQuestionBrick, topRecQuestionBrick, rightRecQuestionBrick, bottomQuestionBrick);
				if (((leftRec <= rightRecQuestionBrick && leftRec >= leftRecQuestionBrick) || (rightRec <= rightRecQuestionBrick && rightRec >= leftRecQuestionBrick) || ((leftRec <= leftRecQuestionBrick) && (rightRec >= rightRecQuestionBrick))) && ((topRec <= bottomQuestionBrick && topRec >= topRecQuestionBrick) || (bottomRec <= bottomQuestionBrick && bottomRec >= topRecQuestionBrick)))
				{

					int id = CGame::GetInstance()->GetCurrentScene()->GetId();
					if (id == PLAY_SCENE_1_1_ID)
					{
						if (this->GetIsAlive())
						{
							if (!this->GetIsAllowQuestionBrickSlide())
							{
								this->SetIsUp(true);
								this->SetIsAlive(false);
								mario->SetMushRoomCheckPosition(mario->x);
								this->SetIsAllowToShowScore(true);
								this->SetIsAllowQuestionBrickSlide(true);
							}
						}
					}
					else if (id == PLAY_SCENE_1_4_ID)
					{
						if (this->GetIsAlive())
						{
							if (this->GetType() == QUESTION_BRICK_HAVE_COIN_MULTIPLE_LIFE)
							{
								if (!this->GetIsAllowQuestionBrickSlide())
								{
									this->SetIsUp(true);
									this->SetIsAllowToShowScore(true);
									this->SetLifeDown();
									this->SetIsAllowQuestionBrickSlide(true);
									this->SetIsAllowToShowMultipleCoin(true);
									this->SetControlMultipleCoin(false);
								}
							}
							else
							{
								if (!this->GetIsAllowQuestionBrickSlide())
								{
									this->SetIsUp(true);
									this->SetIsAlive(false);
									mario->SetMushRoomCheckPosition(mario->x);
									this->SetIsAllowToShowScore(true);
									this->SetIsAllowQuestionBrickSlide(true);
								}
							}
						}
					}
				}
			}
		}
	}





	if (life < 0)
	{
		isAlive = false;
		isAllowToShowMultipleCoin = false;
	}






	if (isAllowQuestionBrickSlide)
	{
		if (isUp)
		{
			if (time_Y_Up > QUESTION_BRICK_TIME_UPS)
			{
				time_Y_Up = QUESTION_BRICK_TIME_Y_UP_ORIGIN;
				isUp = false;
			}
			else
			{
				y -= QUESTION_BRICK_MINUS_Y_UP;
				time_Y_Up++;
			}
		}
		else
		{
			if (time_Y_Up > QUESTION_BRICK_TIME_UPS)
			{
				vy = QUESTION_BRICK_ORIGIN_SPEED;
				isAllowQuestionBrickSlide = false;
				time_Y_Up = QUESTION_BRICK_TIME_Y_UP_ORIGIN;
			}
			else
			{
				y += QUESTION_BRICK_BONUS_Y_UP;
				time_Y_Up++;
			}
		}
	}




	// No collision occured, proceed normally
	if (coEvents.size() == 0)
	{
		y += dy;
	}
	else
	{

		float min_tx, min_ty, nx = 0, ny;
		float rdx = 0;
		float rdy = 0;

		// TODO: This is a very ugly designed function!!!!
		FilterCollision(coEvents, coEventsResult, min_tx, min_ty, nx, ny, rdx, rdy);

		// block 

		//x += min_tx * dx + nx * 0.5f;		// nx*0.4f : need to push out a bit to avoid overlapping next frame
		//y += min_ty * dy + ny * 0.5f;

		if (nx != 0) vx = 0;

		// Collision logic with the others Goombas
		for (UINT i = 0; i < coEventsResult.size(); i++)
		{
			LPCOLLISIONEVENT e = coEventsResult[i];

		}
	}




	// clean up collision events
	for (UINT i = 0; i < coEvents.size(); i++) delete coEvents[i];

}

void CQuestionBrick::Render()
{
	int ani = -1;

	if (isAlive)
	{
		int id = CGame::GetInstance()->GetCurrentScene()->GetId();
		if (id == PLAY_SCENE_1_1_ID)
		{
			if (type == QUESTION_BRICK_JUST_HAVE_MUSHROOM)
				ani = QUESTION_BRICK_ANI_NEW_TYPE;
			else
				ani = QUESTION_BRICK_ANI_ALIVE;
		}
		else if (id == PLAY_SCENE_1_4_ID)
		{
			ani = QUESTION_BRICK_ANI_NEW_TYPE;
		}
	}
	else
		ani = QUESTION_BRICK_ANI_DEAD;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CQuestionBrick::SetState(int state)
{
	//CGameObject::SetState(state);

}


