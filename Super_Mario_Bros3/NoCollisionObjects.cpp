#include "NoCollisionObjects.h"
#include"Game.h"
#include"Scence.h"

CNoCollisionObjects::CNoCollisionObjects(int scene_id, int ctype)
{
	type = ctype;
	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == INTRO_SCENE_ID)
	{
		isRender = false;
	}
	else
		isRender = true;
}

void CNoCollisionObjects::Render()
{

	int id = CGame::GetInstance()->GetCurrentScene()->GetId();
	if (id == INTRO_SCENE_ID)
	{
		CIntroScence* intro_scene = dynamic_cast<CIntroScence *>(CGame::GetInstance()->GetCurrentScene());
		vector<LPGAMEOBJECT> objects = intro_scene->GetObjects();
		for (size_t i = 0; i < objects.size(); i++)
		{
			if (dynamic_cast<CBackGroundStage *>(objects[i]))
			{
				CBackGroundStage * background_stage = dynamic_cast<CBackGroundStage *>(objects[i]);
				if (background_stage->GetType() == BACKGROUND_STAGE_TYPE_FINAL && background_stage->GetIsAppear())
				{
					if (type == NoCollisionObjects_TYPE_NUMBER_THREE)
						isRender = true;
				}
			}
		}
		CMario* player1 = ((CIntroScence*)CGame::GetInstance()->GetCurrentScene())->GetPlayer1();
		if (player1->GetIsAllowToShowBush() && type == NoCollisionObjects_TYPE_BUSH_INTRO_SCENE)
		{
			isRender = true;
		}
	}
	if (isRender)
		animation_set->at(0)->Render(x, y);
	else
		return;
	//RenderBoundingBox();
}

void CNoCollisionObjects::GetBoundingBox(float &l, float &t, float &r, float &b)
{

}