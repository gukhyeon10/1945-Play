#include "stdafx.h"
#include "Item.h"
#include "User.h"
#include "resource.h"

void CItem::Insert(float fDrop_X, float fDrop_Y)
{
	if (rand() % 10 > 4) // 60�ۼ�ƮȮ���� ������ ���
	{
		return;
	}
	
	Item *pItem = new Item;
	pItem->fX = fDrop_X;
	pItem->fY = fDrop_Y;
	if (pItem->fY < 0)
	{
		pItem->fY = 0;
	}
	
	//�������� ���� ���ϱ�
	if ((rand() % 10) % 2 == 0)
	{
		pItem->fMove_X = 1.0;
	}
	else
	{
		pItem->fMove_X = -1.0;
	}

	if ((rand() % 10) % 2 == 0)
	{
		pItem->fMove_Y = 1.0;
	}
	else
	{
		pItem->fMove_Y = -1.0;
	}

	pItem->Image_Step = 0;
	pItem->Next_Step_Gap = GetTickCount();

	//�������� ������ ���ϱ�
	int Item_Rand;
	Item_Rand = rand() % 13;

	if (Item_Rand >= 9)
	{
		pItem->Item_Name = HEART;
	}
	else if (Item_Rand >= 4)
	{
		pItem->Item_Name = POWER_UP;
	}
	else
	{
		pItem->Item_Name = BOMB;
	}

	Item_List.push_back(pItem);

	return;
}

void CItem::Move()
{
	int nImage_Rect_Width, nItem_Name;  // ��Ʈ���� ������ ���� , ������ �̸�
	for (list<Item*>::iterator iter = Item_List.begin(); iter != Item_List.end(); ++iter)
	{
		(*iter)->fX += (*iter)->fMove_X;   // �̵�
		(*iter)->fY += (*iter)->fMove_Y;
	
		switch ((*iter)->Item_Name)     //���ҽ� ����
		{
			case HEART:
			{
				nImage_Rect_Width = 7;
				nItem_Name = HEART;
			}
			break;
			case POWER_UP:
			{
				nImage_Rect_Width = 6;
				nItem_Name = POWER_UP;
			}
			break;
			case BOMB:
			{
				nImage_Rect_Width = 1;
				nItem_Name = BOMB;

			}
			break;
		}

		if((*iter)->Next_Step_Gap+50<GetTickCount())  //�Ų����� ���ҽ� �ִϸ��̼�
		{
		(*iter)->Image_Step++;
		(*iter)->Image_Step %= nImage_Rect_Width;
		(*iter)->Next_Step_Gap = GetTickCount();
		}

		if ((*iter)->fX + Image_Rect[nItem_Name].bmWidth / nImage_Rect_Width > CLIENT_WIDTH || (*iter)->fX < 0)  //������ ȭ��â ƨ���
		{
			(*iter)->fMove_X *= -1;
		}
		else if ((*iter)->fY + Image_Rect[nItem_Name].bmHeight > CLIENT_HEIGTH || (*iter)->fY < 0)
		{
			(*iter)->fMove_Y *= -1;
		}

	}
	return;
}

void CItem::Render(HDC MainDC)
{
	HDC ItemDC = CreateCompatibleDC(MainDC);
	for (list<Item*>::iterator iter = Item_List.begin(); iter != Item_List.end(); ++iter)
	{
		switch ((*iter)->Item_Name)
		{
			case HEART:
			{
				SelectObject(ItemDC, hHeart);
				TransparentBlt(MainDC, (*iter)->fX, (*iter)->fY, Image_Rect[HEART].bmWidth / 7, Image_Rect[HEART].bmHeight, ItemDC, ((*iter)->Image_Step)*Image_Rect[HEART].bmWidth / 7, 0, Image_Rect[HEART].bmWidth / 7, Image_Rect[HEART].bmHeight, RGB(255, 255, 255));
			}
			break;
			case POWER_UP:
			{
				SelectObject(ItemDC, hPower_Up);
				TransparentBlt(MainDC, (*iter)->fX, (*iter)->fY, Image_Rect[POWER_UP].bmWidth / 6, Image_Rect[POWER_UP].bmHeight , ItemDC, ((*iter)->Image_Step)*Image_Rect[POWER_UP].bmWidth / 6, 0, Image_Rect[POWER_UP].bmWidth / 6, Image_Rect[POWER_UP].bmHeight, RGB(0, 128, 128));
			}
			break;
			case BOMB:
			{
				SelectObject(ItemDC, hBomb);
				TransparentBlt(MainDC, (*iter)->fX, (*iter)->fY, Image_Rect[BOMB].bmWidth, Image_Rect[BOMB].bmHeight, ItemDC, 0, 0, Image_Rect[BOMB].bmWidth, Image_Rect[BOMB].bmHeight, RGB(255, 255, 255));
			} 
			break;
		}
	}

	DeleteDC(ItemDC);

	return;
}

void CItem::User_Get(CUser *pUser)
{
	float deltaX, deltaY, length, distance, nItem_Max; //���� �߽��� ������ ����, �߽ɰ��� �Ÿ�, �������� ��, ������ �ִ��
	bool bGet_Check= false;

	for (list<Item*>::iterator iter = Item_List.begin(); iter != Item_List.end(); ++iter)
	{
		deltaX = pUser->fX + 40;
		deltaY = pUser->fY + 40;
		//���� �߽ɰ��� �Ÿ� (�����۰� �������� �߽ɰ��� �Ÿ�)
		switch ((*iter)->Item_Name)     //���ҽ� ����
		{
		case HEART:
		{
			deltaX -= (*iter)->fX + 25;
			deltaY -= (*iter)->fY + 21;
			distance = 35;
			nItem_Max = 9;
		}
		break;
		case POWER_UP:
		{
			deltaX -= (*iter)->fX + 25;
			deltaY -= (*iter)->fY + 16;
			distance = 40;
			nItem_Max = 6;
		}
		break;
		case BOMB:
		{
			deltaX -= (*iter)->fX + 18;
			deltaY -= (*iter)->fY + 18;
			distance = 35;
			nItem_Max = 9;
		}
		break;
		}

		length = sqrt(deltaX * deltaX + deltaY * deltaY);
		if (length <= (distance))
		{
			if ((*iter)->Item_Name == HEART && pUser->Info.nLife < nItem_Max)  // ������ �ִ�ġ 9
			{
				pUser->Info.nLife++;
			}
			else if((*iter)->Item_Name == POWER_UP && pUser->Info.nPower < nItem_Max)
			{
				pUser->Info.nPower++;
			}
			else if ((*iter)->Item_Name == BOMB && pUser->Info.nSkill < nItem_Max)
			{
				pUser->Info.nSkill++;
			}
			else
			{
				pUser->Info.nScore += 100;
			}
			bGet_Check = true;
		}

		if (bGet_Check == true)
		{
			//������ ȹ��
			delete (*iter);
			iter = Item_List.erase(iter);
			return;
		}

	}
	return;
}

void CItem::Clear()
{
	for (list<Item*>::iterator iter = Item_List.begin(); iter != Item_List.end();)
	{
		delete (*iter);
		iter = Item_List.erase(iter);
	}
	return;
}

CItem::CItem(HINSTANCE hInst)
{
	hHeart = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HEART));
	GetObject(hHeart, sizeof(BITMAP), &Image_Rect[HEART]);

	hPower_Up = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_POWER_UP));
	GetObject(hPower_Up, sizeof(BITMAP), &Image_Rect[POWER_UP]);

	hBomb = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOMB));
	GetObject(hBomb, sizeof(BITMAP), &Image_Rect[BOMB]);
}


CItem::~CItem()
{
	Clear();
}
