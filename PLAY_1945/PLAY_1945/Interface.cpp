#include "stdafx.h"
#include "Interface.h"
#include "Play.h"
#include "resource.h"
#include "tinyxml2.h"

using namespace tinyxml2;

int CInterface::Start_Menu(HDC hdc)
{
	if (GetAsyncKeyState(VK_RETURN) & 0x0001)
	{
		switch (nSelect_Number)
		{
		case START:
		{
			return START;
		}
		break;
		case HELP:
		{
			return HELP;
		}
		break;
		case QUIT:
		{
			return QUIT;
		}
		break;
		}
	}

	if (GetAsyncKeyState(VK_DOWN) & 0x0001)
	{
		nSelect_Number++;
		if (nSelect_Number == 4)
		{
			nSelect_Number = 1;
		}
	}
	if (GetAsyncKeyState(VK_UP) & 0x0001)
	{
		nSelect_Number--;
		if (nSelect_Number == 0)
		{
			nSelect_Number = 3;
		}

	}
	HDC MainDC = CreateCompatibleDC(hdc);
	HBITMAP hMainBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_HEIGTH);
	SelectObject(MainDC, hMainBitmap);

	BITMAP Image_Rect;

	HDC BackDC = CreateCompatibleDC(hdc);

	SelectObject(BackDC, hInterface_Image[START_MENU_BACK_IMAGE]);
	GetObject(hInterface_Image[START_MENU_BACK_IMAGE], sizeof(BITMAP), &Image_Rect);
	StretchBlt(MainDC, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, BackDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, SRCCOPY);

	SelectObject(BackDC, hInterface_Image[START]);
	GetObject(hInterface_Image[START], sizeof(BITMAP), &Image_Rect);
	TransparentBlt(MainDC, 200, 200, Image_Rect.bmWidth, Image_Rect.bmHeight, BackDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255, 255, 255));

	SelectObject(BackDC, hInterface_Image[HELP]);
	GetObject(hInterface_Image[HELP], sizeof(BITMAP), &Image_Rect);
	TransparentBlt(MainDC, 200, 300, Image_Rect.bmWidth, Image_Rect.bmHeight, BackDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255, 255, 255));

	SelectObject(BackDC, hInterface_Image[QUIT]);
	GetObject(hInterface_Image[QUIT], sizeof(BITMAP), &Image_Rect);
	TransparentBlt(MainDC, 200, 400, Image_Rect.bmWidth, Image_Rect.bmHeight, BackDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255, 255, 255));

	SelectObject(BackDC, hInterface_Image[SELECT_POINTER]);
	GetObject(hInterface_Image[SELECT_POINTER], sizeof(BITMAP), &Image_Rect);
	
	switch (nSelect_Number)
	{
	case START:
	{
		TransparentBlt(MainDC, 140, 210, Image_Rect.bmWidth, Image_Rect.bmHeight, BackDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255, 255, 255));
	}
	break;
	case HELP:
	{
		TransparentBlt(MainDC, 140, 310, Image_Rect.bmWidth, Image_Rect.bmHeight, BackDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255, 255, 255));
	}
	break;
	case QUIT:
	{
		TransparentBlt(MainDC, 140, 410, Image_Rect.bmWidth, Image_Rect.bmHeight, BackDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255, 255, 255));
	}
	break;
	}

	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, MainDC, 0, 0, SRCCOPY);

	DeleteDC(BackDC);
	DeleteDC(MainDC);
	DeleteObject(hMainBitmap);

	return 0;
}

void CInterface::Show_Help(HDC hdc)
{
	HDC MainDC = CreateCompatibleDC(hdc);
	HBITMAP hMainBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_HEIGTH);
	SelectObject(MainDC, hMainBitmap);

	BITMAP Image_Rect;

	HDC BackDC = CreateCompatibleDC(hdc);

	SelectObject(BackDC, hInterface_Image[START_MENU_BACK_IMAGE]);
	GetObject(hInterface_Image[START_MENU_BACK_IMAGE], sizeof(BITMAP), &Image_Rect);
	StretchBlt(MainDC, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, BackDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, SRCCOPY);

	SelectObject(BackDC, hInterface_Image[KEY_HELP]);
	GetObject(hInterface_Image[KEY_HELP], sizeof(BITMAP), &Image_Rect);
	TransparentBlt(MainDC, 200, 100, Image_Rect.bmWidth, Image_Rect.bmHeight, BackDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255, 255, 255));


	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, MainDC, 0, 0, SRCCOPY);
	while (true)
	{
		if (GetAsyncKeyState(VK_BACK) & 0x0001)
		{
			break;
		}
	}

	DeleteDC(BackDC);
	DeleteDC(MainDC);
	DeleteObject(hMainBitmap);
	return;
}

void CInterface::Next_Stage_Waiting(int nStage_Number, HDC hdc)
{

	HDC BackDC = CreateCompatibleDC(hdc);
	HBITMAP hBackBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_HEIGTH);
	SelectObject(BackDC, hBackBitmap);

	HDC MemDC = CreateCompatibleDC(hdc);

	switch (nStage_Number)
	{
	case 1:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_ONE]);	
	}
	break;
	case 2:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_ONE]);
	}
	break;
	case 3:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_TWO]);
	}
	break;
	case 4:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_TWO]);

	}
	break;
	case 5:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_THREE]);
	}
	break;
	case 6:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_THREE]);
	}
	break;
	case 7:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_FOUR]);
	}
	break;
	case 8:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_FOUR]);
	}
	break;
	case 9:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_FIVE]);
	}
	break;
	case 10:
	{
		SelectObject(MemDC, hNext_Stage[STAGE_FIVE]);
	}
	break;
	default:
	{
			//스테이지 다깨면 
		SelectObject(MemDC, hNext_Stage[STAGE_CLEAR]);
			
	}
	break;
	}

	BitBlt(BackDC, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, MemDC, 0, 0, SRCCOPY);
	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, BackDC, 0, 0, SRCCOPY);
	
	DWORD dwtime;
	dwtime = GetTickCount();

	while (dwtime + 2000 > GetTickCount())
	{
		//보여주는거 2초
	}

	DeleteDC(BackDC);
	DeleteObject(hBackBitmap);
	DeleteDC(MemDC);
	return;
}

void CInterface::Setting(ifstream *pInFile, int nStage)
{
	Map_File_Name.clear();
	Bgm_File_Name.clear();

	char chMap_File_Name[256] = "", chBgm_File_Name[256] = "";
	(*pInFile).getline(chMap_File_Name, sizeof(chMap_File_Name));
	(*pInFile).getline(chBgm_File_Name, sizeof(chBgm_File_Name));

	Map_File_Name = chMap_File_Name;
	Bgm_File_Name = chBgm_File_Name;
	hBackGround = (HBITMAP)LoadImage(NULL, TEXT(Map_File_Name.c_str()), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE | LR_CREATEDIBSECTION);;
	BITMAP Image_Rect;
	GetObject(hBackGround, sizeof(BITMAP), &Image_Rect);
	nBg_Width = Image_Rect.bmWidth;
	nBg_Heigth = Image_Rect.bmHeight;

	if (chBgm_File_Name[0] == NULL) // 음악 경로가 없으면
	{
		sndPlaySoundA(NULL, SND_ASYNC | SND_NODEFAULT);
	}
	else
	{
		sndPlaySoundA(Bgm_File_Name.c_str(), SND_ASYNC | SND_NODEFAULT | SND_LOOP);
	}

	return;
}

void CInterface::Render(HDC MainDC)
{
	HDC BackDC = CreateCompatibleDC(MainDC);
	HBITMAP hBackBitmap = CreateCompatibleBitmap(MainDC, nBg_Width, nBg_Heigth);
	SelectObject(BackDC, hBackBitmap);

	HDC MemDC = CreateCompatibleDC(MainDC);
	SelectObject(MemDC, hBackGround);

	BitBlt(BackDC, 0, (CPlay::nMap_Y%nBg_Heigth), nBg_Width, nBg_Heigth, MemDC, 0, 0, SRCCOPY);
	BitBlt(BackDC, 0, 0, nBg_Width, (CPlay::nMap_Y%nBg_Heigth), MemDC, 0, nBg_Heigth - (CPlay::nMap_Y%nBg_Heigth), SRCCOPY);

	StretchBlt(MainDC, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, BackDC, 0, 0, nBg_Width, nBg_Heigth, SRCCOPY);

	DeleteDC(BackDC);
	DeleteObject(hBackBitmap);
	DeleteDC(MemDC);
	return;
}

void CInterface::Info_Render(HDC MainDC, int nLife, int nBomb, int nScore)
{
	

	HDC MemDC = CreateCompatibleDC(MainDC);

	//라이프
	SelectObject(MemDC, hUser_Interface[LIFE_ICON]);
	TransparentBlt(MainDC, 10, CLIENT_HEIGTH-50, 30, 30, MemDC, 0, 0, 30, 30, RGB(255, 255, 255));

	//개수
	SelectObject(MemDC, hUser_Interface[NUMBER]);
	TransparentBlt(MainDC, 50, CLIENT_HEIGTH - 50, 10, 30, MemDC, nLife*10, 0, 10, 30, RGB(0, 0, 0));
	
	//필살기
	SelectObject(MemDC, hUser_Interface[BOMB_ICON]);
	TransparentBlt(MainDC, CLIENT_WIDTH-80, CLIENT_HEIGTH-50, 40, 40, MemDC, 0, 0, 40, 40, RGB(255, 255, 255));

	//개수
	SelectObject(MemDC, hUser_Interface[NUMBER]);
	TransparentBlt(MainDC, CLIENT_WIDTH-40, CLIENT_HEIGTH-50, 10, 30, MemDC, nBomb * 10, 0, 10, 30, RGB(0, 0, 0));

	//스코어
	SelectObject(MemDC, hUser_Interface[NUMBER]);
	//스코어 한자릿수 씩 출력
	int nDigit=100000;
	int nNum = nScore;

	for (int nIndex = 0; nIndex <6; nIndex++)
	{
		TransparentBlt(MainDC, 350+(15*nIndex), CLIENT_HEIGTH - 50, 10, 30, MemDC, (nNum/nDigit) *10, 0, 10, 30, RGB(0, 0, 0));
		nNum = nNum%nDigit;
		nDigit /= 10;
	}

	DeleteDC(MemDC);

	return;
}

void CInterface::Mission_Clear_Render(HDC MainDC, int Y)
{
	HDC MemDC = CreateCompatibleDC(MainDC);

	SelectObject(MemDC, hInterface_Image[MISSION_CLEAR]);
	TransparentBlt(MainDC, CLIENT_WIDTH / 4, Y, 400, 400, MemDC, 0, 0, 400, 400, RGB(255, 255, 255));
	
	DeleteDC(MemDC);
	return;
}

void CInterface::Game_Over_Render(HDC MainDC)
{
	HDC MemDC = CreateCompatibleDC(MainDC);

	SelectObject(MemDC, hInterface_Image[GAME_OVER]);
	TransparentBlt(MainDC, 250, CLIENT_HEIGTH/2, 400, 75, MemDC, 0, 0, 400, 75, RGB(255, 255, 255));
	
	DeleteDC(MemDC);
	return;
}

CInterface::CInterface(HINSTANCE hInst)
{
	hInterface_Image[START_MENU_BACK_IMAGE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_START_IMAGE));
	hInterface_Image[START] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GAME_START));
	hInterface_Image[HELP] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_HELP));
	hInterface_Image[QUIT] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_QUIT));
	hInterface_Image[SELECT_POINTER] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_SELECT_POINTER));
	hInterface_Image[KEY_HELP] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_KEY_HELP));
	hInterface_Image[GAME_OVER] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GAME_OVER));
	hInterface_Image[MISSION_CLEAR] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_MISSION_CLEAR));

	hNext_Stage[STAGE_ONE]= LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STAGE1));
	hNext_Stage[STAGE_TWO] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STAGE2));
	hNext_Stage[STAGE_THREE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STAGE3));
	hNext_Stage[STAGE_FOUR] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STAGE4));
	hNext_Stage[STAGE_FIVE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_STAGE5));
	hNext_Stage[STAGE_CLEAR] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_GAME_CLEAR));

	//유저 인터페이스
	
	hUser_Interface[NUMBER] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_NUMBER));
	hUser_Interface[LIFE_ICON] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_LIFE_ICO));
	hUser_Interface[BOMB_ICON] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOMB_ICO));

	nSelect_Number = 1;
}


CInterface::~CInterface()
{
}
