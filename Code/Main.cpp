#include "GameTest.h"

int WINAPI  WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	GameTest *gm = new GameTest(hInstance, L"TankBattle",1024,768);
	gm->m_frame = 10;
	gm->StartEngine();

	return 0;
}

