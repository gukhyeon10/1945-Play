#include "stdafx.h"
#include "Enemy_Manager.h"
#include "Bullet_Manager.h"
#include "Play.h"

void CEnemy_Manager::Fire(list<Enemy*>::iterator unit)
{
	float fMuzzle_x, fMuzzle_y; //유닛 기준에서 총알이 나가는 좌표
	switch ((*unit)->nUnit_Number)
	{
	case UNIT_ONE:
	{
		fMuzzle_x = 40;
		fMuzzle_y = 47;
	}
	break;
	case UNIT_TWO:
	{
		fMuzzle_x = 53;
		fMuzzle_y = 78;
	}
	break;
	case UNIT_THREE:
	{
		fMuzzle_x = 64;
		fMuzzle_y = 108;
	}
	break;
	default: //보스들
	{
		BITMAP Unit_Rect;
		GetObject(hUnit[(*unit)->nUnit_Number], sizeof(BITMAP), &Unit_Rect);
		fMuzzle_x = Unit_Rect.bmWidth / 2;
		fMuzzle_y = Unit_Rect.bmHeight / 2;
	}
	break;
	}

	if ((*unit)->nUnit_Number >= UNIT_ONE && (*unit)->nUnit_Number <= UNIT_THREE)
	{
		switch ((*unit)->nShoot_Pattern)
		{
		case 1: // 직선 발사
		{
			if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count < 5) //0.15초당 발사하고 한번씩 5회 발사하고 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, 0, 1, ENEMY_BULLET);
			}
			else if ((*unit)->reload + 1000 < GetTickCount()) // 5번 모두 쏘면 다시 재장전까지 2초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 2: //3방향 발사
		{
			if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count < 3) //0.15초당 발사하고 한번씩 3회 발사하고 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, 0, 1, ENEMY_BULLET);
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, -0.2, 1, ENEMY_BULLET);
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, 0.2, 1, ENEMY_BULLET);
			}
			else if ((*unit)->reload + 1000 < GetTickCount()) // 3번 모두 쏘면 다시 재장전까지 2초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 3: // 좌 우로 발사
		{
			if ((*unit)->reload + 200 < GetTickCount() && (*unit)->nShoot_Count < 2)
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				if ((*unit)->fLocation_X < CLIENT_WIDTH / 2)
				{
					for (int nIndex = 0; nIndex < 3; nIndex++)
					{
						float xx = (rand() % 5) + 10;
						float yy = (rand() % 6) - 2;
						pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, xx / 20, yy / 20, ENEMY_BULLET);
					}
				}
				else
				{
					for (int nIndex = 0; nIndex < 3; nIndex++)
					{
						float xx = (rand() % 5) + 10;
						float yy = (rand() % 6) - 2;
						pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, (xx / 20)*-1, yy / 20, ENEMY_BULLET);
					}
				}
			}
			else if ((*unit)->reload + 1000 < GetTickCount()) //5번 모두 쏘면 다시 재장전까지 0.8초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 4: // 원 발사
		{

			if ((*unit)->reload + 100 < GetTickCount() && (*unit)->nShoot_Count < 1) //0.1초당 발사하고 한번씩 5회 발사하고 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				for (int nIndex = 0; nIndex < 44; nIndex++)
				{
					pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, sin(nIndex) / 2, cos(nIndex) / 2, ENEMY_BULLET);
				}
			}
			else if ((*unit)->reload + 1500 < GetTickCount()) // 5번 모두 쏘면 다시 재장전까지 2초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 5: // 육각 방향으로 발사
		{
			if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count < 6) //0.15초당 발사하고 한번씩 6회 발사하고 
			{
				(*unit)->nShoot_Count++;
				(*unit)->reload = GetTickCount();
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, -0.3, 0.5, ENEMY_BULLET);
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, 0.3, 0.5, ENEMY_BULLET);
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, -0.5, 0, ENEMY_BULLET);
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, 0.5, 0, ENEMY_BULLET);
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, -0.3, -0.5, ENEMY_BULLET);
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, 0.3, -0.5, ENEMY_BULLET);
			}
			else if ((*unit)->reload + 1500 < GetTickCount()) // 30번 모두 쏘면 다시 재장전까지 2초걸리고
			{
				(*unit)->nShoot_Count = 0;
			}
		}
		break;
		case 6: // 달팽이 모양으로 발사
		{
			if ((*unit)->reload + 50 < GetTickCount())
			{
				if ((*unit)->nShoot_Count++ > 90)
				{
					(*unit)->nShoot_Count = 0;

				}
				(*unit)->reload = GetTickCount();
				//시계 패턴
				pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, cos((*unit)->nShoot_Count * 6), sin((*unit)->nShoot_Count * 6), ENEMY_BULLET);
			}
		}
		break;
		}
	}
	else if ((*unit)->nUnit_Number >= BOSS_ONE && (*unit)->nUnit_Number <= BOSS_FIVE)  // 보스 탄막 패턴
	{
		switch ((*unit)->nUnit_Number)
		{
		case BOSS_ONE:
		{
			
			switch ((*unit)->nBoss_Pattern)
			{
			case 0:
			{
				if ((*unit)->reload + 100 < GetTickCount() && (*unit)->nShoot_Count < 1)
				{
					(*unit)->nShoot_Count++;
					(*unit)->reload = GetTickCount();
					for (int nIndex = 0; nIndex < 44; nIndex++)
					{
						pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, sin(nIndex) / 2, cos(nIndex) / 2, ENEMY_BULLET);
					}

				}
				else if ((*unit)->reload + 500 < GetTickCount()) // 5번 모두 쏘면 다시 재장전까지 2초걸리고
				{
					(*unit)->nBoss_Pattern = 1;
					(*unit)->nShoot_Count = 0;
				}
			}
			break;
			case 1:
			{
				if ((*unit)->reload + 150 < GetTickCount() && (*unit)->nShoot_Count < 3) //0.15초당 발사하고 한번씩 3회 발사하고
				{
					(*unit)->nShoot_Count++;
					(*unit)->reload = GetTickCount();
					if ((*unit)->nShoot_Count % 2 == 0)
					{
						for (float fIndex = 0.0; fIndex <= 0.8; fIndex += 0.2)
						{
							pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, -fIndex, 1, ENEMY_BULLET);
							pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, fIndex, 1, ENEMY_BULLET);

						}
					}
					else
					{
						for (float fIndex = 0.1; fIndex <= 0.7; fIndex += 0.2)
						{
							pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, -fIndex, 1, ENEMY_BULLET);
							pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, fIndex, 1, ENEMY_BULLET);

						}
					}
				}
				else if ((*unit)->reload + 500 < GetTickCount()) // 3번 모두 쏘면 다시 재장전까지 2초걸리고
				{
					(*unit)->nShoot_Count = 0;
					(*unit)->nBoss_Pattern = 0;
				}
			}
			break;
			}
			
		}
		break;
		case BOSS_TWO:
		{
			switch ((*unit)->nBoss_Pattern)
			{
			case 0:
			{
				if ((*unit)->reload + 10 < GetTickCount() && (*unit)->nShoot_Count < 55)
				{
					(*unit)->nShoot_Count++;
					(*unit)->reload = GetTickCount();

					pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, sin((*unit)->nShoot_Count) / 2, cos((*unit)->nShoot_Count), ENEMY_BULLET);

				}
				else if ((*unit)->reload + 130 < GetTickCount())
				{
					(*unit)->nShoot_Count = 0;
					(*unit)->nBoss_Pattern = 1;
				}
			}
			break;
			case 1:
			{
				if ((*unit)->reload + 130 < GetTickCount() && (*unit)->nShoot_Count < 1)
				{
					(*unit)->nShoot_Count++;
					(*unit)->reload = GetTickCount();
					for (int nIndex = 0; nIndex < 44; nIndex++)
					{
						pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, sin(nIndex) / 3, cos(nIndex) / 3, ENEMY_BULLET);
					}
				}
				else
				{
					(*unit)->nShoot_Count = 0;
					(*unit)->nBoss_Pattern = 0;
				}
			}
			break;
			}

		}
		break;
		case BOSS_THREE:
		{
			switch ((*unit)->nBoss_Pattern)
			{
			case 0:
			{
				if ((*unit)->reload + 10 < GetTickCount() && (*unit)->nShoot_Count < 155)
				{
					(*unit)->nShoot_Count++;
					(*unit)->reload = GetTickCount();

					pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x - 100, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, sin((*unit)->nShoot_Count) / 2, cos((*unit)->nShoot_Count) / 2, ENEMY_BULLET);
					pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x + 100, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, sin((*unit)->nShoot_Count) / 2, cos((*unit)->nShoot_Count) / 2, ENEMY_BULLET);

				}
				else if ((*unit)->nShoot_Count >= 155)
				{
					(*unit)->nShoot_Count = 0;
					(*unit)->nBoss_Pattern = 1;
				}
			}
			break;
			case 1:
			{
				if ((*unit)->reload + 500 < GetTickCount())
				{
					int Start_X;
					Start_X = (*unit)->fLocation_X + rand() % (int)fMuzzle_x * 2;
					for (int nIndex = 0; nIndex <= 2; nIndex++)
					{
						pBullet_Manager->Bullet_Add(Start_X + (nIndex * 15), ((2 - nIndex) * 20), 0, 2, ENEMY_BULLET);
						pBullet_Manager->Bullet_Add(Start_X - (nIndex * 15), ((2 - nIndex) * 20), 0, 2, ENEMY_BULLET);
					}
					(*unit)->nBoss_Pattern = 0;
				}

			}
			break;
			}
		}
		break;
		case BOSS_FOUR:
		{
			switch ((*unit)->nBoss_Pattern)
			{
			case 0:
			{
				if ((*unit)->reload + 20 < GetTickCount() && (*unit)->nShoot_Count < 150)
				{
					(*unit)->nShoot_Count++;
					(*unit)->reload = GetTickCount();

					pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x - 100, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, cos((*unit)->nShoot_Count * 3) / 2, sin((*unit)->nShoot_Count * 3) / 2, ENEMY_BULLET);
					pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x + 100, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, cos((*unit)->nShoot_Count * 3) / 2, sin((*unit)->nShoot_Count * 3) / 2, ENEMY_BULLET);

				}
				else if ((*unit)->nShoot_Count >= 150)
				{
					(*unit)->nBoss_Pattern = 1;
				}
			}
			break;
			case 1:
			{
				if ((*unit)->reload + 20 < GetTickCount() && (*unit)->nShoot_Count > 0)
				{
					(*unit)->nShoot_Count--;
					(*unit)->reload = GetTickCount();
					//시계 패턴
					pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x - 90, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, cos((*unit)->nShoot_Count * 6), sin((*unit)->nShoot_Count * 6), ENEMY_BULLET);
					pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x + 90, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, cos((*unit)->nShoot_Count * 6), sin((*unit)->nShoot_Count * 6), ENEMY_BULLET);

					if ((*unit)->nShoot_Count % 50 == 0)
					{
						//원
						for (int nIndex = 0; nIndex < 44; nIndex++)
						{
							pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, sin(nIndex) / 2, cos(nIndex) / 2, ENEMY_BULLET);
						}
					}
				}
				else if ((*unit)->nShoot_Count <= 0)
				{
					(*unit)->nBoss_Pattern = 0;
				}
			}
			break;
			}
		}
		break;
		case BOSS_FIVE:
		{

			if ((*unit)->reload + 150 < GetTickCount())
			{
				(*unit)->reload = GetTickCount();

				float fMove_X, fMove_Y;
				for (int nIndex = 0; nIndex < 4; nIndex++)
				{
					fMove_X = cos(rand() % 90 + 1);
					if (fMove_X > 0.7)
					{
						fMove_X += -0.9;
					}
					else if (fMove_X < 0.3)
					{
						fMove_X += 0.4;
					}

					fMove_Y = sin(rand() % 90 + 1);
					if (fMove_Y < 0)
					{
						fMove_Y *= -1;
					}
					if (fMove_Y < 0.7)
					{
						fMove_Y += 0.6;
					}

					pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, fMove_X*0.8, fMove_Y*0.8, ENEMY_BULLET);
					(*unit)->nShoot_Count++;
				}

				if ((*unit)->nShoot_Count >= 200)
				{
					for (int nIndex = 0; nIndex < 44; nIndex++)
					{
						pBullet_Manager->Bullet_Add((*unit)->fLocation_X + fMuzzle_x, (*unit)->fLocation_Y + fMuzzle_y + CPlay::nMap_Y, sin(nIndex) / 2, cos(nIndex) / 2, ENEMY_BULLET);
					}
					(*unit)->nShoot_Count = 0;
				}
			}

		}
		break;
		}

	}
	return;
}