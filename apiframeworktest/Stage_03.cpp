#include "pch.h"
#include "KeyMgr.h"
#include "Stage_03.h"
#include "Object.h"
#include "Player.h"
#include "Monster.h"
#include "Core.h"
#include "Image.h"
#include "PathMgr.h"
#include "CollisionMgr.h"
#include "KeyMgr.h"
#include "SceneMgr.h"
#include "SoundMgr.h"
#include "Platform.h"
#include "ResMgr.h"
#include "PlatformObj.h"
#include "json/json.h"

static Object* pObj;
static PlatformObj* sPlatObj;

Stage_03::Stage_03()
{
}

Stage_03::~Stage_03()
{
}
void Stage_03::Enter()
{
	//SoundMgr::GetInst()->LoadSound(L"BGM", true, L"Sound\\pianobgm.wav");
	//SoundMgr::GetInst()->Play(L"BGM");

	pObj = new Player;

	pObj->SetPos(Vec2(Core::GetInst()->GetResolution().x / 20 + 235, Core::GetInst()->GetResolution().y / 3 + 350));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::PLAYER);

	Platform* pPlatform = ResMgr::GetInst()->PlatformLoad(L"Stage3", L"Platform\\Stage3.json");
	int mapWidth = pPlatform->GetWidth();
	int mapHeight = pPlatform->GetHeight();
	float tileScale = 16.f;
	Json::Value mapArr = pPlatform->GetMapData();
	PlatformObj* pPlatformObj = nullptr;

	for (int i = 1; i <= mapHeight * mapWidth; i++) {
		if (mapArr[i] != 0)
		{
			int h = i / mapWidth + 1;
			int w = i % mapWidth;

			pPlatformObj = new PlatformObj;
			pPlatformObj->SetName(L"PlatformObj");
			pPlatformObj->SetImageToNum(mapArr[i].asInt());
			pPlatformObj->SetPos(Vec2(tileScale * w, tileScale * h));
			pPlatformObj->SetScale(Vec2(tileScale, tileScale));
			pPlatformObj->SetCenterPos(pPlatformObj->GetPos());
			if (mapArr[i] == 1)
				AddObject(pPlatformObj, GROUP_TYPE::PLATFORM);
			else if (mapArr[i] == 2)
			{
				sPlatObj = pPlatformObj;
				AddObject(pPlatformObj, GROUP_TYPE::PORTAL);
			}
			else if (mapArr[i] == 3)
				AddObject(pPlatformObj, GROUP_TYPE::TEXTPLATFORM);
		}
	}

	// 충돌 지정 
	// Player - Monster 그룹 간의 충돌 체크
	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::MONSTER);
	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::PLATFORM);

}

void Stage_03::Exit()
{
	DeleteAll();
	//SoundMgr::GetInst()->Stop(SOUND_CHANNEL::SC_BGM);
	CollisionMgr::GetInst()->CheckReset();
}

void Stage_03::Update()
{
	Scene::Update();
	if (pObj->GetPos().y > Core::GetInst()->GetResolution().y)
	{
		pObj->SetPos(Vec2(Core::GetInst()->GetResolution().x / 20 + 235, Core::GetInst()->GetResolution().y / 3 + 350));
	}
	if (pObj->GetPos().x + 16 >= sPlatObj->GetPos().x &&
		pObj->GetPos().x - 16 <= sPlatObj->GetPos().x &&
		pObj->GetPos().y + 16 >= sPlatObj->GetPos().y &&
		pObj->GetPos().y - 16 <= sPlatObj->GetPos().y)
	{
		ChangeScene(SCENE_TYPE::Stage_04);
	}
}
