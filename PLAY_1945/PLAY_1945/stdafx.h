// stdafx.h : 자주 사용하지만 자주 변경되지는 않는
// 표준 시스템 포함 파일 또는 프로젝트 관련 포함 파일이
// 들어 있는 포함 파일입니다.
//

#pragma once

#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "msimg32.lib")

#include "targetver.h"

#define CLIENT_WIDTH 800
#define CLIENT_HEIGTH 1000

#define WIN32_LEAN_AND_MEAN             // 거의 사용되지 않는 내용은 Windows 헤더에서 제외합니다.
// Windows 헤더 파일:
#include <windows.h>

// C 런타임 헤더 파일입니다.
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>


// TODO: 프로그램에 필요한 추가 헤더는 여기에서 참조합니다.
#include <string>
#include <fstream>
#include<mmsystem.h>
#include<list>
#include<ctime>
#include<cstdlib>

using namespace std;

enum UNIT_NUM
{
	UNIT_ONE = 1,
	UNIT_TWO = 2,
	UNIT_THREE = 3,
	BOSS_ONE =4,
	BOSS_TWO =5,
	BOSS_THREE=6,
	BOSS_FOUR=7,
	BOSS_FIVE=8,
};

