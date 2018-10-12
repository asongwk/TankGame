#pragma once
#include "T_Config.h"

//ͼ�����
enum TRANSFER{ // transfer
	TRANS_NONE, //ԭʼͼ��
	TRANS_NOFLIP_ROT90,//˳ʱ����ת90��  noflip
	TRANS_NOFLIP_ROT180,//ԭʼͼ��˳ʱ����ת180����noflip
	TRANS_NOFLIP_ROT270,//ԭʼͼ��˳ʱ����ת180����noflip

	TRANS_VFLIP_NOROT,//ԭʼͼ��ֱ��ת
	TRANS_VFLIP_ROT90,//ԭʼͼ��ֱ��ת��˳ʱ����ת90��
	TRANS_VFLIP_ROT180,//ԭʼͼ��ֱ��ת��˳ʱ����ת180��
	TRANS_VFLIP_ROT270,//ԭʼͼ��ֱ��ת��˳ʱ����ת270��

	TRANS_HFLIP_NOROT,//ԭʼͼ��ˮƽ��ת
	TRANS_HFLIP_ROT90,//ԭʼͼ��ˮƽ��ת��˳ʱ����ת90��
	TRANS_HFLIP_ROT180,//ԭʼͼ��ˮƽ��ת��˳ʱ����ת180��
	TRANS_HFLIP_ROT270,//ԭʼͼ��ˮƽ��ת��˳ʱ����ת270��

};

class T_Graph
{
public:
	T_Graph();
	T_Graph(LPTSTR fileName);  // ʹ��ָ���ļ�����T_Graph����
	virtual ~T_Graph();

	HBITMAP GetBmpHandle(){ return hBmp; }  //����Ѿ�����ͼ����
	int GetImageWidth(){ return imageWidth; }  //����Ѿ�����ͼ���
	int GetImageHeight(){ return imageHeight; }  //����Ѿ�����ͼ���

public:
	//����ͼ��(BMP,GIF,JPEG,PNG,TIFF)
	bool LoadImageFile(LPCTSTR path);

	void PaintImage(HDC hdc, int x, int y);
	void PaintImage(HDC hdc, int x, int y, int width, int height);
	void PaintImage(HDC hdc, int x, int y, int width, int height, BYTE alpha);

	void Destroy();  //�ͷ���Դ

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
	HBITMAP hBmp;  // �Ѿ����ص�ͼ����
	int imageWidth;  // �Ѿ�����ͼ���
	int imageHeight;  // �Ѿ�����ͼ���
};

