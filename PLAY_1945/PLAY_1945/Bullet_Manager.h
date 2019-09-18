#pragma once
#define Enemy_Bullet_Width 20
#define Enemy_Bullet_Heigth 20
#define Enmey_Bullet_Half 8
#define User_Bullet_Width 10
#define User_Bullet_Heigth 24

class CEnemy_Manager;
class CUser;
class CEffect;
class CItem;

enum Bullet_Owner
{
	USER_BULLET=0,
	ENEMY_BULLET = 1,
};

typedef struct Bullet
{
	float x, y;
	float xx, yy;
	int Bullet_Number;
}Bullet;

class CBullet_Manager
{
private:
	list<Bullet*> Bullet_List;
	HBITMAP hEnemy_Bullet, hUser_Bullet;
public:
	void Bullet_Clear();
	void Bullet_Add(float fx, float fy, float fxx, float fyy, int nBullet_Number);
	void Move_Crash(CEnemy_Manager *pEnemy_Manager, CUser *pUser_Manager, CEffect *pEffect, CItem *pItem);
	void Explosion_Crash(float fDistance);
	void Paint(HDC MainDC);

	CBullet_Manager(HINSTANCE hinst);
	~CBullet_Manager();
};

