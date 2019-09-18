#include "stdafx.h"
#include "Enemy_Manager.h"
#include "resource.h"
#include "Play.h"
#include "Bullet_Manager.h"
#include "Effect.h"
#include "Item.h"
#include "User.h"

#include "tinyxml2.h"

using namespace tinyxml2;

void CEnemy_Manager::Load(const char* File_Path, unsigned int nStage)
{
	//xml파일 읽을때
	
	XMLDocument doc;
	if (doc.LoadFile(File_Path) == XML_ERROR_FILE_NOT_FOUND)
	{
		system("pause"); //파일 못찾으면 pause;
	}
	else
	{
		int nEnemy_Stage_Advantage = (nStage / 2) + (nStage % 2);
		XMLNode* child;
		const XMLNode* parent = doc.FirstChildElement("root"); // 맨 상위 노드
	
		for (child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			Enemy *pEnemy = new Enemy;
			child->FirstChildElement("Unit_Number")->QueryIntText(&(pEnemy->nUnit_Number));
			child->FirstChildElement("Location_X")->QueryFloatText(&(pEnemy->fLocation_X));
			child->FirstChildElement("Location_Y")->QueryFloatText(&(pEnemy->fLocation_Y));
			child->FirstChildElement("Move_Pattern")->QueryUnsignedText(&(pEnemy->nMove_Pattern));
			child->FirstChildElement("Shoot_Pattern")->QueryUnsignedText(&(pEnemy->nShoot_Pattern));

			if (pEnemy->nUnit_Number >= UNIT_ONE && pEnemy->nUnit_Number <= UNIT_THREE)
			{
				pEnemy->nHp = Unit_Hp_Rank[pEnemy->nUnit_Number] * nEnemy_Stage_Advantage;
			}
			else
			{
				pEnemy->nHp = Unit_Hp_Rank[pEnemy->nUnit_Number];
			}

			pEnemy->a = 1;
			pEnemy->fCenter_X = pEnemy->fLocation_X;
			pEnemy->fCenter_Y = pEnemy->fLocation_Y;
			pEnemy->nMove_Angle = 0;
			pEnemy->reload = 0;
			pEnemy->nShoot_Count = 0;
			pEnemy->nBoss_Pattern = 0;
			Enemy_List.push_back(pEnemy);
			
		}
		
	}

	return;
}

void CEnemy_Manager::Render(HDC hdc)
{
	HDC UnitDC = CreateCompatibleDC(hdc);
	BITMAP Image_Rect;
	
	for (list<Enemy*>::iterator iter = Enemy_List.begin(); iter != Enemy_List.end();)
	{
		GetObject(hUnit[(*iter)->nUnit_Number], sizeof(BITMAP), &Image_Rect);
		//유닛이 클라이언트 창 내에 있을때
		if ((*iter)->fLocation_Y + CPlay::nMap_Y + Image_Rect.bmHeight >= 0 && (*iter)->fLocation_Y + CPlay::nMap_Y <= CLIENT_HEIGTH && (*iter)->fLocation_X + Image_Rect.bmWidth >= 0 && (*iter)->fLocation_X <= CLIENT_WIDTH)
		{
			SelectObject(UnitDC, hUnit[(*iter)->nUnit_Number]);
			TransparentBlt(hdc, (*iter)->fLocation_X, (*iter)->fLocation_Y + CPlay::nMap_Y, Image_Rect.bmWidth, Image_Rect.bmHeight, UnitDC, 0, 0, Image_Rect.bmWidth, Image_Rect.bmHeight, RGB(255, 255, 255));
			++iter;
		}//유닛이 클라이언트 창 밖으로 나갔을때
		else if ((*iter)->fLocation_Y + CPlay::nMap_Y > CLIENT_HEIGTH || (*iter)->fLocation_X + Image_Rect.bmWidth<0 || (*iter)->fLocation_X >CLIENT_WIDTH)
		{
			delete (*iter);
			iter = Enemy_List.erase(iter);
		}//다음 유닛이 아직 클라이언트 창에 나타나지 않을때
		else if ((*iter)->fLocation_Y + CPlay::nMap_Y + Image_Rect.bmHeight < 0)
		{
			break;
		}
	}

	DeleteDC(UnitDC);
	return;
}

void CEnemy_Manager::Move()
{
	BITMAP Image_Rect;

	for (list<Enemy*>::iterator iter = Enemy_List.begin(); iter != Enemy_List.end();)
	{
		GetObject(hUnit[(*iter)->nUnit_Number], sizeof(BITMAP), &Image_Rect);

		if ((*iter)->fLocation_Y + Image_Rect.bmHeight + CPlay::nMap_Y < 0)
		{
			break;
		}
		else
		{
			//총알 발사하기전에 나타나고 나서 몇초후부터 발사를 할건지 
			if ((*iter)->reload != 0)
			{
				//Unit_Shoot(iter);
				Fire(iter);
			}
			else
			{
				(*iter)->reload = GetTickCount() + 1000; // 나타나고 2초후에 쏠수있도록
			}

			//이동패턴
			if ((*iter)->nUnit_Number >= UNIT_ONE && (*iter)->nUnit_Number <= UNIT_THREE)
			{
				switch ((*iter)->nMove_Pattern)
				{
				case 1: //수직 하강
				{
					(*iter)->fLocation_Y++;
				}
				break;
				case 2: //지그재그
				{

					(*iter)->fLocation_X += (*iter)->a;
					if ((*iter)->a > 0)
					{
						if ((*iter)->fCenter_X + 40 < (*iter)->fLocation_X)
						{
							(*iter)->a = -1;
						}
					}
					else
					{
						if ((*iter)->fCenter_X - 40 > (*iter)->fLocation_X)
						{
							(*iter)->a = 1;
						}
					}


				}
				break;
				case 3: //어느정도 오다가 멈춤
				{
					(*iter)->fLocation_Y += (*iter)->a;
					if ((*iter)->a > -0.5f)
					{
						(*iter)->a -= 0.005f;
					}
					else
					{
						(*iter)->a = -0.5f;
					}

				}
				break;
				case 4: //내려오다가 대각선으로 이동
				{
					(*iter)->fLocation_Y += (*iter)->a;
					if ((*iter)->a > -0.5f)
					{
						(*iter)->a -= 0.005f;
					}
					else
					{
						if ((*iter)->fCenter_X > CLIENT_WIDTH / 2)
						{
							(*iter)->fLocation_X -= 2;
							(*iter)->fLocation_Y++;
						}
						else
						{
							(*iter)->fLocation_X += 2;
							(*iter)->fLocation_Y++;
						}
					}
				}
				break;
				case 5: //
				{
					(*iter)->fLocation_Y += 0.5;
				}
				break;
				case 6: // 원을 그리면서 하강
				{
					if ((*iter)->fLocation_Y + CPlay::nMap_Y < 30)
					{
						(*iter)->fCenter_X = (*iter)->fLocation_X - 90;
						(*iter)->fCenter_Y = (*iter)->fLocation_Y;
					}
					else
					{
						float r = (*iter)->nMove_Angle*3.14 / 180;

						(*iter)->fLocation_X = (*iter)->fCenter_X + (cos(r) * 90);
						(*iter)->fLocation_Y = (*iter)->fCenter_Y + (sin(r) * 90);
						(*iter)->nMove_Angle++;
					}
				}
				break;

				}
			}//보스 이동 아직 딱히 필요는 없는듯
			else if ((*iter)->nUnit_Number >= BOSS_ONE && (*iter)->nUnit_Number <= BOSS_FIVE)
			{
				(*iter)->fLocation_Y += (*iter)->a;
				if ((*iter)->a > -0.5f)
				{
					(*iter)->a -= 0.0045f;
				}
				else
				{
					(*iter)->a = -0.5f;
				}

				
			}
			++iter;
		}
	}
	return;
}


bool CEnemy_Manager::Crash_Check(float Bullet_X, float Bullet_Y, CUser *pUser_Manager, CEffect *pEffect, CItem *pItem_Manager)
{
	BITMAP Unit_Rect;

	for (list<Enemy*>::iterator iter = Enemy_List.begin(); iter != Enemy_List.end();)
	{
		GetObject(hUnit[(*iter)->nUnit_Number], sizeof(BITMAP), &Unit_Rect);
		if (Bullet_X <(*iter)->fLocation_X + Unit_Rect.bmWidth  && Bullet_X >(*iter)->fLocation_X  && Bullet_Y > (*iter)->fLocation_Y + CPlay::nMap_Y && Bullet_Y < (*iter)->fLocation_Y + CPlay::nMap_Y + Unit_Rect.bmHeight)
		{
			if ((*iter)->nHp <= 1) // 적이 피격 당했을때.
			{
				//점수 갱신
				pEffect->Insert((*iter)->fLocation_X, (*iter)->fLocation_Y+CPlay::nMap_Y, 1);
				Plus_Score(pUser_Manager, (*iter)->nUnit_Number);
				//아이템 일정 확률로 드롭
				pItem_Manager->Insert((*iter)->fLocation_X, (*iter)->fLocation_Y+CPlay::nMap_Y+Unit_Rect.bmHeight/2);
				//적 동적해제
				delete (*iter);
				iter = Enemy_List.erase(iter);
				return true;
			}
			else  //한발 맞았을때
			{
				pEffect->Insert(Bullet_X-13, Bullet_Y-(Unit_Rect.bmHeight / 3), 0);
				(*iter)->nHp--;
				return true;
			}
		}
		++iter;
	}
	return false;
}

void CEnemy_Manager::Explosion_Crash(CEffect *pEffect, float fDistance, CUser *pUser)
{
	float DeltaX, DeltaY;
	float Circle_Center_X = 400;
	float Circle_Center_Y = 450;
	float Length;
	BITMAP Unit_Rect;

	for (list<Enemy*>::iterator iter = Enemy_List.begin(); iter != Enemy_List.end();)
	{
		GetObject(hUnit[(*iter)->nUnit_Number], sizeof(BITMAP), &Unit_Rect);
	
			DeltaX = Circle_Center_X - ((*iter)->fLocation_X + (Unit_Rect.bmWidth/2));
			DeltaY = Circle_Center_Y - ((*iter)->fLocation_Y + CPlay::nMap_Y + (Unit_Rect.bmHeight/2));

			Length = sqrt(DeltaX * DeltaX + DeltaY *DeltaY);

			if (Length < fDistance+Unit_Rect.bmHeight/3)
			{
				if ((*iter)->nHp <= 0.25) // 적이 피격 당했을때.
				{
					//점수 갱신
					pEffect->Insert((*iter)->fLocation_X, (*iter)->fLocation_Y + CPlay::nMap_Y, 1);
					Plus_Score(pUser, (*iter)->nUnit_Number);
					//적 동적해제
					delete (*iter);
					iter = Enemy_List.erase(iter);
				}
				else  //한발 맞았을때
				{
					pEffect->Insert((*iter)->fLocation_X + (Unit_Rect.bmWidth / 2), (*iter)->fLocation_Y + CPlay::nMap_Y + (Unit_Rect.bmHeight / 2), 0);
					(*iter)->nHp-=0.25;
					++iter;
				}
			}
			else
			{
				++iter;
			}
	}

	return;
}

void CEnemy_Manager::Plus_Score(CUser *pUser_Manager, int nUnit_Number)
{
	switch (nUnit_Number)
	{
	case UNIT_ONE:
	{
		pUser_Manager->Info.nScore += 70;
	}
	break;
	case UNIT_TWO:
	{
		pUser_Manager->Info.nScore += 100;
	}
	break;
	case UNIT_THREE:
	{
		pUser_Manager->Info.nScore += 130;
	}
	break;
	default:
	{
		pUser_Manager->Info.nScore += 300;
		if ((pUser_Manager->nStage) / 2 == nUnit_Number - BOSS_ONE + 1) // 유저의 스테이지 넘버와 보스의 넘버가 같으면 게임클리어 체크
		{
			bBoss_Die = true;
		}
	}
	break;
	}
	return;
}

bool CEnemy_Manager::Boss_Kill()
{
	if (bBoss_Die == true || Enemy_List.empty())
	{
		bBoss_Die = false;
		return true;
	}
	else
	{
		return false;
	}
}

void CEnemy_Manager::Clear()
{
	for (list<Enemy*>::iterator iter = Enemy_List.begin(); iter != Enemy_List.end();)
	{
		delete (*iter);
		iter = Enemy_List.erase(iter);
	}
	return;
}

CEnemy_Manager::CEnemy_Manager(HINSTANCE hInst, CBullet_Manager *Bullet_Manager)
{
	hUnit[UNIT_ONE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNIT1));
	hUnit[UNIT_TWO] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNIT2));
	hUnit[UNIT_THREE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_UNIT3));
	hUnit[BOSS_ONE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS1));
	hUnit[BOSS_TWO] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS2));
	hUnit[BOSS_THREE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS3));
	hUnit[BOSS_FOUR] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS4));
	hUnit[BOSS_FIVE] = LoadBitmap(hInst, MAKEINTRESOURCE(IDB_BOSS5));
	
	bBoss_Die = false;

	pBullet_Manager = Bullet_Manager;

	XMLDocument doc;
	if (doc.LoadFile("C:\\Program Files (x86)\\API1945\\STAGE\\Enemy_Rank.xml") == XML_ERROR_FILE_NOT_FOUND)
	{
		system("pause"); //파일 못찾으면 pause;
	}
	else
	{
		int nUnit_Number;
		XMLNode* child;
		XMLElement* Node_Info;
		const XMLNode* parent = doc.FirstChildElement("root"); // 맨 상위 노드
		for (child = (XMLNode*)parent->FirstChild(); child != 0; child = (XMLNode*)child->NextSibling())
		{
			child->FirstChildElement("Unit_Number")->QueryIntText(&(nUnit_Number));
			child->FirstChildElement("Unit_Hp")->QueryIntText(Unit_Hp_Rank+nUnit_Number);
		}
	}
}


CEnemy_Manager::~CEnemy_Manager()
{
	Clear();
}
