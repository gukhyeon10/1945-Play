#include "stdafx.h"
#include "Play.h"
#include "Interface.h"
#include "Enemy_Manager.h"
#include "Bullet_Manager.h"
#include "Effect.h"
#include "Item.h"
#include "Special.h"

int CPlay::nMap_Y = 0; // 화면 횡스크롤 시작 좌표 

void CPlay::Stage_Load()
{

	if (pUser->nStage > 10)
	{
		//스테이지가 모두 끝나면 게임 엔드
		delete pUser;
		pUser = nullptr;
		return;
	}

	string stage_path, back_file_path;

	back_file_path = "C:\\Program Files (x86)\\API1945\\STAGE\\";
	back_file_path += to_string(pUser->nStage);
	back_file_path += ".txt";
	
	ifstream InFile;
	InFile.open(back_file_path);

	//배경맵, 배경음 로드
	pInterface->Setting(&InFile, pUser->nStage);



	stage_path = "C:\\Program Files (x86)\\API1945\\STAGE\\";
	stage_path += to_string(pUser->nStage);
	stage_path += ".xml";

	//해당 스테이지 적 유닛 로드
	pEnemy_Manager->Load(stage_path.c_str(), pUser->nStage);

	//위치 초기화
	nMap_Y = 0;	
	pUser->fX = 360;
	pUser->fY = 850;


	return;
}

bool CPlay::Progress()
{
	if (pUser != nullptr) //게임 진행중
	{
		Paint();
		nMap_Y++;
		for (int Game_Speed = 0; Game_Speed < 2; Game_Speed++)
		{
			pEnemy_Manager->Move();

			pUser->Action();

			pItem_Manager->User_Get(pUser);
			
			pItem_Manager->Move();

			for (int User_Fire_Speed = 0; User_Fire_Speed < 2; User_Fire_Speed++)
			{
				pUser->Fire(pBullet_Manager);
			}

			pSpecial->Progress(pUser, pBullet_Manager, pEnemy_Manager, pEffect); // 필살기
			
			for (int Bullet_Speed = 0; Bullet_Speed < 3; Bullet_Speed++)
			{
				pBullet_Manager->Move_Crash(pEnemy_Manager, pUser, pEffect, pItem_Manager); //총알 이동 및 충돌 체크, 충돌 이펙트, 아이템 랜덤 드롭
			}
		}

		if (pEnemy_Manager->Boss_Kill()) //적유닛 리스트가 비었으면 클리어
		{
			if (pUser->nStage % 2 == 0)  // 2 ,4 , 6 , 8, 10 일때만 보스 폭발 효과
			{
				pEffect->Boss_Clear_Effect();//보스 폭발
			}

			Game_Clear_Show();   // 클리어 후 짤막한 애니메이션

			pUser->nStage++;
			
			pBullet_Manager->Bullet_Clear();
			pEffect->Clear();
			pItem_Manager->Clear();
			pSpecial->Reset(pUser);
			pInterface->Next_Stage_Waiting(pUser->nStage, hdc);	  //->다음 스테이지 전에 짤막한 대기화면 // 새로운 노래깔기
			Stage_Load();                       //-> 다음 스테이지 로드 
		}

		Game_Over_Check(); //게임 오버 체크

	}
	else  //게임 진행 ㄴㄴ
	{

		switch (pInterface->Start_Menu(hdc))
		{
			case 1:  // 유저 생성 (게임 생성)
			{
				pUser = new CUser(hinst);
				pInterface->Next_Stage_Waiting(pUser->nStage, hdc);
				Stage_Load();
			}
			break;
			case 2: // 조작키 도움
			{
				pInterface->Show_Help(hdc);
			}
			break;
			case 3: // 나가기 
			{
				return false;
			}
			break;
		}
	}
	return true;
}

void CPlay::Game_Clear_Show()
{

	HDC MainDC = CreateCompatibleDC(hdc);
	HBITMAP hMainBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_HEIGTH);
	SelectObject(MainDC, hMainBitmap);

	DWORD Game_Clear_Movie = GetTickCount();
	DWORD Game_Clear_Effect = GetTickCount();
	int Clear_Message_Location_Y = -200;
	while (Game_Clear_Movie + 5000 > GetTickCount())
	{
		pInterface->Render(MainDC);
		pInterface->Mission_Clear_Render(MainDC, Clear_Message_Location_Y++);
		if (Clear_Message_Location_Y > CLIENT_HEIGTH / 3)
		{
			Clear_Message_Location_Y--;
		}

		pEffect->Paint(MainDC);

		pUser->Paint(MainDC);
		
		pUser->fY--;

		if (Game_Clear_Effect + 200 < GetTickCount())
		{
			for (int nIndex = 0; nIndex < 2; nIndex++)
			{
				pEffect->Insert(rand() % CLIENT_WIDTH, rand() % CLIENT_HEIGTH, 1); //폭발 이펙트 무작위 생성
			}
			Game_Clear_Effect = GetTickCount();
		}
		BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, MainDC, 0, 0, SRCCOPY);
	}

	DeleteDC(MainDC);
	DeleteObject(hMainBitmap);

	return;
}

void CPlay::Game_Over_Check()
{
	if (pUser==nullptr) // 앞서 게임이 모두 클리어되면 return
	{
		sndPlaySoundA("C:\\Program Files (x86)\\API1945\\Map&Bgm\\menu.wav", SND_ASYNC | SND_NODEFAULT | SND_LOOP);
		return;
	}

	if (pUser->Info.nLife == 0)
	{
		HDC MainDC = CreateCompatibleDC(hdc);
		HBITMAP hMainBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_HEIGTH);
		SelectObject(MainDC, hMainBitmap);

		pInterface->Render(MainDC);

		pEnemy_Manager->Render(MainDC);

		pUser->Paint(MainDC);

		pBullet_Manager->Paint(MainDC);

		pEffect->Paint(MainDC);

		pItem_Manager->Render(MainDC);

		pInterface->Game_Over_Render(MainDC);

		BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, MainDC, 0, 0, SRCCOPY);

		DWORD Game_Over_Movie = GetTickCount();

		while (true)
		{
			if (Game_Over_Movie + 3000 < GetTickCount())
			{
				//여기서 애니메이션 출력
				break;
			}
		}

		pBullet_Manager->Bullet_Clear();
		pEnemy_Manager->Clear();
		pEffect->Clear();
		pItem_Manager->Clear();
		pSpecial->Reset(pUser);

		//게임 오버되면 노래 중지
		sndPlaySoundA("C:\\Program Files (x86)\\API1945\\Map&Bgm\\menu.wav", SND_ASYNC | SND_NODEFAULT | SND_LOOP);
		
		delete pUser;
		pUser = nullptr;

		DeleteDC(MainDC);
		DeleteObject(hMainBitmap);
	}

	return;
}

void CPlay::Paint()
{
	HDC MainDC = CreateCompatibleDC(hdc);
	HBITMAP hMainBitmap = CreateCompatibleBitmap(hdc, CLIENT_WIDTH, CLIENT_HEIGTH);
	SelectObject(MainDC, hMainBitmap);

	//배경맵, 배경음
	pInterface->Render(MainDC);

	//적유닛 
	pEnemy_Manager->Render(MainDC);

	//유저 유닛
	pUser->Paint(MainDC);

	//총알
	pBullet_Manager->Paint(MainDC);

	//아이템
	pItem_Manager->Render(MainDC);

	//필살기 모션
	pSpecial->Render(MainDC);

	//충돌 이펙트 
	pEffect->Paint(MainDC);

	//화면 인터페이스
	pInterface->Info_Render(MainDC, pUser->Info.nLife, pUser->Info.nSkill, pUser->Info.nScore);

	BitBlt(hdc, 0, 0, CLIENT_WIDTH, CLIENT_HEIGTH, MainDC, 0, 0, SRCCOPY);

	DeleteDC(MainDC);
	DeleteObject(hMainBitmap);
	return;
}

void CPlay::Class_Link(CInterface *Interface, CEnemy_Manager *Enemy_Manager, CBullet_Manager *Bullet_Manager, CEffect *Effect_Manager, CItem *Item_Manager, CSpecial *Special)
{
	pInterface = Interface;
	pEnemy_Manager = Enemy_Manager;
	pBullet_Manager = Bullet_Manager;
	pEffect = Effect_Manager;
	pItem_Manager = Item_Manager;
	pSpecial = Special;

	return;
}

CPlay::CPlay(HWND hWnd, HINSTANCE hInst)
{
	hwnd = hWnd;
	hinst = hInst;
	hdc = GetDC(hwnd);
	pUser = nullptr;
}


CPlay::~CPlay()
{
	ReleaseDC(hwnd, hdc);
	delete pUser;
	pUser = nullptr;
}
