#include "WordsEndScene.h"



CWordsEndScene::CWordsEndScene(int ctype)
{
	type = ctype;
	isAppear = false;
}




void CWordsEndScene::GetBoundingBox(float &l, float &t, float &r, float &b)
{
	l = t = r = b = 0;
}

void CWordsEndScene::Update(DWORD dt, vector<LPGAMEOBJECT> *coObjects)
{
	CGameObject::Update(dt);
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (mario->GetIsAllowToShowWordsEndScene())
	{
		StartTimeRecog();
		if (type == WORDS_END_SCENE_TYPE_COURSE_CLEAR)
		{
			if (GetTickCount() - timing_recog_start >= 100)
			{
				this->isAppear = true;
			}
		}
		else if (type == WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD)
		{
			if (GetTickCount() - timing_recog_start >= 700)
			{
				this->isAppear = true;
			}
		}
		else if (type == WORDS_END_SCENE_TYPE_ITEM)
		{
			if (GetTickCount() - timing_recog_start >= 1300)
			{
				this->isAppear = true;
			}
		}
	}
	

}

void CWordsEndScene::Render()
{
	int ani = -1;
	CMario* mario = ((CPlayScene*)CGame::GetInstance()->GetCurrentScene())->GetPlayer();
	if (isAppear)
	{
		switch (type)
		{
		case WORDS_END_SCENE_TYPE_COURSE_CLEAR:
			ani = WORDS_END_SCENE_TYPE_COURSE_CLEAR_ANI;
			break;
		case WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD:
			ani = WORDS_END_SCENE_TYPE_YOU_GOT_A_CARD_ANI;
			break;
		case WORDS_END_SCENE_TYPE_ITEM:
			switch (mario->GetWordsEndSceneItemId())
			{
			case SPECIAL_ITEM_STATE_FLOWER_IDLE:
				ani = WORDS_END_SCENE_TYPE_ITEM_ANI_FLOWER;
				break;
			case SPECIAL_ITEM_STATE_MUSHROOM_IDLE:
				ani = WORDS_END_SCENE_TYPE_ITEM_ANI_MUSHROOM;
				break;
			case SPECIAL_ITEM_STATE_STAR_IDLE:
				ani = WORDS_END_SCENE_TYPE_ITEM_ANI_STAR;
				break;
			}
			break;
		}
	}
	else return;
	animation_set->at(ani)->Render(x, y);

	//RenderBoundingBox();
}

void CWordsEndScene::SetState(int state)
{
	CGameObject::SetState(state);
}

