#include "T_Engine.h"

T_Engine *T_Engine::m_wndProc = NULL;
HWND T_Engine::m_hwnd = NULL;

T_Engine::T_Engine(HINSTANCE hInstance, wstring wndName /*= L""*/, int wndWidth /*= 1200*/, int wndHeight /*= 700*/)
:m_hInstance(hInstance), m_wndName(wndName), m_wndWidth(wndWidth), m_wndHeight(wndHeight)
{
	m_wndProc = this;
	m_gameState = (GAME_STATE)GAME_START;
	m_lastGameState = m_gameState;

	Gdiplus::GdiplusStartupInput gdiplusStartupInput;
	Gdiplus::GdiplusStartup(&ptrGdiplusToken, &gdiplusStartupInput, NULL);

	bufferDC = CreateCompatibleDC(GetDC(m_hwnd));
	bufferBitmap = CreateCompatibleBitmap(GetDC(m_hwnd), m_wndWidth, m_wndHeight);
	SelectObject(bufferDC, bufferBitmap);
}


T_Engine::~T_Engine()
{
	Gdiplus::GdiplusShutdown(ptrGdiplusToken);// 关闭GDI+
}

void T_Engine::GameInit()
{
	wchar_t *szClassName = L"ClassName";
	WNDCLASSEX wndClass = { 0 };
	wndClass.cbSize = sizeof(WNDCLASSEX);
	wndClass.style = CS_HREDRAW | CS_VREDRAW;
	wndClass.lpfnWndProc = WndProc;
	wndClass.cbClsExtra = 0;
	wndClass.cbWndExtra = 0;
	wndClass.hInstance = m_hInstance;
	wndClass.hIcon = ::LoadIcon(m_hInstance, MAKEINTRESOURCEW(IDI_TANK));
	wndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndClass.lpszClassName = szClassName;
	//wndClass.hIconSm = ::LoadIconW(m_hInstance, MAKEINTRESOURCE(IDI_ICON_TANK));

	if (!RegisterClassEx(&wndClass))
		return;
	m_wndx = 60;
	m_wndy = 0;
	m_hwnd = CreateWindow(
		szClassName,
		m_wndName.c_str(),
		WS_OVERLAPPEDWINDOW&~WS_SIZEBOX&~WS_MAXIMIZEBOX,
		m_wndx,
		m_wndy, //cw_usede fault
		m_wndWidth,
		m_wndHeight,
		NULL,
		NULL,
		m_hInstance,
		NULL
		);
	if (m_hwnd == NULL)
	{
		::MessageBox(NULL, L"创建窗口出错", L"error", MB_OK);
		return;
	}
	ShowWindow(m_hwnd, SW_NORMAL);
	UpdateWindow(m_hwnd);
}

void T_Engine::StartEngine()
{
	GameInit();
	
	static int lastTick = GetTickCount();
	MSG msg;
	while (1)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (WM_QUIT == msg.message)
				break;
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else{
			int nowTick = GetTickCount();
			if (nowTick - lastTick > m_frame)
			{
				lastTick = nowTick + m_frame;

				GameLogic();
				GamePaint(bufferDC);
				HDC hdc = GetDC(m_hwnd);
				BitBlt(hdc, 0, 0, m_wndWidth, m_wndHeight,
					bufferDC, 0, 0, SRCCOPY);
				ReleaseDC(m_hwnd, hdc);
			}
		}
	}
	GameEnd();
}



LRESULT CALLBACK T_Engine::WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	return m_wndProc->GameEvent(hwnd, message, wParam, lParam);
}

LRESULT T_Engine::GameEvent(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	switch (msg)
	{
	case WM_CREATE:
		m_hwnd = hwnd;
		GameCreate();
		return 0;
	case WM_PAINT:
		PAINTSTRUCT ps;
		hdc = BeginPaint(hwnd, &ps);
		GamePaint(hdc);
		EndPaint(hwnd, &ps);
		return 0;
	case WM_MOUSEMOVE:
		GameMouseAction(LOWORD(lParam), HIWORD(lParam), MOUSE_MOVE);
		return 0;
	case WM_LBUTTONDOWN:
		GameMouseAction(LOWORD(lParam), HIWORD(lParam), MOUSE_LCLICK);
		return 0;
	case WM_KEYDOWN:
		GameKeyAction(wParam);
		return 0;
	case WM_KEYUP:
		GameKeyAction(wParam, true);
		return 0;
	case WM_SYSCOMMAND:
		if (wParam == SC_CLOSE){
			if (m_gameState == GAME_RUN_SINGLE || m_gameState == GAME_RUN_DOUBLE || m_gameState == GAME_HELP){
				m_gameState = GAME_START;
				ResetData();
				PlayGameSound(SOUND_MENU);
				SendMessage(hwnd, WM_PAINT, 0, 0);
			}
			else{
				DestroyWindow(hwnd);
			}
		}
		else{
			return DefWindowProc(hwnd, WM_SYSCOMMAND, wParam, lParam);
		}
		return 0;

	case WM_DESTROY:
		GameEnd();
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}




