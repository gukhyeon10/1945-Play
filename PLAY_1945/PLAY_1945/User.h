#pragma once

#include "Bullet_Manager.h"
#include "Enemy_Manager.h"
#include "Effect.h"
#include "Special.h"

#define VK_S 0x53
#define VK_W 0x57

enum POWER
{
	ONE_STEP= 1,
	TWO_STEP =2, 
	THREE_STEP =3,
	FOUR_STEP = 4,
	FIVE_STEP =5,
	SIX_STEP =6,
};

typedef struct User_Info
{
	int nPower;
	int nSkill;
	int nLife;
	int nScore;
	bool bBang;
	bool bSpecial_Ing;
}User_Info;

class CUser
{
public:
	int nStage;
	User_Info Info;
	float fX, fY;
private:
	DWORD Bullet_Gap;
	DWORD Invincible_Time;
	HBITMAP hUser, hUser_Invincible;
	BITMAP Unit_Rect;

public:
	void Paint(HDC MainDC);
	void Action();
	void Fire(CBullet_Manager *pBullet_Manager);
	bool Crash_Check(float Bullet_X, float Bullet_Y, float Bullet_Half);
	CUser(HINSTANCE hInst);
	~CUser();
};

