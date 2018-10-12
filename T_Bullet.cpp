#include "T_Bullet.h"


T_Bullet::T_Bullet()
:m_img(NULL)
{
	srand((unsigned int) time(0));
	m_speed = 0;
	m_dir = (GAME_DIR)(rand() % 4);
}


T_Bullet::~T_Bullet()
{
	if (m_img != NULL){
		delete m_img;
	}
}

void T_Bullet::LoadBulletImage(wstring path)
{
	wchar_t p[256];
	wcsncpy_s(p, 255, path.c_str(), path.size());
	m_img = new T_Graph(p);
	m_width = m_img->GetImageWidth() ;
	m_height = m_img->GetImageHeight();
}

void T_Bullet::Draw(HDC hdc)
{
	if (m_img == NULL){
		return;
	}

	//m_img->PaintImage(hdc, m_x, m_y, m_width, m_height);
	m_img->PaintImage(hdc, m_x, m_y, m_width, m_height,255);
	/*m_img->PaintRegion(m_img->GetBmpHandle(), hdc, m_x
		, m_y, 0, 0, m_width, m_height, 1, 0, 255);*/

}

Rect T_Bullet::GetNextRect()
{
	Rect rec;
	rec.X = m_x;
	rec.Y = m_y;

	switch (m_dir)
	{
	case DIR_LEFT:
		rec.X -= m_speed;
		break;
	case DIR_RIGHT:
		rec.X += m_speed;
		break;
	case DIR_UP:
		rec.Y -= m_speed;
		break;
	case DIR_DOWN:
		rec.Y += m_speed;
		break;
	}

	rec.Width = m_width;
	rec.Height = m_height;
	return rec;
}

Rect T_Bullet::GetCurrentRect()
{
	Rect rec;
	rec.X = m_x;
	rec.Y = m_y;
	rec.Width = m_width;
	rec.Height = m_height;
	return rec;
}