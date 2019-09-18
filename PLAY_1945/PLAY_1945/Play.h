#pragma once
#include "User.h"

class CInterface;
class CEnemy_Manager;
class CBullet_Manager;
class CCrash;
class CEffect;
class CItem;
class CSpecial;

class CPlay
{
public:
	static int nMap_Y;
private:
	HDC hdc;
	HWND hwnd;
	HINSTANCE hinst;
	CInterface *pInterface;
	CEffect *pEffect;
	CEnemy_Manager *pEnemy_Manager;
	CBullet_Manager *pBullet_Manager;
	CItem *pItem_Manager;
	CSpecial *pSpecial;
	CUser *pUser;
public:
	void Stage_Load();
	bool Progress();
	void Paint();
	void Game_Clear_Show();
	void Game_Over_Check();
	void Class_Link(CInterface *Interface, CEnemy_Manager *Enemy_Manager, CBullet_Manager *Bullet_Manager, CEffect *Effect_Manager, CItem *Item_Manager, CSpecial *Special);
	CPlay(HWND hWnd, HINSTANCE hInst);
	~CPlay();
};

