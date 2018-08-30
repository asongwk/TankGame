#pragma once
#include "T_Layer.h"

/*
游戏地图类
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

	
	bool IsCollide(Rect rec);
	bool IsCollide(int x, int y, int width, int height);

	/*
	*/
	Rect GetRectOfEmptyOnMap(int tankWidth, int tankHeight);

	/*
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

	T_Graph *m_picture;

	HDC m_dc;//
	HBITMAP m_hbmpLayer;
	HBITMAP m_hbmpOldLayer;

	vector<vector<int> > m_backgroundData;//
	vector<vector<int> > m_barrierData;//
private:
	/*
	*/
	/**/
	bool IsExistStr(const wstring destStr, const wstring str);

	/**/
	int GetStrToInt(const wstring destStr, const wstring str);

	
	vector<int> GetVectorData(const wstring str);

	
	BOOL IsNumCommaNum(const wstring str);
};

