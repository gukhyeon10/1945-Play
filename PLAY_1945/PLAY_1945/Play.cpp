#include "stdafx.h"
#include "Play.h"
#include "Interface.h"
#include "Enemy_Manager.h"
#include "Bullet_Manager.h"
#include "Effect.h"
#include "Item.h"
#include "Special.h"

int CPlay::nMap_Y = 0; // ȭ�� Ⱦ��ũ�� ���� ��ǥ 

void CPlay::Stage_Load()
{

	if (pUser->nStage > 10)
	{
		//���������� ��� ������ ���� ����
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

	//����, ����� �ε�
	pInterface->Setting(&InFile, pUser->nStage);



	stage_path = "C:\\Program Files (x86)\\API1945\\STAGE\\";
	stage_path += to_string(pUser->nStage);
	stage_path += ".xml";

	//�ش� �������� �� ���� �ε�
	pEnemy_Manager->Load(stage_path.c_str(), pUser->nStage);

	//��ġ �ʱ�ȭ
	nMap_Y = 0;	
	pUser->fX = 360;
	pUser->fY = 850;


	return;
}

bool CPlay::Progress()
{
	if (pUser != nullptr) //���� ������
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

			pSpecial->Progress(pUser, pBullet_Manager, pEnemy_Manager, pEffect); // �ʻ��
			
			for (int Bullet_Speed = 0; Bullet_Speed < 3; Bullet_Speed++)
			{
				pBullet_Manager->Move_Crash(pEnemy_Manager, pUser, pEffect, pItem_Manager); //�Ѿ� �̵� �� �浹 üũ, �浹 ����Ʈ, ������ ���� ���
			}
		}

		if (pEnemy_Manager->Boss_Kill()) //������ ����Ʈ�� ������� Ŭ����
		{
			if (pUser->nStage % 2 == 0)  // 2 ,4 , 6 , 8, 10 �϶��� ���� ���� ȿ��
			{
				pEffect->Boss_Clear_Effect();//���� ����
			}

			Game_Clear_Show();   // Ŭ���� �� ©���� �ִϸ��̼�

			pUser->nStage++;
			
			pBullet_Manager->Bullet_Clear();
			pEffect->Clear();
			pItem_Manager->Clear();
			pSpecial->Reset(pUser);
			pInterface->Next_Stage_Waiting(pUser->nStage, hdc);	  //->���� �������� ���� ©���� ���ȭ�� // ���ο� �뷡���
			Stage_Load();                       //-> ���� �������� �ε� 
		}

		Game_Over_Check(); //���� ���� üũ

	}
	else  //���� ���� ����
	{

		switch (pInterface->Start_Menu(hdc))
		{
			case 1:  // ���� ���� (���� ����)
			{
				pUser = new CUser(hinst);
				pInterface->Next_Stage_Waiting(pUser->nStage, hdc);
				Stage_Load();
			}
			break;
			case 2: // ����Ű ����
			{
				pInterface->Show_Help(hdc);
			}
			break;
			case 3: // ������ 
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
				pEffect->Insert(rand() % CLIENT_WIDTH, rand() % CLIENT_HEIGTH, 1); //���� ����Ʈ ������ ����
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
	if (pUser==nullptr) // �ռ� ������ ��� Ŭ����Ǹ� return
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
				//���⼭ �ִϸ��̼� ���
				break;
			}
		}

		pBullet_Manager->Bullet_Clear();
		pEnemy_Manager->Clear();
		pEffect->Clear();
		pItem_Manager->Clear();
		pSpecial->Reset(pUser);

		//���� �����Ǹ� �뷡 ����
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

	//����, �����
	pInterface->Render(MainDC);

	//������ 
	pEnemy_Manager->Render(MainDC);

	//���� ����
	pUser->Paint(MainDC);

	//�Ѿ�
	pBullet_Manager->Paint(MainDC);

	//������
	pItem_Manager->Render(MainDC);

	//�ʻ�� ���
	pSpecial->Render(MainDC);

	//�浹 ����Ʈ 
	pEffect->Paint(MainDC);

	//ȭ�� �������̽�
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
