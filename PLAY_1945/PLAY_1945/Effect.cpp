#include "stdafx.h"
#include "Effect.h"
#include "resource.h"

void CEffect::Insert(int x, int y, int nEffect_Number)
{
	if (nEffect_Number == BULLET_CRASH)
	{
		x += ((rand() % 12) - 7);  //유닛 중심을 기준으로 가로 세로 12, 7 범위내에서 총알에 맞는 이펙트 추가
		y += ((rand() % 7) - 3);  //
	}

	Effect *pEffect = new Effect;
	pEffect->Effect_Number = nEffect_Number;
	pEffect->Effect_Step = 0;
	pEffect->X = x;
	pEffect->Y = y;
	pEffect->Next_Step_Gap = GetTickCount();

	Effect_List.push_back(pEffect);

	return;
}

void CEffect::Paint(HDC hdc)
{
	HDC EffectDC = CreateCompatibleDC(hdc);

	for (list<Effect*>::iterator iter = Effect_List.begin(); iter != Effect_List.end();)
	{
		switch ((*iter)->Effect_Number)
		{
		case BULLET_CRASH:
		{
			
			if ((*iter)->Effect_Step < 15)   //15는 충돌이펙트 애니메이션 끝을 의미
			{
				
				SelectObject(EffectDC, hCrash_Effect);
				TransparentBlt(hdc, (*iter)->X, (*iter)->Y, Image_Rect[BULLET_CRASH].bmWidth / 5, Image_Rect[BULLET_CRASH].bmHeight / 3, EffectDC, ((*iter)->Effect_Step % 5)*Image_Rect[BULLET_CRASH].bmWidth / 5, ((*iter)->Effect_Step / 5)*Image_Rect[BULLET_CRASH].bmHeight / 3, Image_Rect[BULLET_CRASH].bmWidth / 5, Image_Rect[BULLET_CRASH].bmHeight / 3, RGB(0, 248, 0));
				(*iter)->Effect_Step++;
				
				++iter;
			}
			else
			{
				
				delete (*iter);
				iter = Effect_List.erase(iter);
			}
		}
		break;
		case ENEMY_EXPLOSION:
		{
			
			if ((*iter)->Effect_Step < 12)   //12는 피격이펙트 애니메이션 끝을 의미
			{

				SelectObject(EffectDC, hExplosion_Effect);
				TransparentBlt(hdc, (*iter)->X, (*iter)->Y, Image_Rect[ENEMY_EXPLOSION].bmWidth / 6, Image_Rect[ENEMY_EXPLOSION].bmHeight / 2, EffectDC, ((*iter)->Effect_Step % 6)*Image_Rect[ENEMY_EXPLOSION].bmWidth / 6, ((*iter)->Effect_Step / 6)*Image_Rect[ENEMY_EXPLOSION].bmHeight / 2, Image_Rect[ENEMY_EXPLOSION].bmWidth / 6, Image_Rect[ENEMY_EXPLOSION].bmHeight / 2, RGB(0, 248, 0));
				if ((*iter)->Next_Step_Gap + 50 < GetTickCount())
				{
					(*iter)->Effect_Step++;
					(*iter)->Next_Step_Gap = GetTickCount();
				}
				
				++iter;
			}
			else
			{

				delete (*iter);
				iter = Effect_List.erase(iter);
			}
		}
		break;
		case BOSS_EXPLOSION:
		{
			if ((*iter)->Effect_Step < 12)   //12는 피격이펙트 애니메이션 끝을 의미
			{

				SelectObject(EffectDC, hBoss_Explosion_Effect);
				TransparentBlt(hdc, (*iter)->X, (*iter)->Y, Image_Rect[BOSS_EXPLOSION].bmWidth / 2, Image_Rect[BOSS_EXPLOSION].bmHeight / 6, EffectDC, ((*iter)->Effect_Step % 2)*Image_Rect[BOSS_EXPLOSION].bmWidth / 2, ((*iter)->Effect_Step / 2)*Image_Rect[BOSS_EXPLOSION].bmHeight / 6, Image_Rect[BOSS_EXPLOSION].bmWidth / 2, Image_Rect[BOSS_EXPLOSION].bmHeight / 6, RGB(0, 248, 0));
				if ((*iter)->Next_Step_Gap + 30 < GetTickCount())
				{
					(*iter)->Effect_Step++;
					(*iter)->Next_Step_Gap = GetTickCount();
				}

				++iter;
			}
			else
			{

				delete (*iter);
				iter = Effect_List.erase(iter);
			}
		}
		break;
		default:
		{
			++iter;
		}
		break;
		}
	}

	DeleteDC(EffectDC);

	return;
}

void CEffect::Boss_Clear_Effect()
{
	Insert(150, 0, BOSS_EXPLOSION);
	return;
} 

void CEffect::Clear()
{
	for (list<Effect*>::iterator iter = Effect_List.begin(); iter != Effect_List.end();)
	{
		delete (*iter);
		iter = Effect_List.erase(iter);
	}


	return;
}

CEffect::CEffect(HINSTANCE hInst)
{
	hCrash_Effect = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_CRASH_EFFECT));
	GetObject(hCrash_Effect, sizeof(BITMAP), &Image_Rect[BULLET_CRASH]);

	hExplosion_Effect = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_EXPLOSION_EFFECT));
	GetObject(hExplosion_Effect, sizeof(BITMAP), &Image_Rect[ENEMY_EXPLOSION]);

	hBoss_Explosion_Effect = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS_EXPLOSION));
	GetObject(hBoss_Explosion_Effect, sizeof(BITMAP), &Image_Rect[BOSS_EXPLOSION]);
}


CEffect::~CEffect()
{
	Clear();
}
