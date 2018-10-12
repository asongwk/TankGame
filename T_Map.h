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

	/*判断该参数代表的矩形是否和障碍背景发生碰撞
	true 表示相撞
	false 表示没有相撞
	*/
	bool IsCollide(Rect rec);
	bool IsCollide(int x, int y, int width, int height);

	/*得到一个可以放置坦克的矩形位置
	*/
	Rect GetRectOfEmptyOnMap(int tankWidth, int tankHeight);

	/*当炮弹击中时，更新地图数据，
	并返回击中障碍块的中心坐标
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

	T_Graph *m_picture;//保存位图图块

	HDC m_dc;//当前图层内存设备
	HBITMAP m_hbmpLayer;
	HBITMAP m_hbmpOldLayer;

	vector<vector<int> > m_backgroundData;//地图背景图层数据
	vector<vector<int> > m_barrierData;//地图障碍图层数据
private:
	/*下面的函数是专为解析tmx文件编写的，
	*/
	/*判断destStr中是否存在str*/
	bool IsExistStr(const wstring destStr, const wstring str);

	/*得到字符串str等号后面的值，并转为(int)整型*/
	int GetStrToInt(const wstring destStr, const wstring str);

	/*把字符串str解析为vector<int>存储类型，并返回 */
	vector<int> GetVectorData(const wstring str);

	/*判断字符串的格式是不是 （数字，数字）的格式*/
	BOOL IsNumCommaNum(const wstring str);
};

