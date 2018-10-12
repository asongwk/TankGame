#include "T_Graph.h"


T_Graph::T_Graph()
{
	hBmp = NULL;
}


T_Graph::T_Graph(LPTSTR fileName)  // ʹ��ָ���ļ�����T_Graph����
{
	LoadImageFile(fileName);
}


T_Graph::~T_Graph()
{
	Destroy();
}


/*����ͼ��(BMP,GIF,JPEG,PNG,TIFF)
����GDI+ʵ��λͼ�ļ����أ����سɹ���ת��Ϊλͼ������浽hBmp�����ݳ�Ա�У�
������ȡ��λͼ����ֵ���浽imageWidth,imageHeight�����ݳ�Ա��
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
void T_Graph::Destroy()  //�ͷ���Դ
{
	if (hBmp)
	{
		DeleteObject(hBmp);
		hBmp = NULL;
	}
}

/*PaintImage()������3��������ʽ��ȫ������GDI��API����ʵ��*/

/*ʵ��ԭʼλͼ�ļ򵥻��ƣ�����GDI��BitBlt()ʵ��ԭʼλͼ�Ŀ��ٿ���*/
void T_Graph::PaintImage(HDC hdc, int x, int y)
{
	HDC memDC = CreateCompatibleDC(hdc);  //  ����֡ͼ���ڴ��豸
	HBITMAP OldFrameBmp = (HBITMAP)SelectObject(memDC, hBmp);  // ���հ׵�֡ͼѡ���ڴ��豸
	// ����ʹ��GDI��BitBlt����
	BitBlt(hdc, x, y, imageWidth, imageHeight, memDC, 0, 0, SRCCOPY);
	//��ԭ�ڴ��豸�����е�ԭ��λͼ����
	SelectObject(memDC, OldFrameBmp);
	DeleteDC(memDC);  // ɾ���ڴ��豸
	DeleteObject(OldFrameBmp);  // ɾ��λͼ����
}

/*ʵ��λͼ�����Ż���*/
void T_Graph::PaintImage(HDC hdc, int x, int y, int width, int height)
{
	HDC memDC = CreateCompatibleDC(hdc);  //  ����֡ͼ���ڴ��豸
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBmp);  // ���հ׵�֡ͼѡ���ڴ��豸

	SetStretchBltMode(hdc, COLORONCOLOR);  //��������ģʽ

	//ʵ�����Ż���
	StretchBlt(hdc, x, y, width, height, memDC, 0, 0, imageWidth, imageHeight, SRCCOPY);

	//��ԭ�ڴ��豸�����е�ԭ��λͼ����
	SelectObject(memDC, hOldBitmap);
	DeleteDC(memDC);  // ɾ���ڴ��豸
	DeleteObject(hOldBitmap);  // ɾ��λͼ����
}

/*ָ��͸���ȶ�ԭʼλͼ�������Ż���*/
void T_Graph::PaintImage(HDC hdc, int x, int y, int width, int height, BYTE alpha)
{
	HDC memDC = CreateCompatibleDC(hdc);  //  ����֡ͼ���ڴ��豸
	HBITMAP hOldBitmap = (HBITMAP)SelectObject(memDC, hBmp);  // ���հ׵�֡ͼѡ���ڴ��豸

	BLENDFUNCTION dc_bf;
	dc_bf.BlendOp = AC_SRC_OVER;
	dc_bf.BlendFlags = 0;
	dc_bf.SourceConstantAlpha = alpha;
	dc_bf.AlphaFormat = AC_SRC_ALPHA;
	SetStretchBltMode(hdc, COLORONCOLOR);  //��������ģʽ

	AlphaBlend(hdc, x, y, width, height, memDC, 0, 0, imageWidth, imageHeight, dc_bf);

	//��ԭ�ڴ��豸�����е�ԭ��λͼ����
	SelectObject(memDC, hOldBitmap);
	DeleteDC(memDC);  // ɾ���ڴ��豸
	DeleteObject(hOldBitmap);  // ɾ��λͼ����
}


/*���ڴ���ָ����ߣ���ɫ�Ŀհ�λͼ*/
/* Ϊ�˱����豸������λͼ��͸����Ϣ���ú�������CreateDIBSection()������һ��32λ�Ŀհ�λͼ
Ȼ�����PaintBlank()����Ϊ����հ�λͼ���ָ����ɫ
*/
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
	// ����λͼ�����ȡBITMAP�ṹ����������
	memset((void*)&bmp, 0, sizeof(BITMAP));
	GetObject(hbmp, sizeof(BITMAP), (void*)&bmp);
	width = bmp.bmWidth;
	height = bmp.bmHeight;
	byte* bmpBytes = (byte*)bmp.bmBits;
	if (width <= 0 || height <= 0)
		return NULL;
	//����λͼ����½�һ��PixelFormat32bppPARGB��ʽ��Bitmap����
	Bitmap* BmpCpy = new Bitmap(width, height, PixelFormat32bppPARGB);
	//����LockBits����Ϊλͼ�������ݿ���׼���ڴ滺����
	BitmapData bmpData;
	Rect rect(0, 0, width, height);
	BmpCpy->LockBits(&rect, ImageLockModeWrite, PixelFormat32bppPARGB, &bmpData);
	//�����λͼÿ��������������ָ��ָ������
	int lineSize = width * 4;
	byte* cpyBytes = (byte*)(bmpData.Scan0);
	//����λͼ�ĸ����п�������������������
	for (int y = 0; y < height; y++)
	{
		memcpy((y*lineSize) + cpyBytes, ((height - y - 1)*lineSize) + bmpBytes, lineSize);
	}
	//�����ڴ滺����
	BmpCpy->UnlockBits(&bmpData);
	return BmpCpy;
}


void T_Graph::PaintRegion(HBITMAP in_hbitmap, HDC destDC, int destX, int destY,
	int srcX, int srcY, int regionWidth, int regionHeight,
	float ratio, int rotType /*= 0*/, BYTE alpha /*= 255*/)
{
	//���ݾֲ�ͼ��Ŀ�ߺ����ű����¼���ֲ�ͼ��Ŀ��
	int width = (int)(regionWidth*ratio);
	int height = (int)(regionHeight*ratio);
	//�趨Ҫ��������ʱDIBλͼ�Ĳ���
	BYTE* pBits = NULL;
	BITMAPINFO bmi;
	bmi.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	bmi.bmiHeader.biWidth = width;
	bmi.bmiHeader.biHeight = height;
	bmi.bmiHeader.biPlanes = 1;
	bmi.bmiHeader.biBitCount = 32;
	bmi.bmiHeader.biCompression = BI_RGB;
	bmi.bmiHeader.biSizeImage = width*height * 4;

	//������ʱ�ֲ�λͼ����
	HBITMAP hbitmap = CreateDIBSection(NULL, &bmi, DIB_RGB_COLORS,
		(void**)&pBits, NULL, NULL);
	HDC frameDC = ::CreateCompatibleDC(NULL); //�����ֲ�λͼ��Ӧ���豸
	//���ֲ�λͼѡ���豸 
	HBITMAP OldFrameBmp = (HBITMAP)SelectObject(frameDC, hbitmap);
	HDC memDC = CreateCompatibleDC(destDC);
	//��ԭʼλͼѡ���豸
	HBITMAP OldMemBmp = (HBITMAP)SelectObject(memDC, in_hbitmap);
	BLENDFUNCTION frame_bf; //blendfunction
	frame_bf.BlendFlags = 0;
	frame_bf.BlendOp = AC_SRC_OVER;
	frame_bf.SourceConstantAlpha = 255;
	frame_bf.AlphaFormat = AC_SRC_ALPHA;
	//��AlphaBlend������ԭʼλͼ�豸�ж�ȡ�ֲ�λͼ���ֲ�λͼ�豸
	AlphaBlend(frameDC, 0, 0, (int)(regionWidth*ratio), (int)(regionHeight*ratio),
		memDC, srcX, srcY, regionWidth, regionHeight, frame_bf);
	//ͨ���Զ��庯��HBITMAP_To_Bitmap��ȡ��Ӧ��Bitmap����
	Bitmap* nowFrameBmp = HBITMAP_To_Bitmap(hbitmap, destDC);
	SelectObject(frameDC, OldFrameBmp); //ʹ��ԭ������ԭ�ڴ��豸�е�λͼ����
	DeleteDC(frameDC);  //ɾ���ڴ��豸
	DeleteObject(OldFrameBmp);// ɾ��λͼ����
	OldFrameBmp = NULL;
	DeleteObject(hbitmap);//ɾ��λͼ����
	hbitmap = NULL;
	SelectObject(memDC, OldMemBmp);
	DeleteDC(memDC);
	DeleteObject(OldMemBmp);
	OldMemBmp = NULL;

	//ͨ��GDI+��RotateFlip�����Ծֲ�λͼ������ת��ת
	//ע���漰90��270��ʱ���뽫���ֵ���е�ת
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
	HBITMAP hbmp = NULL; //���¶���һ���µ�λͼ���
	nowFrameBmp->GetHBITMAP(NULL, &hbmp); //��������Bitmap����ת��Ϊλͼ���
	HDC bufDC = CreateCompatibleDC(destDC); //����Ҫ���Ƶ�Ŀ���豸
	HBITMAP OldbufBmp = (HBITMAP)SelectObject(bufDC, hbmp); // �����յ�λͼѡ���豸

	BLENDFUNCTION buf_bf; //�趨ҪAlphaBlend������ϵĲ���
	buf_bf.BlendOp = AC_SRC_OVER;
	buf_bf.BlendFlags = 0;
	buf_bf.SourceConstantAlpha = alpha;
	buf_bf.AlphaFormat = AC_SRC_ALPHA;

	//��AlphaBlend���������յľֲ�λͼ��Ŀ���豸���Ƶ����յ�Ŀ���豸
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
	int col = FrameCount % RowFrames;  // ���ݶ�����֡����ÿ����֡�������ÿһ��֡ͼ��ԭͼ�е����к�
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
	Graphics graph(hdc); /*ʹ��GDI+�������л���*/
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

/*ʵ�ֿհ�λͼ�Ļ���*/

/*����һ��λͼ�������ָ����λͼ������һ������ָ������ɫ*/
void T_Graph::PaintBlank(HBITMAP hbmp, int width, int height, COLORREF crColor)
{
	HDC memdc = ::CreateCompatibleDC(NULL);
	HBITMAP OldBmp = (HBITMAP)SelectObject(memdc, hbmp);

	HBRUSH hBrush = CreateSolidBrush(crColor);
	RECT rcBitmap = { 0, 0, width, height };
	FillRect(memdc, &rcBitmap, hBrush);

	SelectObject(memdc, OldBmp);  //�ظ��ڴ��豸�е�λͼ����
	DeleteDC(memdc);  // ɾ���ڴ��豸
	DeleteObject(OldBmp); // ɾ��λͼ����
	OldBmp = NULL; // �ͷž�� 
	DeleteObject(hBrush); //ɾ����ˢ

}

/*����GDI+ʵ�֣�����λͼ��͸���Ⱥ���ɫͨ��Color������ָ��*/
void T_Graph::PaintBlank(HDC hdc, int x, int y, int width, int height, Color crColor)
{
	Gdiplus::Graphics g(hdc);
	Rect rectangle(x, y, width, height);
	SolidBrush solidBrush(crColor);
	g.FillRectangle(&solidBrush, rectangle);
	g.ReleaseHDC(hdc);
}

/*����GDI��AlphaBlendʵ�ֻ��ƿհ�λͼ��*/
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

	SelectObject(memdc, OldBmp);  //�ָ��ڴ��豸�е�λͼ����s
	DeleteDC(memdc);  //ɾ���ڴ��豸
	DeleteObject(OldBmp);  //ɾ��λͼ����
	OldBmp = NULL;
	DeleteObject(hbitmap); //ɾ��
	hbitmap = NULL;
	DeleteObject(hBrush);

}

