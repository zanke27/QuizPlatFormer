#include "pch.h"
#include "KeyMgr.h"
#include "Scene_01.h"
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
#include "Background.h"
#include "ResMgr.h"
#include "PlatformObj.h"
#include "json/json.h"

static Object* pObj;
static PlatformObj* sPlatObj;

Scene_01::Scene_01()
{
}

Scene_01::~Scene_01()
{
}
void Scene_01::Enter()
{
	//SoundMgr::GetInst()->LoadSound(L"BGM", true, L"Sound\\pianobgm.wav");
	//SoundMgr::GetInst()->Play(L"BGM");
	Object* bObj = new Background;

	bObj->SetPos(Vec2(Core::GetInst()->GetResolution().x / 2, Core::GetInst()->GetResolution().y / 2));
	bObj->SetScale(Vec2(200.f, 200.f));
	AddObject(bObj, GROUP_TYPE::DEFAULT);

	pObj = new Player;

	pObj->SetPos(Vec2(Core::GetInst()->GetResolution().x / 20, Core::GetInst()->GetResolution().y / 3 - 50));
	pObj->SetScale(Vec2(100.f, 100.f));
	AddObject(pObj, GROUP_TYPE::PLAYER);

	Platform* pPlatform = ResMgr::GetInst()->PlatformLoad(L"Stage2", L"Platform\\Stage2.json");
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

	// ?浹 ???? 
	// Player - Monster ?׷? ???? ?浹 üũ
	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::MONSTER);
	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::BULLET_PLAYER, GROUP_TYPE::MONSTER);
	CollisionMgr::GetInst()->CheckGroup(GROUP_TYPE::PLAYER, GROUP_TYPE::PLATFORM);

}

void Scene_01::Exit()
{
	DeleteAll();
	SoundMgr::GetInst()->Stop(SOUND_CHANNEL::SC_BGM);
	CollisionMgr::GetInst()->CheckReset();
}

void Scene_01::Update()
{
	Scene::Update();
	if (pObj->GetPos().y > Core::GetInst()->GetResolution().y)
	{
		pObj->SetPos(Vec2(Core::GetInst()->GetResolution().x / 20, Core::GetInst()->GetResolution().y / 3 - 50));
	}
	if (pObj->GetPos().x + 16 >= sPlatObj->GetPos().x &&
		pObj->GetPos().x - 16 <= sPlatObj->GetPos().x &&
		pObj->GetPos().y + 16 >= sPlatObj->GetPos().y &&
		pObj->GetPos().y - 16 <= sPlatObj->GetPos().y)
	{
		//ChangeScene(SCENE_TYPE::SCENE_02);
	}
}
