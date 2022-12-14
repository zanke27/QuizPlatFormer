#pragma once
#include "Object.h"
class Image;
class PlatformObj :
	public Object
{
private:
	Image* m_platformImage;
	Vec2    m_vCenterPos;
public:
	void SetCenterPos(Vec2 _vPos) { m_vCenterPos = _vPos; }
	void SetImageToNum(int num);
public:
	virtual void Update() override;
	virtual void Render(HDC _dc) override;
	virtual void	EnterCollision(Collider* _pOther);
	CLONE(PlatformObj);
public:
	PlatformObj();
	~PlatformObj();
};

