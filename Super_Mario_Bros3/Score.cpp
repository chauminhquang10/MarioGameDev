#include "Score.h"



CScore::CScore()
{
	isUsed = false;
	SetState(SCORE_STATE_IDLE);
}




void CScore::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = t = r = b = 0;
}

void CScore::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);

	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	

	for (UINT i = 0; i < coObjects->size(); i++)
	{
		LPGAMEOBJECT obj = coObjects->at(i);
		if (dynamic_cast<CQuestionBrick *>(obj) || dynamic_cast<CGoomba *>(obj) || dynamic_cast<CKoopas *>(obj) || dynamic_cast<CFlower *>(obj) || dynamic_cast<CLeaf *>(obj) || dynamic_cast<CMushRoom *>(obj) || dynamic_cast<CFireBullet *>(obj))
		{
			CQuestionBrick *question_brick = dynamic_cast<CQuestionBrick *>(obj);
			CGoomba *goomba = dynamic_cast<CGoomba *>(obj);
			CKoopas *koopas = dynamic_cast<CKoopas *>(obj);
			CFlower *flower = dynamic_cast<CFlower *>(obj);
			CLeaf *leaf= dynamic_cast<CLeaf *>(obj);
			CMushRoom* mushroom= dynamic_cast<CMushRoom *>(obj);
			CFireBullet* fire_bullet = dynamic_cast<CFireBullet *>(obj);;
			if (state == SCORE_STATE_IDLE && (question_brick->GetIsAllowToShowScore() || goomba->GetIsAllowToShowScore() || koopas->GetIsAllowToShowScore() || flower->GetIsAllowToShowScore() || leaf->GetIsAllowToShowScore() || mushroom->GetIsAllowToShowScore() || fire_bullet->GetIsAllowToShowScore()) && isUsed )
			{
				StartTiming();
				this->x = mario->GetShowPointX() + 5;
				this->y = mario->GetShowPointY() - 10;
				SetState(SCORE_STATE_UP);
			}


			if (state == SCORE_STATE_UP)
			{
				if (GetTickCount() - timing_start >= 600)
				{
					isUsed = false;
					SetPosition(6000, 6000);
					SetState(SCORE_STATE_IDLE);
					question_brick->SetIsAllowToShowScore(false);
					goomba->SetIsAllowToShowScore(false);
					koopas->SetIsAllowToShowScore(false);
					flower->SetIsAllowToShowScore(false);
					leaf->SetIsAllowToShowScore(false);
					mushroom->SetIsAllowToShowScore(false);
					fire_bullet->SetIsAllowToShowScore(false);
					timing_start = 0;
					value = 0;
				}
			}
		}
	}



	
	
	x += dx;
	y += dy;
	


}

void CScore::Render()
{
	int ani = -1;
	if (isUsed  && value!=0)
	{
		switch (value)
		{
		case SCORE_TYPE_100:
			ani = SCORE_100_ANI;
			break;
		case SCORE_TYPE_200:
			ani = SCORE_200_ANI;
			break;
		case SCORE_TYPE_400:
			ani = SCORE_400_ANI;
			break;
		case SCORE_TYPE_800:
			ani = SCORE_800_ANI;
			break;
		case SCORE_TYPE_1000:
			ani = SCORE_1000_ANI;
			break;
		case SCORE_TYPE_2000:
			ani = SCORE_2000_ANI;
			break;
		case SCORE_TYPE_4000:
			ani = SCORE_4000_ANI;
			break;
		case SCORE_TYPE_8000:
			ani = SCORE_8000_ANI;
			break;
		case SCORE_TYPE_1LIFE:
			ani = SCORE_1LIFE_ANI;
			break;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CScore::SetState(int state)
{
	CGameObject::SetState(state);
	switch (state)
	{
	case  SCORE_STATE_IDLE:
		vx = vy = 0;
		break;
	case  SCORE_STATE_UP:
		vy = -0.08f;
		break;
	}
}

