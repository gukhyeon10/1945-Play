#include "stdafx.h"
#include "Special.h"
#include "Enemy_Manager.h"
#include "Bullet_Manager.h"
#include "Effect.h"
#include "User.h"
#include "resource.h"

void CSpecial::Progress(CUser *pUser, CBullet_Manager *pBullet_Manager, CEnemy_Manager *pEnemy_Manager, CEffect *pEffect)
{
	if (bProgress == true && nExplosion_Step == -1)    // ���濡 �̻����� ���ư���
	{
		if (nMissile_Step == 26)
		{
			nMissile_Step = 0;
			nExplosion_Step = 0;
		}
		else if (Special_Motion_Gap + 30 < GetTickCount())
		{
			nMissile_Step++;
			Special_Motion_Gap = GetTickCount();
		}
		
		fMissile_Y -= 6;
		return;
	}
	else if (bProgress == true && nExplosion_Step >= 0)     // �̻����� �����Ҷ�
	{
		Explosion_Crash(pBullet_Manager, pEnemy_Manager, pEffect, pUser);

		if (nExplosion_Step < 13)
		{
			nExplosion_Step+=0.25;
		}
		else
		{
			//�̺κ��� ���� �ٳ����� �ʱ�ȭ
			nExplosion_Step = -1;
			fMissile_Y = CLIENT_HEIGTH;
			pUser->Info.bSpecial_Ing = false;
			bProgress = false;
		}
		return;
	}
	
	if (pUser->Info.bSpecial_Ing == true)    //   �ʻ�� ������
	{
			bProgress = true;
	}

	return;
}

void CSpecial::Render(HDC MainDC)
{
	if(bProgress == true)
	{
		if (nExplosion_Step == -1)
		{
			HDC MissileDC = CreateCompatibleDC(MainDC);
			SelectObject(MissileDC, hMissile);
			TransparentBlt(MainDC, fMissile_X, fMissile_Y, Missile_Rect.bmWidth / 9, Missile_Rect.bmHeight / 3, MissileDC, (nMissile_Step % 9) * MISSILE_WIDTH, (nMissile_Step / 9) * MISSILE_HEIGHT, Missile_Rect.bmWidth / 9, Missile_Rect.bmHeight / 3, RGB(248, 0, 248));
	
			DeleteDC(MissileDC);
		}
		else
		{
			
			HDC ExplosionDC = CreateCompatibleDC(MainDC);
			SelectObject(ExplosionDC, hExplosion);
			TransparentBlt(MainDC, 0, 50, EXPLOSION_LENGTH, EXPLOSION_LENGTH, ExplosionDC, ((int)nExplosion_Step % 4) * EXPLOSION_LENGTH, ((int)nExplosion_Step / 4) * EXPLOSION_LENGTH, EXPLOSION_LENGTH, EXPLOSION_LENGTH,  RGB(0, 248, 0));
			DeleteDC(ExplosionDC);
		}
	}

	return;
}

void CSpecial::Explosion_Crash(CBullet_Manager *pBullet_Manager, CEnemy_Manager *pEnemy_Manager, CEffect *pEffect, CUser *pUser)
{
	float Distance;
	//������ �߽��� ��ǥ�� 400, 450
	//���⼭ distance ���ϰ� �������� �Ѿ� �Ҹ�, �� ������ 2���� ����?  ����Ʈ �߰��ϰ�
	switch ((int)nExplosion_Step)
	{
	case 0:    //  ���߿� ������ 90
	{
		Distance = 90;
	}
	break;
	case 1:    //  ���߿� ������ 100
	{
		Distance = 100;
	}
	break;
	case 2:    //  ���߿� ������ 110
	{
		Distance = 110;
	}
	break;
	case 3:    // ���߿� ������  142
	{
		Distance = 142;
	}
	break;
	case 4:    //���߿� ������ 182
	{
		Distance = 182;
	}
	break;
	case 5:   //���߿� ������  240
	{
		Distance = 240;
	}
	break;
	case 6:   //���߿� ������ 275
	{
		Distance = 275;
	}
	break;
	case 7:   //���߿� ������ 350
	{
		Distance = 350;
	}
	break;
	case 8: // ���߿� ������  375
	{
		Distance = 375;
	}
	break;
	default: // ���߿� ������ 400
	{
		Distance = 400;
	}
	break;
	}

	pBullet_Manager->Explosion_Crash(Distance);
	pEnemy_Manager->Explosion_Crash(pEffect, Distance, pUser);

	return;
}

void CSpecial::Reset(CUser *pUser)
{
	nExplosion_Step = -1;
	fMissile_Y = CLIENT_HEIGTH;
	pUser->Info.bSpecial_Ing = false;
	bProgress = false;
	return;
}

CSpecial::CSpecial(HINSTANCE hInst)
{
	bProgress = false;

	nMissile_Step = 0;
	nExplosion_Step = -1;
	Special_Motion_Gap = GetTickCount();

	fMissile_X = CLIENT_WIDTH / 2 - 30;
	fMissile_Y = CLIENT_HEIGTH;

	hMissile = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOMB_MOTION));
	GetObject(hMissile, sizeof(BITMAP), &Missile_Rect);
	hExplosion = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EXPLOSION));
	GetObject(hExplosion, sizeof(BITMAP), &Explosion_Rect);

}


CSpecial::~CSpecial()
{
}
