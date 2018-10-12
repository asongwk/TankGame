#include "T_Map.h"


T_Map::T_Map()
{
	srand(unsigned int(time(0)));
	m_tileWidth = 0;
	m_tileHeight = 0;
	m_tileRows = 0;
	m_tileCols = 0;
	m_mapWidth = 0;
	m_mapHeight = 0;
	m_updated = true;

	m_picture = NULL;
	m_dc = CreateCompatibleDC(GetDC(0));
	m_hbmpLayer = NULL;
	m_hbmpOldLayer = NULL;
}


T_Map::~T_Map()
{
	if (m_hbmpLayer != NULL)
		SelectObject(m_dc, m_hbmpOldLayer);
	if (m_hbmpLayer != NULL)
		DeleteObject(m_hbmpLayer);
	if (m_hbmpOldLayer != NULL)
		DeleteObject(m_hbmpOldLayer);
	DeleteDC(m_dc);
	m_hbmpLayer = NULL;
	m_hbmpOldLayer = NULL;
}

void T_Map::LoadTmxFile(const char *tmxFileName)
{
	/*
	函数中写的是解析tmx文件的算法，tmx文件中的数据格式是很整齐的，
	算法很依赖文件数据的格式，比如换行，逗号，双引号
	*/
	string str;
	wifstream infile(tmxFileName, ios::in);
	if (!infile)
	{
		::OutputDebugString(L"LoadTmxFile( ) can\'t open tmx file\n");
		return;
	}
	wchar_t *l_str = new wchar_t[1024];
	wstring line_str;

	int key = 0;
	int vflag = 1;
	wchar_t *filename = new wchar_t[1024];
	while (!infile.eof())
	{
		infile.getline(l_str, 1024);
		line_str = wstring(l_str);

		//判断line_str 是否含有<map  <image <layer
		if (wstring::npos != line_str.find(L"<map"))
			key = 1;
		if (wstring::npos != line_str.find(L"<image"))
			key = 2;
		if (wstring::npos != line_str.find(L"<layer"))
			key = 3;
		//如有包含上述三个字符串,分别对应1,2,3 得到key(int)的值，
		int pos1, pos2;

		wstring strt;
		int i = 0;
		switch (key)
		{
		case 1:
			//得到m_tileRows,m_tileCols,m_tileWidth,m_tileHeight,
			m_tileRows = GetStrToInt(line_str, L"height");
			m_tileCols = GetStrToInt(line_str, L"width");
			m_tileWidth = GetStrToInt(line_str, L"tilewidth");
			m_tileHeight = GetStrToInt(line_str, L"tileheight");
			m_mapWidth = m_tileCols*m_tileWidth;
			m_mapHeight = m_tileRows*m_tileHeight;

			key = 0;
			break;
		case 2:
			//给m_picture赋值
			pos1 = line_str.find(L"\"");
			pos1++;
			pos2 = line_str.find(L"\"", pos1);
			pos2;
			strt = line_str.substr(pos1, pos2 - pos1);
			//wcscpy_s(filename, strt.length(), strt.c_str());
			strt = L"res\\game\\" + strt;
			for (auto it : strt)
			{
				filename[i++] = it;
			}
			filename[i] = L'\0';
			m_picture = new T_Graph(filename);
			key = 0;
			break;
		case 3:
			while (!infile.eof()){
				/*判断line_str是否有</layer>
				如果有
				更换 存储容器
				如果没有
				如果是（数字，数字）格式就存入到m_backgroundData中，
				*/
				if (wstring::npos != line_str.find(L"</layer>"))
				{
					vflag = 2;
				}
				if (IsNumCommaNum(line_str))
				{
					if (vflag == 1)
						m_backgroundData.push_back(GetVectorData(line_str));
					else if (vflag == 2)
						m_barrierData.push_back(GetVectorData(line_str));
				}
				infile.getline(l_str, 1024);
				line_str = wstring(l_str);
			}
			break;
		}


	}
	m_width = m_mapWidth;
	m_height = m_mapHeight;
	delete l_str;
	delete filename;
}

void T_Map::Draw(HDC hdc)
{
	if (m_updated)
	{
		Redraw();
	}

	BLENDFUNCTION frame_bf;
	frame_bf.BlendOp = AC_SRC_OVER;
	frame_bf.BlendFlags = 0;
	frame_bf.SourceConstantAlpha = 255;
	frame_bf.AlphaFormat = AC_SRC_ALPHA;
	AlphaBlend(hdc, 0, 0,
		m_mapWidth, m_mapHeight,
		m_dc,
		0, 0,
		m_mapWidth, m_mapHeight,
		frame_bf);
}

void T_Map::Redraw()
{
	if (m_hbmpLayer == NULL)
	{
		if (m_mapWidth != 0 && m_mapHeight != 0)
		{
			m_hbmpLayer = T_Graph::CreateBlankBitmap(m_mapWidth, m_mapHeight, Color::White);
			m_hbmpOldLayer = (HBITMAP)SelectObject(m_dc, m_hbmpLayer);
		}
	}

	/*
	把背景图块绘制到设备中
	*/
	HDC memDC = CreateCompatibleDC(GetDC(0));
	T_Graph t(L"res\\game\\tiles.png");
	HBITMAP memOldBmp = (HBITMAP)SelectObject(memDC, m_picture->GetBmpHandle());


	int row = 0, col = 0;
	for (auto it1 : m_backgroundData)
	{
		for (auto it2 : it1)
		{
			int x_tile, y_tile;
			x_tile = ((it2 - 1) % (m_picture->GetImageWidth() / m_tileWidth))* m_tileWidth;
			y_tile = (it2 - 1) / (m_picture->GetImageWidth() / m_tileWidth) *m_tileHeight;
			BLENDFUNCTION frame_bf;
			frame_bf.BlendOp = AC_SRC_OVER;
			frame_bf.BlendFlags = 0;
			frame_bf.SourceConstantAlpha = 255;
			frame_bf.AlphaFormat = AC_SRC_ALPHA;

			AlphaBlend(m_dc, col*m_tileWidth, row*m_tileHeight, m_tileWidth, m_tileHeight,
				memDC, x_tile, y_tile, m_tileWidth, m_tileHeight, frame_bf);
			col++;
		}
		row++;
		col = 0;

	}
	col = 0, row = 0;
	for (auto it1 : m_barrierData)
	{
		for (auto it2 : it1)
		{
			if (it2 != 0){
				int x_tile, y_tile;
				x_tile = ((it2 - 1) % (m_picture->GetImageWidth() / m_tileWidth))* m_tileWidth;
				y_tile = (it2 - 1) / (m_picture->GetImageWidth() / m_tileWidth) *m_tileHeight;
				BLENDFUNCTION frame_bf;
				frame_bf.BlendOp = AC_SRC_OVER;
				frame_bf.BlendFlags = 0;
				frame_bf.SourceConstantAlpha = 255;
				frame_bf.AlphaFormat = AC_SRC_ALPHA;

				AlphaBlend(m_dc, col*m_tileWidth, row*m_tileHeight, m_tileWidth, m_tileHeight,
					memDC, x_tile, y_tile, m_tileWidth, m_tileHeight, frame_bf);
			}
			col++;

		}
		row++;
		col = 0;

	}
	SelectObject(memDC, memOldBmp);
	DeleteDC(memDC);
	DeleteObject(memOldBmp);
	m_updated = false;
}

/*判断destStr中是否存在str*/
bool T_Map::IsExistStr(const wstring destStr, const wstring str)
{
	return wstring::npos != destStr.find(str);
}

/*得到字符串str等号后面的值，并转为(int)整型*/
int T_Map::GetStrToInt(const wstring destStr, const wstring str)
{
	int pos = destStr.find(str);
	pos += str.length() + 2;
	int pos2 = destStr.find(L"\"", pos);
	return _wtoi(destStr.substr(pos, pos2 - pos).c_str());
}

/*把字符串str解析为vector<int>存储类型，并返回 */
vector<int> T_Map::GetVectorData(const wstring str)
{
	vector<int> v;
	int value, pos1 = 0, pos2 = -1;
	while (pos2 = str.find(L",", pos1))
	{
		if (pos2 == wstring::npos)
			break;
		value = _wtoi(str.substr(pos1, pos2 - pos1).c_str());
		v.push_back(value);
		pos1 = pos2 + 1;
	}
	v.push_back(_wtoi(str.substr(pos1, str.length() - pos1 + 1).c_str()));
	return v;
}

BOOL T_Map::IsNumCommaNum(const wstring str)
{
	return wstring::npos != str.find(L",");
}

bool T_Map::IsCollide(Rect rec)
{
	int x = rec.X;
	int y = rec.Y;
	int width = rec.Width;
	int height = rec.Height;
	
	
	return IsCollide(x,y,width,height);
}

bool T_Map::IsCollide(int x, int y, int width, int height)
{
	if (x < 0 || y < 0){
		return true;
	}
	if (x + width > m_mapWidth-15 || y+height > m_mapHeight -35){
		return true;
	}
	int left = x / m_tileWidth;
	int right = (x + width) / m_tileWidth;
	int top = y / m_tileHeight;
	int bottom = (y + height) / m_tileHeight;
	
	if (right >= m_barrierData[0].size()){
		return true;
	}
	if (bottom >= m_barrierData.size()){
		return true;
	}
	for (int i = top; i <= bottom;i++){
		for (int j = left; j <= right;j++){
			if(0 != m_barrierData[i][j]){
				return true;
			}
		}
	}
	return false;
}

Rect T_Map::GetRectOfEmptyOnMap(int tankWidth,int tankHeight)
{
	
	int x = rand() % (m_mapWidth - tankWidth);
	int y = rand() % (m_mapHeight - tankHeight);

	Rect rec;
	rec.X = 0;
	rec.Y = 0;
	rec.Width = 0;
	rec.Height = 0;
	wchar_t str[30];
	while (true){
		
		rec.X= x;
		rec.Y = y;
		rec.Width = tankWidth;
		rec.Height =  tankHeight;
		if (IsCollide(rec) == false){
			
			wsprintf(str, L"\t(%d-%d)\n", rec.X, rec.Y);
			//::OutputDebugString(str);
			return rec;
		}
		x = rand() % (m_mapWidth - tankWidth);
		y = rand() % (m_mapHeight - tankHeight);
	}
	return rec;
}
POINT T_Map::UpdateMapBarrier(int x, int y, int bombWidth, int bombHeight)
{

	POINT pt;
	pt.x = -1;
	pt.y = -1;
	if (IsCollide(x,y,bombWidth,bombHeight) == false){
		return pt;
	}
	m_updated = true;
	int left = x / m_tileWidth;
	int right = (x + bombWidth) / m_tileWidth;
	int top = y / m_tileHeight;
	int bottom = (y + bombHeight) / m_tileHeight;

	for (int i = top; i <= bottom; i++){
		if (i >= m_barrierData.size())continue;
		for (int j = left; j <= right; j++){
			if (j >= m_barrierData[i].size())continue;
			if (0 != m_barrierData[i][j]){
				m_barrierData[i][j] = 0;
				pt.y = (i-1)*m_tileHeight + m_tileHeight / 2;
				pt.x = (j-1)*m_tileWidth + m_tileWidth / 2;
				return pt;
			}
		}
	}

	return pt;
}


