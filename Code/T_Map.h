#pragma once
#include "T_Layer.h"

/*
��Ϸ��ͼ��
*/

class T_Map
	:public T_Layer
{
public:
	T_Map();
	~T_Map();

	void LoadTmxFile(const char *tmxFileName);
	void Draw(HDC hdc);
	void Redraw();

	/*�жϸò�������ľ����Ƿ���ϰ�����������ײ
	true ��ʾ��ײ
	false ��ʾû����ײ
	*/
	bool IsCollide(Rect rec);
	bool IsCollide(int x, int y, int width, int height);

	/*�õ�һ�����Է���̹�˵ľ���λ��
	*/
	Rect GetRectOfEmptyOnMap(int tankWidth, int tankHeight);

	/*���ڵ�����ʱ�����µ�ͼ���ݣ�
	�����ػ����ϰ������������
	*/
	POINT UpdateMapBarrier(int x, int y ,int bombWidth, int bombHeight);


private:
	int m_tileWidth;
	int m_tileHeight;
	int m_tileRows;
	int m_tileCols;
	int m_mapWidth;
	int m_mapHeight;

	bool m_updated;	

	T_Graph *m_picture;//����λͼͼ��

	HDC m_dc;//��ǰͼ���ڴ��豸
	HBITMAP m_hbmpLayer;
	HBITMAP m_hbmpOldLayer;

	vector<vector<int> > m_backgroundData;//��ͼ����ͼ������
	vector<vector<int> > m_barrierData;//��ͼ�ϰ�ͼ������
private:
	/*����ĺ�����רΪ����tmx�ļ���д�ģ�
	*/
	/*�ж�destStr���Ƿ����str*/
	bool IsExistStr(const wstring destStr, const wstring str);

	/*�õ��ַ���str�Ⱥź����ֵ����תΪ(int)����*/
	int GetStrToInt(const wstring destStr, const wstring str);

	/*���ַ���str����Ϊvector<int>�洢���ͣ������� */
	vector<int> GetVectorData(const wstring str);

	/*�ж��ַ����ĸ�ʽ�ǲ��� �����֣����֣��ĸ�ʽ*/
	BOOL IsNumCommaNum(const wstring str);
};

