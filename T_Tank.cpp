#include "T_Tank.h"


int T_Tank::FRAME_LEFT[4] = {0,1,2,3};
int T_Tank::FRAME_RIGHT[4] = {4,5,6,7};
int T_Tank::FRAME_UP[4] = {8,9,10,11};
int T_Tank::FRAME_DOWN[4] = {12,13,14,15};


T_Tank::T_Tank()
:m_allFrames(G_ALL_FRAMES), m_img(NULL)
{
	srand(unsigned int(time(0)));
	m_active = true;
	m_dir = GAME_DIR(rand() % m_allFrames);
	m_frames = rand() % m_allFrames;
	m_speed = 0;
	m_lastTime = 0;
	m_life = 1;
	/*m_height = 40;
	m_width = 40;*/
}


T_Tank::~T_Tank()
{
	if (NULL != m_img){
		delete m_img;
	}
}

void T_Tank::Draw(HDC hdc)
{
	if (m_img != NULL){
		if (m_isVisible == true){
			m_img->PaintFrame(m_img->GetBmpHandle(), hdc, m_x, m_y,
				 GetCurrentFrames(), m_allFrames, m_width, m_height);
		}
	}
}

Rect T_Tank::GetCurrentTankRect()
{
	Rect rec;
	rec.X = m_x;
	rec.Y = m_y;
	rec.Width = m_width;
	rec.Height = m_height;
	return rec;
}

Rect T_Tank::GetNextTankRect()
{
	Rect rec;
	switch (m_dir)
	{
	case DIR_LEFT:
		rec.X = m_x - m_speed;
		rec.Y = m_y;
		break;
	case DIR_RIGHT:
		rec.X = m_x + m_speed;
		rec.Y = m_y;
		break;
	case DIR_UP:
		rec.Y = m_y - m_speed;
		rec.X = m_x;
		break;
	case DIR_DOWN:
		rec.Y = m_y + m_speed;
		rec.X = m_x;
		break;
	}
	rec.Width =  m_width;
	rec.Height = m_height;

	return rec;
}

T_Graph* T_Tank::GetImage() const
{
	return m_img;
}

void T_Tank::SetImage(wstring path)
{
	wchar_t p[256] = L"";
	wcsncpy_s(p, 255, path.c_str(), path.size());
	if (m_img != NULL){
		delete m_img;
	}
	m_img = new T_Graph(p);	
	m_width = m_img->GetImageWidth() / G_ALL_FRAMES;
	m_height = m_img->GetImageHeight() / G_ALL_FRAMES;
}

GAME_DIR T_Tank::GetDir() const
{
	return m_dir;
}

void T_Tank::SetDir(GAME_DIR dir)
{
	m_dir = dir;
}

void T_Tank::SetSpeed(int sp)
{
	m_speed = sp;
}

int T_Tank::GetSpeed()const
{
	return m_speed;
}

void T_Tank::IntoNextFrames()
{
	m_frames = (++m_frames) % m_allFrames;
}
int T_Tank::GetCurrentFrames()
{
	switch (m_dir)
	{
	case DIR_LEFT:
		return FRAME_LEFT[m_frames];
		
	case DIR_RIGHT:
		return FRAME_RIGHT[m_frames];
		
	case DIR_UP:
		return FRAME_UP[m_frames];

	case DIR_DOWN:
		return FRAME_DOWN[m_frames];

	}
	
	return 0;
}

