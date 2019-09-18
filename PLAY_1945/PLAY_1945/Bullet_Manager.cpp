#include "stdafx.h"
#include "Bullet_Manager.h"
#include "Enemy_Manager.h"
#include "User.h"
#include "Effect.h"
#include "Item.h"
#include "resource.h"

void CBullet_Manager::Bullet_Add(float fx, float fy, float fxx, float fyy, int nBullet_Number)
{
	Bullet *pBullet = new Bullet;
	pBullet->x = fx;
	pBullet->y = fy;
	pBullet->xx = fxx;
	pBullet->yy = fyy;
	pBullet->Bullet_Number= nBullet_Number;
	Bullet_List.push_back(pBullet);
	return;
}

void CBullet_Manager::Move_Crash(CEnemy_Manager *pEnemy_Manager, CUser *pUser_Manager, CEffect *pEffect, CItem *pItem_Manager)
{
	bool bCrash_Succes;
	int Bullet_Width, Bullet_Heigth;

	for (list<Bullet*>::iterator iter = Bullet_List.begin(); iter != Bullet_List.end();)
	{
		switch ((*iter)->Bullet_Number)
		{
		case USER_BULLET:
		{
			Bullet_Width= User_Bullet_Width;
			Bullet_Heigth= User_Bullet_Heigth;
		}
		break;
		case ENEMY_BULLET:
		{
			Bullet_Width = Enemy_Bullet_Width;
			Bullet_Heigth = Enemy_Bullet_Heigth;
		}
		break;
		}

		bCrash_Succes = false;
		if ((*iter)->x <= CLIENT_WIDTH && (*iter)->x + Bullet_Width >= 0 && (*iter)->y + Bullet_Heigth >= 0 && (*iter)->y <= CLIENT_HEIGTH)
		{
			switch((*iter)->Bullet_Number)
			{
			case USER_BULLET:
			{
				bCrash_Succes=pEnemy_Manager->Crash_Check((*iter)->x+5, (*iter)->y+12, pUser_Manager, pEffect, pItem_Manager); //충돌체크 총알의 중심 좌표를 넘겨준다.
			}
			break;
			case ENEMY_BULLET:
			{
				bCrash_Succes=pUser_Manager->Crash_Check((*iter)->x+10, (*iter)->y+10, Enmey_Bullet_Half); //충돌체크 총알의 중심 좌표와 총알의 반지름을 넘겨준다.
			}
			break;
			}

			if (bCrash_Succes == false)
			{
				(*iter)->x += (*iter)->xx;
				(*iter)->y += (*iter)->yy;
				++iter;
			}
			else
			{
				delete (*iter);
				iter = Bullet_List.erase(iter);
			}

		}
		else
		{
			delete (*iter);
			iter = Bullet_List.erase(iter);
		}
	}
	return;
}

void CBullet_Manager::Explosion_Crash(float fDistance)
{
	float DeltaX, DeltaY;
	float Circle_Center_X = 400;
	float Circle_Center_Y = 450;
	float Length;

	for (list<Bullet*>::iterator iter = Bullet_List.begin(); iter != Bullet_List.end();)
	{
		if ((*iter)->Bullet_Number == ENEMY_BULLET)
		{
			DeltaX = Circle_Center_X - (*iter)->x + 10;
			DeltaY = Circle_Center_Y - (*iter)->y + 10;

			Length = sqrt(DeltaX * DeltaX + DeltaY *DeltaY);
			
			if (Length < fDistance+ Enmey_Bullet_Half)
			{
				delete (*iter);
				iter = Bullet_List.erase(iter);
			}
			else
			{
				++iter;
			}
		}
		else
		{
			++iter;
		}
	}

	return;
}

void CBullet_Manager::Paint(HDC MainDC)
{
	HDC MemDC = CreateCompatibleDC(MainDC);
	
	for (list<Bullet*>::iterator iter = Bullet_List.begin(); iter != Bullet_List.end(); ++iter)
	{
		switch ((*iter)->Bullet_Number)
		{
		case USER_BULLET:
		{
			SelectObject(MemDC, hUser_Bullet);
			TransparentBlt(MainDC, (*iter)->x, (*iter)->y, User_Bullet_Width, User_Bullet_Heigth, MemDC, 0, 0, User_Bullet_Width, User_Bullet_Heigth, RGB(0, 0, 0));

		}
		break;
		case ENEMY_BULLET:
		{
			SelectObject(MemDC, hEnemy_Bullet);
			TransparentBlt(MainDC, (*iter)->x, (*iter)->y, Enemy_Bullet_Width, Enemy_Bullet_Heigth, MemDC, 0, 0, Enemy_Bullet_Width, Enemy_Bullet_Heigth, RGB(255, 255, 255));
		}
		break;
		}
	}

	DeleteDC(MemDC);
	return;
}

void CBullet_Manager::Bullet_Clear()
{
	for (list<Bullet*>::iterator iter = Bullet_List.begin(); iter != Bullet_List.end();)
	{
		delete (*iter);
		iter = Bullet_List.erase(iter);
	}
	return;
}

CBullet_Manager::CBullet_Manager(HINSTANCE hinst)
{
	hEnemy_Bullet = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_ENEMY_BULLET));
	hUser_Bullet = LoadBitmap(hinst, MAKEINTRESOURCE(IDB_USER_BULLET));

}


CBullet_Manager::~CBullet_Manager()
{
	Bullet_Clear();

}
