#include "T_Explode.h"


T_Explode::T_Explode()
:m_img(NULL)
{
	m_frames = 0;
	m_allFrames = G_EXPLODE_FRAMES;

}


T_Explode::~T_Explode()
{
	if (m_img != NULL){
		delete m_img;
	}
}

void T_Explode::LoadExplodeImg(wstring path)
{
	wchar_t p[256];
	wcsncpy_s(p, 255, path.c_str(), path.size());
	m_img = new T_Graph(p);
	m_width = m_img->GetImageWidth() / m_allFrames;
	m_height = m_img->GetImageHeight();
}

void T_Explode::Draw(HDC hdc)
{
	if (m_img == NULL){
		return;
	}

	T_Graph::PaintFrame(m_img->GetBmpHandle(), hdc, m_x, m_y, m_frames, m_allFrames,
		m_width, m_height);

}