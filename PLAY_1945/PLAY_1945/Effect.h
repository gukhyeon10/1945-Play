#pragma once

enum EFFECT_NAME
{
	BULLET_CRASH=0,
	ENEMY_EXPLOSION=1,
	SKILL=2,
	BOSS_EXPLOSION=3,
};

typedef struct Effect
{
	int X;
	int Y;
	unsigned int Effect_Number;
	unsigned int Effect_Step;
	DWORD Next_Step_Gap;
}Effect;

class CEffect
{
private:
	HBITMAP hCrash_Effect;
	HBITMAP hExplosion_Effect;
	HBITMAP hBoss_Explosion_Effect;
	list<Effect*> Effect_List;
	BITMAP Image_Rect[4];
public:
	void Insert(int x, int y, int nEffect_Number);
	void Paint(HDC hdc);
	void Boss_Clear_Effect();
	void Clear();
	CEffect(HINSTANCE hInst);
	~CEffect();
};

