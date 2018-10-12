#pragma once
#include "T_Config.h"
#include "T_Graph.h"

/*
�˵���
*/

class T_Menu
{
public:
	/*�ô��ڵĿ�߳�ʼ���˵�*/
	T_Menu(int wndWidth,int wndHeight);

	~T_Menu();

	/*�ڴ����ϻ��Ʋ˵�*/
	void DrawMenu(HDC hdc);

	/*��������õ��˵�ѡ��������ֵΪ-1����ʾ��ûѡ��*/
	int GetMenuIndex(int x,int y);

	/*�õ��������Ĳ˵�ѡ��*/
	MENU_INDEX GetMenuIndexMouseClick(int x, int y);

	void SetMouseCoordinate(int x, int y){ m_mouseX = x; m_mouseY = y; }

private:
	T_Graph *m_back; //�˵�����ͼ
	int m_wndWidth;
	int m_wndHeight;

	int m_mouseX;
	int m_mouseY;
	
	int m_rectX; //�˵�ѡ��ĺ�����
	int m_rectY;//�˵�ѡ��ĺ�����
	int m_rectWidth;//�˵�ѡ��Ŀ�
	int m_rectHeight;//�˵�ѡ��ĸ�

	const int ROWS;
	const int COLS;
};



