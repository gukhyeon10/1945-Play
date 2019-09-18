#pragma once

class CUser;
class CBullet_Manager;
class CEffect;
class CItem;

typedef struct Enemy
{
	int nUnit_Number;
	float fLocation_X, fLocation_Y; // 출현 시작점
	float fCenter_X, fCenter_Y; // 이동패턴의 중심좌표
	float a; // 속도
	DWORD reload; // 발사 텀
	unsigned int nShoot_Count;
	unsigned int nMove_Angle;
	unsigned int nMove_Pattern;
	unsigned int nShoot_Pattern;
	float nHp;
	unsigned int nBoss_Pattern;
}Enemy;

class CEnemy_Manager
{
private:
	int Unit_Hp_Rank[9];
	CBullet_Manager *pBullet_Manager;
	HBITMAP hUnit[9];
	list<Enemy*> Enemy_List;
	bool bBoss_Die;
public:
	void Fire(list<Enemy*>::iterator unit);
	void Load(const char* File_Path, unsigned int nStage);
	void Render(HDC hdc);
	void Move();
	bool Crash_Check(float Bullet_X, float Bullet_Y, CUser *pUser_Manager, CEffect *pEffect, CItem *pItem_Manager);
	void Explosion_Crash(CEffect *pEffect, float fDistance, CUser *pUser);
	bool Boss_Kill();
	void Plus_Score(CUser *pUser_Manager, int nUnit_Number);
	void Clear();
	CEnemy_Manager(HINSTANCE hInst, CBullet_Manager *Bullet_Manager);
	~CEnemy_Manager();
};

