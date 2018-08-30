#pragma once
#include "T_Config.h"
#include "T_Graph.h"
/*


*/

class T_Layer
{
public:
	T_Layer(){
		m_isVisible = true;
		m_x = -1;
		m_y = -1;
		m_width = 0;
		m_height = 0;

	}
	~T_Layer(){}
	virtual void Draw(HDC hdc) = 0;

	void SetCoordinate(int x, int y){
		m_x = x;
		m_y = y;
	}
	int GetCoordinateX(){
		return m_x;
	}
	int GetCoordinateY(){
		return m_y;
	}

	void SetWidth(int w){
		m_width = w;
	}
	void SetHeight(int h){
		m_height = h;
	}

	int GetWidth(){
		return m_width;
	}
	int GetHeight(){
		return m_height;
	}
	void SetVisible(bool b){
		m_isVisible = b;
	}
	bool IsVisible(){
		return m_isVisible;
	}
protected:
	bool m_isVisible; /*是否可见*/
	int m_x;
	int m_y;
	int m_width;
	int m_height;
	
};

