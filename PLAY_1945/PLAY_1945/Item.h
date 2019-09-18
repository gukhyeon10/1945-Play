#pragma once

class CUser;

enum ITEM_NAME
{
	HEART=0,
	POWER_UP=1,
	BOMB=2,
	GOLD=3,
};

typedef struct Item
{
	float fX;
	float fY;
	float fMove_X;
	float fMove_Y;
	unsigned int Item_Name;
	unsigned int Image_Step;
	DWORD Next_Step_Gap;
}Item;

class CItem
{
private:
	HBITMAP hHeart;
	HBITMAP hPower_Up;
	HBITMAP hBomb;
	list<Item*> Item_List;
	BITMAP Image_Rect[3];
public:
	void Insert(float fDrop_X, float fDrop_Y);
	void Move();
	void Render(HDC MainDC);
	void User_Get(CUser *pUser);
	void Clear();
	CItem(HINSTANCE hInst);
	~CItem();
};

