#include "T_Menu.h"


T_Menu::T_Menu(int wndWidth, int wndHeight)
: ROWS(10), COLS(4)
{
	m_wndWidth = wndWidth;
	m_wndHeight = wndHeight;

	
	m_back = new T_Graph(L"res//menu.jpg");
	if (m_back == NULL){
		::OutputDebugString(L"\n\nerror:\tcan't load the menu background picture\n\n");
	}
	m_rectX = wndWidth * (COLS-1) / COLS;
	m_rectY = wndHeight / ROWS ;
	m_rectWidth = wndWidth / COLS;
	m_rectHeight = wndHeight / ROWS;

	m_mouseX = 0;
	m_mouseY = 0;

}

T_Menu::~T_Menu()
{
	if (NULL != m_back){
		delete m_back;
		m_back = NULL;
	}

}


void T_Menu::DrawMenu(HDC hdc)
{
	m_back->PaintRegion(m_back->GetBmpHandle(), hdc, 0, 0, 0, 0, m_wndWidth, m_wndHeight, 1);
	int index = GetMenuIndex(m_mouseX, m_mouseY);
	Color col0 = Color::GreenYellow, col1 = Color::GreenYellow, col2 = Color::GreenYellow, col3 = Color::GreenYellow;

	switch(index)
	{
	case 0:
		col0 = Color::Red;
		break;
	case 1:
		col1 = Color::Red;
		break;
	case 2:
		col2 = Color::Red;
		break;
	case 3:
		col3 = Color::Red;
		break;
	}

	RectF rec;
	rec.X = m_rectX;
	rec.Width = m_rectWidth;
	rec.Height = m_rectHeight;

	rec.Y =m_rectY +  m_rectHeight*0;
	T_Graph::PaintText(hdc, rec, L"single people", (REAL)20, L"宋体", col0);
	
	rec.Y = m_rectY + m_rectHeight * 1;
	T_Graph::PaintText(hdc, rec, L"double people", (REAL)20, L"宋体", col1);

	rec.Y = m_rectY + m_rectHeight * 2;
	T_Graph::PaintText(hdc, rec, L"help", (REAL)20, L"宋体", col2);

	rec.Y = m_rectY + m_rectHeight * 3;
	T_Graph::PaintText(hdc, rec, L"exit", (REAL)20, L"宋体", col3);

	
	::MoveToEx(hdc, m_rectX, m_rectY - 10,NULL);
	::LineTo(hdc, m_rectX + m_rectWidth, m_rectY - 10);
}


int T_Menu::GetMenuIndex(int x, int y)
{
	int index = -1;
	int temp = 0;
	if (x > m_rectX && y > m_rectY){
		temp = (y - m_rectY) / m_rectHeight;
		if (0 <= temp && temp <= 3){
			index = temp;
		}
	}
	return index;
}

/*


*/
MENU_INDEX T_Menu::GetMenuIndexMouseClick(int x, int y)
{
	return (MENU_INDEX)GetMenuIndex(x, y);
}
