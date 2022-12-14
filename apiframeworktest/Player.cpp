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

static bool isLeft = false;
static bool isJump = false;
static bool isMove = false;
static bool isFall = false;
static bool isCanMoveL = true;
static bool isCanMoveR = true;
static int	collCount = 0;
static float m_dt = 0.0f;
static float m_fdt = 0.2f;
static Vec2 vPos;
static Image* pImg;

Player::Player()
{
	// collider 새성
	CreateCollider();
	GetCollider()->SetScale(Vec2(16.f, 16.f));

	// image 업로드
	pImg = ResMgr::GetInst()->ImgLoad(L"PlayerAni", L"Image\\Playerb.bmp");

	// animator 생성 및 animation 사용
	CreateAnimator();
	GetAnimator()->CreateAnimation(L"IdleL", pImg, Vec2(0.f, 0.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 2, 0.5f);
	GetAnimator()->CreateAnimation(L"WalkL", pImg, Vec2(0.f, 16.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 3, 0.25f);
	GetAnimator()->CreateAnimation(L"JumpL", pImg, Vec2(0.f, 32.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 3, 0.1f);
	GetAnimator()->CreateAnimation(L"LandL", pImg, Vec2(0.f, 48.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 3, 0.1f);
	GetAnimator()->CreateAnimation(L"FallL", pImg, Vec2(0.f, 64.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 1, 100.f);
	GetAnimator()->CreateAnimation(L"IdleR", pImg, Vec2(0.f, 80.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 2, 0.5f);
	GetAnimator()->CreateAnimation(L"WalkR", pImg, Vec2(0.f, 96.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 3, 0.25f);
	GetAnimator()->CreateAnimation(L"JumpR", pImg, Vec2(0.f, 112.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 3, 0.1f);
	GetAnimator()->CreateAnimation(L"LandR", pImg, Vec2(0.f, 128.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 3, 0.1f);
	GetAnimator()->CreateAnimation(L"FallR", pImg, Vec2(0.f, 144.f), Vec2(16.f, 16.f), Vec2(16.f, 0.f), 1, 100.f);

	GetAnimator()->Play(L"IdleR", true);
	collCount = 0;

}
Player::~Player()
{
	//if(nullptr !=m_pImage)
	//	delete m_pImage;
}
void Player::EnterCollision(Collider* _pOther)
{
	Vec2 vLeftPos = GetCollider()->GetFinalPos();
	Vec2 vRightPos = _pOther->GetFinalPos();
	Vec2 vLeftScale = GetCollider()->GetScale();
	Vec2 vRightScale = _pOther->GetScale();

	if (abs(vRightPos.y - vLeftPos.y) < (vLeftScale.y + vRightScale.y) / 2.25f || vRightPos.y - vLeftPos.y <= 0.01f)
	{
		isJump = false;
		isFall = true;

		if (isLeft)
			isCanMoveL = false;
		else
			isCanMoveR = false;
	}
	else if (isFall)
	{
		if(isLeft)
			GetAnimator()->Play(L"LandL", true);
		else
			GetAnimator()->Play(L"LandR", true);
		isFall = false;
		m_dt = 0;
	}
	collCount++;
}
void Player::ExitCollision(Collider* _pOther)
{
 	collCount--;
	if (!isCanMoveL) isCanMoveL = true;
	if (!isCanMoveR) isCanMoveR = true;
}
void Player::StayCollision(Collider* _pOther)
{      
}
void Player::Update()
{
	vPos = GetPos();

	if (KEY_HOLD(KEY::LEFT) || KEY_HOLD(KEY::A)) 
	{
		if (isCanMoveL)
		{
			GetAnimator()->Play(L"WalkL", true);
			isLeft = true;
			isMove = true;
			vPos.x -= 200.f * fDT;
		}
	}
	else if (KEY_HOLD(KEY::RIGHT) || KEY_HOLD(KEY::D))
	{
		if (isCanMoveR)
		{
			GetAnimator()->Play(L"WalkR", true);
			isLeft = false;
			isMove = true;
			vPos.x += 200.f * fDT;
		}
	}
	else if(isMove && !isFall && !isJump)
	{
		isMove = false;
		if(isLeft)
			GetAnimator()->Play(L"IdleL", true);
		else
			GetAnimator()->Play(L"IdleR", true);
	}

	if (KEY_TAP(KEY::SPACE) || KEY_TAP(KEY::W))
	{
		if (!isJump && !isFall)
		{
			isJump = true;
			if(isLeft)
				GetAnimator()->Play(L"JumpL", true);
			else
				GetAnimator()->Play(L"JumpR", true);
		}
	}
	if (isJump)
	{
		m_dt += fDT;
		vPos.y -= 208.f * fDT + (0.2f-m_dt) * 0.4f;
		if (m_dt >= 0.3f)
		{
			m_dt = 0.0f;
			isJump = false;
			isFall = true;
			if(isLeft)
				GetAnimator()->Play(L"FallL", true);
			else
				GetAnimator()->Play(L"FallR", true);
		}
	}
	if (isFall)
	{
		m_dt += fDT;
		vPos.y += m_dt * 4.f;
		if (m_dt >= 0.4f)
			m_dt = 0.4f;
	}
	if (!isJump && collCount <= 0)
	{
		isFall = true;
		if(isLeft)
			GetAnimator()->Play(L"FallL", true);
		else
			GetAnimator()->Play(L"FallR", true);
	}

	SetPos(vPos);
	GetAnimator()->Update();
}

void Player::Render(HDC _dc)
{
	Component_Render(_dc);

	//POINT dot[3] = {
	//	GetCollider()->GetFinalPos().x, 
	//	GetCollider()->GetFinalPos().x + GetCollider()->GetScale().x, 
	//	GetCollider()->GetFinalPos().x + GetCollider()->GetScale().y};
	//// 현재 위치.

	//// 회전 행렬 계산
	//for (int i = 0; i < 3; i++)
	//{
	//	dot[i].x = dot[i].x * cos(test) - dot[i].y * sin(test);
	//    dot[i].y = dot[i].x * sin(test) + dot[i].y * cos(test);
	//}
	//
	//PlgBlt(_dc, dot, )

	// 회전된 것으로 렌더링
	//int Width = (int)pImg->GetWidth();
	//int Height = (int)pImg->GetHeight();

	//BitBlt(_dc
	//	,(int)(vPos.x - (float)(Width / 2))
	//	,(int)(vPos.y - (float)(Height / 2))
	//    , Width, Height
	//    , pImg->GetDC()
	//    , 0,0, SRCCOPY);

	////마젠타 색상 뺄때 transparent: 투명한
	//TransparentBlt(_dc
	//	, (int)(vPos.x - (float)(Width / 2))
	//	, (int)(vPos.y - (float)(Height / 2))
	//	,Width, Height
	//    , pImg->GetDC()
	//    ,0,0, Width, Height
	//    , RGB(255,255,255));

}
