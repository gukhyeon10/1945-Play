#pragma once

#define MISSILE_WIDTH 50
#define MISSILE_HEIGHT 124
#define EXPLOSION_LENGTH 800

class CBullet_Manager;
class CEnemy_Manager;
class CEffect;
class CUser;

class CSpecial
{
private:
	DWORD Special_Motion_Gap;
	HBITMAP hMissile;
	HBITMAP hExplosion;
	BITMAP Missile_Rect;
	BITMAP Explosion_Rect;
	int nMissile_Step;
	float nExplosion_Step;
	float fMissile_X, fMissile_Y;
	bool bProgress;
public:
	void Progress(CUser *pUser, CBullet_Manager *pBullet_Manager, CEnemy_Manager *pEnemy_Manager, CEffect *pEffect);
	void Render(HDC MainDC);
	void Explosion_Crash(CBullet_Manager *pBullet_Manager, CEnemy_Manager *pEnemy_Manager, CEffect *pEffect, CUser *pUser);
	void Reset(CUser *pUser);
	CSpecial(HINSTANCE hInst);
	~CSpecial();
};

