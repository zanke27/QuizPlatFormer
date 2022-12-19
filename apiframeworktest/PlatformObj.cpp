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
	m_platformImage = ResMgr::GetInst()->ImgLoad(L"Brick", L"Image\\Brick.bmp");
	CreateCollider();
	GetCollider()->SetScale(Vec2(16.f, 16.f));
}

PlatformObj::~PlatformObj()
{

}

void PlatformObj::Update()
{

}

void PlatformObj::Render(HDC _dc)
{
	int Width = (int)m_platformImage->GetWidth();
	int Height = (int)m_platformImage->GetHeight();

	Vec2 vPos = GetPos();
	//����Ÿ ���� ���� transparent: ������
	TransparentBlt(_dc
		, (int)(vPos.x - (float)(Width / 2))
		, (int)(vPos.y - (float)(Height / 2))
		, Width, Height
		, m_platformImage->GetDC()
		, 0, 0, Width, Height
		, RGB(255, 0, 255));
	Component_Render(_dc);
}

void PlatformObj::EnterCollision(Collider* _pOther)
{

}