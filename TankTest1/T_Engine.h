/*
T_Engine.h
游戏的底层引擎类，实现了整个游戏的游戏循环和游戏逻辑

*/

#pragma once
#include "T_Config.h"
#include "T_Graph.h"


class T_Engine
{
public:
	T_Engine(HINSTANCE hInstance, wstring wndName = L"", int wndWidth = 1200, int wndHeight = 700);
	virtual ~T_Engine();

	void StartEngine();
	void GameInit();
public:
	static T_Engine *m_wndProc;
	static LRESULT CALLBACK WndProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT GameEvent(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	virtual void GameCreate() = 0;
	virtual void GamePaint(HDC hdc) = 0;
	virtual void GameLogic() = 0;
	virtual void GameKeyAction(WPARAM wParam, bool isUp = false) = 0;
	virtual void GameMouseAction(int, int, int) = 0;
	virtual void GameEnd() = 0;

	virtual void ResetData() = 0;
	virtual void LoadGameSound(HWND hwnd)=0;
	virtual void PlayGameSound(SOUND_INDEX)=0;
public:
	int GetFrame(){ return m_frame; }
	void SetFrame(int f){ m_frame = f; }
	int GetWndWidth(){ return m_wndWidth; }
	int GetWndHeight(){ return m_wndHeight; }
	int GetWndX(){ return m_wndx; }
	int GetWndY(){ return m_wndy; }
	void SetWndName(wstring n){ m_wndName = n; }

public:
	static HWND m_hwnd;
	int m_frame;
	wstring m_wndName;
	int m_wndx;
	int m_wndy;
	int m_wndWidth;
	int m_wndHeight;

	//HINSTANCE hInstance, wstring wndName = "", int wndWidth = 800, int wndHeight = 600
	HINSTANCE m_hInstance;
	//wstring wndName;

	ULONG_PTR ptrGdiplusToken;
	HDC bufferDC;
	HBITMAP bufferBitmap;

public:
	enum GAME_STATE m_gameState;
	enum GAME_STATE m_lastGameState;
};

