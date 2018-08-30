#include "T_Graph.h"


T_Graph::T_Graph()
{
	hBmp = NULL;
}


T_Graph::T_Graph(LPTSTR fileName)  // 
{
	LoadImageFile(fileName);
}


T_Graph::~T_Graph()
{
	Destroy();
}


/*
*/
bool T_Graph::LoadImageFile(LPCTSTR path)
{
	Bitmap* pBmp = NULL;
	pBmp = Bitmap::FromFile(path);
	if (!pBmp){
		return false;
	}
	imageWidth = pBmp->GetWidth();
	imageHeight = pBmp->GetHeight();
	if (imageWidth == 0 || imageHeight == 0){
		::OutputDebugString((wstring(path) + L" load fail\n").c_str());
		return false;
	}
	else
	{
		//::OutputDebugString((wstring(path) + L" load success\n").c_str());
	}

	if (imageHeight == 0 || imageWidth == 0)
		return false;
	Status status = pBmp->GetHBITMAP(NULL, &hBmp);
	if (pBmp)
	{
		delete pBmp;
		pBmp = NULL;
	}
	if (status == S_OK)
	{
		return true;
	}
	else
	{
		return false;
	}
}
void T_Graph::Destroy()  //释放资源
{
	if (hBmp)
	{
		DeleteObject(hBmp);
		hBmp = NULL;
	}
}



void T_Graph::PaintImage(HDC hdc, int x, int y)
{
	HDC memDC = CreateCompatibleDC(hdc);  //  
	HBITMAP OldFrameBmp = (HBITMAP)SelectObject(memDC, hBmp);  // 

	BitBlt(hdc, x, y, imageWidth, imageHeight, memDC, 0, 0, SRCCOPY);
	
	SelectObject(memDC, OldFrameBmp);
	DeleteDC(memDC);  //
	DeleteObject(OldFrameBmp);  //
}


void T_Graph::PaintImage(HDC hdc, int x, int y, int width, int height)
{
	HDC memDC = CreateCompatibleDC(hdc);  // 
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBmp);  // 

	SetStretchBltMode(hdc, COLORONCOLOR);  


	StretchBlt(hdc, x, y, width, height, memDC, 0, 0, imageWidth, imageHeight, SRCCOPY);


	SelectObject(memDC, hOldBitmap);
	DeleteDC(memDC);  // 删除内存设备
	DeleteObject(hOldBitmap);  // 删除位图对象
}


void T_Graph::PaintImage(HDC hdc, int x, int y, int width, int height, BYTE alpha)
{
	HDC memDC = CreateCompatibleDC(hdc);  //  
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBmp);  // 

	BLENDFUNCTION dc_bf;
	dc_bf.BlendOp = AC_SRC_OVER;
	dc_bf.BlendFlags = 0;
	dc_bf.SourceConstantAlpha = alpha;
	dc_bf.AlphaFormat = AC_SRC_ALPHA;
	SetStretchBltMode(hdc, COLORONCOLOR);  //

	AlphaBlend(hdc, x, y, width, height, memDC, 0, 0, imageWidth, imageHeight, dc_bf);

	
	SelectObject(memDC, hOldBitmap);
	DeleteDC(memDC);  // 
	DeleteObject(hOldBitmap);  // 
}




HBITMAP T_Graph::CreateBlankBitmap(int width, int height, COLORREF color)
{
	BYTE *pBits = NULL;
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width*height * 4;
	HBITMAP blankBmp = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS, (void**)&pBits,
		NULL, NULL);
	PaintBlank(blankBmp, width, height, color);
	return blankBmp;
}

// HBITMAP_To_Bitmap
Bitmap* T_Graph::HBITMAP_To_Bitmap(HBITMAP hbmp, HDC hdc)
{
	BITMAP bmp;
	int width, height;

	memset((void*)&bmp, 0, sizeof(BITMAP));
	GetObject(hbmp, sizeof(BITMAP), (void*)&bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;
	byte* bmpBytes = (byte*)bmp.bmBits;
	if (width <= 0 || height <= 0)
		return NULL;

	Bitmap* BmpCpy = new Bitmap(width, height, PixelFormat32bppPARGB);

	BitmapData bmpData;
	Rect rect(0, 0, width, height);
	BmpCpy->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppPARGB, &bmpData);

	int lineSize = width * 4;
	byte* cpyBytes = (byte*)(bmpData.Scan0);

	for (int y = 0; y < height; y++)
	{
		memcpy((y*lineSize) + cpyBytes, ((height - y - 1)*lineSize) + bmpBytes, lineSize);
	}

	BmpCpy->UnlockBits(&bmpData);
	return BmpCpy;
}


void T_Graph::PaintRegion(HBITMAP in_hbitmap, HDC destDC, int destX, int destY,
	int srcX, int srcY, int regionWidth, int regionHeight,
	float ratio, int rotType /*= 0*/, BYTE alpha /*= 255*/)
{

	int width = (int)(regionWidth*ratio);
	int height = (int)(regionHeight*ratio);

	BYTE* pBits = NULL;
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width*height * 4;


	HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS,
		(void**)&pBits, NULL, NULL);
	HDC frameDC = ::CreateCompatibleDC(NULL); //创建局部位图对应的设备

	HBITMAP OldFrameBmp = (HBITMAP)SelectObject(frameDC, hbitmap);
	HDC memDC = CreateCompatibleDC(destDC);

	HBITMAP OldMemBmp = (HBITMAP)SelectObject(memDC, in_hbitmap);
	BLENDFUNCTION frame_bf; //blendfunction
	frame_bf.BlendFlags = 0;
	frame_bf.BlendOp = AC_SRC_OVER;
	frame_bf.SourceConstantAlpha = 255;
	frame_bf.AlphaFormat = AC_SRC_ALPHA;
	//用AlphaBlend函数从原始位图设备中读取局部位图到局部位图设备
	AlphaBlend(frameDC, 0, 0, (int)(regionWidth*ratio), (int)(regionHeight*ratio),
		memDC, srcX, srcY, regionWidth, regionHeight, frame_bf);
	//通过自定义函数HBITMAP_To_Bitmap获取对应的Bitmap对象
	Bitmap* nowFrameBmp = HBITMAP_To_Bitmap(hbitmap, destDC);
	SelectObject(frameDC, OldFrameBmp); //使用原来对象还原内存设备中的位图对象
	DeleteDC(frameDC);  //删除内存设备
	DeleteObject(OldFrameBmp);// 删除位图对象
	OldFrameBmp = NULL;
	DeleteObject(hbitmap);//删除位图对象
	hbitmap = NULL;
	SelectObject(memDC, OldMemBmp);
	DeleteDC(memDC);
	DeleteObject(OldMemBmp);
	OldMemBmp = NULL;

	//通过GDI+的RotateFlip函数对局部位图进行旋转或翻转
	//注意涉及90和270度时必须将宽高值进行调转
	switch (rotType)
	{
	case TRANS_NONE:
		break;
	case TRANS_NOFLIP_ROT90:
		nowFrameBmp->RotateFlip(Rotate90FlipNone);
		width = (int)(regionHeight*ratio);
		height = (int)(regionWidth*ratio);
		break;
	case TRANS_NOFLIP_ROT180:
		nowFrameBmp->RotateFlip(Rotate180FlipNone);
		width = (int)(regionHeight*ratio);
		height = (int)(regionWidth*ratio);
		break;
	}
	HBITMAP hbmp = NULL; //重新定义一个新的位图句柄
	nowFrameBmp->GetHBITMAP(NULL, &hbmp); //将处理后的Bitmap对象转换为位图句柄
	HDC bufDC = CreateCompatibleDC(destDC); //创建要绘制的目的设备
	HBITMAP OldbufBmp = (HBITMAP)SelectObject(bufDC, hbmp); // 将最终的位图选入设备

	BLENDFUNCTION buf_bf; //设定要AlphaBlend函数混合的参数
	buf_bf.BlendOp = AC_SRC_OVER;
	buf_bf.BlendFlags = 0;
	buf_bf.SourceConstantAlpha = alpha;
	buf_bf.AlphaFormat = AC_SRC_ALPHA;

	//用AlphaBlend函数把最终的局部位图从目的设备绘制到最终的目标设备
	AlphaBlend(destDC, destX, destY, width, height, bufDC, 0, 0, width, height, buf_bf);
	SelectObject(bufDC, OldbufBmp);
	DeleteDC(bufDC);
	DeleteObject(OldbufBmp);
	OldbufBmp = NULL;
	DeleteObject(hbmp);
	hbmp = NULL;
	DeleteObject(nowFrameBmp);
	delete nowFrameBmp;
	nowFrameBmp = NULL;
}


void T_Graph::PaintFrame(HBITMAP in_hbitmap, HDC destDC, int destX, int destY,
	int FrameCount, int RowFrames, int wFrame, int hFrame,
	float ratio /*= 1*/, int rotType /*= 0*/, BYTE alpha /*= 255*/)
{
	int col = FrameCount % RowFrames;  // 根据动画总帧数和每行数帧数计算出每一张帧图在原图中的行列号
	int row = (FrameCount - col) / RowFrames;
	PaintRegion(in_hbitmap, destDC, destX, destY, col*wFrame, row*hFrame,
		wFrame, hFrame, ratio, rotType, alpha);
}


void T_Graph::PaintText(HDC hdc, RectF fontRect, LPCTSTR text,
	REAL fontSize, LPCTSTR fontName,
	Color fontColor /*= Color::White*/,
	FontStyle style /*= FontStyleBold*/,
	StringAlignment align /*= StringAlignmentCenter*/)
{
	Graphics graph(hdc); /*使用GDI+方法进行绘制*/
	FontFamily fontFamily(fontName);
	Font font(&fontFamily, fontSize, style, UnitPoint);
	StringFormat format;
	format.SetLineAlignment(StringAlignmentCenter);
	format.SetAlignment(align);
	SolidBrush solidBrush1(fontColor);
	graph.SetTextRenderingHint(TextRenderingHintAntiAlias);
	graph.DrawString(text, (INT)wcslen(text), &font, fontRect, &format, &solidBrush1);
	graph.ReleaseHDC(hdc);
}

/*实现空白位图的绘制*/

/*将第一个位图句柄参数指定的位图填充最后一个参数指定的颜色*/
void T_Graph::PaintBlank(HBITMAP hbmp, int width, int height, COLORREF crColor)
{
	HDC memdc = ::CreateCompatibleDC(NULL);
	HBITMAP OldBmp = (HBITMAP)SelectObject(memdc, hbmp);

	HBRUSH hBrush = CreateSolidBrush(crColor);
	RECT rcBitmap = { 0, 0, width, height };
	FillRect(memdc, &rcBitmap, hBrush);

	SelectObject(memdc, OldBmp);  //回复内存设备中的位图对象
	DeleteDC(memdc);  // 删除内存设备
	DeleteObject(OldBmp); // 删除位图对象
	OldBmp = NULL; // 释放句柄 
	DeleteObject(hBrush); //删除画刷

}

/*采用GDI+实现，其中位图的透明度和颜色通过Color参数来指定*/
void T_Graph::PaintBlank(HDC hdc, int x, int y, int width, int height, Color crColor)
{
	Gdiplus::Graphics g(hdc);
	Rect rectangle(x, y, width, height);
	SolidBrush solidBrush(crColor);
	g.FillRectangle(&solidBrush, rectangle);
	g.ReleaseHDC(hdc);
}

/*采用GDI的AlphaBlend实现绘制空白位图，*/
void T_Graph::PaintBlank(HDC hdc, int x, int y, int width, int height,
	COLORREF crColor, int alpLevel)
{
	HBITMAP hbitmap = CreateCompatibleBitmap(hdc, width, height);
	HDC memdc = ::CreateCompatibleDC(NULL);
	HBITMAP OldBmp = (HBITMAP)SelectObject(memdc, hbitmap);

	HBRUSH hBrush = CreateSolidBrush(crColor);
	RECT rcBitmap = { x, y, width, height };
	FillRect(memdc, &rcBitmap, hBrush);

	BLENDFUNCTION frame_bf;  // blendfunction
	frame_bf.BlendOp = AC_SRC_OVER;
	frame_bf.SourceConstantAlpha = alpLevel;
	frame_bf.AlphaFormat = 0;
	AlphaBlend(hdc, x, y, width, height, memdc, 0, 0, width, height, frame_bf);

	SelectObject(memdc, OldBmp);  //恢复内存设备中的位图对象s
	DeleteDC(memdc);  //删除内存设备
	DeleteObject(OldBmp);  //删除位图对象
	OldBmp = NULL;
	DeleteObject(hbitmap); //删除
	hbitmap = NULL;
	DeleteObject(hBrush);

}

