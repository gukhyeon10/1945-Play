#include "stdafx.h"
#include "User.h"
#include "resource.h"
#include "Play.h"

void CUser::Action()
{
	if (Info.bBang == true)
	{
		//피격당해서 몇초간 무적인상태
		if (Invincible_Time + 500 > GetTickCount())
		{
			fY -= 1.7;
			return;
		}
		else if(Invincible_Time + 2000 < GetTickCount())
		{
			Info.bBang = false;
		}
	}

	if(GetAsyncKeyState(VK_UP))
	{
		fY-=1.7;
		if (fY < -10)
		{
			fY = -10;
		}
	}
	if (GetAsyncKeyState(VK_DOWN))
	{
		fY+=1.7;
		if (fY + Unit_Rect.bmHeight > CLIENT_HEIGTH)
		{
			fY = CLIENT_HEIGTH - Unit_Rect.bmHeight;
		}
	}
	if (GetAsyncKeyState(VK_LEFT))
	{
		fX-=1.7;
		if (fX < 0)
		{
			fX = 0;
		}
	}
	if (GetAsyncKeyState(VK_RIGHT))
	{
		fX+=1.7;
		
		if (fX + Unit_Rect.bmWidth > CLIENT_WIDTH)
		{
			fX = CLIENT_WIDTH - Unit_Rect.bmWidth;
		}
	}
	return;
}

void CUser::Fire(CBullet_Manager *pBullet_Manager)
{
	if (GetAsyncKeyState(VK_S) && Bullet_Gap + 100 <GetTickCount())
	{
		switch (Info.nPower)
		{
		case ONE_STEP: //파워가 1일때
		{
			Bullet_Gap = GetTickCount();
			pBullet_Manager->Bullet_Add(fX+30, fY, 0, -2, USER_BULLET);
		}
		break;
		case TWO_STEP: //파워가 2일때
		{
			Bullet_Gap = GetTickCount() - 30;
			pBullet_Manager->Bullet_Add(fX + 30, fY, 0, -2, USER_BULLET);
		}
		break;
		case THREE_STEP: // 파워가 3일때
		{
			Bullet_Gap = GetTickCount();
			pBullet_Manager->Bullet_Add(fX + 30, fY, 0, -2, USER_BULLET);
			pBullet_Manager->Bullet_Add(fX + 50, fY, 0, -2, USER_BULLET);
		}
		break;
		case FOUR_STEP:
		{
			Bullet_Gap = GetTickCount() - 30;
			pBullet_Manager->Bullet_Add(fX + 30, fY, 0, -2, USER_BULLET);
			pBullet_Manager->Bullet_Add(fX + 50, fY, 0, -2, USER_BULLET);
		}
		break;
		case FIVE_STEP:
		{
			Bullet_Gap = GetTickCount();
			pBullet_Manager->Bullet_Add(fX + 30, fY, 0, -2, USER_BULLET);
			pBullet_Manager->Bullet_Add(fX + 40, fY, 0, -2, USER_BULLET);
			pBullet_Manager->Bullet_Add(fX + 50, fY, 0, -2, USER_BULLET);
		}
		break;
		case SIX_STEP:
		{
			Bullet_Gap = GetTickCount() - 30;
			pBullet_Manager->Bullet_Add(fX + 30, fY, 0, -2, USER_BULLET);
			pBullet_Manager->Bullet_Add(fX + 40, fY, 0, -2, USER_BULLET);
			pBullet_Manager->Bullet_Add(fX + 50, fY, 0, -2, USER_BULLET);
		}
		break;
		}
	}

	if (GetAsyncKeyState(VK_W) && Info.bSpecial_Ing == false && Info.nSkill > 0)
	{
		Info.bSpecial_Ing = true;
		Info.nSkill--;
	}
	return;
}


bool CUser::Crash_Check(float Bullet_X, float Bullet_Y, float Bullet_Half)
{
	float deltaX = (fX+40)-Bullet_X;
	float deltaY = (fY+40) - Bullet_Y;

	//원의 중심간의 거리
	float Length= sqrt(deltaX*deltaX + deltaY*deltaY);

	//총알과 유저유닛의 반지름의 합
	float User_Bullet_Distance= 16+Bullet_Half;

	if (Length < User_Bullet_Distance)
	{
		if (Info.bBang == true)
		{
			return true; //뱅상태(무적상태) 이면 총알만 소멸되고 유저유닛은 멀쩡
		}

		Info.nLife--;
		if (Info.nPower >= 2)
		{
			Info.nPower--;
		}
		Info.bBang = true;

		fY = CLIENT_HEIGTH + 3.4;
		fX = 360;
		Invincible_Time = GetTickCount();
		return true;
	}

	return false;
}

void CUser::Paint(HDC MainDC)
{
	HDC UnitDC = CreateCompatibleDC(MainDC);

	if (Info.bBang == false) 
	{
		SelectObject(UnitDC, hUser);
		TransparentBlt(MainDC, fX, fY, Unit_Rect.bmWidth, Unit_Rect.bmHeight, UnitDC, 0, 0, Unit_Rect.bmWidth, Unit_Rect.bmHeight, RGB(0, 0, 0));
	}
	else //무적상태일때는 반투명하게하려했으나 잘 안되서 무지개색으로 도색 ㅋㅋ
	{
		SelectObject(UnitDC, hUser_Invincible);
		TransparentBlt(MainDC, fX, fY, Unit_Rect.bmWidth, Unit_Rect.bmHeight, UnitDC, 0, 0, Unit_Rect.bmWidth, Unit_Rect.bmHeight, RGB(0, 0, 0));
	}

	DeleteDC(UnitDC);
	return;
}

CUser::CUser(HINSTANCE hInst)
{
	fX = 360;
	fY = 850;
	Info.nLife = 3;
	Info.nPower = 1;
	Info.nSkill = 4;
	Info.nScore = 0;
	Info.bBang = false;
	Info.bSpecial_Ing = false;
	nStage = 1; //1스테이지부터 시작
	Bullet_Gap = GetTickCount();
	
	hUser = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_USER1));
	hUser_Invincible = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_USER_INVINCIBLE));
	GetObject(hUser, sizeof(BITMAP), &Unit_Rect);
}


CUser::~CUser()
{

}
