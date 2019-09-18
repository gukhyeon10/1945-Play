#pragma once

enum BITMAP_NAME
{
	START_MENU_BACK_IMAGE = 0,
	START=1,
	HELP=2,
	QUIT=3,
	SELECT_POINTER=4,
	KEY_HELP=5,
	GAME_OVER=6,
	MISSION_CLEAR=7,
};

enum STAGE_NUMBER
{
	STAGE_ONE=1,
	STAGE_TWO=2,
	STAGE_THREE=3,
	STAGE_FOUR=4,
	STAGE_FIVE =5,
	STAGE_CLEAR=6,
};

enum USER_INTERFACE
{
	NUMBER=0,
	BOMB_ICON=1,
	LIFE_ICON=2,
};

class CInterface
{
private:
	HBITMAP hInterface_Image[8]; //첫 화면 메뉴 이미지
	HBITMAP hNext_Stage[7];   //스테이지 이미지 
	HBITMAP hUser_Interface[3]; // 숫자들과 필살기, 생명 아이콘
	string Map_File_Name;
	string Bgm_File_Name;
	HBITMAP hBackGround;
	int nSelect_Number;
	int nBg_Width, nBg_Heigth;
public:
	int Start_Menu(HDC hdc);
	void Show_Help(HDC hdc);
	void Next_Stage_Waiting(int nStage_Number, HDC hdc);
	void Setting(ifstream *pInFile, int nStage);
	void Render(HDC MainDC);
	void Info_Render(HDC MainDC, int nLife, int nBomb, int nScroe);
	void Mission_Clear_Render(HDC MainDC, int Y);
	void Game_Over_Render(HDC MainDC);
	CInterface(HINSTANCE hInst);
	~CInterface();
};

