#include "pch.h"
#include "PlatformObj.h"
#include "TimeMgr.h"
#include "Collider.h"
#include "Image.h"
#include "ResMgr.h"

PlatformObj::PlatformObj()
	: m_vCenterPos(Vec2(0.f, 0.f))
{
	// �̰� ����� �̹��� �����ϱ� ��������
	Image* pImg = ResMgr::GetInst()->ImgLoad(L"Brick", L"Image\\Brick.bmp"); // �縷 �������� ���� ���� �ϳ� �׷��� ��������
	CreateCollider();
	GetCollider()->SetScale(Vec2(40.f, 40.f));
}

PlatformObj::~PlatformObj()
{

}

void PlatformObj::Update()
{

}

void PlatformObj::EnterCollision(Collider* _pOther)
{

}