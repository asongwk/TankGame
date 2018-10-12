#pragma once
#include "T_Config.h"

//图像绘制
enum TRANSFER{ // transfer
	TRANS_NONE, //原始图像
	TRANS_NOFLIP_ROT90,//顺时针旋转90度  noflip
	TRANS_NOFLIP_ROT180,//原始图像顺时针旋转180读，noflip
	TRANS_NOFLIP_ROT270,//原始图像顺时针旋转180读，noflip

	TRANS_VFLIP_NOROT,//原始图像垂直翻转
	TRANS_VFLIP_ROT90,//原始图像垂直翻转并顺时针旋转90度
	TRANS_VFLIP_ROT180,//原始图像垂直翻转并顺时针旋转180度
	TRANS_VFLIP_ROT270,//原始图像垂直翻转并顺时针旋转270度

	TRANS_HFLIP_NOROT,//原始图像水平翻转
	TRANS_HFLIP_ROT90,//原始图像水平翻转并顺时针旋转90度
	TRANS_HFLIP_ROT180,//原始图像水平翻转并顺时针旋转180度
	TRANS_HFLIP_ROT270,//原始图像水平翻转并顺时针旋转270度

};

class T_Graph
{
public:
	T_Graph();
	T_Graph(LPTSTR fileName);  // 使用指定文件创建T_Graph对象
	virtual ~T_Graph();

	HBITMAP GetBmpHandle(){ return hBmp; }  //获得已经加载图像句柄
	int GetImageWidth(){ return imageWidth; }  //获得已经加载图像宽
	int GetImageHeight(){ return imageHeight; }  //获得已经加载图像高

public:
	//加载图像(BMP,GIF,JPEG,PNG,TIFF)
	bool LoadImageFile(LPCTSTR path);

	void PaintImage(HDC hdc, int x, int y);
	void PaintImage(HDC hdc, int x, int y, int width, int height);
	void PaintImage(HDC hdc, int x, int y, int width, int height, BYTE alpha);

	void Destroy();  //释放资源

public:
	static HBITMAP CreateBlankBitmap(int width, int height, COLORREF color);

	static Bitmap* HBITMAP_To_Bitmap(HBITMAP hbmp, HDC hdc);

	static void PaintRegion(HBITMAP in_hbitmap, HDC destDC, int destX, int destY,
		int srcX, int srcY, int regionWidth, int regionHeight,
		float ratio, int rotType = 0, BYTE alpha = 255);

	static void PaintFrame(HBITMAP in_hbitmap, HDC destDC, int destX, int destY,
		int FrameCount, int RowFrames, int wFrame, int hFrame,
		float ratio = 1, int rotType = 0, BYTE alpha = 255);

	static void PaintText(HDC hdc, RectF fontRect, LPCTSTR text,
		REAL fontSize, LPCTSTR fontName,
		Color fontColor = Color::White,
		FontStyle style = FontStyleBold,
		StringAlignment align = StringAlignmentCenter);

	static void PaintBlank(HBITMAP hbmp, int width, int height, COLORREF crColor);
	static void PaintBlank(HDC hdc, int x, int y, int width, int height, Color crColor);
	static void PaintBlank(HDC hdc, int x, int y, int width, int height,
		COLORREF crColor, int alpLevel);

public:
	HBITMAP hBmp;  // 已经加载的图像句柄
	int imageWidth;  // 已经加载图像宽
	int imageHeight;  // 已经加载图像高
};

