#pragma once
#include "T_Config.h"
#include "T_Graph.h"

/*
菜单类
*/

class T_Menu
{
public:
	/*用窗口的宽高初始化菜单*/
	T_Menu(int wndWidth,int wndHeight);

	~T_Menu();

	/*在窗口上绘制菜单*/
	void DrawMenu(HDC hdc);

	/*根据坐标得到菜单选项，如果返回值为-1，表示都没选中*/
	int GetMenuIndex(int x,int y);

	/*得到鼠标点击后的菜单选项*/
	MENU_INDEX GetMenuIndexMouseClick(int x, int y);

	void SetMouseCoordinate(int x, int y){ m_mouseX = x; m_mouseY = y; }

private:
	T_Graph *m_back; //菜单背景图
	int m_wndWidth;
	int m_wndHeight;

	int m_mouseX;
	int m_mouseY;
	
	int m_rectX; //菜单选项的横坐标
	int m_rectY;//菜单选项的横坐高
	int m_rectWidth;//菜单选项的宽
	int m_rectHeight;//菜单选项的高

	const int ROWS;
	const int COLS;
};



