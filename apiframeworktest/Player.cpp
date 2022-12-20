#include "pch.h"
#include "Player.h"
#include "KeyMgr.h"
#include "TimeMgr.h"
#include "Bullet.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Image.h"
#include "PathMgr.h"
#include "ResMgr.h"
#include "Collider.h"
#include "Animator.h"
#include "Animation.h"

static bool isJump = false;
static bool isMove = false;
static bool isFall = false;
static bool isCanMove = true;
static int	collCount = 0;
static float m_dt = 0.0f;

Player::Player()
{
	// collider ����
	CreateCollider();
	GetCollider()->SetScale(Vec2(20.f, 20.f));

	// image ���ε�
	Image* pImg = ResMgr::GetInst()->ImgLoad(L"PlayerAni", L"Image\\Playerb.bmp");

	// animator ���� �� animation ���
	CreateAnimator();
	GetAnimator()->CreateAnimation(L"Idle", pImg, Vec2(0.f, 0.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 2, 0.5f);
	GetAnimator()->CreateAnimation(L"Walk", pImg, Vec2(0.f, 16.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 3, 0.25f);
	GetAnimator()->CreateAnimation(L"Jump", pImg, Vec2(0.f, 32.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 3, 0.1f);
	GetAnimator()->CreateAnimation(L"Land", pImg, Vec2(0.f, 48.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 3, 0.1f);
	GetAnimator()->CreateAnimation(L"Fall", pImg, Vec2(0.f, 64.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 1, 100.f);
	GetAnimator()->Play(L"Idle", true);

}
Player::~Player()
{
	//if(nullptr !=m_pImage)
	//	delete m_pImage;
}
void Player::EnterCollision(Collider* _pOther)
{
	collCount++;

	Vec2 vLeftPos = GetCollider()->GetFinalPos();
	Vec2 vRightPos = _pOther->GetFinalPos();
	Vec2 vLeftScale = GetCollider()->GetScale();
	Vec2 vRightScale = _pOther->GetScale();

	if (abs(vRightPos.y - vLeftPos.y) < (vLeftScale.y + vRightScale.y) / 2.1f)
	{
		isJump = false;
		isCanMove = false;
		collCount = 0;
	}
	else if (isFall)
	{
		GetAnimator()->Play(L"Land", true);
		isFall = false;
		m_dt = 0;
	}

}
void Player::ExitCollision(Collider* _pOther)
{
 	collCount--;
	if (!isCanMove) isCanMove = true;
}
void Player::StayCollision(Collider* _pOther)
{
}
void Player::Update()
{
	Vec2 vPos = GetPos();

	if (KEY_HOLD(KEY::LEFT) || KEY_HOLD(KEY::A)) 
	{
		if (isCanMove)
		{
			GetAnimator()->Play(L"Walk", true);
			isMove = true;
			vPos.x -= 200.f * fDT;
		}
	}
	else if (KEY_HOLD(KEY::RIGHT) || KEY_HOLD(KEY::D))
	{
		if (isCanMove)
		{
			GetAnimator()->Play(L"Walk", true);
			isMove = true;
			vPos.x += 200.f * fDT;
		}
	}
	else if(isMove && !isFall && !isJump)
	{
		isMove = false;
		GetAnimator()->Play(L"Idle", true);
	}

	if (KEY_TAP(KEY::SPACE) || KEY_TAP(KEY::W))
	{
		if (!isJump && !isFall)
		{
			isJump = true;
			GetAnimator()->Play(L"Jump", true);
		}
	}
	if (isJump)
	{
		m_dt += fDT;
		vPos.y -= 200.f * fDT + (0.3f-m_dt) * 0.2f;
		if (m_dt >= 0.3f)
		{
			m_dt = 0.0f;
			isJump = false;
			isFall = true;
			GetAnimator()->Play(L"Fall", true);
		}
	}
	if (isFall)
	{
		m_dt += fDT;
		vPos.y += m_dt * 2.f;
	}
	if (!isJump && collCount <= 0)
	{
		isFall = true;
		GetAnimator()->Play(L"Fall", true);
	}

	SetPos(vPos);
	GetAnimator()->Update();
}

void Player::Render(HDC _dc)
{
	Component_Render(_dc);
	/*int Width = (int)m_pImage->GetWidth();
	int Height = (int)m_pImage->GetHeight();

	Vec2 vPos = GetPos();*/
	//BitBlt(_dc
	//	,(int)(vPos.x - (float)(Width / 2))
	//	,(int)(vPos.y - (float)(Height / 2))
	//    , Width, Height
	//    , m_pImage->GetDC()
	//    , 0,0, SRCCOPY);

	//����Ÿ ���� ���� transparent: ������
	//TransparentBlt(_dc
	//	, (int)(vPos.x - (float)(Width / 2))
	//	, (int)(vPos.y - (float)(Height / 2))
	//	,Width, Height
	//    , m_pImage->GetDC()
	//    ,0,0, Width, Height
	//    , RGB(255,0,255));

}
